# -*- mode: autoconf -*-
# wiiu_env_librpxloader.m4 - Macros to handle librpxloader
# URL: https://github.com/dkosmari/devkitpro-autoconf/

# Copyright (c) 2026 Daniel K. O. <dkosmari>
#
# Copying and distribution of this file, with or without modification, are permitted in
# any medium without royalty provided the copyright notice and this notice are
# preserved. This file is offered as-is, without any warranty.

#serial 1

# WIIU_ENV_SETUP_LIBRPXLOADER([RELATIVE-PATH-TO-LIBRPXLOADER])
# ------------------------------------------------------------
#
# Sets up usage of librpxloader as a submodule.
#
# Output variables:
#   - `LIBRPXLOADER_DIR'
#   - `LIBRPXLOADER_CPPFLAGS'
#   - `LIBRPXLOADER_LIB'
#   - custom Makefile rules (use @INC_AMINCLUDE@ in your `Makefile.am')

AC_DEFUN([WIIU_ENV_SETUP_LIBRPXLOADER],[dnl
    WIIU_ENV_SETUP_LIB([LIBRPXLOADER], [], [$1], [${DEVKITPRO}/wums])
])dnl WIIU_ENV_SETUP_LIBRPXLOADER
