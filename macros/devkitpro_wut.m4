# SYNOPSIS
#
#   DEVKITPRO_WUT_INIT
#
# DESCRIPTION
#
#   This macro adjusts the environment for Wii U homebrew, using WUT.
#
#   Output variables:
#     - `DEVKITPRO_CFLAGS'
#     - `DEVKITPRO_CPPFLAGS'
#     - `DEVKITPRO_LDFLAGS'
#     - `DEVKITPRO_LIBS'
#     - `DEVKITPRO_RPL_LDFLAGS'
#     - `ELF2RPL': set to `elf2rpl' binary.
#     - `PATH': appends `DEVKITPRO/tools/bin' if necessary.
#     - `WUT_ROOT': set to `DEVKITPRO/wut'
#
# LICENSE
#
#   Copyright (c) 2024 Daniel K. O. <dkosmari>
#
#   Copying and distribution of this file, with or without modification, are permitted in
#   any medium without royalty provided the copyright notice and this notice are
#   preserved. This file is offered as-is, without any warranty.

#serial 1

AC_DEFUN([DEVKITPRO_WUT_INIT],[

    AC_REQUIRE([DEVKITPRO_PPC_INIT])

    # See if we can find elf2rpl in PATH; if not, append $DEVKITPRO/tools/bin to PATH
    AC_MSG_CHECKING([if $DEVKITPRO/tools/bin is in PATH])
    AS_IF([! which elf2rpl 1>/dev/null 2>/dev/null],
          [
              AC_MSG_RESULT([no, will append to PATH])
              AS_VAR_APPEND([PATH], [":$DEVKITPRO/tools/bin"])
              AC_SUBST([PATH])
          ],
          [AC_MSG_RESULT([yes])])

    AC_CHECK_PROGS([ELF2RPL], [elf2rpl])


    # set PORTLIBS_WIIU_ROOT
    AS_VAR_SET([PORTLIBS_WIIU_ROOT], [$PORTLIBS_ROOT/wiiu])
    # AC_SUBST([PORTLIBS_WIIU_ROOT])


    # prepend to PORTLIBS_CPPFLAGS, PORTLIBS_LIBS
    # AS_VAR_SET([PORTLIBS_CPPFLAGS], ["-I$PORTLIBS_WIIU_ROOT/include $PORTLIBS_CPPFLAGS"])
    # AS_VAR_SET([PORTLIBS_LIBS],     ["-L$PORTLIBS_WIIU_ROOT/lib $PORTLIBS_LIBS"])


    # set WUT_ROOT
    AS_VAR_SET([WUT_ROOT], [$DEVKITPRO/wut])
    AC_SUBST([WUT_ROOT])


    # set WUT_CPPFLAGS
    AS_VAR_SET([WUT_CPPFLAGS],
               ["-D__WIIU__ -D__WUT__ -I$WUT_ROOT/include -I$WUT_ROOT/usr/include"])
    # prepend to DEVKITPRO_CPPFLAGS
    AS_VAR_SET([DEVKITPRO_CPPFLAGS], ["$WUT_CPPFLAGS $DEVKITPRO_CPPFLAGS"])

    # set WUT_CFLAGS
    AS_VAR_SET([WUT_CFLAGS], ["-mcpu=750 -meabi -mhard-float"])
    # prepend to DEVKITPRO_CFLAGS
    AS_VAR_SET([DEVKITPRO_CFLAGS], ["$WUT_CFLAGS $DEVKITPRO_CFLAGS"])
    
    # set WUT_LIBS
    AS_VAR_SET([WUT_LIBS], ["-L$WUT_ROOT/lib -L$WUT_ROOT/usr/lib -lwut"])
    # prepend to DEVKITPRO_LIBS
    AS_VAR_SET([DEVKITPRO_LIBS], ["$WUT_LIBS $DEVKITPRO_LIBS"])

    # set WUT_LDFLAGS
    AS_VAR_SET([WUT_LDFLAGS], ["-specs=$WUT_ROOT/share/wut.specs"])
    # prepend to DEVKITPRO_LDFLAGS
    AS_VAR_SET([DEVKITPRO_LDFLAGS], ["$WUT_LDFLAGS $DEVKITPRO_LDFLAGS"])
    
    # set DEVKITPRO_RPL_LDFLAGS
    AS_VAR_SET([DEVKITPRO_RPL_LDFLAGS], ["$WUT_LDFLAGS -specs=$WUT_ROOT/share/rpl.specs"])
    AC_SUBST([DEVKITPRO_RPL_LDFLAGS])

])
