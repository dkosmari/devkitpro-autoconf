# -*- mode: autoconf -*-
# devkitpro.m4 - Macros to handle devkitPro setup.

# Copyright (c) 2024 Daniel K. O. <dkosmari>
#
# Copying and distribution of this file, with or without modification, are permitted in
# any medium without royalty provided the copyright notice and this notice are
# preserved. This file is offered as-is, without any warranty.

#serial 1

# DEVKITPRO_INIT
# --------------
# This macro sets up base devkitPro variables to be used by other macros. The option
# `--with-devkitpro=' is also processed, to override the `DEVKITPRO' variable.
#
# Output variables:
#   - `DEVKITPRO': path to devkitPro.
#   - `DEVKITPRO_CFLAGS': declared precious.
#   - `DEVKITPRO_CPPFLAGS': declared precious.
#   - `DEVKITPRO_CXXFLAGS': declared precious.
#   - `DEVKITPRO_LDFLAGS': declared precious.
#   - `DEVKITPRO_LIBS': declared precious.
#
# The file `aminclude.am` is generated with extra Makefile rules:
#   - Add `@INC_AMINCLUDE@` to the Makefile that needs them.
#   - Add `DISTCLEANFILES = $(AMINCLUDE)' to the toplevel `Makefile.am` to remove this
#     file during `make distclean'.

AC_DEFUN([DEVKITPRO_INIT],[

    AC_REQUIRE([AC_CANONICAL_HOST])

    # Make sure macros that look up programs don't appear before this, since we may need
    # to adjust PATH.
    AC_BEFORE([$0], [AM_INIT_AUTOMAKE])
    # specific program tests
    AC_BEFORE([$0], [AC_PROG_CC])
    AC_BEFORE([$0], [AC_PROG_CXX])
    AC_BEFORE([$0], [AC_PROG_CPP])
    AC_BEFORE([$0], [AC_PROG_RANLIB])
    # automake also has these
    AC_BEFORE([$0], [AM_PROG_AR])
    AC_BEFORE([$0], [AM_PROG_AS])
    # cross-compilation tool tests
    AC_BEFORE([$0], [AC_CHECK_TOOL])
    AC_BEFORE([$0], [AC_CHECK_TOOLS])
    AC_BEFORE([$0], [AC_PATH_TARGET_TOOL])
    AC_BEFORE([$0], [AC_PATH_TOOL])


    # make DEVKITPRO precious
    AC_ARG_VAR([DEVKITPRO], [path to devkitPro])

    # --with-devkitpro
    AC_ARG_WITH([devkitpro],
                [AS_HELP_STRING([--with-devkitpro=PATH-TO-DEVKITPRO],
                                [Set the base path to devkitPro. This overrides the variable DEVKITPRO])],
                [AS_VAR_SET([DEVKITPRO], [$withval])])

    AC_MSG_CHECKING([devkitPro path])

    AS_VAR_SET_IF([DEVKITPRO],
                  [
                      AC_MSG_RESULT([$DEVKITPRO])
                  ],
                  [
                      AC_MSG_RESULT([not found])
                      AC_MSG_ERROR([You need either `--with-devkitpro=PATH-TO-DEVKITPRO' or the `DEVKITPRO' variable.])
                  ])


    # set PORTLIBS_ROOT
    AS_VAR_SET([PORTLIBS_ROOT], [$DEVKITPRO/portlibs])


    AC_ARG_VAR([DEVKITPRO_CFLAGS], [C compilation flags for devkitPro])
    AC_ARG_VAR([DEVKITPRO_CPPFLAGS], [includes search path for devkitPro])
    AC_ARG_VAR([DEVKITPRO_CXXFLAGS], [C++ compilation flags for devkitPro])
    AC_ARG_VAR([DEVKITPRO_LDFLAGS], [linker flags for devkitPro])
    AC_ARG_VAR([DEVKITPRO_LIBS], [libraries for devkitPro])

    # custom Makefile rules
    AX_ADD_AM_MACRO([
clean: clean-strip-elf

.PHONY: clean-strip-elf
clean-strip-elf:; \$(RM) *.strip.elf

%.strip.elf: %.elf; \$(STRIP) -g \$< -o \$[@]
])

])
