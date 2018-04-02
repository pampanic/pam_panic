SUBDIRS = man

securelibdir = $(SECUREDIR)
securelibexecdir = $(securelibdir)

securelibexec_LTLIBRARIES = pam_panic.la
pam_panic_la_SOURCES = \
	pam_panic.c \
	pam_panic_authdevice.c \
	pam_panic_password.c \
	pam_panic_reject.c
pam_panic_la_LDFLAGS = -module -no-undefined -avoid-version
pam_panic_la_LIBADD = -lpam -lcrypto
