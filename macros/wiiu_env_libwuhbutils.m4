# -*- mode: autoconf -*-
# wiiu_env_libwuhbutils.m4 - Macros to handle libwuhbutils
# URL: https://github.com/dkosmari/devkitpro-autoconf/

# Copyright (c) 2026 Daniel K. O. <dkosmari>
#
# Copying and distribution of this file, with or without modification, are permitted in
# any medium without royalty provided the copyright notice and this notice are
# preserved. This file is offered as-is, without any warranty.

#serial 1

# WIIU_ENV_SETUP_LIBWUHBUTILS([RELATIVE-PATH-TO-LIBWUHBUTILS])
# ------------------------------------------------------------
#
# Sets up usage of libwuhbutils as a submodule.
#
# Output variables:
#   - `LIBWUHBUTILS_DIR'
#   - `LIBWUHBUTILS_CPPFLAGS'
#   - `LIBWUHBUTILS_LIB'
#   - custom Makefile rules (use @INC_AMINCLUDE@ in your `Makefile.am')

AC_DEFUN([WIIU_ENV_SETUP_LIBWUHBUTILS],[dnl
    WIIU_ENV_SETUP_LIB([LIBWUHBUTILS], [], [$1], [${DEVKITPRO}/wums])
])dnl WIIU_ENV_SETUP_LIBWUHBUTILS
