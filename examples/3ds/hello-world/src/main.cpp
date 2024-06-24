#include <cstdio>

#include <3ds.h>


int main()
{
    gspInit();
    gfxInitDefault();
    aptInit();
    hidInit();
    consoleInit(GFX_TOP, nullptr);

    std::printf("Hello World!\n");
    std::printf("Press Start to exit.\n");

    while (aptMainLoop()) {
        hidScanInput();
        u32 keys_down = hidKeysDown();
        if (keys_down) {
            if (keys_down & KEY_START)
                break;
            std::printf("Press Start to exit.\n");
        }

        gfxSwapBuffers();
        gspWaitForVBlank();
    }

    hidExit();
    aptExit();
    gfxExit();
    gspExit();
}
