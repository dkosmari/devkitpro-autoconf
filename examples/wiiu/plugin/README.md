# WUPS plugin example for the Wii U

This example shows how to build a [WUPS](https://github.com/wiiu-env/WiiUPluginSystem)
(Wii U Plugin System) plugin for the Wii U.

The plugin uses the [libnotifications](https://github.com/wiiu-env/libnotifications)
library, as well WUPS, through submodules. That means you must have cloned the repository
using `--recurse-submodules`.

> It's better for a plugin to be self-contained, and not require manual installation of
> libraries into the build system.


## Build instructions

1. `./bootstrap`

2. `./configure --host=powerpc-eabi`

3. `make`

Then copy the `dummy.wps` plugin to the Aroma plugins folder in the SD card.

> If your Wii U is named `wiiu` in your local network and running `ftpiiu`, you can run
> `make wiiu-install` to upload the file using curl; similarly, `make wiiu-uninstall` will delete
> the remote file. Or you temporarily load the plugin (using `wiiload`) with `make run`.


## Explanation

### [`bootstrap`](bootstrap)

The `bootstrap` script makes sure the macros are available without needing them to be
installed in the system.

A normal `bootstrap` script would just invoke `autoreconf --install`.


### [`configure.ac`](configure.ac)

The first step is invoking the `DEVKITPRO_WUT_INIT` macro.

Then we set up the local copy of WUPS (in `external/WiiUPluginSystem`) to be built and
used in our project, using the `WIIU_ENV_SETUP_WUPS` macro.

> This macro generates variables that must be used in the `Makefile.am.

The `libnotifications` library is set up in a similar way, using the
`WIIU_ENV_SETUP_LIBNOTIFICATIONS` macro.

> This macro generates variables that must be used in the `Makefile.am.


### [`Makefile.am`](Makefile.am)

The external modules are listed in the `SUBDIRS` variable, to recurse the `make all` and
`make clean` targets. We override `DIST_SUBDIRS` to prevent Automake-specific targets from
recursing into them (such as `make distclean`).

The output variables for `WUPS` and `libnotification` must be used, to set up `CPPFLAGS`,
`LDFLAGS` and linked libraries.

We define a `.elf` target that will converted to `.wps` later. Despite the plugin being a
library (it has no `main()` function) we still use the `_PROGRAMS` primary to create a
`.elf` file, and then convert it to a `.wps` file in the `all-local` target. The `noinst_`
prefix ensures the `.elf` is not installed on `make install`.

Due to some Automake limitations, we have to manually convince it to build the external
libraries when building the plugin. Otherwise the build would only succeed when doing
`make install`.

As an extra, we can use `wiiu-install` and `wiiu-uninstall` targets to upload/delete the
plugin through FTP, using `curl`.

There's also a `run` target to load the plugin without installing it, using `wiiload`.

To import the extra `Makefile` rules generated in `aminclude.am`, we add
`@INC_AMINCLUDE@`.

