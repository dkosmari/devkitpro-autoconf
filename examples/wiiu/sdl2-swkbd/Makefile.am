EXTRA_DIST = \
	bootstrap

WUHB_FILE = sdl2-swkbd.wuhb
RPX_FILE = sdl2-swkbd.rpx


AM_CPPFLAGS = $(SDL2_CFLAGS)

AM_CXXFLAGS = -Wall -Wextra -Werror

LDADD = $(SDL2_LIBS)


noinst_PROGRAMS = sdl2-swkbd.elf

sdl2_swkbd_elf_SOURCES =			\
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


run: $(WUHB_FILE)
	WIILOAD=tcp:wiiu wiiload $<



DISTCLEANFILES = $(AMINCLUDE)

CLEANFILES = $(WUHB_FILE) $(RPX_FILE)

PHONY = company

company: compile_flags.txt

compile_flags.txt: Makefile
	printf "%s" "$(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS)" | xargs -n1 | sort -u > $(srcdir)/compile_flags.txt
	$(CPP) -xc++ /dev/null -E -Wp,-v 2>&1 | sed -n 's,^ ,-I,p' >> $(srcdir)/compile_flags.txt


@INC_AMINCLUDE@

