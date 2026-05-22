@INC_AMINCLUDE@


EXTRA_DIST = \
	assets \
	bootstrap \
	README.md


SUBDIRS = external/sdl2xx


RPX_FILE = sdl2-sound-test.rpx

WUHB_FILE = $(RPX_FILE:.rpx=.wuhb)



AM_CPPFLAGS = \
	$(SDL2_CFLAGS) \
	-I$(srcdir)/external/sdl2xx/include

AM_CXXFLAGS = \
	-Wall -Wextra -Werror

LDADD = \
	external/sdl2xx/lib/libsdl2xx_mixer.la \
	external/sdl2xx/lib/libsdl2xx_ttf.la \
	external/sdl2xx/lib/libsdl2xx.la \
	$(SDL2_LIBS)


noinst_PROGRAMS = sdl2-sound-test.elf

sdl2_sound_test_elf_SOURCES = \
	src/main.cpp \
	src/tracer.cpp \
	src/tracer.hpp \
	src/wiiu-stdout.cpp

sdl2_sound_test_elf_DEPENDENCIES = \
	external/sdl2xx/lib/libsdl2xx_mixer.la \
	external/sdl2xx/lib/libsdl2xx_ttf.la \
	external/sdl2xx/lib/libsdl2xx.la


$(WUHB_FILE): 	$(RPX_FILE)
	$(WUHBTOOL) \
		$(RPX_FILE) $@ \
		--content=assets/content \
		--name="$(PACKAGE_STRING)" \
		--short-name="$(PACKAGE_TARNAME)"


all-local: $(WUHB_FILE)


.PHONY: run
WIILOAD ?= tcp:wiiu
run: $(WUHB_FILE)
	WIILOAD=$(WIILOAD) wiiload $<


.PHONY: wiiu-install wiiu-uninstall

wiiu-install: $(WUHB_FILE)
	curl "ftp://wiiu:/fs/vol/external01/wiiu/apps/" --upload-file $<

wiiu-uninstall:
	curl "ftp://wiiu" --quote "DELE /fs/vol/external01/wiiu/apps/$(WUHB_FILE)"



DISTCLEANFILES = $(AMINCLUDE)

CLEANFILES = $(WUHB_FILE)


.PHONY: company
company: compile_flags.txt

compile_flags.txt: Makefile
	printf "%s" "$(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS)" | xargs -n1 | sort -u > $(srcdir)/compile_flags.txt
	$(CPP) -xc++ /dev/null -E -Wp,-v 2>&1 | sed -n 's,^ ,-I,p' >> $(srcdir)/compile_flags.txt
