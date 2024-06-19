#include <whb/log.h>
#include <whb/log_udp.h>

#include <wums.h>

WUMS_MODULE_EXPORT_NAME("dummy");
// WUMS_MODULE_AUTHOR("Daniel K. O.");
// WUMS_MODULE_VERSION(PACKAGE_VERSION);
// WUMS_MODULE_LICENSE("GPLv3");
// WUMS_MODULE_DESCRIPTION("A dummy module that does nothing.");


WUMS_INITIALIZE(args)
{
    WHBLogUdpInit();
    WHBLogPrintf("Hello! Here's the dummy module.\n");
    WHBLogUdpDeinit();
}
