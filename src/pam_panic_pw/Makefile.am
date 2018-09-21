SUBDIRS = man

AM_CFLAGS = -DLOCALEDIR='"$(localedir)"'

bin_PROGRAMS = pam_panic_pw
pam_panic_pw_SOURCES = pam_panic_pw.c pam_panic_pw.h
pam_panic_pw_LDFLAGS = -lcrypt
