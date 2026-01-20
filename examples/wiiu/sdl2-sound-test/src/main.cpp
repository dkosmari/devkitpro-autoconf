#include <algorithm>
#include <cmath>
#include <filesystem>
#include <functional>
#include <iostream>
#include <optional>
#include <span>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <coreinit/memory.h>    // OSGetSharedData()

#include <sdl2xx/sdl.hpp>
#include <sdl2xx/mix.hpp>
#include <sdl2xx/ttf.hpp>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


#ifndef PACKAGE_STRING
#define PACKAGE_STRING ""
#endif


using namespace std::literals;
using std::cout;
using std::endl;

using sdl::vec2;
using sdl::vec2f;

struct Button {

    sdl::texture texture;
    sdl::rect tex_rect;
    sdl::rect rect;

    std::function<void()> on_click;

    bool click_started = false;

    Button(sdl::renderer& renderer,
           sdl::ttf::font& font,
           const std::string& text,
           vec2f align,
           std::function<void()> callback)
    {
        const vec2 padding = {20, 10};

        auto img = font.render_blended(text, sdl::color::black);
        texture.create(renderer, img);
        auto tex_size = img.get_size();
        auto full_size = tex_size + 2 * padding;
        auto ren_size = renderer.get_logical_size();
        auto pos = scaled(vec2f{ren_size}, align) - vec2f{full_size} / 2.0f;
        auto max = ren_size - full_size;
        // Ensure texture is not larger than the screen
        if (max.x > 0)
            pos.x = std::clamp(pos.x, 0.0f, float(max.x));
        else
            pos.x = 0;
        // Ensure texture is not larger than the screen
        if (max.y > 0)
            pos.y = std::clamp(pos.y, 0.0f, float(max.y));
        else
            pos.y = 0;

        auto ipos = vec2{pos};
        rect.set_min_corner(ipos);
        rect.set_size(full_size);

        tex_rect.set_min_corner(ipos + padding);
        tex_rect.set_size(tex_size);

        on_click = std::move(callback);
    }


    Button(Button&&) noexcept = default;
    Button& operator=(Button&&) noexcept = default;


    bool
    is_inside(int x, int y)
        const noexcept
    {
        return rect.contains({x, y});
    }


    bool
    handle_event(const sdl::events::event& e)
    {
        switch (e.type) {

            case sdl::events::type::e_mouse_down:
                if (is_inside(e.button.x, e.button.y))
                    click_started = true;
                // TODO: change rendering when click_started
                return click_started;

            case sdl::events::type::e_mouse_up:
                if (!is_inside(e.button.x, e.button.y)) {
                    click_started = false;
                    return false;
                }
                if (click_started && on_click) {
                    cout << "Button calling on_click callback." << endl;
                    on_click();
                }
                click_started = false;
                return true;

            default:
                return false;
        }
    }


    void
    draw(sdl::renderer& renderer)
        const
    {
        if (click_started)
            renderer.set_color(sdl::color::red);
        else
            renderer.set_color(sdl::color::white);
        renderer.fill_box(rect);

        renderer.set_color(sdl::color::black);
        renderer.draw_box(rect);

        renderer.copy(texture, {}, tex_rect);
    }

};


struct App {

    sdl::init sdl_initializer{
        sdl::init::flag::video,
        sdl::init::flag::audio,
        sdl::init::flag::game_controller
    };

    sdl::ttf::init ttf_initializer;

    sdl::mix::init mix_initializer{sdl::mix::init::flag::ogg};

    sdl::mix::device mix_dev{
        48000,
        AUDIO_S16SYS,
        6,
        6 * 1024,
        nullptr,
        sdl::audio::allow_change::format,
        sdl::audio::allow_change::frequency,
        sdl::audio::allow_change::samples
    };

    sdl::window window{
        PACKAGE_STRING,
        sdl::window::pos_undefined,
        {1280, 720}
    };

    sdl::renderer renderer{
        window,
        -1,
        sdl::renderer::flag::accelerated,
        sdl::renderer::flag::present_vsync
    };

    sdl::ttf::font font;

    sdl::mix::chunk sound_front_left   {"/vol/content/front-left.ogg"};
    sdl::mix::chunk sound_front_right  {"/vol/content/front-right.ogg"};
    sdl::mix::chunk sound_front_center {"/vol/content/front-center.ogg"};
    sdl::mix::chunk sound_rear_left    {"/vol/content/rear-left.ogg"};
    sdl::mix::chunk sound_rear_right   {"/vol/content/rear-right.ogg"};
    sdl::mix::chunk sound_lfe          {"/vol/content/lfe.ogg"};

    std::vector<Button> buttons;


    bool running = false;


    App()
    {
        // Make sure renderer coordinates match window coordinates
        renderer.set_logical_size(window.get_size());

        {
            cout << "Loading system font" << endl;
            void* system_font_data = nullptr;
            uint32_t system_font_size = 0;
            if (!OSGetSharedData(OS_SHAREDDATATYPE_FONT_STANDARD,
                                 0xefface,
                                 &system_font_data,
                                 &system_font_size))
                throw std::runtime_error{"could not get system font"};

            font.create(SDL_RWFromConstMem(system_font_data, system_font_size),
                        1,
                        48);
            cout << "... done" << endl;
        }

        buttons.emplace_back(renderer, font,
                             "Front Left",
                             vec2f{0.0f, 0.0f},
                             [this] { play_front_left(); });
        buttons.emplace_back(renderer, font,
                             "Front Right",
                             vec2f{1.0f, 0.0f},
                             [this] { play_front_right(); });

        buttons.emplace_back(renderer, font,
                             "Front Center",
                             vec2f{0.5f, 0.0f},
                             [this] { play_front_center(); });

        buttons.emplace_back(renderer, font,
                             "Rear Left",
                             vec2f{0.0f, 1.0f},
                             [this] { play_rear_left(); });

        buttons.emplace_back(renderer, font,
                             "Rear Right",
                             vec2f{1.0f, 1.0f},
                             [this] { play_rear_right(); });

        buttons.emplace_back(renderer, font,
                             "LFE",
                             vec2f{0.5f, 0.5f},
                             [this] { play_lfe(); });

    }


    void
    run()
    {
        running = true;
        while (running) {
            draw();
            handle_events();
        }
    }


    void
    handle_events()
    {
        sdl::events::event event;
        while (running && sdl::events::poll(event))
            handle_event(event);
    }


    void handle_event(const sdl::events::event& event)
    {
        switch (event.type) {

            case sdl::events::type::e_quit:
                cout << "Quitting..." << endl;
                running = false;
                break;

            case sdl::events::type::e_controller_device_added: {
                auto c = SDL_GameControllerOpen(event.cdevice.which);
                cout << "Added controller : " << SDL_GameControllerName(c) << endl;
                break;
            }

            case sdl::events::type::e_controller_device_removed:
                if (auto ctrlr = SDL_GameControllerFromInstanceID(event.cdevice.which))
                    SDL_GameControllerClose(ctrlr);
                break;

            case sdl::events::type::e_mouse_down:
            case sdl::events::type::e_mouse_up:
                cout << "Mouse at " << event.button.x << " , " << event.button.y << endl;
                for (auto& b : buttons)
                    b.handle_event(event);
                break;

        }
    }


    void
    draw()
    {
        renderer.set_color(sdl::color::olive);
        renderer.clear();

        for (const auto& b : buttons)
            b.draw(renderer);

        renderer.present();
    }


    void
    play_front_left()
    {
        cout << "Playing front left." << endl;
        sound_front_left.play();
    }


    void
    play_front_right()
    {
        cout << "Playing front right." << endl;
        sound_front_right.play();
    }


    void
    play_front_center()
    {
        cout << "Playing front center." << endl;
        sound_front_center.play();
    }


    void
    play_rear_left()
    {
        cout << "Playing rear left." << endl;
        sound_rear_left.play();
    }


    void
    play_rear_right()
    {
        cout << "Playing rear right." << endl;
        sound_rear_right.play();
    }


    void
    play_lfe()
    {
        cout << "Playing LFE." << endl;
        sound_lfe.play();
    }

}; // struct App


int main(int, char* [])
{

    try {
        App app;
        app.run();
    }
    catch (std::exception& e) {
        cout << "Exception: " << e.what() << endl;
    }
    catch (...) {
        cout << "Unknown exception!" << endl;
    }
}
