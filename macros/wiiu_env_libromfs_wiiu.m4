# -*- mode: autoconf -*-
# wiiu_env_libromfs_wiiu.m4 - Macros to handle libromfs_wiiu
# URL: https://github.com/dkosmari/devkitpro-autoconf/

# Copyright (c) 2026 Daniel K. O. <dkosmari>
#
# Copying and distribution of this file, with or without modification, are permitted in
# any medium without royalty provided the copyright notice and this notice are
# preserved. This file is offered as-is, without any warranty.

#serial 1

# WIIU_ENV_SETUP_LIBROMFS_WIIU([RELATIVE-PATH-TO-LIBROMFS_WIIU])
# --------------------------------------------------------------
#
# Sets up usage of libromfs_wiiu as a submodule.
#
# Output variables:
#   - `LIBROMFS_WIIU_DIR'
#   - `LIBROMFS_WIIU_CPPFLAGS'
#   - `LIBROMFS_WIIU_LIB'
#   - custom Makefile rules (use @INC_AMINCLUDE@ in your `Makefile.am')

AC_DEFUN([WIIU_ENV_SETUP_LIBROMFS_WIIU],[dnl
    WIIU_ENV_SETUP_LIB([LIBROMFS_WIIU], [], [$1], [${DEVKITPRO}/wut/usr])
])dnl WIIU_ENV_SETUP_LIBROMFS_WIIU
