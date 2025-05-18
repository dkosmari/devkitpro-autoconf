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

First we call `DEVKITPRO_WII_INIT` to set up paths for the cross compilation.

Later we use `DEVKITPRO_WII_SETUP` to add flags to the compilation variables.

A third-party library, `libgxflux`, is also used, with the `DEVKITPRO_WII_CHECK_LIBGXFLUX`
macro.


### [`Makefile.am`](Makefile.am)

The `Makefile.am` defines an `.elf` program, that will be converted to a `.dol` later.

A `run:` target allows us to launch the app using `wiiload`.

The recipe to convert the `.elf` into a `.dol` comes the line `@INC_AMINCLUDE@`.
