# Hello World example for 3DS

This simple example shows how to build a "Hello World" application for the 3DS using
Automake.


## Build instructions

1. `./bootstrap`

2. `./configure --host=arm-none-eabi`

3. `make`

4. (Optional) `make run`


## Explanation

### [`bootstrap`](bootstrap)

The `bootstrap` script makes sure the macros are available without needing them
to be installed in the system.


### [`configure.ac`](configure.ac)

First we call `DEVKITPRO_3DS_INIT` to set up paths for the cross compilation.

Later we use `DEVKITPRO_3DS_SETUP` to add compilation flags.


### [`Makefile.am`](Makefile.am)

The program is built as an `.elf` file, that is later converted to `.3dsx`.

The recipe that does this conversion is imported in the line `@INC_AMINCLUDE@`.

A phony `run` target can be used to call `3dslink` to execute the app remotely on a 3DS
(named "3ds" in the local network). The 3DS must have the "Homebrew Launcher" app running,
in "3dslink mode" (press Y inside the Homebrew Launcher) for this to work.
