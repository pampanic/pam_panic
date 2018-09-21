AC_INIT(
    [pam_panic],
    [0.2.0],
    [https://github.com/pampanic/pam_panic/issues],
    [pam_panic])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([-Wall subdir-objects])
AC_PREREQ([2.69])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADER([src/config.h])
AC_CANONICAL_HOST

AC_SUBST(PACKAGE)
AC_SUBST(VERSION)

AC_USE_SYSTEM_EXTENSIONS

AC_PROG_CC
CFLAGS+=" -std=gnu11"

AM_PROG_AR

AM_GNU_GETTEXT_VERSION(0.19.8)
AM_GNU_GETTEXT([external])

AC_PATH_PROG([REBOOT], [reboot])
AC_DEFINE_UNQUOTED([REBOOT], "$REBOOT",
    [path to the reboot command])
AC_PATH_PROG([POWEROFF], [poweroff])
AC_DEFINE_UNQUOTED([POWEROFF], "$POWEROFF",
    [path to the poweroff command])
AC_PATH_PROG([CRYPTSETUP], [cryptsetup])
AC_DEFINE_UNQUOTED([CRYPTSETUP], "$CRYPTSETUP",
    [path to the cryptsetup command])

AC_ARG_ENABLE(securedir,
    AS_HELP_STRING([--enable-securedir=DIR],
        [path to location of PAMs @<:@default=LIBDIR/security@:>@]),
    SECUREDIR=$enableval, SECUREDIR=$libdir/security)
AC_SUBST(SECUREDIR)

AC_ARG_ENABLE(ppassfile,
    AS_HELP_STRING([--enable-ppassfile=FILE],
        [path to file to be used for storing passwords @<:@default=/var/lib/pam_panic/auth@:>@]),
    PPASSFILE=$enableval, PPASSFILE=/var/lib/pam_panic/auth)
AC_DEFINE_UNQUOTED([PPASSFILE], "$PPASSFILE",
    [path to file to be used for storing passwords])
AC_SUBST(PPASSFILE)

LT_INIT([disable-static])
AC_ENABLE_STATIC([no])
AC_ENABLE_SHARED([yes])

AC_CONFIG_FILES([
    Makefile
    po/Makefile.in
    src/pam_panic/Makefile
    src/pam_panic/man/Makefile
    src/pam_panic_pw/Makefile
    src/pam_panic_pw/man/Makefile
    test/Makefile
])

AC_OUTPUT
