EXTRA_DIST = \
	bootstrap

WUHB_FILE = sdl-keyboard.wuhb
RPX_FILE = sdl-keyboard.rpx


AM_CPPFLAGS = \
	$(DEVKITPRO_CPPFLAGS) \
	$(SDL2_CFLAGS)

AM_CXXFLAGS = \
	-Wall -Wextra -Werror \
	$(DEVKITPRO_CXXFLAGS)

AM_LDFLAGS = $(DEVKITPRO_LDFLAGS)

LIBS = \
	$(SDL2_LIBS) \
	$(DEVKITPRO_LIBS)


noinst_PROGRAMS = sdl-keyboard.elf

sdl_keyboard_elf_SOURCES =			\
	src/main.cpp				\
	src/sdl.cpp src/sdl.hpp			\
	src/sdl_debug.cpp src/sdl_debug.hpp	\
	src/sdl_ttf.cpp src/sdl_ttf.hpp		\
	src/stdout.cpp


$(WUHB_FILE): $(RPX_FILE)
	$(WUHBTOOL) 					\
		$< $@					\
		--name="$(PACKAGE_STRING)"		\
		--short-name="$(PACKAGE_TARNAME)"


all-local: $(WUHB_FILE)


run: $(RPX_FILE)
	WIILOAD=tcp:wiiu wiiload $<



DISTCLEANFILES = $(AMINCLUDE)

CLEANFILES = $(WUHB_FILE) $(RPX_FILE)

PHONY = company

company: compile_flags.txt

compile_flags.txt: Makefile
	printf "%s" "$(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS)" | xargs -n1 | sort -u > $(srcdir)/compile_flags.txt
	$(CPP) -xc++ /dev/null -E -Wp,-v 2>&1 | sed -n 's,^ ,-I,p' >> $(srcdir)/compile_flags.txt

@INC_AMINCLUDE@

