EXTRA_DIST = \
	bootstrap \
	README.md


AM_CPPFLAGS = \
	$(SDL2_CFLAGS) \
	$(DEVKITPRO_CPPFLAGS)
AM_CXXFLAGS = $(DEVKITPRO_CXXFLAGS)
AM_LDFLAGS = $(DEVKITPRO_LDFLAGS)
LIBS = \
	$(SDL2_LIBS) \
	$(DEVKITPRO_LIBS)


DOL_FILE = sdl2-bouncing-box.dol
noinst_PROGRAMS = sdl2-bouncing-box.elf

sdl2_bouncing_box_elf_SOURCES = \
	src/main.cpp \
	src/show.cpp src/show.hpp

all-local: $(DOL_FILE)


@INC_AMINCLUDE@
DISTCLEANFILES = $(AMINCLIDE)


.PHONY: run
run: $(DOL_FILE)
	WIILOAD=/dev/ttyUSB0 wiiload $<

