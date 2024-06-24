# -*- mode: autoconf -*-
# devkitpro_3ds.m4 - Macros to handle 3DS setup.

# Copyright (c) 2024 Daniel K. O. <dkosmari>
#
# Copying and distribution of this file, with or without modification, are permitted in
# any medium without royalty provided the copyright notice and this notice are
# preserved. This file is offered as-is, without any warranty.

#serial 1

# DEVKITPRO_3DS_INIT
# ------------------
#
# This macro adjusts the environment for 3DS homebrew.
#
# Output variables:
#   - `DEVKITPRO_CFLAGS'
#   - `DEVKITPRO_CPPFLAGS'
#   - `DEVKITPRO_CXXFLAGS'
#   - `DEVKITPRO_LDFLAGS'
#   - `DEVKITPRO_LIBS'
#   - `PATH': appends `DEVKITPRO/tools/bin' if necessary.

AC_DEFUN([DEVKITPRO_3DS_INIT], [

    AC_REQUIRE([DEVKITPRO_ARM_INIT])

    # See if we can find smdhtool in PATH
    DEVKITPRO_TOOL_PATH([smdhtool])

    AC_CHECK_PROGS([SMDHTOOL], [smdhtool])
    AC_CHECK_PROGS([_3DSXTOOL], [3dsxtool])
    AC_CHECK_PROGS([_3DSLINK], [3dslink])
    AC_CHECK_PROGS([BANNERTOOL], [bannertool])

    # set LIBCTRU_ROOT
    AS_VAR_SET([LIBCTRU_ROOT], [$DEVKITPRO/libctru])

    # set PORTLIBS_3DS_ROOT
    AS_VAR_SET([PORTLIBS_3DS_ROOT], [$PORTLIBS_ROOT/3ds])

    # See if we need to append PORTLIBS_3DS_ROOT/bin to PATH
    # TODO: we should actually check the contents of PATH
    AC_MSG_CHECKING([if $PORTLIBS_3DS_ROOT/bin is in PATH])
    AS_IF([! which arm-none-eabi-pkg-config 1>/dev/null 2>/dev/null],
          [
              AC_MSG_RESULT([no, will a ppend to PATH])
              AS_VAR_APPEND([PATH], [":$PORTLIBS_3DS_ROOT/bin"])
              AC_SUBST([PATH])
          ],
          [AC_MSG_RESULT([yes])])
    
    AX_PREPEND_FLAG([-D__3DS__],               [DEVKITPRO_CPPFLAGS])
    AX_PREPEND_FLAG([-I$LIBCTRU_ROOT/include], [DEVKITPRO_CPPFLAGS])

    AX_PREPEND_FLAG([-march=armv6k],    [DEVKITPRO_CFLAGS])
    AX_PREPEND_FLAG([-mfloat-abi=hard], [DEVKITPRO_CFLAGS])
    AX_PREPEND_FLAG([-mtp=soft],        [DEVKITPRO_CFLAGS])
    AX_PREPEND_FLAG([-mtune=mpcore],    [DEVKITPRO_CFLAGS])

    AX_PREPEND_FLAG([-march=armv6k],    [DEVKITPRO_CXXFLAGS])
    AX_PREPEND_FLAG([-mfloat-abi=hard], [DEVKITPRO_CXXFLAGS])
    AX_PREPEND_FLAG([-mtp=soft],        [DEVKITPRO_CXXFLAGS])
    AX_PREPEND_FLAG([-mtune=mpcore],    [DEVKITPRO_CXXFLAGS])

    AX_PREPEND_FLAG([-specs=3dsx.specs], [DEVKITPRO_LDFLAGS])

    AX_PREPEND_FLAG([-lm],    [DEVKITPRO_LIBS])
    AX_PREPEND_FLAG([-lctru], [DEVKITPRO_LIBS])

    AX_PREPEND_FLAG([-L$LIBCTRU_ROOT/lib], [DEVKITPRO_LIBS])

    # custom Makefile rules
    AX_ADD_AM_MACRO([
clean: clean-3dsx
.PHONY: clean-3dsx
clean-3dsx:; \$(RM) *.3dsx
%.3dsx: %.strip.elf; \$(_3DSXTOOL) \$< \$[@]

clean: clean-smdh
.PHONY: clean-smdh
clean-smdh:; \$(RM) *.smdh
])

])
