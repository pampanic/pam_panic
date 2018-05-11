CFLAGS += -DTEST
bin_PROGRAMS = test
test_SOURCES = test.h ../src/pam_panic/pam_panic_authdevice.c ../src/pam_panic/pam_panic_reject.c test.c
test_LDFLAGS = -lpam -lcunit

all:
	@printf "Running test...\n"
	./test
