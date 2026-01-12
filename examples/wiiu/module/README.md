# WUMS module example for the Wii U

This example shows how to build a [WUMS](https://github.com/wiiu-env/WiiUModuleSystem)
(Wii U Module System) module for the Wii U. We use WUMS itself as an external submodule,
to avoid having to install it manually into the build system; make sure the repository was
cloned/pulled with `--recurse-submodules`.


## Build instructions

1. `./bootstrap`

2. `./configure --host=powerpc-eabi`

3. `make`

Then copy the `dummy.wms` module to the Aroma modules folder in the SD card. If your Wii U
is named `wiiu` in your local network, and running `ftpiiu`, you can run `make
wiiu-install` to upload the file using curl; similarly, `make wiiu-uninstall` will delete
the remote file.


## Explanation

### [`bootstrap`](bootstrap)

The `bootstrap` script makes sure the macros are available without needing
them to be installed in the system.


### [`configure.ac`](configure.ac)

We use two macros:

  - `DEVKITPRO_WUT_INIT`: this sets up variables to ensure we use the correct
     compiler/tools from the devkitPro environment, and not from the native environment.

  - `WIIU_ENV_SETUP_WUMS`: this sets up the use of WUMS. We use the output variables from
    this macro to build and use WUMS in our module.


### [`Makefile.am`](Makefile.am)

The `SUBDIRS` variable ensures we build WUMS before building our module; we clear
`DIST_SUBDIRS` to prevent Automake from invoking special targets (such as `distclean`)
into WUMS, since it's not an Automake project.

`AM_CPPFLAGS`, `AM_LDFLAGS` and `LDADD` are all set up using the output variables from the
`WIIU_ENV_SETUP_WUMS` macro.

The module itself, called `dummy`, is built as a `.elf` file. Despite the module being a
library (it has no `main()` function) we still use the `_PROGRAMS` primary to create a
`.elf` file, and then convert it to a `.wms` file, later in the `all-local` target. The
`noinst_` prefix ensures the `.elf` is not installed during `make install`.

Due to some Automake limitations, we have to manually list the WUMS library as a
dependency for our module; otherwise, the build would only work for the `make all` target,
but not when doing `make dummy.wms`.

As an extra, we can use `wiiu-install` and `wiiu-uninstall` targets to upload/delete the
module via FTP, using `curl`.

To import the extra `Makefile` rules (that convert the `.elf` into `.wms`), we add
`@INC_AMINCLUDE@`.

