EXTRA_DIST = \
	bootstrap \
	README.md


SUBDIRS = \
	external/libnotifications \
	external/WiiUPluginSystem

DIST_SUBDIRS =


AM_CPPFLAGS = \
	$(LIBNOTIFICATIONS_CPPFLAGS) \
	$(WUPS_CPPFLAGS)


AM_LDFLAGS = $(WUPS_LDFLAGS)


LDADD = \
	$(WUPS_LIB) \
	$(LIBNOTIFICATIONS_LIB)


WPS_FILE = dummy.wps

noinst_PROGRAMS = dummy.elf

dummy_elf_SOURCES = src/main.cpp

EXTRA_dummy_elf_DEPENDENCIES = \
	$(WUPS_LIB) \
	$(LIBNOTIFICATIONS_LIB)


# Convert .elf to .wps
all-local: $(WPS_FILE)


.PHONY: run


run: $(WPS_FILE)
	WIILOAD=tcp:wiiu wiiload $<


.PHONY: wiiu-install wiiu-uninstall

# Use `make install-wiiu' to send it straight to the Aroma plugins folder through ftpiiu
wiiu-install: $(WPS_FILE)
	curl "ftp://wiiu:/fs/vol/external01/wiiu/environments/aroma/plugins/" --upload-file $<

wiiu-uninstall:
	curl "ftp://wiiu" --quote "DELE /fs/vol/external01/wiiu/environments/aroma/plugins/$(WPS_FILE)"


# Incorporate the extra Makefile rules
@INC_AMINCLUDE@


# Delete `aminclude.am' during `make distclean'
DISTCLEANFILES = $(AMINCLUDE)

