#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([dummy-plugin], [1.0])
AC_CONFIG_SRCDIR([src/main.cpp])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])

WIIU_WUPS_INIT
WIIU_WUMS_INIT

AM_INIT_AUTOMAKE([foreign subdir-objects])

AC_PROG_CXX

AC_LANG([C++])

WIIU_WUPS_SETUP
WIIU_WUMS_SETUP
WIIU_WUMS_CHECK_LIBNOTIFICATIONS

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
