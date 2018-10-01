ACLOCAL_AMFLAGS = -I m4

AM_CPPFLAGS = -I src

SUBDIRS = po po4a src/pam_panic src/pam_panic_pw

.PHONY: all test clean


update-po:
	make -C po update-po

all: update-po

test:
	make clean
	make -C test
	@printf "Note: If you want to build pam_panic now, you should run 'make clean' first.\n"
