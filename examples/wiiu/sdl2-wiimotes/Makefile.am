EXTRA_DIST = \
	assets \
	bootstrap \
	README.md


RPX_FILE = sdl2-wiimotes.rpx

WUHB_FILE = $(RPX_FILE:.rpx=.wuhb)



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


noinst_PROGRAMS = sdl2-wiimotes.elf

sdl2_wiimotes_elf_SOURCES = \
	src/main.cpp


$(WUHB_FILE): 	$(RPX_FILE) \
		assets/icon.png \
		assets/splash-tv.png \
		assets/splash-drc.png
	$(WUHBTOOL) \
		$(RPX_FILE) $@ \
		--content=assets/content \
		--name="$(PACKAGE_STRING)" \
		--short-name="$(PACKAGE_TARNAME)" \
		--icon=assets/icon.png \
		--tv-image=assets/splash-tv.png \
		--drc-image=assets/splash-drc.png


all-local: $(WUHB_FILE)


.PHONY: run
run: $(WUHB_FILE)
	WIILOAD=tcp:wiiu wiiload $<


install-exec-local: $(WUHB_FILE)
	curl "ftp://wiiu:/fs/vol/external01/wiiu/apps/" --upload-file $<


uninstall-local:
	curl "ftp://wiiu" --quote "DELE /fs/vol/external01/wiiu/apps/$(WUHB_FILE)"



DISTCLEANFILES = $(AMINCLUDE)

CLEANFILES = $(WUHB_FILE)

@INC_AMINCLUDE@



.PHONY: company
company: compile_flags.txt

compile_flags.txt: Makefile
	printf "%s" "$(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS)" | xargs -n1 | sort -u > $(srcdir)/compile_flags.txt
	$(CPP) -xc++ /dev/null -E -Wp,-v 2>&1 | sed -n 's,^ ,-I,p' >> $(srcdir)/compile_flags.txt
