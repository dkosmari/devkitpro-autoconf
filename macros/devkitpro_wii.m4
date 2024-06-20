# -*- mode: autoconf -*-
# devkitpro_wii.m4 - Macros to handle Wii setup.

# Copyright (c) 2024 Daniel K. O. <dkosmari>
#
# Copying and distribution of this file, with or without modification, are permitted in
# any medium without royalty provided the copyright notice and this notice are
# preserved. This file is offered as-is, without any warranty.

#serial 1

# DEVKITPRO_WII_INIT
# ------------------
# This macro adjusts the environment for Wii homebrew.

AC_DEFUN([DEVKITPRO_WII_INIT], [

    AC_REQUIRE([DEVKITPRO_PPC_INIT])

    # See if we can find elf2dol in PATH; if not, append $DEVKITPRO/tools/bin to PATH
    AC_MSG_CHECKING([if $DEVKITPRO/tools/bin is in PATH])
    AS_IF([! which elf2dol 1>/dev/null 2>/dev/null],
          [
              AC_MSG_RESULT([no, will append to PATH])
              AS_VAR_APPEND([PATH], [":$DEVKITPRO/tools/bin"])
              AC_SUBST([PATH])
          ],
          [AC_MSG_RESULT([yes])])

    AC_CHECK_PROGS([ELF2DOL], [elf2dol])
    AC_CHECK_PROGS([GXTEXCONF], [gxtexconv])


    # set LIBOGC_ROOT
    AS_VAR_SET([LIBOGC_ROOT], [$DEVKITPRO/libogc])
    
    # set PORTLIBS_WII_ROOT
    AS_VAR_SET([PORTLIBS_WII_ROOT], [$PORTLIBS_ROOT/wii])


    # See if we need to PORTLIBS_WII_ROOT/bin to PATH
    # TODO: we should actually check the contents of PATH
    AC_MSG_CHECKING([if $PORTLIBS_WII_ROOT/bin is in PATH])
    AS_IF([! which powerpc-eabi-pkg-config 1>/dev/null 2>/dev/null],
          [
              AC_MSG_RESULT([no, will a ppend to PATH])
              AS_VAR_APPEND([PATH], [":$PORTLIBS_WII_ROOT/bin"])
              AC_SUBST([PATH])
          ],
          [AC_MSG_RESULT([yes])])


    AX_PREPEND_FLAG([-D__WII__],                    [DEVKITPRO_CPPFLAGS])
    AX_PREPEND_FLAG([-DGEKKO],                      [DEVKITPRO_CPPFLAGS])
    AX_PREPEND_FLAG([-I$PORTLIBS_WII_ROOT/include], [DEVKITPRO_CPPFLAGS])
    AX_PREPEND_FLAG([-I$LIBOGC_ROOT/include],       [DEVKITPRO_CPPFLAGS])

    AX_PREPEND_FLAG([-mcpu=750],    [DEVKITPRO_CFLAGS])
    AX_PREPEND_FLAG([-meabi],       [DEVKITPRO_CFLAGS])
    AX_PREPEND_FLAG([-mrvl],        [DEVKITPRO_CFLAGS])
    AX_PREPEND_FLAG([-mhard-float], [DEVKITPRO_CFLAGS])

    AX_PREPEND_FLAG([-mcpu=750],    [DEVKITPRO_CXXFLAGS])
    AX_PREPEND_FLAG([-meabi],       [DEVKITPRO_CXXFLAGS])
    AX_PREPEND_FLAG([-mrvl],        [DEVKITPRO_CXXFLAGS])
    AX_PREPEND_FLAG([-mhard-float], [DEVKITPRO_CXXFLAGS])

                            
    AX_PREPEND_FLAG([-logc],         [DEVKITPRO_LIBS])
    AX_PREPEND_FLAG([-laesnd],       [DEVKITPRO_LIBS])
    AX_PREPEND_FLAG([-lasnd],        [DEVKITPRO_LIBS])
    AX_PREPEND_FLAG([-lbte],         [DEVKITPRO_LIBS])
    AX_PREPEND_FLAG([-ldb],          [DEVKITPRO_LIBS])
    AX_PREPEND_FLAG([-ldi],          [DEVKITPRO_LIBS])
    AX_PREPEND_FLAG([-liso9660],     [DEVKITPRO_LIBS])
    AX_PREPEND_FLAG([-lmad],         [DEVKITPRO_LIBS])
    AX_PREPEND_FLAG([-lmodplay],     [DEVKITPRO_LIBS])
    AX_PREPEND_FLAG([-ltinysmb],     [DEVKITPRO_LIBS])
    AX_PREPEND_FLAG([-lwiikeyboard], [DEVKITPRO_LIBS])
    AX_PREPEND_FLAG([-lwiiuse],      [DEVKITPRO_LIBS])
    AX_PREPEND_FLAG([-L$LIBOGC_ROOT/lib/wii],   [DEVKITPRO_LIBS])

    AX_PREPEND_FLAG([-L$PORTLIBS_WII_ROOT/lib], [DEVKITPRO_LIBS])


    # custom Makefile rules
    AX_ADD_AM_MACRO([
clean: clean-dol
.PHONY: clean-dol
clean-dol:; \$(RM) *.dol
%.dol: %.strip.elf; \$(ELF2DOL) \$< \$[@]

clean: clean-tpl
.PHONY: clean-tpl
clean-tpl:; \$(RM) *.tpl
%.tpl: %.scf; \$(GXTEXCONV) -s \$< -o \$[@]
])

])
