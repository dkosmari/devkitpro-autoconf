#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([SDL2 Sound Test], [1.0], [], [sdl2-sound-test])
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

PKG_CHECK_MODULES([SDL2], [sdl2 SDL2_ttf SDL2_mixer])

AX_SUBDIRS_CONFIGURE([external/sdl2xx],
                     [[--disable-image],
                      [--enable-mixer],
                      [--enable-ttf],
                      [--disable-examples],
                      [CPPFLAGS=$CPPFLAGS],
                      [CFLAGS=$CFLAGS],
                      [CXXFLAGS=$CXXFLAGS],
                      [LDFLAGS=$LDFLAGS],
                      [LIBS=$LIBS],
                      [PATH=$PATH]])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
