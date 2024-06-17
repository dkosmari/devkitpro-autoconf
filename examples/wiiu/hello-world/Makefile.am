# First, some basic Automake stuff
EXTRA_DIST = \
	bootstrap \
	README.md


# First, we assign the DEVKITPRO_* variables to the AM_* variables.
AM_CPPFLAGS = $(DEVKITPRO_CPPFLAGS)
AM_CXXFLAGS = $(DEVKITPRO_CXXFLAGS)
AM_LDFLAGS = $(DEVKITPRO_LDFLAGS)
# There's no AM_LIBS, so we use LIBS
LIBS = $(DEVKITPRO_LIBS)


# Tip: use noinst_PROGRAMS instead of bin_PROGRAMS, to not have it on `make install'.
noinst_PROGRAMS = hello-world.elf

hello_world_elf_SOURCES = src/main.cpp


# To generate a .rpx, we need to hook it into the `all' rule.
all-local: hello-world.rpx


# Some custom Makefile rules are created in ${top_builddir}/$(AMINCLUDE). This is how we
# include it in our Makefile:
@INC_AMINCLUDE@
# Note: we need this in every Makefile that needs custom devktpro rules.


# If we wish to delete that file during `make distclean', we must tell Automake to clean
# it:
DISTCLEANFILES = $(AMINCLUDE)
# Note: we do this only on the toplevel Makefile.


# Use `make run' to run it on the Wii U using wiiload (assume the host name is `wiiu')
run: hello-world.rpx
	WIILOAD=tcp:wiiu wiiload $<

