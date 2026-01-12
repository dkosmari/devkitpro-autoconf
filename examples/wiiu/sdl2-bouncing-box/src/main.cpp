#include <iostream>

#include <SDL.h>
#include <SDL_mixer.h>

#include "dump_sdl_event.hpp"


using std::cout;
using std::endl;


constexpr int screen_width = 1280;
constexpr int screen_height = 720;

SDL_Window* window;
SDL_Renderer* renderer;
Mix_Chunk* bonk;


void
run_main_loop()
{
    const float size = 64;
    const float max_x = screen_width;
    const float max_y = screen_height;

    SDL_FRect box{
        .x = 0,
        .y = 0,
        .w = size,
        .h = size,
    };

    // TODO: randomize initial velocity.
    float vel_x = 4;
    float vel_y = 4;

    SDL_RenderSetLogicalSize(renderer, screen_width, screen_height);

    bool running = true;
    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {

            dump_sdl_event(e);

            switch (e.type) {
                case SDL_QUIT:
                    cout << "quit requested" << endl;
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
                        // Note: gamepad touch screen only generates events after it's open.
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
            return;

        box.x += vel_x;
        box.y += vel_y;

        bool bounced = false;
        if ((box.x + box.w >= max_x && vel_x > 0)
            || (box.x <= 0 && vel_x < 0)) {
            vel_x = -vel_x;
            bounced = true;
        }
        if ((box.y + box.h >= max_y && vel_y > 0)
            || (box.y <= 0 && vel_y < 0)) {
            vel_y = -vel_y;
            bounced = true;
        }

        if (bounced) {
            if (bonk)
                Mix_PlayChannel(-1, bonk, 0);
            cout << "bonk" << endl;
        }

        // TODO: add some randomness to vel_x/vel_y after every bounce

        SDL_SetRenderDrawColor(renderer, 0, 0, 40, 255);
        SDL_RenderClear(renderer);

        // TODO: randomly change box color after every bounce
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_RenderFillRectF(renderer, &box);

        SDL_RenderPresent(renderer);
    }
}


int
main()
{
    int status = 0;

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

    window = SDL_CreateWindow("Bouncing Box",
                              SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              screen_width, screen_height,
                              SDL_WINDOW_FULLSCREEN);
    if (!window) {
        cout << "Failed to create window: " << SDL_GetError() << endl;
        status = -3;
        goto error_close_mixer;
    }

    renderer = SDL_CreateRenderer(window, -1,
                                  SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        cout << "Failed to create renderer: " << SDL_GetError() << endl;
        status = -4;
        goto error_destroy_window;
    }

    // Load asset from the .wuhb
    bonk = Mix_LoadWAV("/vol/content/bonk.wav");
    if (!bonk)
        cout << "Failed to load bonk.wav: " << Mix_GetError() << endl;

    run_main_loop();

    cout << "Shutting down" << endl;

    Mix_FreeChunk(bonk); // bonk might be null, but it's harmless
    SDL_DestroyRenderer(renderer);
 error_destroy_window:
    SDL_DestroyWindow(window);
 error_close_mixer:
    Mix_CloseAudio();
 error_quit_sdl:
    Mix_Quit();
    SDL_Quit();
 error_end_program:
    return status;
}
