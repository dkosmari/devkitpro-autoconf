#include <algorithm>
#include <cstdlib>
#include <vector>

#include <SDL.h>

#include <whb/log.h>
#include <whb/log_udp.h>


int main()
{
    SDL_Window* win = nullptr;
    SDL_Renderer* ren = nullptr;
    int status = 0;

    constexpr int screen_width = 854;
    constexpr int screen_height = 480;

    WHBLogUdpInit();
    std::atexit(reinterpret_cast<void (*)()>(WHBLogUdpDeinit));


    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        WHBLogPrintf("Failed to init SDL: %s\n", SDL_GetError());
        return -1;
    }
    std::atexit(SDL_Quit);

    win = SDL_CreateWindow("Bouncing Box",
                           SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                           screen_width, screen_height,
                           SDL_WINDOW_FULLSCREEN);
    if (!win) {
        WHBLogPrintf("Failed to create window: %s\n", SDL_GetError());
        status = -2;
        goto error_final;
    }

    ren = SDL_CreateRenderer(win, -1,
                             SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!ren) {
        WHBLogPrintf("Failed to create renderer: %s\n", SDL_GetError());
        status = -3;
        goto error_win;
    }


    {
        const float size = 50;
        const float max_x = screen_width;
        const float max_y = screen_height;
        float px = 0;
        float py = 0;
        float vx = 1;
        float vy = 1;

        SDL_Rect box{};
        box.w = size;
        box.h = size;

        SDL_RenderSetLogicalSize(ren, screen_width, screen_height);

        bool running = true;
        while (running) {
            SDL_Event e;
            while (SDL_PollEvent(&e)) {
                switch (e.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                }
            }

            px += vx;
            py += vy;
            if ((px + size >= max_x && vx > 0)
                || (px <= 0 && vx < 0))
                vx = -vx;
            if ((py + size >= max_y && vy > 0)
                || (py <= 0 && vy < 0))
                vy = -vy;
            // TODO: add some randomness to vx/vy after every bounce

            SDL_SetRenderDrawColor(ren, 0, 0, 40, 255);
            SDL_RenderClear(ren);

            box.x = px;
            box.y = py;
            // TODO: randomly change box color after every bounce
            SDL_SetRenderDrawColor(ren, 255, 100, 0, 255);
            SDL_RenderFillRect(ren, &box);

            SDL_RenderPresent(ren);
        }
    }

    SDL_DestroyRenderer(ren);
 error_win:
    SDL_DestroyWindow(win);
 error_final:

    return status;
}
