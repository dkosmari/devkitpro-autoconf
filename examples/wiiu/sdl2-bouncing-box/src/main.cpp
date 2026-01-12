#include <iostream>

#include <SDL.h>
#include <SDL_mixer.h>

#include "dump_sdl_event.hpp"


using std::cout;
using std::endl;


int main()
{
    SDL_Window* win = nullptr;
    SDL_Renderer* ren = nullptr;
    Mix_Chunk* bonk = nullptr;
    int status = 0;

    constexpr int screen_width = 1280;
    constexpr int screen_height = 720;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_AUDIO) < 0) {
        cout << "Failed to init SDL: " << SDL_GetError() << endl;
        status = -1;
        goto error_end_program;
    }

    Mix_Init(0);

    if (Mix_OpenAudio(48000, AUDIO_S16SYS, 2, 4096) == -1) {
        cout << "Failed to open audio: " << SDL_GetError() << endl;
        status = -2;
        goto error_quit_sdl;
    }


    win = SDL_CreateWindow("Bouncing Box",
                           SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                           screen_width, screen_height,
                           SDL_WINDOW_FULLSCREEN);
    if (!win) {
        cout << "Failed to create window: " << SDL_GetError() << endl;
        status = -3;
        goto error_close_mixer;
    }

    ren = SDL_CreateRenderer(win, -1,
                             SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!ren) {
        cout << "Failed to create renderer: " << SDL_GetError() << endl;
        status = -4;
        goto error_destroy_window;
    }

    // Load asset from the .wuhb
    bonk = Mix_LoadWAV("/vol/content/bonk.wav");
    if (!bonk)
        cout << "Failed to load bonk.wav: " << Mix_GetError() << endl;


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
                dump_sdl_event(e);
                switch (e.type) {
                    case SDL_QUIT:
                        cout << "Handling SDL_QUIT" << endl;
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

                    case SDL_KEYDOWN:
                        switch (e.key.keysym.sym) {
                            case SDLK_q:
                                // quit app with CTRL+Q
                                if (e.key.keysym.mod & KMOD_CTRL)
                                    running = false;
                                break;

                            case SDLK_F4:
                                // quit app with ALT+F4
                                if (e.key.keysym.mod & KMOD_ALT)
                                    running = false;
                                break;

                            case SDLK_ESCAPE:
                                // quit app with ESC
                                running = false;
                                break;

                        }
                        break;

                    case SDL_CONTROLLERDEVICEADDED:
                        {
                            auto c = SDL_GameControllerOpen(e.cdevice.which);
                            cout << "Added controller: " << SDL_GameControllerName(c) << endl;
                        }
                        break;
                    case SDL_CONTROLLERDEVICEREMOVED:
                        {
                            auto c = SDL_GameControllerFromInstanceID(e.cdevice.which);
                            cout << "Removed controller: " << SDL_GameControllerName(c) << endl;
                            SDL_GameControllerClose(c);
                        }
                        break;
                }
            }

            if (!running)
                break;

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

            if (bounced) {
                if (bonk)
                    Mix_PlayChannel(-1, bonk, 0);
                cout << "bonk" << endl;
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

    cout << "Shutting down" << endl;

    Mix_FreeChunk(bonk); // bonk might be null, but it's harmless
    SDL_DestroyRenderer(ren);
 error_destroy_window:
    SDL_DestroyWindow(win);
 error_close_mixer:
    Mix_CloseAudio();
 error_quit_sdl:
    Mix_Quit();
    SDL_Quit();
 error_end_program:
    return status;
}
