EXTRA_DIST = \
	bootstrap \
	README.md


PLUGIN_FILE = dummy.wps

noinst_PROGRAMS = dummy.elf

dummy_elf_SOURCES = src/main.cpp


# Convert .elf to .wps
all-local: $(PLUGIN_FILE)


run: $(PLUGIN_FILE)
	WIILOAD=tcp:wiiu wiiload $<


# Incorporate the extra Makefile rules
@INC_AMINCLUDE@


# Delete `aminclude.am' during `make distclean'
DISTCLEANFILES = $(AMINCLUDE)


# Use `make install' to send it straight to the Aroma plugins folder through ftpiiu
install-exec-local: $(PLUGIN_FILE)
	curl "ftp://wiiu:/fs/vol/external01/wiiu/environments/aroma/plugins/" --upload-file $<

uninstall-local:
	curl "ftp://wiiu" --quote "DELE /fs/vol/external01/wiiu/environments/aroma/plugins/$(PLUGIN_FILE)"
