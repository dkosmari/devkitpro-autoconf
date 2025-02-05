#include <array>
#include <atomic>
#include <cassert>
#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
#include <thread>
#include <cmath>
#include <numbers>

#include <sys/iosupport.h>      // devoptab_list, devoptab_t

#include <coreinit/memory.h>    // OSGetSharedData()
#include <padscore/kpad.h>
#include <vpadbase/base.h>      // VPADBASESetSensorBarSetting()

#include <whb/log.h>
#include <whb/log_module.h>

#include <SDL.h>
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


extern "C" void KPADSetPosPlayMode(KPADChan channel, BOOL mode);
extern "C" void KPADSetPosParam(KPADChan channel, float radius, float sensitivity);


// These functions connect STDOUT to the WHBLog*() API.

ssize_t
write_msg_to_whb_log(struct _reent*,
                     void*,
                     const char* buf,
                     size_t len)
    noexcept
{
    try {
        std::string msg(buf, len);
        WHBLogWrite(msg.data());
        return len;
    }
    catch (...) {
        return -1;
    }
}


__attribute__(( __constructor__ ))
void
init_stdout()
{
    WHBLogModuleInit();
    static devoptab_t dev;
    dev.name = "STDOUT";
    dev.structSize = sizeof dev;
    dev.write_r = write_msg_to_whb_log;
    devoptab_list[STD_OUT] = &dev;
}

__attribute__(( __destructor__ ))
void
fini_stdout()
{
    WHBLogModuleDeinit();
}


// Convenience exception class that obtains description from `SDL_GetError()`.
struct sdl_error : std::runtime_error{

    sdl_error(const std::string& msg) :
        std::runtime_error{msg + ": "s + SDL_GetError()}
    {}

};


struct Cursor {
    SDL_Texture* texture = nullptr;
    SDL_Color color;
    bool visible = false;
    float x = 0;
    float y = 0;
    int width = 0;
    int height = 0;
    double angle = 0;
    std::vector<SDL_FPoint> trail;


    constexpr Cursor() noexcept = default;


    Cursor(SDL_Texture* t,
           const SDL_Color& c)
        noexcept :
        texture{t},
        color{c}
    {
        SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
    }


    // move constructor
    Cursor(Cursor&& other) noexcept :
        texture{other.texture},
        color{other.color},
        visible{other.visible},
        x{other.x},
        y{other.y},
        width{other.width},
        height{other.height},
        angle{other.angle},
        trail{std::move(other.trail)}
    {
        other.texture = nullptr;
    }


    // move assignment
    Cursor&
    operator =(Cursor&& other)
        noexcept
    {
        if (&other != this) {
            // Steal `texture` from `other`.
            if (texture)
                SDL_DestroyTexture(texture);
            texture = other.texture;
            other.texture = nullptr;

            color   = other.color;
            visible = other.visible;
            x       = other.x;
            y       = other.y;
            width   = other.width;
            height  = other.height;
            angle   = other.angle;
            trail   = std::move(other.trail);
        }
        return *this;
    }


    ~Cursor()
    {
        if (texture)
            SDL_DestroyTexture(texture);
    }


    void
    add_point(float x, float y)
    {
        const auto max_points = 32z;
        if (trail.size() >= max_points)
            trail.erase(trail.begin(), trail.end() - max_points + 1);

        trail.emplace_back(x, y);
    }


};


struct App {

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    bool running = false;
    std::array<bool, 4> wiimote_present{};

    TTF_Font* font = nullptr;
    std::vector<Cursor> cursors;

    // These are needed by the KPAD connect callback.
    static std::atomic<App*> instance;
    std::mutex mutex;


    App()
    {
        instance = this;
        VPADBASESetSensorBarSetting(VPAD_CHAN_0, 1);
        {
            KPADInit();
            std::lock_guard guard{mutex};
            for (unsigned c = 0; c < 4; ++c)
                KPADSetConnectCallback(KPADChan(c), &on_kpad_connect);
        }

        if (SDL_Init(SDL_INIT_VIDEO))
            throw sdl_error{"SDL_Init() failed"};

        SDL_SetHint(SDL_HINT_RENDER_LINE_METHOD, "3");

        window = SDL_CreateWindow(PACKAGE_STRING,
                                  SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                  1280, 720,
                                  0);
        if (!window)
            throw sdl_error{"SDL_CreateWindow() failed"};

        renderer = SDL_CreateRenderer(window, -1,
                                      SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (!renderer)
            throw sdl_error{"SDL_CreateRenderer() failed"};

        {
            int width, height;
            SDL_GetWindowSize(window, &width, &height);
            SDL_RenderSetLogicalSize(renderer, width, height);
        }

        // Ask SDL to not round coordinates to integers.
        SDL_RenderSetIntegerScale(renderer, SDL_FALSE);


        if (TTF_Init())
            throw sdl_error{"TTF_Init() failed"};


        void* system_font_data = nullptr;
        uint32_t system_font_size = 0;
        if (!OSGetSharedData(OS_SHAREDDATATYPE_FONT_STANDARD,
                            0xefface,
                            &system_font_data,
                            &system_font_size))
            throw std::runtime_error{"could not get system font"};

        // Note: size 80 looks good for 1280x720.
        font = TTF_OpenFontRW(SDL_RWFromConstMem(system_font_data, system_font_size),
                              1,
                              80);
        if (!font)
            throw sdl_error{"TTF_OpenFontRW() failed"};

        // Codepoints for the hand cursors in "CafeStd.ttf".
        const char32_t glyphs[4] = {
            U'\ue059',
            U'\ue05a',
            U'\ue05b',
            U'\ue05c',
        };
        const SDL_Color colors[4] = {
            { 0x00, 0x80, 0xff, 0xff }, // player 1 is light blue
            { 0xff, 0x40, 0x40, 0xff }, // player 2 is light red
            { 0x30, 0xff, 0x20, 0xff }, // player 3 is light green
            { 0xff, 0xc0, 0x00, 0xff }, // player 4 is gold
        };

        for (unsigned i = 0; i < 4; ++i) {
            auto img = TTF_RenderGlyph32_Blended(font, glyphs[i], colors[i]);
            if (!img)
                throw sdl_error{"TTF_RenderGlyph32_Blended() failed"};

            auto tex = SDL_CreateTextureFromSurface(renderer, img);
            if (!tex)
                throw sdl_error{"SDL_CreateTextureFromSurface() failed"};

            cursors.emplace_back(tex, colors[i]);

            SDL_FreeSurface(img);
        }

    }


    ~App()
    {
        cursors.clear();

        TTF_Quit();

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);

        SDL_Quit();

        KPADShutdown();

        instance = nullptr;
    }


    void
    run()
    {
        running = true;
        while (running) {
            handle_events();
            handle_wiimotes();
            draw();
        }
    }


    void
    draw()
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        for (const auto& cur : cursors) {
            if (!cur.visible)
                continue;

            /*
             * We use the "tip" offset to indicate where is the "center" of the cursor.
             * The rotation is also applied around this "center".
             */
            SDL_FPoint tip{
                0.4f * cur.width,
                0.2f * cur.height
            };

            SDL_FRect dst_rect{
                .x = cur.x - tip.x,
                .y = cur.y - tip.y,
                .w = float(cur.width),
                .h = float(cur.height)
            };

            SDL_RenderCopyExF(renderer,
                              cur.texture,
                              nullptr,
                              &dst_rect,
                              cur.angle,
                              &tip,
                              SDL_FLIP_NONE);

            if (cur.trail.size() > 1) {
                SDL_SetRenderDrawColor(renderer,
                                       cur.color.r,
                                       cur.color.g,
                                       cur.color.b,
                                       cur.color.a);
                SDL_RenderDrawLinesF(renderer, cur.trail.data(), cur.trail.size());
            }
        }

        SDL_RenderPresent(renderer);
    }


    void
    handle_events()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            handle_event(event);
        }
    }


    void
    handle_event(const SDL_Event& event)
    {
        switch (event.type) {
        case SDL_QUIT:
            running = false;
            break;
        }
    }


    // Note: this callback runs in a different thread.
    static
    void
    on_kpad_connect(WPADChan channel,
                    WPADError status)
    {
        App* app = App::instance;
        assert(app);

        std::lock_guard guard{app->mutex};
        if (status == WPAD_ERROR_NONE) {
            app->wiimote_present[channel] = true;
            KPADEnableDPD(channel);
            // KPADSetPosPlayMode(channel, 1);
            KPADSetPosParam(channel, 0.05, 1.0);
        }
        else
            app->wiimote_present[channel] = false;
    }


    void
    handle_wiimotes()
    {
        int width, height;
        SDL_RenderGetLogicalSize(renderer, &width, &height);

        std::lock_guard guard{mutex};
        for (unsigned i = 0; i < 4; ++i) {
            auto& cur = cursors[i];

            // when no wiimote connected
            if (!wiimote_present[i]) {
                cur.visible = false;
                cur.trail.clear();
                continue;
            }

            cur.visible = true;

            KPADStatus kpad;
            while (KPADRead(KPADChan(i), &kpad, 1) == 1) {
                if (!kpad.posValid) {
                    // When `pos` wasn't calculated properly, hide the cursor and trail.
                    cur.visible = false;
                    cur.trail.clear();
                } else {
                    /*
                     * KPAD coordinates are normalized between -1.0 and +1.0, so we remap
                     * to the renderer's logical size.
                     */
                    cur.x = 0.5f * (kpad.pos.x + 1.0f) * width;
                    cur.y = 0.5f * (kpad.pos.y + 1.0f) * height;
                    // Calculate angle in degrees for SDL Renderer API.
                    cur.angle = 180.0
                                * std::atan2(kpad.angle.y, kpad.angle.x)
                                * std::numbers::inv_pi_v<float>;
                    // Add point to the trail.
                    cur.add_point(cur.x, cur.y);
                }
            }
        }

    }

};


std::atomic<App*> App::instance = nullptr;


int
main(int, char*[])
{
    try {
        App app;
        app.run();
    }
    catch (std::exception& e) {
        cout << "Exception: " << e.what() << endl;
    }
}
