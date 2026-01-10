# -*- mode: autoconf -*-
# wiiu_env_libfunctionpatcher.m4 - Macros to handle libfunctionpatcher
# URL: https://github.com/dkosmari/devkitpro-autoconf/

# Copyright (c) 2026 Daniel K. O. <dkosmari>
#
# Copying and distribution of this file, with or without modification, are permitted in
# any medium without royalty provided the copyright notice and this notice are
# preserved. This file is offered as-is, without any warranty.

#serial 1

# WIIU_ENV_SETUP_LIBFUNCTIONPATCHER([RELATIVE-PATH-TO-LIBFUNCTIONPATCHER])
# ------------------------------------------------------------------------
#
# Sets up usage of libfunctionpatcher as a submodule.
#
# Output variables:
#   - `LIBFUNCTIONPATCHER_DIR'
#   - `LIBFUNCTIONPATCHER_CPPFLAGS'
#   - `LIBFUNCTIONPATCHER_LIB'
#   - custom Makefile rules (use @INC_AMINCLUDE@ in your `Makefile.am')

AC_DEFUN([WIIU_ENV_SETUP_LIBFUNCTIONPATCHER],[dnl
    WIIU_ENV_SETUP_LIB([LIBFUNCTIONPATCHER], [], [$1], [${DEVKITPRO}/wums])
])dnl WIIU_ENV_SETUP_LIBFUNCTIONPATCHER
