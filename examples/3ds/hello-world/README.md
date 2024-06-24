# Hello World example for 3DS

This simple example shows how to build a "Hello World" application for the 3DS using
Automake.


## Build instructions

1. `./bootstrap`

2. `./configure --host=powerpc-eabi`

3. `make`

4. (Optional) `make run`


## Explanation

### [`bootstrap`](bootstrap)

The `bootstrap` script makes sure the macros are available without needing them
to be installed in the system.

### [`configure.ac`](configure.ac)

We use the `DEVKITPRO_3DS_INIT` macro, that must appear before `AM_INIT_AUTOMAKE`.

### [`Makefile.am`](Makefile.am)

The `Makefile.am` uses all of the `DEVKITPRO_*` variables in the corresponding Automake
variables.

The output is a `.elf` file that is converted to a `.3dsx` file in the `all-local` target.

Extra Makefile rules are imported by adding the line `@INC_AMINCLUDE@` to the Makefile.

A phony `run` target can be used to call `3dslink` to execute the app remotely on a 3DS
(named "3ds" in the local network). The 3DS must have the "Homebrew Launcher" app running,
in "3dslink mode" (press Y inside the Homebrew Launcher) for this to work.

