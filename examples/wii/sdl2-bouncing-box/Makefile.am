EXTRA_DIST = \
	bootstrap \
	README.md


# Use the DEVKITPRO_* and SDL2_* variables.
AM_CPPFLAGS = $(SDL2_CFLAGS) $(DEVKITPRO_CPPFLAGS)
AM_CXXFLAGS = $(DEVKITPRO_CXXFLAGS)
AM_LDFLAGS = $(DEVKITPRO_LDFLAGS)
LIBS = $(SDL2_LIBS) $(DEVKITPRO_LIBS)


# Create a .elf executable, then convert it to .dol using the all-local target.
DOL_FILE = sdl2-bouncing-box.dol
noinst_PROGRAMS = sdl2-bouncing-box.elf

sdl2_bouncing_box_elf_SOURCES = \
	src/main.cpp \
	src/show.cpp src/show.hpp

all-local: $(DOL_FILE)


# Import the extra rules into the Makefile.
@INC_AMINCLUDE@
# Clean up "aminclude.am" during "make distclean".
DISTCLEANFILES = $(AMINCLIDE)


# Use "make run" to execute the program on the Wii, using USB Gecko.
.PHONY: run
run: $(DOL_FILE)
	WIILOAD=/dev/ttyUSB0 wiiload $<

