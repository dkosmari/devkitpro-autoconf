# SDL2 Bouncing Box example for the Wii

This example shows how to build a SDL2 application for the Wii.


## Build instructions

1. `./bootstrap`

2. `./configure --host=powerpc-eabi`

3. `make`

4. (Optional) `make run`


## Explanation

### [`bootstrap`](bootstrap)

The `bootstrap` script makes sure the macros are available without needing them to be
installed in the system.

### [`configure.ac`](configure.ac)

First, call the `DEVKITPRO_WII_INIT` to set up the environment for Wii homebrew. This
adjusts the `PATH` variable if necessary, and defines various `DEKVITPRO_*` variables to
be used in Makefiles.

The `PKG_CHECK_MODULES` is used to look up SDL2 using `pkg-config`. Note that its flags
end up in `SDL2_CFLAGS` and `SDL2_LIBS`.

### [`Makefile.am`](Makefile.am)

The `Makefile.am` is where we use all the `DEVKITPRO_*` variables; in addition, we use the
`SDL2_*` variables.

We want to make a `.dol` file, so we first create a `.elf` binary, and request the `.dol`
binary (with the same name) to be generated. The rules to make this conversion work are
imported with the line `@INC_AMINCLUDE@`.

Lastly, an optional rule is created, to make it more convenient to run the program on the
Wii (as long as the Homebrew Channel is running), using the `wiiload` utility from
devkitPro.
