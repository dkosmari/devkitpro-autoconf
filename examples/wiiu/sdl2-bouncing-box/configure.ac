#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([sdl2-bouncing-box], [1.0])
AC_CONFIG_SRCDIR([src/main.cpp])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])

DEVKITPRO_WUT_INIT

AM_INIT_AUTOMAKE([foreign])

AC_PROG_CXX
AC_LANG([C++])


PKG_CHECK_MODULES([SDL2], [sdl2])


AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
