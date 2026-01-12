# -*- mode: autoconf -*-
# wiiu_env_wums.m4 - Macros to handle Wii U Module System.
# URL: https://github.com/dkosmari/devkitpro-autoconf/

# Copyright (c) 2025-2026 Daniel K. O. <dkosmari>
#
# Copying and distribution of this file, with or without modification, are permitted in
# any medium without royalty provided the copyright notice and this notice are
# preserved. This file is offered as-is, without any warranty.

#serial 1

# WIIU_ENV_SETUP_WUMS([RELATIVE-PATH-TO-WUMS])
# --------------------------------------------
#
# This macro adjusts compilation flags to create a module for the Wii U Module System
# (WUMS).
#
# Output variables:
#   - `WUMS_DIR'
#   - `WUMS_CPPFLAGS'
#   - `WUMS_LDFLAGS'
#   - `WUMS_LIB'

AC_DEFUN([WIIU_ENV_SETUP_WUMS],[

    AC_REQUIRE([DEVKITPRO_WUT_SETUP])

    # Allow user to override WUMS_DIR while running ./configure
    AC_ARG_VAR([WUMS_DIR], [path to WUMS])
    AS_VAR_SET_IF([WUMS_DIR],
                  [AC_MSG_NOTICE([user override for WUMS_DIR="${WUMS_DIR}"])],
                  [AS_VAR_SET([WUMS_DIR], "m4_default([$1], [${DEVKITPRO}/wums])")])
    AC_SUBST([WUMS_DIR])

    AC_ARG_VAR([WUMS_CPPFLAGS], [preprocessor flags for WUMS])
    AS_VAR_SET([WUMS_CPPFLAGS], ["-D__WUMS__ -I${WUMS_DIR}/include"])
    AC_SUBST([WUMS_CPPFLAGS])

    AC_ARG_VAR([WUMS_LDFLAGS], [linker flags for WUMS])
    AS_VAR_SET([WUMS_LDFLAGS], ["-specs=${WUMS_DIR}/share/wums.specs -T${WUMS_DIR}/share/wums.ld"])
    AC_SUBST([WUMS_LDFLAGS])

    AC_ARG_VAR([WUMS_LIB], [relative path to the WUMS library])
    AS_VAR_SET([WUMS_LIB], ["${WUMS_DIR}/lib/libwums.a"])
    AC_SUBST([WUMS_LIB])

    AC_ARG_VAR([WUMS_DEBUG_LIB], [relative path to the WUMS debug library])
    AS_VAR_SET([WUMS_DEBUG_LIB], ["${WUMS_DIR}/lib/libwumsd.a"])
    AC_SUBST([WUMS_DEBUG_LIB])

    m4_pattern_allow([AM_V_at])

    # Makefile rules for building .wms modules
    AX_ADD_AM_MACRO([
.PHONY: clean-wms

clean: clean-wms

clean-wms:
	\$(RM) *.wms

%.wms: %.strip.elf
	\$(ELF2RPL) \$< \$[@]
	\$([AM_V_at])printf '\xAF\xFE' | dd of=\$[@] bs=1 seek=9 count=2 conv=notrunc status=none

])

    # Makefile rule for building and cleaning WUMS
    AX_ADD_AM_MACRO([
$WUMS_LIB:
	-\$([AM_V_at])\$(MAKE) \$(AM_MAKEFLAGS) -C \$(WUMS_DIR) lib/libwums.a

$WUMS_DEBUG_LIB:
	-\$([AM_V_at])\$(MAKE) \$(AM_MAKEFLAGS) -C \$(WUMS_DIR) lib/libwumsd.a

.PHONY: clean-wiiu-env-WUMS

clean: clean-wiiu-env-WUMS

clean-wiiu-env-WUMS:
	-\$([AM_V_at])\$(MAKE) \$(AM_MAKEFLAGS) -C \$(WUMS_DIR) clean

])

])dnl WIIU_ENV_SETUP_WUMS
