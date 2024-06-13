# Macro to check for the location of devkitPro
#
# SYNOPSIS
#
#   DEVKITPRO_INIT
#
# DESCRIPTION
#
#   This macro declares DEVKITPRO as a precious variable, and overrides it with the
#   `--with-devkitpro' option.
#
#   The variables `PORTLIBS_CPPFLAGS' and `PORTLIBS_LIBS' are also declared precious, but
#   left unmodified. Other toolchain macros will append flags to these variables.
#
# LICENSE
#
#   Copyright (c) 2024 Daniel K. O. <dkosmari>
#
#   Copying and distribution of this file, with or without modification, are permitted in
#   any medium without royalty provided the copyright notice and this notice are
#   preserved. This file is offered as-is, without any warranty.

#serial 1

AC_DEFUN([DEVKITPRO_INIT],[

    AC_REQUIRE([AC_CANONICAL_HOST])

    # make DEVKITPRO precious
    AC_ARG_VAR([DEVKITPRO], [path to devkitPro])

    # --with-devkitpro
    AC_ARG_WITH([devkitpro],
                [AS_HELP_STRING([--with-devkitpro=PATH-TO-DEVKITPRO],
                                [Set the base path to devkitPro. This overrides the variable DEVKITPRO])],
                [AS_VAR_SET([DEVKITPRO], [$withval])])

    AC_MSG_CHECKING([devkitPro path])

    AS_VAR_SET_IF([DEVKITPRO],
                  [
                      AC_MSG_RESULT([$DEVKITPRO])
                  ],
                  [
                      AC_MSG_RESULT([not found])
                      AC_MSG_ERROR([You need either `--with-devkitpro=PATH-TO-DEVKITPRO' or the `DEVKITPRO' variable.])
                  ])


    # set PORTLIBS_ROOT
    AS_VAR_SET([PORTLIBS_ROOT], [$DEVKITPRO/portlibs])
    AC_SUBST([PORTLIBS_ROOT])

    # make PORTLIBS_CPPFLAGS and PORTLIBS_LIBS precious
    AC_ARG_VAR([PORTLIBS_CPPFLAGS], [flags to find portlibs headers])
    AC_ARG_VAR([PORTLIBS_LIBS], [flags to link against portlibs libraries])

])
