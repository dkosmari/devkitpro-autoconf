# -*- mode: autoconf -*-
# wiiu-wups.m4 - Macros to handle Wii U Plugin System

# Copyright (c) 2024 Daniel K. O. <dkosmari>
#
# Copying and distribution of this file, with or without modification, are permitted in
# any medium without royalty provided the copyright notice and this notice are
# preserved. This file is offered as-is, without any warranty.

#serial 1

# WIIU_WUPS_INIT
# --------------
# This macro adjusts the environment for the Wii U Plugin System (WUPS).
#
# Output variables:
#   - `DEVKITPRO_CPPFLAGS'
#   - `DEVKITPRO_LDFLAGS'
#   - `DEVKITPRO_LIBS'

AC_DEFUN([WIIU_WUPS_INIT],[

    AC_REQUIRE([DEVKITPRO_WUT_INIT])

    # set WIIU_WUPS_ROOT
    AS_VAR_SET([WIIU_WUPS_ROOT], [$DEVKITPRO/wups])

    AX_PREPEND_FLAG([DEVKITPRO_CPPFLAGS],
                    [-D__WUPS__ -I$WUPS_ROOT/include])

    AX_PREPEND_FLAG([DEVKITPRO_LDFLAGS],
                    [-T$WUPS_ROOT/share/wups.ld -specs=$WUPS_ROOT/share/wups.specs])

    AX_PREPEND_FLAG([DEVKITPRO_LIBS],
                    [-L$WUPS_ROOT/lib -lwups])

])
