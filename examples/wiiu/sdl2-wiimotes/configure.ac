#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([SDL2 Wiimotes Sample], [1.0], [], [sdl2-wiimotes])
AC_CONFIG_SRCDIR([src/main.cpp])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_HEADERS([config.h])

DEVKITPRO_WUT_INIT

AM_INIT_AUTOMAKE([foreign subdir-objects])


AC_PROG_CXX
AX_APPEND_COMPILE_FLAGS([-std=c++23], [CXX])
AC_LANG([C++])

DEVKITPRO_WUT_SETUP

PKG_CHECK_MODULES([SDL2], [sdl2 SDL2_ttf])


AC_CONFIG_FILES([Makefile])
AC_OUTPUT
