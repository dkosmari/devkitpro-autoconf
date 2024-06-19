# WUMS module example for the Wii U

This example shows how to build a [WUMS](https://github.com/wiiu-env/WiiUModuleSystem)
(Wii U Module System) module for the Wii U.


## Build instructions

1. `./bootstrap`

2. `./configure --host=powerpc-eabi`

3. `make`

Then copy the `dummy.wms` module to the Aroma modules folder in the SD card. If your Wii U
is named `wiiu` in your local network, you can run `make install` to upload the file using
curl; similarly, `make uninstall` will delete the remote file.


## Explanation

### `bootstrap`

The [`bootstrap`](bootstrap) script makes sure the macros are available without needing
them to be installed in the system.

### `configure.ac`

In the [`configure.ac`](configure.ac) we use the `WIIU_WUMS_MODULE_INIT` macro. This will
adjust the `DEVKITPRO_*` variable to generate WUMS modules.

### `Makefile.am`

The [`Makefile.am`](Makefile.am) starts with using the `DEVKITPRO_*` variables to adjust
all the compiler and linker flags.

Despite the module being a library (no `main()` function) we still use the `PROGRAMS`
primary to create a `.elf` file, and then convert it to a `.wps` file in the `all-local`
target. The `noinst_` prefix ensures the `.elf` is not installed on `make install`.

To import the extra Makefile rules, we add `@INC_AMINCLUDE@`.

As an extra, we can use `install-exec-local` and `uninstall-local` targets to
upload/delete the module on the Wii U, if the ftpiiu plugin is active.
