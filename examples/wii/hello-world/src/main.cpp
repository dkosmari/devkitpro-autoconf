#include <cstdio>

#include <gccore.h>
#include <wiiuse/wpad.h>


int main()
{
    VIDEO_Init();
    WPAD_Init();

    GXRModeObj* mode = VIDEO_GetPreferredMode(nullptr);
    void* fb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(mode));

    CON_Init(fb,
             20, 20,
             mode->fbWidth, mode->xfbHeight,
             mode->fbWidth * VI_DISPLAY_PIX_SZ);

    VIDEO_Configure(mode);
    VIDEO_SetNextFramebuffer(fb);
    VIDEO_SetBlack(false);
    VIDEO_Flush();
    VIDEO_WaitVSync();

    if (mode->viTVMode & VI_NON_INTERLACE)
        VIDEO_WaitVSync();

    std::printf("Hello World!\n");

    bool running = true;
    while (running) {
        WPAD_ScanPads();
        u32 pressed = WPAD_ButtonsDown(0);
        if (pressed & WPAD_BUTTON_HOME)
            running = false;

        VIDEO_WaitVSync();
    }
}
