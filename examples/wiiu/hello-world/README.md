# Hello World example for Wii U

This simple example shows how to build a "Hello World" application for the Wii U using
Automake.


## Build instructions

1. `./bootstrap`

2. `./configure --host=powerpc-eabi`

3. `make`


## Explanation

### [`bootstrap`](bootstrap)

The bootstrap script makes sure the macros are available without needing them
to be installed in the system.


### [`configure.ac`](configure.ac)

In `configure.ac` we use two macros:

  - `DEVKITPRO_WUT_INIT`: this adjust a number of variables, including `PATH`, for cross
    compilation. This saves some typing from having to later invoke the configure script
    with a bajillion arguments. It must be called *before* `AM_INIT_AUTOMAKE` and any
    tools, otherwise the wrong compiler/tool might be used by the `Makefile`.

  - `DEVKITPRO_WUT_SETUP`: this adjusts all the usual compilation flags (`CPPFLAGS`,
    `CFLAGS`, `CXXFLAGS`, `LDFLAGS`, `LIBS`) to point the WUT environment. Call this after
    `AM_INIT_AUTOMAKE`, and Automake will fill in some defaults for `CFLAGS`/`CXXFLAGS`
    (`-O2 -g`) if you don't set them to anything.


### [`Makefile.am`](Makefile.am)

The `Makefile.am` is the main focus:

  - Always produce a `.elf` program first. Automake knows how to create .elf binaries from
    source code, so we take advantage of that.
    
  - Use the `all-local` target to process the `.elf` output into a `.rpx` (and `.wuhb`) file.
  
  - Add `@INC_AMINCLUDE@` so the Makefile has the special devkitPro rules.
