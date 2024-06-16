# -*- mode: autoconf -*-
# wiiu-wums.m4 - Macros to handle Wii U Module System.

# Copyright (c) 2024 Daniel K. O. <dkosmari>
#
# Copying and distribution of this file, with or without modification, are permitted in
# any medium without royalty provided the copyright notice and this notice are
# preserved. This file is offered as-is, without any warranty.

#serial 1

# WIIU_WUMS_INIT
# --------------
# This macro adjusts the environment for the Wii U Module System (WUMS).
#
# Output variables:
#   - `DEVKITPRO_CPPFLAGS'
#   - `DEVKITPRO_LIBS'

AC_DEFUN([WIIU_WUMS_INIT],[

    AC_REQUIRE([DEVKITPRO_WUT_INIT])

    # set WIIU_WUMS_ROOT
    AS_VAR_SET([WIIU_WUMS_ROOT], [$DEVKITPRO/wums])

    AX_PREPEND_FLAG([-I$WIIU_WUMS_ROOT/include], [DEVKITPRO_CPPFLAGS])

    AX_PREPEND_FLAG([-L$WIIU_WUMS_ROOT/lib], [DEVKITPRO_LIBS])

])


# WIIU_WUMS_CHECK_LIB_CURLWRAPPER
# -------------------------------
# Checks for presence of libcurlwrapper.
#
# Output variables:
#   - `DEVKITPRO_LIBS'

AC_DEFUN([WIIU_WUMS_CHECK_LIB_CURLWRAPPER],[

    AC_REQUIRE([WIIU_WUMS_INIT])

    AX_VAR_PUSHVALUE([CPPFLAGS], [$DEVKITPRO_CPPFLAGS $CPPFLAGS])
    AX_VAR_PUSHVALUE([LIBS], [$DEVKITPRO_LIBS $LIBS])

    AX_CHECK_LIBRARY([WIIU_WUMS_LIB_CURLWRAPPER],
                     [curl/curl.h],
                     [curlwrapper],
                     [AX_PREPEND_FLAG([-lcurlwrapper], [DEVKITPRO_LIBS])],
                     [AC_MSG_ERROR([libcurlwrapper not found in $WIIU_WUMS_ROOT; get it from https://github.com/wiiu-env/libcurlwrapper])])

    AX_VAR_POPVALUE([LIBS])
    AX_VAR_POPVALUE([CPPFLAGS])
])


# WIIU_WUMS_CHECK_LIB_NOTIFICATIONS
# ---------------------------------
# Checks for presence of libnotifications.
#
# Output variables:
#   - `DEVKITPRO_LIBS'

AC_DEFUN([WIIU_WUMS_CHECK_LIB_NOTIFICATIONS],[

    AC_REQUIRE([WIIU_WUMS_INIT])

    AX_VAR_PUSHVALUE([CPPFLAGS], [$DEVKITPRO_CPPFLAGS $CPPFLAGS])
    AX_VAR_PUSHVALUE([LIBS], [$DEVKITPRO_LIBS $LIBS])

    AX_CHECK_LIBRARY([WIIU_WUMS_LIB_NOTIFICATIONS],
                     [notifications/notifications.h],
                     [notifications],
                     [AX_PREPEND_FLAG([-lnotifications], [DEVKITPRO_LIBS])],
                     [AC_MSG_ERROR([libnotifications not found in $WIIU_WUMS_ROOT; get it from https://github.com/wiiu-env/libnotifications])])

    AX_VAR_POPVALUE([LIBS])
    AX_VAR_POPVALUE([CPPFLAGS])

])
