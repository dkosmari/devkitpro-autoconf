EXTRA_DIST = \
	bootstrap \
	README.md


AM_CPPFLAGS = $(DEVKITPRO_CPPFLAGS)
AM_CXXFLAGS = $(DEVKITPRO_CXXFLAGS)
AM_LDFLAGS = $(DEVKITPRO_LDFLAGS)
LIBS = $(DEVKITPRO_LIBS)


3DSX_FILE = hello-world.3dsx
noinst_PROGRAMS = hello-world.elf

hello_world_elf_SOURCES = src/main.cpp

all-local: $(3DSX_FILE)


@INC_AMINCLUDE@

DISTCLEANFILES = $(AMINCLUDE)


.PHONY: run
run: $(3DSX_FILE)
	$(_3DSLINK) --address 3ds $<
