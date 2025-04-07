#include <array>
#include <clocale>
#include <iostream>
#include <string>
#include <vector>

#include <coreinit/memory.h>    // OSGetSharedData()
#include <nn/swkbd.h>

#include "sdl.hpp"
#include "sdl_debug.hpp"
#include "sdl_ttf.hpp"

#include <SDL2/SDL_syswm.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


using std::cout;
using std::endl;

using namespace std::string_literals;

using namespace sdl::literals;

using sdl::vec2;


const std::array locales{
    // use system config
    "C",
    // USA region
    "en_US",
    "fr_CA",
    "es_MX",
    "pt_BR",
    // Europe region
    "en_GB",
    "fr_FR",
    "de_DE",
    "it_IT",
    "es_ES",
    "de_DE",
    "nl_NL",
    "ru_RU",
    // Japan
    "ja_JP",
    // country-less languages, should get a reasonable default region
    "en", // -> USA region
    "fr", // -> Europe region
    "es", // -> Europe region
    "pt", // -> Europe region
};


struct Label {

    static constexpr int border = 4;
    static constexpr int padding = 16;

    sdl::renderer& renderer;
    sdl::ttf::font& font;

    std::string text;
    sdl::texture texture;

    sdl::rect text_rect{};
    sdl::rect box_rect{};
    sdl::rect border_rect{};


    Label(sdl::renderer& renderer,
          sdl::ttf::font& font,
          const std::string& text_) :
        renderer(renderer),
        font(font)
    {
        set_text(text_);
    }

    void
    set_text(const std::string& new_text)
    {
        vec2 old_pos = get_position();

        text = new_text;
        texture.destroy();
        try {
            if (!text.empty()) {
                auto surface = font.render_blended_wrapped(text.data());
                texture = renderer.create_texture(surface);
            }
        }
        catch (std::exception& e) {
            cout << "Label::set_text() failed: " << e.what() << endl;
        }

        // Update rect sizes

        auto [w, h] = text_size();
        text_rect.w = w;
        text_rect.h = h;

        box_rect.w = text_rect.w + 2 * padding;
        box_rect.h = text_rect.h + 2 * padding;

        border_rect.w = box_rect.w + 2 * border;
        border_rect.h = box_rect.h + 2 * border;

        // Positioning requires updated rect sizes
        set_position(old_pos);
    }

    vec2
    text_size()
        const
    {
        if (!texture)
            return {0, font.height()};
        return texture.size();
    }

    void
    set_position(const vec2& p)
    {
        text_rect.x = p.x - text_rect.w / 2;
        text_rect.y = p.y - text_rect.h / 2;

        box_rect.x = text_rect.x - padding;
        box_rect.y = text_rect.y - padding;

        border_rect.x = box_rect.x - border;
        border_rect.y = box_rect.y - border;
    }

    void
    set_position(int x, int y)
    {
        set_position({x, y});
    }

    vec2
    get_position()
        const noexcept
    {
        return text_rect.center();
    }

    void
    draw(bool selected)
        const
    {
        // draw border
        renderer.set_color(0xffffff_rgb);
        renderer.fill_rect(border_rect);

        // draw background
        if (selected)
            renderer.set_color(0x404080_rgb);
        else
            renderer.set_color(0x000000_rgb);
        renderer.fill_rect(box_rect);

        // draw text
        if (texture)
            renderer.copy(texture, {}, text_rect);
    }

    bool
    inside(const vec2& p)
        const noexcept
    {
        return SDL_PointInRect(&p, &box_rect);
    }

    vec2
    relative(const vec2& p)
    {
        return p - get_position();
    }

};


struct App {

    sdl::init sdl_init{SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER};
    sdl::ttf::init ttf_init;

    sdl::window window{
        PACKAGE_STRING,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        0, 0,
        SDL_WINDOW_FULLSCREEN
    };

    sdl::renderer renderer{
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    };

    sdl::ttf::font font;

    bool running = false;

    std::string new_text;

    std::vector<Label> labels;

    Label* selected = nullptr;
    vec2 grab_offset;
    vec2 press_pos;
    vec2 release_pos;
    std::string editing_text;
    bool creating = false;

    int current_locale = 0;
    sdl::texture locale_texture;


    App()
    {
        auto win_size = window.size();
        cout << "Window size is " << win_size.x << "x" << win_size.y << endl;

        renderer.set_logical_size(window.size());
        renderer.set_blend_mode(SDL_BLENDMODE_BLEND);

        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

        // Receive notifications about the swkbd closing.
        SDL_EventState(SDL_SYSWMEVENT, SDL_ENABLE);

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
                    40);
        cout << "... done" << endl;

        if (SDL_HasScreenKeyboardSupport())
            cout << "We got swkbd support!" << endl;
        else
            cout << "No swkbd support!" << endl;

        {
            Label instructions{renderer, font, "Double tap to add/edit a label"};
            instructions.set_position(window.size() / 2);
            labels.push_back(std::move(instructions));
        }

        update_locale(0);
    }

    // ~App()
    // {}

    void
    run()
    {
        running = true;
        cout << "Running..." << endl;
        while (running) {
            draw();
            handle_events();
        }
        cout << "Quitting..." << endl;
    }

    void
    draw()
    {
        if (window.is_screen_keyboard_shown())
            renderer.set_color(0x808000_rgb);
        else
            renderer.set_color(0x006000_rgb);
        renderer.clear();

        // SDL BUG
        // Draw a transparent point to force SDL to update viewport/clipping.
        renderer.set_color(0, 0, 0, 0);
        renderer.draw_point(1, 1);


        for (auto& label : labels)
            label.draw(&label == selected);

        if (locale_texture) {
            sdl::rect locale_rect;
            locale_rect.x = 10;
            locale_rect.y = 10;
            locale_rect.w = locale_texture.width();
            locale_rect.h = locale_texture.height();
            renderer.copy(locale_texture, {}, locale_rect);
        }

        renderer.present();
    }

    void
    handle_events()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
            handle_event(event);
    }

    void
    handle_event(const SDL_Event& event)
    {
        sdl::debug::print(event);

        switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;

            case SDL_CONTROLLERDEVICEADDED:
                SDL_GameControllerOpen(event.cdevice.which);
                break;

            case SDL_CONTROLLERDEVICEREMOVED:
                if (auto ctrlr = SDL_GameControllerFromInstanceID(event.cdevice.which))
                    SDL_GameControllerClose(ctrlr);
                break;

            case SDL_CONTROLLERBUTTONDOWN:
                handle_button_down(event.cbutton);
                break;

            case SDL_MOUSEBUTTONDOWN:
                handle_mouse_down(event.button);
                break;

            case SDL_MOUSEBUTTONUP:
                handle_mouse_up(event.button);
                break;

            case SDL_MOUSEMOTION:
                handle_mouse_motion(event.motion);
                break;

            case SDL_TEXTINPUT:
                handle_text_input(event.text);
                break;

            case SDL_SYSWMEVENT:
                handle_syswm(event.syswm);
                break;

        }
    }

    void
    handle_mouse_down(const SDL_MouseButtonEvent& event)
    {
        if ((event.button & 1) == 0)
            return;

        press_pos = {event.x, event.y};
        selected = nullptr;
        for (auto& label : labels)
            if (label.inside(press_pos)) {
                selected = &label;
                grab_offset = label.relative({event.x, event.y});
                break;
            }
    }

    void
    handle_mouse_up(const SDL_MouseButtonEvent& event)
    {
        release_pos = {event.x, event.y};

        const float drag_threshold = 20;
        bool dragged = (release_pos - press_pos).length() > drag_threshold;

        if (selected) {
            if (!dragged) {
                if (event.clicks == 2) {
                    // double-clicked on a label
                    creating = false;
                    editing_text.clear();

                    SDL_WiiUSetSWKBDInitialText(selected->text.data());
                    SDL_WiiUSetSWKBDHighlightInitialText(SDL_TRUE);
                    SDL_WiiUSetSWKBDShowWordSuggestions(SDL_TRUE);
                    SDL_WiiUSetSWKBDOKLabel("Modify");
                    window.focus();
                    SDL_StartTextInput();
                }
            }
        } else {
            if (event.clicks == 2) {
                // double-clicked on the background
                creating = true;
                editing_text.clear();
                labels.emplace_back(renderer, font, "");
                selected = &labels.back();
                selected->set_position(release_pos);

                SDL_WiiUSetSWKBDHintText("New label text...");
                SDL_WiiUSetSWKBDShowWordSuggestions(SDL_TRUE);
                SDL_WiiUSetSWKBDOKLabel("Create");
                window.focus();
                SDL_StartTextInput();
            }
        }
    }

    void
    handle_mouse_motion(const SDL_MouseMotionEvent& event)
    {
        // ignore motion with no button
        if ((event.state & 1) == 0)
            return;

        if (selected) {
            vec2 mouse_pos = {event.x, event.y};
            selected->set_position(mouse_pos - grab_offset);
        }
    }

    void
    handle_text_input(const SDL_TextInputEvent& event)
    {
        if (!selected) {
            cout << "ignoring text input, nothing selected" << endl;
            return;
        }
        cout << "appending to editing_text: \"" << event.text << "\"" << endl;
        editing_text += event.text;
    }

    void
    handle_syswm(const SDL_SysWMEvent& event)
    {
        if (event.msg->subsystem != SDL_SYSWM_WIIU)
            return;

        switch (event.msg->msg.wiiu.event) {
            case SDL_WIIU_SYSWM_SWKBD_OK_EVENT:
                if (selected)
                    selected->set_text(editing_text);
                editing_text.clear();
                break;

            case SDL_WIIU_SYSWM_SWKBD_CANCEL_EVENT:
                editing_text.clear();
                if (creating) {
                    labels.pop_back();
                    selected = nullptr;
                }
                break;
        }
    }

    void
    handle_button_down(const SDL_ControllerButtonEvent& event)
    {
        switch (event.button) {

            case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
                update_locale(current_locale - 1);
                break;

            case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
                update_locale(current_locale + 1);
                break;

        }
    }

    void
    update_locale(int new_locale)
    {
        if (new_locale < 0)
            new_locale = locales.size() - 1;
        if (new_locale >= static_cast<int>(locales.size()))
            new_locale = 0;
        current_locale = new_locale;

        char* result = std::setlocale(LC_CTYPE, locales[current_locale]);
        if (result)
            cout << "set LC_CTYPE to " << result << endl;
        else
            cout << "failed to set LC_CTYPE!" << endl;

        std::string locale_str = "LC_CTYPE: \""s + locales[current_locale] + "\""s;
        auto surface = font.render_blended(locale_str.data());
        locale_texture = renderer.create_texture(surface);
    }

};


int main()
{
    cout << "Starting " << PACKAGE << endl;

    try {
        App app;
        app.run();
    }
    catch (std::exception& e) {
        cout << "Exception: " << e.what() << endl;
    }
}
