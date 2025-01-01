# SDL Wiimotes example for the Wii U

This example uses SDL2, but with the KPAD API for direct access to wiimotes.

It uses SDL2_ttf to load the system font, and render the hand cursors to textures.


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

`PKG_CHECK_MODULES` is used to look for all SDL libraries.


### [`Makefile.am`](Makefile.am)

A single `Makefile.am` is used here. Both `SDL_*` and `DEVKITPRO_*` variables must be used
for compilation.


