# -*- mode: autoconf -*-
# devkitpro_arm.m4 - Macros to handle ARM toolchains.
# URL: https://github.com/dkosmari/devkitpro-autoconf/

# Copyright (c) 2025-2026 Daniel K. O. <dkosmari>
#
# Copying and distribution of this file, with or without modification, are permitted in
# any medium without royalty provided the copyright notice and this notice are
# preserved. This file is offered as-is, without any warranty.

#serial 4

# DEVKITPRO_ARM_INIT
# ------------------
#
# This macro adjuts the environment for ARM-based targets. It must be called before
# `AM_INIT_AUTOMAKE', and before any cross-compilation tool is checked.
#
# Output variables:
#   - `DEVKITARM': path to devkitARM
#   - `DEVKITPRO_HOST': set to `arm-none-eabi-'
#   - `DEVKITPRO_PATH': appends `:devkitARM/bin' if necessary.

AC_DEFUN([DEVKITPRO_ARM_INIT],[

    DEVKITPRO_INIT

    # Sanity check for host type.
    AS_CASE([${host}],
            [arm-*-eabi], [],
            [AC_MSG_ERROR([invalid host (${host}), you should use --host=arm-none-eabi])])

    AS_VAR_SET_IF([DEVKITPRO_HOST],
                  [],
                  [AS_VAR_SET([DEVKITPRO_HOST], [arm-none-eabi-])])

    # set DEVKITARM
    AC_ARG_VAR([DEVKITARM], [location of devkitARM])
    # if not set, set it to ${DEVKITPRO}/devkitARM
    AS_VAR_SET_IF([[DEVKITARM]],
                  [],
                  [AS_VAR_SET([DEVKITARM], [${DEVKITPRO}/devkitARM])])
    AC_SUBST([DEVKITARM])

    # See if we can find cross tools in PATH already; if not, append $DEVKITARM/bin to
    # DEVKITPRO_PATH
    DEVKITPRO_APPEND_TOOL_PATH([${DEVKITPRO_HOST}nm], [${DEVKITARM}/bin])

    # Now check that DEVKITARM/bin binaries are usable
    AX_VAR_PUSHVALUE([PATH], [${PATH}:${DEVKITPRO_PATH}])
    AS_IF([! which ${DEVKITPRO_HOST}nm 1>/dev/null 2>/dev/null],
          [AC_MSG_ERROR([devkitARM binaries not found in PATH=${PATH}])])
    AX_VAR_POPVALUE([PATH])

])dnl DEVKITPRO_ARM_INIT

