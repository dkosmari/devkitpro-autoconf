#include <vpad/input.h>
#include <whb/log.h>
#include <whb/log_console.h>
#include <whb/log_module.h>
#include <whb/proc.h>

int main()
{
    WHBProcInit();
    WHBLogModuleInit();
    WHBLogConsoleInit();
    WHBLogConsoleSetColor(0x10'20'40'00);

    WHBLogPrint("Hello World!");
    WHBLogPrint("Press a button.");

    while (WHBProcIsRunning()) {
        bool something_happened = false;
        VPADStatus buf;
        if (VPADRead(VPAD_CHAN_0, &buf, 1, nullptr) == 1) {
            if (buf.trigger) {
                WHBLogPrintf("pressed  = %08x", buf.trigger);
                something_happened = true;
            }
            if (buf.release) {
                WHBLogPrintf("released = %08x", buf.release);
                something_happened = true;
            }
            if (something_happened)
                WHBLogPrint("Press another button.");
        }

        WHBLogConsoleDraw();
    }

    WHBLogConsoleFree();
    WHBLogModuleDeinit();
    WHBProcShutdown();
}
