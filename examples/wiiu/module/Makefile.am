EXTRA_DIST = \
	bootstrap \
	README.md


SUBDIRS = external/WiiUModuleSystem

DIST_SUBDIRS =


AM_CPPFLAGS = $(WUMS_CPPFLAGS)
AM_LDFLAGS  = $(WUMS_LDFLAGS)
LDADD       = $(WUMS_LIB)


# We compile a regular PROGRAMS .elf, then ask it to be converted to .wms later.
WMS_FILE = dummy.wms

noinst_PROGRAMS = dummy.elf

dummy_elf_SOURCES = src/main.cpp

EXTRA_dummy_elf_DEPENDENCIES = $(WUMS_LIB)


# Convert .elf to .wms
all-local: $(WMS_FILE)


.PHONY: wiiu-install wiiu-uninstall

# Use make wiiu-install to copy it to the Wii U
wiiu-install: $(WMS_FILE)
	curl "ftp://wiiu:/fs/vol/external01/wiiu/environments/aroma/modules/" --upload-file $<

# Use make wiiu-uninstall to delete it from the Wii U
wiiu-uninstall:
	curl "ftp://wiiu" --quote "DELE /fs/vol/external01/wiiu/environments/aroma/modules/$(WMS_FILE)"


# Include the aminclude.am file, to import the extra rules.
@INC_AMINCLUDE@

# Delete aminclude.am during `make distclean`
DISTCLEANFILES = $(AMINCLUDE)

