CFLAGS = --std=c99 -O2 -fPIC -DPOWEROFF=\"`which poweroff`\" -DREBOOT=\"`which reboot`\" -DCRYPTSETUP=\"`which cryptsetup`\"
LDFLAGS = -x --shared

all:
	mkdir -p ../obj
	mkdir -p ../build
	cc $(CFLAGS) -c pam_panic.c -o ../obj/pam_panic.o
	ld $(LDFLAGS) -o ../build/pam_panic.so ../obj/pam_panic.o

