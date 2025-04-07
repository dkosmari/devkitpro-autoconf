#include <string>

#include <sys/iosupport.h>      // devoptab_list, devoptab_t

#include <whb/log.h>
#include <whb/log_module.h>


// These functions connect STDOUT to the WHBLog*() API.

static
ssize_t
write_msg_to_whb_log(struct _reent*,
                     void*,
                     const char* buf,
                     size_t len)
    noexcept
{
    try {
        std::string msg(buf, len);
        WHBLogWrite(msg.data());
        return len;
    }
    catch (...) {
        return -1;
    }
}


__attribute__(( __constructor__ ))
void
init_stdout()
{
    WHBLogModuleInit();
    static devoptab_t dev;
    dev.name = "STDOUT";
    dev.structSize = sizeof dev;
    dev.write_r = write_msg_to_whb_log;
    devoptab_list[STD_OUT] = &dev;
}

__attribute__(( __destructor__ ))
void
fini_stdout()
{
    WHBLogModuleDeinit();
}
