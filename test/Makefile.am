AM_CPPFLAGS = -DTEST
bin_PROGRAMS = test
test_SOURCES = ../src/pam_panic/pam_panic_authdevice.c ../src/pam_panic/pam_panic_reject.c ../src/pam_panic_pw/pam_panic_pw.c test.c
test_LDFLAGS = -lpam -lcrypt -lcunit

all:
	@printf "Running test...\n"
	./test
	@printf "Cleaning up...\n"
	$(RM) ./pwfile

