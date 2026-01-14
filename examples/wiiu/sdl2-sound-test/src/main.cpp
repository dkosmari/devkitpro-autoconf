#include <filesystem>
#include <functional>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <coreinit/memory.h>    // OSGetSharedData()

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


#ifndef PACKAGE_STRING
#define PACKAGE_STRING ""
#endif


using namespace std::literals;
using std::cout;
using std::endl;


namespace sdl {

    // Convenience exception class that obtains description from `SDL_GetError()`.
    struct error : std::runtime_error{

        error(const std::string& msg) :
            std::runtime_error{msg + ": "s + SDL_GetError()}
        {}

    };


    // RAII class for SDL_Init()/SDL_Quit()
    struct init {

        bool initialized = false;

        init() = default;

        init(Uint32 flags)
        {
            if (SDL_Init(flags))
                throw error{"SDL_Init() failed"};
            initialized = true;
        }

        ~init()
        {
            if (initialized)
                SDL_Quit();
        }

    };


    // RAII class for SDL_Window
    struct window {

        SDL_Window* ptr = nullptr;

        window() noexcept = default;

        window(const char* title,
               int x, int y,
               int w, int h,
               Uint32 flags)
        {
            create(title, x, y, w, h, flags);
        }

        ~window()
            noexcept
        {
            destroy();
        }

        void
        create(const char* title,
               int x, int y,
               int w, int h,
               Uint32 flags)
        {
            destroy();
            ptr = SDL_CreateWindow(title, x, y, w, h, flags);
            if (!ptr)
                throw error{"SDL_CreateWindow() failed"};
        }

        void
        destroy()
            noexcept
        {
            if (ptr) {
                SDL_DestroyWindow(ptr);
                ptr = nullptr;
            }
        }

        int
        height()
            const noexcept
        {
            int h = 0;
            SDL_GetWindowSize(ptr, nullptr, &h);
            return h;
        }

        int
        width()
            const noexcept
        {
            int w = 0;
            SDL_GetWindowSize(ptr, &w, nullptr);
            return w;
        }


    };


    // RAII class for SDL_Surface
    struct surface {

        SDL_Surface* ptr = nullptr;

        surface() noexcept = default;

        explicit
        surface(SDL_Surface* raw_ptr)
            noexcept :
            ptr{raw_ptr}
        {}

        // move constructor
        surface(surface&& other)
            noexcept
        {
            ptr = other.ptr;
            other.ptr = nullptr;
        }

        // move assignment
        surface&
        operator =(surface&& other)
            noexcept
        {
            if (this != &other) {
                destroy();
                ptr = other.ptr;
                other.ptr = nullptr;
            }
            return *this;
        }

        ~surface()
        {
            destroy();
        }

        void
        create(SDL_Surface* raw_ptr)
        {
            destroy();
            ptr = raw_ptr;
        }

        void
        destroy()
            noexcept
        {
            if (ptr) {
                SDL_FreeSurface(ptr);
                ptr = nullptr;
            }
        }

    };


    // RAII class for SDL_Texture
    struct texture {
        SDL_Texture* ptr = nullptr;

        texture() noexcept = default;

        explicit
        texture(SDL_Texture* raw_ptr)
            noexcept :
            ptr{raw_ptr}
        {}

        // move constructor
        texture(texture&& other)
            noexcept
        {
            ptr = other.ptr;
            other.ptr = nullptr;
        }

        // move assignment
        texture&
        operator =(texture&& other)
            noexcept
        {
            if (this != &other) {
                destroy();
                ptr = other.ptr;
                other.ptr = nullptr;
            }
            return *this;
        }

        ~texture()
        {
            destroy();
        }

        void
        destroy()
            noexcept
        {
            if (ptr) {
                SDL_DestroyTexture(ptr);
                ptr = nullptr;
            }
        }

        int
        height()
            const
        {
            int h = 0;
            if (SDL_QueryTexture(ptr, nullptr, nullptr, nullptr, &h))
                throw error{"SDL_QueryTexture() failed"};
            return h;
        }

        int
        width()
            const
        {
            int w = 0;
            if (SDL_QueryTexture(ptr, nullptr, nullptr, &w, nullptr))
                throw error{"SDL_QueryTexture() failed"};
            return w;
        }

    };


    // RAII class for SDL_Renderer
    struct renderer {

        SDL_Renderer* ptr = nullptr;


        renderer() noexcept = default;

        renderer(window& win, int index, Uint32 flags)
        {
            create(win, index, flags);
        }

        ~renderer()
        {
            destroy();
        }

        void
        create(window& win, int index, Uint32 flags)
        {
            destroy();
            ptr = SDL_CreateRenderer(win.ptr, index, flags);
            if (!ptr)
                throw error{"SDL_CreateRenderer() failed"};
        }

        void
        destroy()
            noexcept
        {
            if (ptr) {
                SDL_DestroyRenderer(ptr);
                ptr = nullptr;
            }
        }

        void
        clear()
        {
            if (SDL_RenderClear(ptr))
                throw error{"SDL_RenderClear() failed"};
        }

        void
        set_color(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 0xff)
        {
            if (SDL_SetRenderDrawColor(ptr, r, g, b, a))
                throw error{"SDL_SetRenderDrawColor() failed"};
        }

        void
        present()
        {
            SDL_RenderPresent(ptr);
        }

        texture
        create_texture(const surface& surf)
        {
            auto raw_tex = SDL_CreateTextureFromSurface(ptr, surf.ptr);
            if (!raw_tex)
                throw error{"SDL_CreateTextureFromSurface() failed"};
            return texture{raw_tex};
        }

        void
        render(const texture& tex,
               const SDL_Rect* src_rect = nullptr,
               const SDL_Rect* dst_rect = nullptr)
        {
            if (SDL_RenderCopy(ptr, tex.ptr, src_rect, dst_rect))
                throw error{"SDL_RenderCopy() failed"};
        }

        void
        render(const texture& tex,
               const std::optional<const SDL_Rect>& src_rect = {},
               const std::optional<SDL_Rect>& dst_rect = {})
        {
            render(tex,
                   src_rect ? &*src_rect : nullptr,
                   dst_rect ? &*dst_rect : nullptr);
        }

    };


    namespace mix {

        // RAII class for Mix_Init()/Mix_Quit()
        struct init {

            bool initialized = false;

            init() noexcept = default;

            init(int flags)
            {
                if (Mix_Init(flags) != flags)
                    throw error{"Mix_Init() failed"};
                initialized = true;
            }

            ~init()
            {
                if (initialized)
                    Mix_Quit();
            }

        };


        // RAII class for Mix_OpenAudio[Device]()/Mix_CloseAudio()
        struct device {

            bool initialized = false;

            device() noexcept = default;

            device(int frequency, SDL_AudioFormat format, int channels, int chunksize)
            {
                create(frequency, format, channels, chunksize);
            }

            device(int frequency,
                   SDL_AudioFormat format,
                   int channels,
                   int chunksize,
                   const char* device,
                   int allowed_changes)
            {
                create(frequency, format, channels, chunksize, device, allowed_changes);
            }

            ~device()
            {
                destroy();
            }

            void
            create(int frequency, SDL_AudioFormat format, int channels, int chunksize)
            {
                if (Mix_OpenAudio(frequency, format, channels, chunksize))
                    throw error{"Mix_OpenAudio() failed"};

                int out_freq;
                SDL_AudioFormat out_fmt;
                int out_chan;
                Mix_QuerySpec(&out_freq, &out_fmt, &out_chan);
                cout << "Opened audio: freq=" << out_freq
                     << ", channels=" << out_chan
                     << endl;
            }


            void
            create(int frequency,
                   SDL_AudioFormat format,
                   int channels,
                   int chunksize,
                   const char* device,
                   int allowed_changes)
            {
                if (Mix_OpenAudioDevice(frequency, format, channels,
                                        chunksize, device, allowed_changes))
                    throw error{"Mix_OpenAudioDevice() failed"};

                int out_freq;
                SDL_AudioFormat out_fmt;
                int out_chan;
                Mix_QuerySpec(&out_freq, &out_fmt, &out_chan);
                cout << "Opened audio: freq=" << out_freq
                     << ", channels=" << out_chan
                     << endl;
            }


            void
            destroy()
            {
                if (initialized) {
                    Mix_CloseAudio();
                    initialized = false;
                }
            }

        };

        // RAII class for Mix_Chunk
        struct chunk {

            Mix_Chunk* ptr = nullptr;

            chunk() noexcept = default;

            chunk(const std::filesystem::path& filename)
            {
                create(filename);
            }

            ~chunk()
                noexcept
            {
                destroy();
            }

            void
            create(const std::filesystem::path& filename)
            {
                destroy();
                ptr = Mix_LoadWAV(filename.c_str());
                if (!ptr)
                    throw error{"Mix_LoadWAV() failed"};
            }

            void
            destroy()
                noexcept
            {
                if (ptr) {
                    Mix_FreeChunk(ptr);
                    ptr = nullptr;
                }
            }

            int
            play(int channel = -1, int loops = 0)
                noexcept
            {
                return Mix_PlayChannel(channel, ptr, loops);
            }

        };

    } // namespace mix


    namespace ttf {

        // RAII class for TTF_Init()/TTF_Quit()
        struct init {

            init()
            {
                if (TTF_Init())
                    throw error{"TTF_Init() failed"};
            }

            ~init()
            {
                TTF_Quit();
            }

        };


        // RAII class for TTF_Font
        struct font {

            TTF_Font* ptr = nullptr;

            font() noexcept = default;

            font(SDL_RWops* src, bool take_src, int pt_size)
            {
                create(src, take_src, pt_size);
            }

            ~font()
                noexcept
            {
                destroy();
            }

            void
            create(SDL_RWops* src, bool take_src, int pt_size)
            {
                destroy();
                ptr = TTF_OpenFontRW(src, take_src, pt_size);
                if (!ptr)
                    throw error{"TTF_OpenFontRW() failed"};
            }

            void
            destroy()
                noexcept
            {
                if (ptr) {
                    TTF_CloseFont(ptr);
                    ptr = nullptr;
                }
            }

            surface
            render_blended(const char* text,
                           SDL_Color fg = { .r = 0xff, .g = 0xff, .b = 0xff, .a = 0xff })
            {
                auto s = TTF_RenderUTF8_Blended(ptr, text, fg);
                if (!s)
                    throw error{"TTF_RenderUTF8_Blended() failed"};
                return surface{s};
            }

        };


    } // namespace ttf


} // namespace sdl


struct Button {

    sdl::texture texture;
    SDL_Rect rect;

    std::function<void()> on_click;

    bool click_started = false;


    bool
    is_inside(int x, int y)
        const noexcept
    {
        return x >= rect.x
            && x < rect.x + rect.w
            && y >= rect.y
            && y < rect.y + rect.h;
    }


    bool
    handle_event(const SDL_Event& e)
    {
        switch (e.type) {

            case SDL_MOUSEBUTTONDOWN:
                if (is_inside(e.button.x, e.button.y))
                    click_started = true;
                // TODO: change rendering when click_started
                return click_started;

            case SDL_MOUSEBUTTONUP:
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

};



struct App {

    sdl::init sdl_initializer{SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER};
    sdl::mix::device mix_dev{48000, AUDIO_S16SYS,
                             6, 6 * 1024,
                             nullptr,
                             SDL_AUDIO_ALLOW_FREQUENCY_CHANGE |
                             SDL_AUDIO_ALLOW_FORMAT_CHANGE |
                             SDL_AUDIO_ALLOW_SAMPLES_CHANGE
    };

    sdl::mix::init mix_initializer{MIX_INIT_OGG};
    sdl::ttf::init ttf_initializer;

    sdl::window window{PACKAGE_STRING,
                       SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                       1280, 720,
                       0};

    sdl::renderer renderer{window, -1,
                           SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC};

    sdl::ttf::font font;

    sdl::mix::chunk sound_front_center {"/vol/content/front-center.ogg"};
    sdl::mix::chunk sound_front_left   {"/vol/content/front-left.ogg"};
    sdl::mix::chunk sound_front_right  {"/vol/content/front-right.ogg"};
    sdl::mix::chunk sound_rear_left    {"/vol/content/rear-left.ogg"};
    sdl::mix::chunk sound_rear_right   {"/vol/content/rear-right.ogg"};
    sdl::mix::chunk sound_lfe          {"/vol/content/lfe.ogg"};

    std::vector<Button> buttons;


    int window_width = 0;
    int window_height = 0;

    bool running = false;


    App()
    {
        // Make sure renderer coordinates match window coordinates
        {
            SDL_GetWindowSize(window.ptr, &window_width, &window_height);
            SDL_RenderSetLogicalSize(renderer.ptr, window_width, window_height);
        }

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
                             60);
            cout << "... done" << endl;
        }

        {
            Button b;
            b.texture = renderer.create_texture(font.render_blended("Front Left"));
            b.on_click = [this] { sound_front_left.play(); };
            b.rect.x = 10;
            b.rect.y = 10;
            b.rect.w = b.texture.width();
            b.rect.h = b.texture.height();
            buttons.push_back(std::move(b));
        }

        {
            Button b;
            b.texture = renderer.create_texture(font.render_blended("Front Right"));
            b.on_click = [this] { sound_front_right.play(); };
            b.rect.x = window_width - 10 - b.texture.width();
            b.rect.y = 10;
            b.rect.w = b.texture.width();
            b.rect.h = b.texture.height();
            buttons.push_back(std::move(b));
        }

        {
            Button b;
            b.texture = renderer.create_texture(font.render_blended("Front Center"));
            b.on_click = [this] { sound_front_center.play(); };
            b.rect.x = (window_width - b.texture.width()) / 2;
            b.rect.y = 10;
            b.rect.w = b.texture.width();
            b.rect.h = b.texture.height();
            buttons.push_back(std::move(b));
        }

        {
            Button b;
            b.texture = renderer.create_texture(font.render_blended("Rear Left"));
            b.on_click = [this] { sound_rear_left.play(); };
            b.rect.x = 10;
            b.rect.y = window_height - 10 - b.texture.height();
            b.rect.w = b.texture.width();
            b.rect.h = b.texture.height();
            buttons.push_back(std::move(b));
        }

        {
            Button b;
            b.texture = renderer.create_texture(font.render_blended("Rear Right"));
            b.on_click = [this] { sound_rear_right.play(); };
            b.rect.x = window_width - 10 - b.texture.width();
            b.rect.y = window_height - 10 - b.texture.height();
            b.rect.w = b.texture.width();
            b.rect.h = b.texture.height();
            buttons.push_back(std::move(b));
        }

        {
            Button b;
            b.texture = renderer.create_texture(font.render_blended("LFE"));
            b.on_click = [this] { sound_lfe.play(); };
            b.rect.x = (window_width - b.texture.width()) / 2;
            b.rect.y = window_height - 10 - b.texture.height();
            b.rect.w = b.texture.width();
            b.rect.h = b.texture.height();
            buttons.push_back(std::move(b));
        }

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
        SDL_Event event;
        while (running && SDL_PollEvent(&event))
            handle_event(event);
    }


    void handle_event(const SDL_Event& event)
    {
        switch (event.type) {

        case SDL_QUIT:
            cout << "Quitting..." << endl;
            running = false;
            break;

        case SDL_CONTROLLERDEVICEADDED: {
            auto c = SDL_GameControllerOpen(event.cdevice.which);
            cout << "Added controller : " << SDL_GameControllerName(c) << endl;
            break;
        }

        case SDL_CONTROLLERDEVICEREMOVED:
            if (auto ctrlr = SDL_GameControllerFromInstanceID(event.cdevice.which))
                SDL_GameControllerClose(ctrlr);
            break;

        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
            cout << "Mouse at " << event.button.x << " , " << event.button.y << endl;
            for (auto& b : buttons)
                b.handle_event(event);
            break;

        }
    }


    void
    draw()
    {
        renderer.set_color(0x00, 0x40, 0x00);
        renderer.clear();

        for (const auto& b : buttons)
            renderer.render(b.texture, {}, b.rect);

        renderer.present();
    }

};


int main(int, char* [])
{

    try {
        App app;
        app.run();
    }
    catch (std::exception& e) {
        cout << "Exception: " << e.what() << endl;
    }

}
