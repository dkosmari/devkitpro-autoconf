#include <whb/log.h>
#include <whb/log_udp.h>

#include <wups.h>

#include <notifications/notifications.h>


WUPS_PLUGIN_NAME("Dummy");
WUPS_PLUGIN_DESCRIPTION("This is just an example plugin, it does nothing.");
WUPS_PLUGIN_VERSION("v1.0");
WUPS_PLUGIN_AUTHOR("Daniel K. O.");
WUPS_PLUGIN_LICENSE("GPLv3");


INITIALIZE_PLUGIN()
{
    WHBLogUdpInit();
    NotificationModule_InitLibrary();

    WHBLogPrintf("Hello, from the Dummy plugin!\n");

    NotificationModule_AddInfoNotification("Dummy plugin said \"hi\"!");

    NotificationModule_DeInitLibrary();
    WHBLogUdpDeinit();
}
