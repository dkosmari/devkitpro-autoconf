#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([SDL2 Bouncing Box], [1.0])
AC_CONFIG_SRCDIR([src/main.cpp])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_HEADERS([config.h])

DEVKITPRO_WII_INIT

AM_INIT_AUTOMAKE([foreign subdir-objects])

DEVKITPRO_WII_SETUP

AC_PROG_CXX
AC_LANG([C++])

AC_CHECK_TOOL([LD], [ld])

PKG_CHECK_MODULES([SDL2], [sdl2 SDL2_mixer])


AC_ARG_ENABLE([assets],
              [AS_HELP_STRING([--disable-assets],
                              [disable bundling assets into the final executable])],
              [],
              [enable_assets=yes])

AM_CONDITIONAL([ASSETS], [test "x$enable_assets" = "xyes"])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
