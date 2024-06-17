# -*- mode: autoconf -*-
# devkitpro_arm.m4 - Macros to handle ARM toolchains.

# Copyright (c) 2024 Daniel K. O. <dkosmari>
#
# Copying and distribution of this file, with or without modification, are permitted in
# any medium without royalty provided the copyright notice and this notice are
# preserved. This file is offered as-is, without any warranty.

#serial 1

# DEVKITPRO_ARM_INIT
# ------------------
# This macro adjuts the environment for ARM-based targets. It must be called before
# `AM_INIT_AUTOMAKE', and before any cross-compilation tool is checked.
#
# Output variables:
#   - `DEVKITARM': path to devkitARM
#   - `PATH': appends `devkitARM/bin' if necessary.
#
# The file `aminclude.am` is generated with extra Makefile rules. Add `@INC_AMINCLUDE@` to
# the Makefile that needs them. Add `DISTCLEANFILES = $(AMINCLUDE)' to the toplevel
# `Makefile.am` to remove this file during `make distclean'.

AC_DEFUN([DEVKITPRO_ARM_INIT],[

    AC_REQUIRE([DEVKITPRO_INIT])


    # Sanity check for host type.
    AS_CASE($host,
            [arm-*-eabi], [],
            [AC_MSG_ERROR([invalid host ($host), you should use --host=arm-none-eabi])])


    # set DEVKITARM
    AS_VAR_SET([DEVKITARM], [$DEVKITPRO/devkitARM])
    AC_SUBST([DEVKITARM])


    # See if we can find cross tools in PATH already; if not, append $DEVKITARM/bin to
    # PATH
    AC_MSG_CHECKING([if $DEVKITARM/bin is in PATH])
    AS_IF([! which arm-none-eabi-nm 1>/dev/null 2>/dev/null],
          [
              AC_MSG_RESULT([no, will append to PATH])
              AS_VAR_APPEND([PATH], [":$DEVKITARM/bin"])
              AC_SUBST([PATH])
          ],
          [AC_MSG_RESULT([yes])])


    # custom Makefile rules
    AX_ADD_AM_MACRO([
CLEANFILES ?=
CLEANFILES = *.strip.elf
%.strip.elf: %.elf; \$(STRIP) -g \$< -o \$[@]
])



])
