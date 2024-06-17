#include <vpad/input.h>
#include <whb/log.h>
#include <whb/log_console.h>
#include <whb/proc.h>


int main()
{
    WHBProcInit();
    WHBLogConsoleInit();
    WHBLogConsoleSetColor(0x1024000);

    WHBLogPrint("Hello World!\n");
    WHBLogPrint("Press a button:");

    while (WHBProcIsRunning()) {
        bool something_happened = false;
        VPADStatus buf;
        int r = VPADRead(VPAD_CHAN_0, &buf, 1, nullptr);
        if (r == 1) {
            if (buf.trigger) {
                WHBLogPrintf("pressed  = %08x\n", buf.trigger);
                something_happened = true;
            }
            if (buf.release) {
                WHBLogPrintf("released = %08x\n", buf.release);
                something_happened = true;
            }
            if (something_happened)
                WHBLogPrint("Press a button:");
        }

        WHBLogConsoleDraw();
    }

    WHBLogConsoleFree();
    WHBProcShutdown();
}
