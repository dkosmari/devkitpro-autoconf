EXTRA_DIST = \
	bootstrap \
	README.md

# We know that $(DEVKITPRO_WII_LIBGXFLUX_LIBS) expands to -lgxflux
LDADD = -lgxflux

# We build a .elf and then convert it to .dol
DOL_FILE = hello-world.dol
noinst_PROGRAMS = hello-world.elf

hello_world_elf_SOURCES = src/main.cpp

all-local: $(DOL_FILE)


# Convenience target to run it on the console.
.PHONY: run
run: $(DOL_FILE)
	WIILOAD=tcp:wii wiiload $<

# Import the extra Makefile rules.
@INC_AMINCLUDE@
