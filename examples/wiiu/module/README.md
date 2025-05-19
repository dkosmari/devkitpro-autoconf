# WUMS module example for the Wii U

This example shows how to build a [WUMS](https://github.com/wiiu-env/WiiUModuleSystem)
(Wii U Module System) module for the Wii U.


## Build instructions

1. `./bootstrap`

2. `./configure --host=powerpc-eabi`

3. `make`

Then copy the `dummy.wms` module to the Aroma modules folder in the SD card. If your Wii U
is named `wiiu` in your local network, and running `ftpiiu`, you can run `make install` to
upload the file using curl; similarly, `make uninstall` will delete the remote file.


## Explanation

### [`bootstrap`](bootstrap)

The `bootstrap` script makes sure the macros are available without needing
them to be installed in the system.


### [`configure.ac`](configure.ac)

We use two macros:

  - `WIIU_WUMS_MODULE_INIT`: this sets up variables to ensure we use the correct
     compiler/tools from the devkitPro environment, and not from the native environment.

  - `WIIU_WUMS_MODULE_SETUP`: this sets up compilation flags needed to build modules.


### `Makefile.am`

The [`Makefile.am`](Makefile.am) has a "dummy" module, that's first compiled as an `.elf`
file. Despite the module being a library (it has no `main()` function) we still use the
`_PROGRAMS` primary to create a `.elf` file, and then convert it to a `.wms` file, later
in the `all-local` target. The `noinst_` prefix ensures the `.elf` is not installed on
`make install`.

To import the extra Makefile rules (that convert the `.elf` into `.wms`), we add
`@INC_AMINCLUDE@`.

As an extra, we can use `install-exec-local` and `uninstall-local` targets to
upload/delete the module through FTP.
