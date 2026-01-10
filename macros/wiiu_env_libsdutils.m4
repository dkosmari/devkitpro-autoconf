# -*- mode: autoconf -*-
# wiiu_env_libsdutils.m4 - Macros to handle libsdutils
# URL: https://github.com/dkosmari/devkitpro-autoconf/

# Copyright (c) 2026 Daniel K. O. <dkosmari>
#
# Copying and distribution of this file, with or without modification, are permitted in
# any medium without royalty provided the copyright notice and this notice are
# preserved. This file is offered as-is, without any warranty.

#serial 1

# WIIU_ENV_SETUP_LIBSDUTILS([RELATIVE-PATH-TO-LIBSDUTILS])
# --------------------------------------------------------
#
# Sets up usage of libsdutils as a submodule.
#
# Output variables:
#   - `LIBSDUTILS_DIR'
#   - `LIBSDUTILS_CPPFLAGS'
#   - `LIBSDUTILS_LIB'
#   - custom Makefile rules (use @INC_AMINCLUDE@ in your `Makefile.am')

AC_DEFUN([WIIU_ENV_SETUP_LIBSDUTILS],[dnl
    WIIU_ENV_SETUP_LIB([LIBSDUTILS], [], [$1], [${DEVKITPRO}/wums])
])dnl WIIU_ENV_SETUP_LIBSDUTILS
