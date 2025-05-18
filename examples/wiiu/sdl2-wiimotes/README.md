# SDL Wiimotes example for the Wii U

This example uses `SDL2`, but with the KPAD API for direct access to wiimotes.

It uses `SDL2_ttf` to load the system font, and render the hand cursors to textures.


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

 - `DEVKITPRO_WUT_INIT` is called as early as possible, to set up paths for the cross
    compilation tools.
 - `DEVKITPRO_WUT_SETUP` is called to add flags to the compilation variables.
 
 - `PKG_CHECK_MODULES` is used to look for all SDL libraries.


### [`Makefile.am`](Makefile.am)

The `SDL2_` flags are added to `AM_CPPFLAGS` and `LDADD`.

A `.wuhb` target is created, to pack both the `.rpx` executable and the assets.

A `run:` target is created for convenience, to execute the program using `wiiload`.

Targets to install/uninstall are also created, using curl to upload/delete the file
through FTP.

Extra Makefile recipes are imported using `@INC_AMINCLUDE@`.
