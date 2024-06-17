AM_CPPFLAGS = \
	$(DEVKITPRO_CPPFLAGS) \
	$(SDL2_CFLAGS)

AM_CXXFLAGS = $(DEVKITPRO_CXXFLAGS)

AM_LDFLAGS = $(DEVKITPRO_LDFLAGS)

LIBS = \
	$(DEVKITPRO_LIBS) \
	$(SDL2_LIBS)



noinst_PROGRAMS = sdl2-bouncing-box.elf

sdl2_bouncing_box_elf_SOURCES = main.cpp


RPX_TARGET = sdl2-bouncing-box.rpx

$(RPX_TARGET): sdl2-bouncing-box.elf

all-local: $(RPX_TARGET)




@INC_AMINCLUDE@


run: $(RPX_TARGET)
	WIILOAD=tcp:wiiu wiiload $<


.PHONY: company

company: compile_flags.txt

compile_flags.txt: Makefile
	printf "%s" "$(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS)" | xargs -n1 | sort -u > $(srcdir)/compile_flags.txt


