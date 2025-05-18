EXTRA_DIST = \
	assets \
	bootstrap \
	README.md


AM_CPPFLAGS = $(SDL2_CFLAGS)

LDADD = $(SDL2_LIBS)


noinst_PROGRAMS = sdl2-bouncing-box.elf

sdl2_bouncing_box_elf_SOURCES = \
	src/main.cpp \
	src/show.cpp \
	src/show.hpp


RPX_FILE = sdl2-bouncing-box.rpx
WUHB_FILE = sdl2-bouncing-box.wuhb


# .wuhb files are not built automatically, so they're not cleaned automatically.
# So we tell automake to clean it.
CLEANFILES = $(WUHB_FILE)

$(WUHB_FILE): $(RPX_FILE)
	$(WUHBTOOL) \
		$< $@ \
		--content=assets \
		--name="$(PACKAGE_STRING)" \
		--short-name="$(PACKAGE_TARNAME)"


all-local: $(WUHB_FILE)


# Also use the `run' target to run it.
run: $(WUHB_FILE)
	WIILOAD=tcp:wiiu wiiload $<


@INC_AMINCLUDE@


DISTCLEANFILES = $(AMINCLUDE)

