#ifdef __WII__

#include <sys/iosupport.h>      // devoptab

#include <gccore.h>


// This is how we hook into stdout/stderr with newlib. Let's send it to USB Gecko.

// It's tradition to use gecko on port B, aka channel 1.
static const int gecko_channel = 1;

static
ssize_t
write_to_gecko(struct _reent *, void *, const char *buf, size_t len)
{
    if (usb_isgeckoalive(gecko_channel))
        return usb_sendbuffer_ex(gecko_channel, buf, len, 1);
    return len;
}

__attribute__((__constructor__))
void
init_stdout()
{
    static devoptab_t my_stdout;
    my_stdout.name = "stdout";
    my_stdout.structSize = sizeof(devoptab_t);
    my_stdout.write_r = write_to_gecko;

    devoptab_list[STD_OUT] = &my_stdout;
    devoptab_list[STD_ERR] = &my_stdout;
}

#endif // __WII__
