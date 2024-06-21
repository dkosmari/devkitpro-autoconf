EXTRA_DIST = \
	bootstrap \
	README.md

# Use all DEVKITPRO_* variables.
AM_CPPFLAGS = $(DEVKITPRO_CPPFLAGS)
AM_CXXFLAGS = $(DEVKITPRO_CXXFLAGS)
AM_LDFLAGS = $(DEVKITPRO_LDFLAGS)
LIBS = $(DEVKITPRO_LIBS)

# We build a .elf and then convert it to .dol
DOL_FILE = hello-world.dol
noinst_PROGRAMS = hello-world.elf

hello_world_elf_SOURCES = src/main.cpp

all-local: $(DOL_FILE)


# Convenience target to run it on the console.
.PHONY: run
run: $(DOL_FILE)
	WIILOAD=/dev/ttyUSB0 wiiload $<


# Import the extra Makefile rules.
@INC_AMINCLUDE@
