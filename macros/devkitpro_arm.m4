# -*- mode: autoconf -*-
# devkitpro_arm.m4 - Macros to handle ARM toolchains.
# URL: https://github.com/dkosmari/devkitpro-autoconf/

# Copyright (c) 2025 Daniel K. O. <dkosmari>
#
# Copying and distribution of this file, with or without modification, are permitted in
# any medium without royalty provided the copyright notice and this notice are
# preserved. This file is offered as-is, without any warranty.

#serial 3

# DEVKITPRO_ARM_INIT
# ------------------
#
# This macro adjuts the environment for ARM-based targets. It must be called before
# `AM_INIT_AUTOMAKE', and before any cross-compilation tool is checked.
#
# Output variables:
#   - `DEVKITARM': path to devkitARM
#   - `PATH': appends `devkitARM/bin' if necessary.'

AC_DEFUN([DEVKITPRO_ARM_INIT],[

    DEVKITPRO_INIT

    # Sanity check for host type.
    AS_CASE($host,
            [arm-*-eabi], [],
            [AC_MSG_ERROR([invalid host ($host), you should use --host=arm-none-eabi])])

    # set DEVKITARM
    AC_ARG_VAR([DEVKITARM], [path to devkitARM])
    # if not set, set it to $DEVKITPRO/devkitPPC
    AS_VAR_SET_IF([[DEVKITARM]],
                  [],
                  [AS_VAR_SET([DEVKITARM], [$DEVKITPRO/devkitARM])])
    AC_SUBST([DEVKITARM])

    # See if we can find cross tools in PATH already; if not, append $DEVKITARM/bin to
    # PATH
    DEVKITPRO_APPEND_TOOL_PATH([arm-none-eabi-nm], [$DEVKITARM/bin])

])dnl DEVKITPRO_ARM_INIT

