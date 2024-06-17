AM_CPPFLAGS = \
	$(DEVKITPRO_CPPFLAGS) \
	$(SDL2_CFLAGS)

AM_CXXFLAGS = $(DEVKITPRO_CXXFLAGS)

AM_LDFLAGS = $(DEVKITPRO_LDFLAGS)

LIBS = \
	$(SDL2_LIBS) \
	$(DEVKITPRO_LIBS)


noinst_PROGRAMS = sdl2-bouncing-box.elf

sdl2_bouncing_box_elf_SOURCES = \
	main.cpp \
	show.cpp show.hpp


# We want to create the .rpx on both `all' and `run' targets.

RPX_TARGET = sdl2-bouncing-box.rpx

all-local: $(RPX_TARGET)
run-local: $(RPX_TARGET)


@INC_AMINCLUDE@
