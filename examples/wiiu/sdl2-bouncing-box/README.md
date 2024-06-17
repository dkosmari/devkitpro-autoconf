# SDL2 Bouncing Box example for the Wii U

This example shows how to build a SDL2 application for the Wii U. It also creates a "wuhb"
bundle, with an audio file.


## Build instructions

1. `./bootstrap`

2. `./configure --host=powerpc-eabi`

3. `make`

4. (Optional) `make run`


## Explanation

### `bootstrap`

The [`bootstrap`](bootstrap) script makes sure the macros are available without needing
them to be installed in the system.

### `configure.ac`

The main detail of [configure.ac](configure.ac) is calling `PKG_CHECK_MODULES` to look up
both SDL2 and SDL2-mixer flags, using `pkg-config`.

This time around, we are going to be using multiple recursive `Makefile.am`, so we don't
need the `subdir-objects` option. We also use the recursive `run` target, so we can use
`make run` in all Makefiles, using the `run-local` target.

### `Makefile.am`

In the top-level [`Makefile.am`](Makefile.am) we make a rule for building the `.wuhb`
file. Both `all-local` and `run-local` recursive targets are used to build it.

### `src/Makefile.am`

In [`src/Makefile.am`](src/Makefile.am) we use both `SDL2_CFLAGS` and `SDL2_LIBS`, in
addition to the `DEVKITPRO_*` variables.

We also want to build the `.rpx` for both `all-local` and `run-local` targets.

