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

In `configure.ac`, the only special line is the use of the
`DEVKITPRO_WUT_INIT` macro. It must be called before `AM_INIT_AUTOMAKE`; there's an
ordering check to make sure you don't call them out of order.

### [`Makefile.am`](Makefile.am)

The `Makefile.am` is the main focus:

  - Use flags variables where appropriate:
      - `DEVKITPRO_CFLAGS`
      - `DEVKITPRO_CPPFLAGS`
      - `DEVKITPRO_CXXFLAGS`
      - `DEVKITPRO_LDFLAGS`
      - `DEVKITPRO_LIBS`
      - `DEVKITPRO_RPL_LDFLAGS` if you're building a `.rpl`.

  - Always produce a `.elf` program first. Automake knows how to create .elf binaries from
    source code, so we take advantage of that.
    
  - Use the `all-local` target to trigger a conversion to the final binary you want.
  
  - Add `@INC_AMINCLUDE@` so the Makefile has the special devkitPro rules.
