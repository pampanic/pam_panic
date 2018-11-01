SUBDIRS = man data

securelibdir = $(SECUREDIR)
securelibexecdir = $(securelibdir)

AM_CPPFLAGS = -DLOCALEDIR='"$(localedir)"'

securelibexec_LTLIBRARIES = pam_panic.la
pam_panic_la_SOURCES = \
	pam_panic.c \
	pam_panic.h \
	pam_panic_authdevice.c \
	pam_panic_authdevice.h \
	pam_panic_password.c \
	pam_panic_password.h \
	pam_panic_reject.c \
	pam_panic_reject.h
pam_panic_la_LDFLAGS = -module -no-undefined -avoid-version
pam_panic_la_LIBADD = -lpam -lcrypt

install-exec-hook:
	rm -f $(securelibdir)/pam_panic.la
