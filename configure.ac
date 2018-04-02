AC_INIT([pam_panic], [0.1.0], [], [pam_panic])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([-Wall])
AC_PREREQ([2.69])
AC_CONFIG_MACRO_DIR([m4])
AC_CANONICAL_HOST

AC_SUBST(PACKAGE)
AC_SUBST(VERSION)

AC_USE_SYSTEM_EXTENSIONS
AC_PROG_CC
AM_PROG_AR

AC_PATH_PROG([REBOOT], [reboot])
AC_SUBST(REBOOT)
AC_PATH_PROG([POWEROFF], [poweroff])
AC_SUBST(POWEROFF)
AC_PATH_PROG([CRYPTSETUP], [cryptsetup])
AC_SUBST(CRYPTSETUP)

AC_ARG_ENABLE(securedir,
    AS_HELP_STRING([--enable-securedir=DIR],
        [path to location of PAMs @<:@default=$libdir/security@:>@]),
    SECUREDIR=$enableval, SECUREDIR=$libdir/security)
AC_SUBST(SECUREDIR)

AC_ARG_ENABLE(ppassfile,
    AS_HELP_STRING([--enable-ppassfile=FILE],
        [path to file to be used for storing passwords @<:@default=$sysconfdir/pam_panic/auth@:>@]),
    PPASSFILE=$enableval, PPASSFILE=$sysconfdir/pam_panic/auth)
AC_SUBST(PPASSFILE)

LT_INIT([disable-static])
AC_ENABLE_STATIC([no])
AC_ENABLE_SHARED([yes])

AC_CONFIG_FILES([
    Makefile
    src/pam_panic/config.h
    src/pam_panic/Makefile
    src/pam_panic/man/Makefile
    src/pam_panic_pw/config.h
    src/pam_panic_pw/Makefile
    src/pam_panic_pw/man/Makefile
])

AC_OUTPUT
