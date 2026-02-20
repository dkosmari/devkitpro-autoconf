# -*- mode: autoconf -*-
# devkitpro_3ds.m4 - Macros to handle 3DS setup.
# URL: https://github.com/dkosmari/devkitpro-autoconf/

# Copyright (c) 2025-2026 Daniel K. O. <dkosmari>
#
# Copying and distribution of this file, with or without modification, are permitted in
# any medium without royalty provided the copyright notice and this notice are
# preserved. This file is offered as-is, without any warranty.

#serial 4

# DEVKITPRO_3DS_INIT
# ------------------
#
# This macro adjusts the environment for 3DS homebrew.
#
# Output variables:
#   - `DEVKITPRO_LIBCTRU'
#   - `DEVKITPRO_PORTLIBS_3DS'
#   - `PATH': appends tools and portlibs paths.

AC_DEFUN([DEVKITPRO_3DS_INIT], [

    DEVKITPRO_ARM_INIT

    # Ensure ${DEVKITPRO}/tools/bin is in PATH
    DEVKITPRO_APPEND_TOOL_PATH([smdhtool], [${DEVKITPRO}/tools/bin])

    AC_CHECK_PROGS([SMDHTOOL], [smdhtool], [], [${PATH}:${DEVKITPRO_PATH}])
    AC_CHECK_PROGS([_3DSXTOOL], [3dsxtool], [], [${PATH}:${DEVKITPRO_PATH}])
    AC_CHECK_PROGS([_3DSLINK], [3dslink], [], [${PATH}:${DEVKITPRO_PATH}])
    AC_CHECK_PROGS([BANNERTOOL], [bannertool], [], [${PATH}:${DEVKITPRO_PATH}])

    # set DEVKITPRO_LIBCTRU
    AC_ARG_VAR([DEVKITPRO_LIBCTRU], [path to libctru])
    AS_VAR_SET_IF([DEVKITPRO_LIBCTRU],
                  [],
                  [AS_VAR_SET([DEVKITPRO_LIBCTRU], [${DEVKITPRO}/libctru])])
    AC_SUBST([DEVKITPRO_LIBCTRU])

    # set DEVKITPRO_PORTLIBS_3DS
    AC_ARG_VAR([DEVKITPRO_PORTLIBS_3DS], [path to portlibs/3ds])
    AS_VAR_SET_IF([DEVKITPRO_PORTLIBS_3DS],
                  [],
                  [AS_VAR_SET([DEVKITPRO_PORTLIBS_3DS], [${DEVKITPRO_PORTLIBS}/3ds])])
    AC_SUBST([DEVKITPRO_PORTLIBS_3DS])

    # Prepend portlibs/3ds/bin to DEVKITPRO_PATH
    # Note: we don't know if any portlibs package is installed or even needed.
    DEVKITPRO_PATH_PREPEND([${DEVKITPRO_PORTLIBS_3DS}/bin])

    # Finally, append DEVKITPRO_PATH to PATH if needed
    AS_VAR_SET_IF([DEVKITPRO_PATH],
                  [AS_VAR_APPEND([PATH], [":${DEVKITPRO_PATH}"])])

])dnl DEVKITPRO_3DS_INIT


# DEVKITPRO_3DS_SETUP
# -------------------
#
# This macro adjusts the compilation flags for 3DS homebrew.
#
# Output variables:
#   - `CFLAGS'
#   - `CPPFLAGS'
#   - `CXXFLAGS'
#   - `LDFLAGS'
#   - `LIBS'

AC_DEFUN([DEVKITPRO_3DS_SETUP], [

    AS_VAR_SET_IF([DEVKITPRO_LIBCTRU], [], [AC_MSG_ERROR([DEVKITPRO_LIBCTRU not set.])])

    AX_PREPEND_FLAG([-D__3DS__],                         [CPPFLAGS])
    AX_PREPEND_FLAG([-I${DEVKITPRO_PORTLIBS_3DS}/include], [CPPFLAGS])
    AX_PREPEND_FLAG([-I${DEVKITPRO_LIBCTRU}/include],      [CPPFLAGS])

    AX_PREPEND_FLAG([-march=armv6k],    [CFLAGS])
    AX_PREPEND_FLAG([-mfloat-abi=hard], [CFLAGS])
    AX_PREPEND_FLAG([-mtp=soft],        [CFLAGS])
    AX_PREPEND_FLAG([-mtune=mpcore],    [CFLAGS])

    AX_PREPEND_FLAG([-march=armv6k],    [CXXFLAGS])
    AX_PREPEND_FLAG([-mfloat-abi=hard], [CXXFLAGS])
    AX_PREPEND_FLAG([-mtp=soft],        [CXXFLAGS])
    AX_PREPEND_FLAG([-mtune=mpcore],    [CXXFLAGS])

    AX_PREPEND_FLAG([-specs=3dsx.specs], [LDFLAGS])

    AX_PREPEND_FLAG([-lm],    [LIBS])
    AX_PREPEND_FLAG([-lctru], [LIBS])

    AX_PREPEND_FLAG([-L${DEVKITPRO_PORTLIBS_3DS}/lib], [LIBS])
    AX_PREPEND_FLAG([-L${DEVKITPRO_LIBCTRU}/lib],      [LIBS])

    m4_pattern_allow([AM_V_at])

    # custom Makefile recipes
    AX_ADD_AM_MACRO([

clean: clean-3dsx

.PHONY: clean-3dsx

clean-3dsx:
	\$([AM_V_at])\$(RM) *.3dsx

%.3dsx: %.strip.elf
	\$([AM_V_at])\$(_3DSXTOOL) \$< \$[@]

clean: clean-smdh

.PHONY: clean-smdh

clean-smdh:
	\$([AM_V_at])\$(RM) *.smdh

])

])dnl DEVKITPRO_3DS_SETUP
