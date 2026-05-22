@INC_AMINCLUDE@

EXTRA_DIST = \
	bootstrap

SUBDIRS = external/sdl2xx


ELF_FILE := sdl2-swkbd.elf
RPX_FILE := $(ELF_FILE:.elf=.rpx)
WUHB_FILE := $(RPX_FILE:.rpx=.wuhb)


AM_CPPFLAGS = \
	$(SDL2_CFLAGS) \
	-I$(srcdir)/external/sdl2xx/include

AM_CXXFLAGS = -Wall -Wextra -Werror

LDADD =						\
	external/sdl2xx/lib/libsdl2xx_ttf.la	\
	external/sdl2xx/lib/libsdl2xx.la	\
	$(SDL2_LIBS)


noinst_PROGRAMS = $(ELF_FILE)

sdl2_swkbd_elf_SOURCES =	\
	src/main.cpp		\
	src/sdl_debug.cpp 	\
	src/sdl_debug.hpp	\
	src/tracer.cpp		\
	src/tracer.hpp		\
	src/wiiu-stdout.cpp


$(WUHB_FILE): $(RPX_FILE)
	$(WUHBTOOL) 					\
		$< $@					\
		--name="$(PACKAGE_STRING)"		\
		--short-name="$(PACKAGE_TARNAME)"


all-local: $(WUHB_FILE)

WIILOAD ?= tcp:wiiu
run: $(WUHB_FILE)
	WIILOAD=$(WIILOAD) wiiload $<



DISTCLEANFILES = $(AMINCLUDE)

CLEANFILES = $(WUHB_FILE) $(RPX_FILE)

PHONY = company

company: compile_flags.txt

compile_flags.txt: Makefile
	printf "%s" "$(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS)" | xargs -n1 | sort -u > $(srcdir)/compile_flags.txt
	$(CPP) -xc++ /dev/null -E -Wp,-v 2>&1 | sed -n 's,^ ,-I,p' >> $(srcdir)/compile_flags.txt
