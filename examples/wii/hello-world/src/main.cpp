#include <cstdio>

#include <gccore.h>
#include <wiiuse/wpad.h>

#include <gxflux/gfx.h>
#include <gxflux/gfx_con.h>


int main()
{
    VIDEO_Init();
    WPAD_Init();

    gfx_video_init(nullptr);
    gfx_init();
    gfx_con_init(nullptr);

    std::printf("Hello World!\n");

    bool running = true;
    while (running) {
        WPAD_ScanPads();
        u32 pressed = WPAD_ButtonsDown(0);
        if (pressed) {
            if (pressed & WPAD_BUTTON_HOME) {
                running = false;
                std::printf("exiting...\n");
            } else {
                std::printf("press HOME to exit\n");
            }
        }

        gfx_frame_start();
        gfx_con_draw();
        gfx_frame_end();

        VIDEO_WaitVSync();
    }

    gfx_con_deinit();
    gfx_deinit();
    gfx_video_deinit();
}
