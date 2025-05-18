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

`DEVKITPRO_WII_INIT` is used to set up paths for the cross compilation.

`DEVKITPRO_WII_SETUP` is used later to add compilation flags to the usual variables.

The `PKG_CHECK_MODULES` is used to look up SDL2 using `pkg-config`. Note that its flags
end up in `SDL2_CFLAGS` and `SDL2_LIBS`.


### [`Makefile.am`](Makefile.am)

In order to incorporate the `SDL_` variables, we add them to `AM_CPPFLAGS` and `LDADD`.

The program is built as an `.elf` file, that is later converted to `.dol`. The recipe that
does this conversion is imported using the line `@INC_AMINCLUDE@`.

A `run:` target is used to launch the app on the Wii using `wiiload`.
