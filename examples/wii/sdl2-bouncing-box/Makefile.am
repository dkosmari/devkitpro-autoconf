EXTRA_DIST = \
	bootstrap \
	README.md


AM_CPPFLAGS = \
	$(SDL2_CFLAGS)

AM_CXXFLAGS = -Wall -Wextra -Werror

LDADD = $(SDL2_LIBS)


# Create a .elf executable, then convert it to .dol using the all-local target.
DOL_FILE = sdl2-bouncing-box.dol
noinst_PROGRAMS = sdl2-bouncing-box.elf

sdl2_bouncing_box_elf_SOURCES = \
	src/main.cpp \
	src/dump_sdl_event.cpp \
	src/dump_sdl_event.hpp \
	src/wii-stdout.cpp

SUFFIXES = .wav

.wav.o:
	$(LD) --relocatable --format binary $< -o $@


if ASSETS
AM_CPPFLAGS += -DASSETS
sdl2_bouncing_box_elf_SOURCES += assets/bonk.wav
endif ASSETS


all-local: $(DOL_FILE)


# Import the extra rules into the Makefile.
@INC_AMINCLUDE@

# Clean up "aminclude.am" during "make distclean".
DISTCLEANFILES = $(AMINCLIDE)


# Use "make run" to execute the program on the Wii, using USB Gecko.
.PHONY: run

WIILOAD ?= tcp:wii
run: $(DOL_FILE)
	WIILOAD=$(WIILOAD) wiiload $<

