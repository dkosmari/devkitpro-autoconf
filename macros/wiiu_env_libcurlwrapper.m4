# -*- mode: autoconf -*-
# wiiu_env_libcurlwrapper.m4 - Macros to handle libcurlwrapper
# URL: https://github.com/dkosmari/devkitpro-autoconf/

# Copyright (c) 2026 Daniel K. O. <dkosmari>
#
# Copying and distribution of this file, with or without modification, are permitted in
# any medium without royalty provided the copyright notice and this notice are
# preserved. This file is offered as-is, without any warranty.

#serial 1

# WIIU_ENV_SETUP_LIBCURLWRAPPER([RELATIVE-PATH-TO-LIBCURLWRAPPER])
# ----------------------------------------------------------------
#
# Sets up usage of libcurlwrapper as a submodule.
#
# Output variables:
#   - `LIBCURLWRAPPER_DIR'
#   - `LIBCURLWRAPPER_CPPFLAGS'
#   - `LIBCURLWRAPPER_LIB'
#   - custom Makefile rules (use @INC_AMINCLUDE@ in your `Makefile.am')

AC_DEFUN([WIIU_ENV_SETUP_LIBCURLWRAPPER],[dnl
    WIIU_ENV_SETUP_LIB([LIBCURLWRAPPER], [], [$1], [${DEVKITPRO}/wums])
])dnl WIIU_ENV_SETUP_LIBCURLWRAPPER
