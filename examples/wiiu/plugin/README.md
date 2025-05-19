# WUPS plugin example for the Wii U

This example shows how to build a [WUPS](https://github.com/wiiu-env/WiiUPluginSystem)
(Wii U Plugin System) plugin for the Wii U.

The plugin uses the [Notification Module](https://github.com/wiiu-env/NotificationModule)
through the [libnotifications](https://github.com/wiiu-env/libnotifications) library.


## Build instructions

1. `./bootstrap`

2. `./configure --host=powerpc-eabi`

3. `make`

Then copy the `dummy.wps` plugin to the Aroma plugins folder in the SD card. If your Wii U
is named `wiiu` in your local network and running `ftpiiu`, you can run `make install` to
upload the file using curl; similarly, `make uninstall` will delete the remote file.


## Explanation

### [`bootstrap`](bootstrap)

The `bootstrap` script makes sure the macros are available without needing them to be
installed in the system.


### [`configure.ac`](configure.ac)

We need both WUPS and WUMS environments
initialized, using the macros `WIIU_WUPS_INIT` and `WIIU_WUMS_INIT`.

To set up the basic compilation flags for WUPS, we call `WIIU_WUPS_SETUP`.

We're going to use the WUMS notifications module, so we also need `WIIU_WUMS_SETUP` before
calling `WIIU_WUMS_CHECK_LIBNOTIFICATIONS`. Optional libraries are not appended to `LIBS`,
they're stored in their own variables. In this case, it's
`WIIU_WUMS_LIBNOTIFICATIONS_LIBS`, which just expands to `-lnotifications`. It's up to
you, if you want to use the full variable name, or hardcode `-lnotifications`.


### `Makefile.am`

The [`Makefile.am`](Makefile.am) defines a `.elf` target that will converted to `.wps`
later. Despite the plugin being a library (it has no `main()` function) we still use the
`_PROGRAMS` primary to create a `.elf` file, and then convert it to a `.wps` file in the
`all-local` target. The `noinst_` prefix ensures the `.elf` is not installed on `make
install`.

To import the extra Makefile rules, we add `@INC_AMINCLUDE@`.

As an extra, we can use `install-exec-local` and `uninstall-local` targets to
upload/delete the plugin using FTP.
