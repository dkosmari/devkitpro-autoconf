# Hello World example for Wii

This simple example shows how to build a "Hello World" application for the Wii using
Automake.


## Build instructions

1. `./bootstrap`

2. `./configure --host=powerpc-eabi`

3. `make`


## Explanation

### [`bootstrap`](bootstrap)

The `bootstrap` script makes sure the macros are available without needing them
to be installed in the system.

### [`configure.ac`](configure.ac)

We use the `DEVKITPRO_WII_INIT` macro, that must appear before `AM_INIT_AUTOMAKE`.

A third-party library, `libgxflux`, is also used, with the `DEVKITPRO_WII_CHECK_LIBGXFLUX`
macro.


### [`Makefile.am`](Makefile.am)

The `Makefile.am` uses all of the `DEVKITPRO_*` variables in the corresponding Automake
variables.

The output is a `.elf` file that is converted to a `.dol` file in the `all-local` target.

Extra Makefile rules are imported by adding the line `@INC_AMINCLUDE@` to the Makefile.
