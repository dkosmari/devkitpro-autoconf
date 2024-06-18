#include <SDL.h>
#include <SDL_mixer.h>

#include <sysapp/launch.h>
#include <whb/log.h>
#include <whb/log_udp.h>

#include "show.hpp"


int main()
{
    SDL_Window* win = nullptr;
    SDL_Renderer* ren = nullptr;
    Mix_Chunk* bonk = nullptr;
    int status = 0;

    constexpr int screen_width = 854;
    constexpr int screen_height = 480;

    WHBLogUdpInit();


    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_AUDIO) < 0) {
        WHBLogPrintf("Failed to init SDL: %s\n", SDL_GetError());
        status = -1;
        goto error_stop_log_udp;
    }

    Mix_Init(0);

    if (Mix_OpenAudio(48000, AUDIO_S16SYS, 2, 4096) == -1) {
        WHBLogPrintf("Failed to open audio: %s\n", SDL_GetError());
        status = -2;
        goto error_quit_sdl;
    }


    win = SDL_CreateWindow("Bouncing Box",
                           SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                           screen_width, screen_height,
                           SDL_WINDOW_FULLSCREEN);
    if (!win) {
        WHBLogPrintf("Failed to create window: %s\n", SDL_GetError());
        status = -3;
        goto error_close_audio;
    }

    ren = SDL_CreateRenderer(win, -1,
                             SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!ren) {
        WHBLogPrintf("Failed to create renderer: %s\n", SDL_GetError());
        status = -4;
        goto error_destroy_window;
    }

    // Load asset from the .wuhb
    bonk = Mix_LoadWAV("/vol/content/bonk.wav");
    if (!bonk) {
        WHBLogPrintf("error loading bonk.wav: %s\n", Mix_GetError());
        goto error_destroy_renderer;
    }


    {
        const float size = 50;
        const float max_x = screen_width;
        const float max_y = screen_height;
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
                    WHBLogPrintf("got SDL_QUIT\n");
                    running = false;
                    //goto out_of_main_loop;
                    break;
                case SDL_CONTROLLERBUTTONDOWN:
#if 1
                    switch (e.cbutton.button) {
                    case SDL_CONTROLLER_BUTTON_B:
                    case SDL_CONTROLLER_BUTTON_START:
                        SYSLaunchMenu(); // causes a SDL_QUIT to be sent.
                        break;
                    }
#endif
                    break;
                case SDL_CONTROLLERDEVICEADDED:
                    {
                        auto c = SDL_GameControllerOpen(e.cdevice.which);
                        WHBLogPrintf("Added controller %d: %s\n",
                                     e.cdevice.which,
                                     SDL_GameControllerName(c));
                    }
                    break;
                case SDL_CONTROLLERDEVICEREMOVED:
                    {
                        auto c = SDL_GameControllerFromInstanceID(e.cdevice.which);
                        WHBLogPrintf("Removed controller: %s\n",
                                     SDL_GameControllerName(c));
                        SDL_GameControllerClose(c);
                    }
                    break;
                }
            }

            px += vx;
            py += vy;
            bool bounced = false;
            if ((px + size >= max_x && vx > 0)
                || (px <= 0 && vx < 0)) {
                vx = -vx;
                bounced = true;
            }
            if ((py + size >= max_y && vy > 0)
                || (py <= 0 && vy < 0)) {
                vy = -vy;
                bounced = true;
            }

            if (bounced)
                Mix_PlayChannel(-1, bonk, 0);

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

 // out_of_main_loop:
    WHBLogPrintf("Shutting down\n");

    Mix_FreeChunk(bonk);
 error_destroy_renderer:
    SDL_DestroyRenderer(ren);
 error_destroy_window:
    SDL_DestroyWindow(win);
 error_close_audio:
    Mix_CloseAudio();
 error_quit_sdl:
    Mix_Quit();
    SDL_Quit();
 error_stop_log_udp:
    WHBLogUdpDeinit();

    return status;
}
