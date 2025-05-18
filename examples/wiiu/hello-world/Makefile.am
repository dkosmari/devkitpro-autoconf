# First, some basic Automake stuff
EXTRA_DIST = \
	bootstrap \
	README.md


RPX_FILE = hello-world.rpx

# Tip: use noinst_PROGRAMS instead of bin_PROGRAMS, to not have it on `make install'.
noinst_PROGRAMS = hello-world.elf

hello_world_elf_SOURCES = src/main.cpp


# To generate a .rpx, we need to hook it into the `all' rule.
all-local: $(RPX_FILE)


# Some custom Makefile rules are created in ${top_builddir}/$(AMINCLUDE). This is how we
# include it in our Makefile:
@INC_AMINCLUDE@
# Note: we need this in every Makefile that needs custom devktpro rules.


# If we wish to delete that file during `make distclean', we must tell Automake to clean
# it:
DISTCLEANFILES = $(AMINCLUDE)
# Note: we do this only in the toplevel Makefile.


# Use `make run' to run it on the Wii U using wiiload (assume the host name is `wiiu')
.PHONY: run
run: $(RPX_FILE)
	WIILOAD=tcp:wiiu wiiload $<

