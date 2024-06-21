EXTRA_DIST = \
	assets \
	bootstrap \
	README.md

SUBDIRS = src


DISTCLEANFILES = $(AMINCLUDE)

# build a .wuhb
WUHB_FILE = sdl2-bouncing-box.wuhb

# .wuhb files are not built automatically, so they're not cleaned automatically.
# So we tell automake to clean it.
CLEANFILES = $(WUHB_FILE)

$(WUHB_FILE): src/sdl2-bouncing-box.rpx all
	$(WUHBTOOL) \
		$< $@ \
		--content=assets \
		--name="$(PACKAGE_STRING)" \
		--short-name="$(PACKAGE_TARNAME)"

# Make sure to build the .wuhb after the `all' target.
all-hook: $(WUHB_FILE)

# Also use the `run' target to run it.
run-local: $(WUHB_FILE)
	WIILOAD=tcp:wiiu wiiload $<

