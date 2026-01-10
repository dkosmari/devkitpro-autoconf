# -*- mode: autoconf -*-
# wiiu_env_libkernel.m4 - Macros to handle libkernel
# URL: https://github.com/dkosmari/devkitpro-autoconf/

# Copyright (c) 2026 Daniel K. O. <dkosmari>
#
# Copying and distribution of this file, with or without modification, are permitted in
# any medium without royalty provided the copyright notice and this notice are
# preserved. This file is offered as-is, without any warranty.

#serial 1

# WIIU_ENV_SETUP_LIBKERNEL([RELATIVE-PATH-TO-LIBKERNEL])
# ------------------------------------------------------
#
# Sets up usage of libkernel as a submodule.
#
# Output variables:
#   - `LIBKERNEL_DIR'
#   - `LIBKERNEL_CPPFLAGS'
#   - `LIBKERNEL_LDFLAGS'
#   - `LIBKERNEL_LIB'
#   - custom Makefile rules (use @INC_AMINCLUDE@ in your `Makefile.am')

AC_DEFUN([WIIU_ENV_SETUP_LIBKERNEL],[dnl
    WIIU_ENV_SETUP_LIB([LIBKERNEL], [], [$1], [${DEVKITPRO}/wums])

    AC_ARG_VAR([LIBKERNEL_LDFLAGS], [linker flags for LIBKERNEL])
    AS_VAR_SET([LIBKERNEL_LDFLAGS], ["-T${LIBKERNEL_DIR}/share/libkernel.ld"])
    AC_SUBST([LIBKERNEL_LDFLAGS])
]
])dnl WIIU_ENV_SETUP_LIBKERNEL
