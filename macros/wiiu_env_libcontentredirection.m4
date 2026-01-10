# -*- mode: autoconf -*-
# wiiu_env_libcontentredirection.m4 - Macros to handle libcontentredirection
# URL: https://github.com/dkosmari/devkitpro-autoconf/

# Copyright (c) 2026 Daniel K. O. <dkosmari>
#
# Copying and distribution of this file, with or without modification, are permitted in
# any medium without royalty provided the copyright notice and this notice are
# preserved. This file is offered as-is, without any warranty.

#serial 1

# WIIU_ENV_SETUP_LIBCONTENTREDIRECTION([RELATIVE-PATH-TO-LIBCONTENTREDIRECTION])
# ------------------------------------------------------------------------------
#
# Sets up usage of libcontentredirection as a submodule.
#
# Output variables:
#   - `LIBCONTENTREDIRECTION_DIR'
#   - `LIBCONTENTREDIRECTION_CPPFLAGS'
#   - `LIBCONTENTREDIRECTION_LIB'
#   - custom Makefile rules (use @INC_AMINCLUDE@ in your `Makefile.am')

AC_DEFUN([WIIU_ENV_SETUP_LIBCONTENTREDIRECTION],[dnl
    WIIU_ENV_SETUP_LIB([LIBCONTENTREDIRECTION], [], [$1], [${DEVKITPRO}/wums])
])dnl WIIU_ENV_SETUP_LIBCONTENTREDIRECTION
