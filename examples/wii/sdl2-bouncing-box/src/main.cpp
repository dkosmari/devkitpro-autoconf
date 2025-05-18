#include <cstdio>               // printf()

#include <sys/iosupport.h>      // devoptab

#include <gccore.h>

#include <SDL.h>

#include "show.hpp"


// This is how we hook into stdout/stderr with newlib. Let's send it to USB Gecko.

// It's tradition to use gecko on port B, aka channel 1.
static const int gecko_channel = 1;

static
ssize_t
write_to_gecko(struct _reent *, void *, const char *buf, size_t len)
{
    return usb_sendbuffer(gecko_channel, buf, len);
}

__attribute__((__constructor__))
void init_stdio()
{
    if (!usb_isgeckoalive(gecko_channel))
        return;

    static devoptab_t my_stdio;
    my_stdio.name = "stdio";
    my_stdio.structSize = sizeof(devoptab_t);
    my_stdio.write_r = write_to_gecko;

    devoptab_list[STD_OUT] = &my_stdio;
    devoptab_list[STD_ERR] = &my_stdio;
}


// Note: SDL2 on the Wii replaces main() with SDL_main(), so we must use
// the arguments version.
int main(int, char*[])
{
    std::printf("SDL2 program started\n");

    SDL_Window* win = nullptr;
    SDL_Renderer* ren = nullptr;
    int status = 0;

    int screen_width = 640;
    int screen_height = 480;

    CONF_Init();
    if (CONF_GetAspectRatio() == CONF_ASPECT_16_9) {
        std::printf("Wide screen detected.\n");
        screen_width = 854;
    }


    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0) {
        std::fprintf(stderr, "Failed to init SDL: %s\n", SDL_GetError());
        status = -1;
        goto error_return;
    }

    win = SDL_CreateWindow("Bouncing Box",
                           SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                           screen_width, screen_height,
                           SDL_WINDOW_FULLSCREEN);
    if (!win) {
        std::fprintf(stderr, "Failed to create window: %s\n", SDL_GetError());
        status = -3;
        goto error_quit_sdl;
    }

    ren = SDL_CreateRenderer(win, -1,
                             SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!ren) {
        std::fprintf(stderr, "Failed to create renderer: %s\n", SDL_GetError());
        status = -4;
        goto error_destroy_window;
    }

    {
        const float size = 50;
        const float max_x = screen_width - size;
        const float max_y = screen_height - size;
        float px = 0;
        float py = 0;
        float vx = 3;
        float vy = 3;

        SDL_Rect box{};
        box.w = size;
        box.h = size;

        SDL_RenderSetLogicalSize(ren, screen_width, screen_height);

        bool running = true;
        while (running) {
            SDL_Event e;
            while (SDL_PollEvent(&e)) {
                show(e);
                switch (e.type) {
                case SDL_QUIT:
                    std::printf("Handling SDL_QUIT\n");
                    running = false;
                    break;
                case SDL_CONTROLLERBUTTONDOWN:
                    switch (e.cbutton.button) {
                    case SDL_CONTROLLER_BUTTON_B:
                    case SDL_CONTROLLER_BUTTON_START:
                        running = false;
                        break;
                    }
                    break;
                case SDL_CONTROLLERDEVICEADDED:
                    {
                        auto c = SDL_GameControllerOpen(e.cdevice.which);
                        std::printf("Added controller %d: %s\n",
                                    e.cdevice.which,
                                    SDL_GameControllerName(c));
                    }
                    break;
                case SDL_CONTROLLERDEVICEREMOVED:
                    {
                        auto c = SDL_GameControllerFromInstanceID(e.cdevice.which);
                        std::printf("Removed controller: %s\n",
                                    SDL_GameControllerName(c));
                        SDL_GameControllerClose(c);
                    }
                    break;
                }
            }

            px += vx;
            py += vy;
            [[maybe_unused]] bool bounced = false;
            if ((px >= max_x && vx > 0)
                || (px <= 0 && vx < 0)) {
                vx = -vx;
                bounced = true;
            }
            if ((py >= max_y && vy > 0)
                || (py <= 0 && vy < 0)) {
                vy = -vy;
                bounced = true;
            }

            // TODO: add some randomness to vx/vy after every bounce

            SDL_SetRenderDrawColor(ren, 0, 0, 40, 255);
            SDL_RenderClear(ren);

            box.x = px;
            box.y = py;
            // TODO: randomly change box color after every bounce
            SDL_SetRenderDrawColor(ren, 255, 255, 0, 255);
            SDL_RenderFillRect(ren, &box);

            SDL_RenderPresent(ren);
        }
    }

    std::printf("Shutting down\n");

    SDL_DestroyRenderer(ren);
 error_destroy_window:
    SDL_DestroyWindow(win);
 error_quit_sdl:
    SDL_Quit();
 error_return:

    return status;
}
