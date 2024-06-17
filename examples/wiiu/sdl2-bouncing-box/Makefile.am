EXTRA_DIST = \
	assets \
	bootstrap \
	README.md

SUBDIRS = src


DISTCLEANFILES = $(AMINCLUDE)

# build a .wuhb
WUHB_TARGET = sdl2-bouncing-box.wuhb
CLEANFILES = $(WUHB_TARGET)
$(WUHB_TARGET): src/sdl2-bouncing-box.rpx
	$(WUHBTOOL) \
		$< $@ \
		--content=assets \
		--name="$(PACKAGE_STRING)" \
		--short-name="$(PACKAGE_TARNAME)"

# Make sure to build the .wuhb for the `all' target.
all-local: $(WUHB_TARGET)

# Also use the `run' target to run it.
run-local: $(WUHB_TARGET)
	WIILOAD=tcp:wiiu wiiload $<

