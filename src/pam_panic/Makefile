CFLAGS = -Wall --std=gnu11 -O2 -fPIC -DPOWEROFF=\"`which poweroff`\" -DREBOOT=\"`which reboot`\" -DCRYPTSETUP=\"`which cryptsetup`\" -DPPASSFILE=\"$(PPASSFILE)\" 
#LDFLAGS = -x -shared -lcrypt -lpam -lpam_misc
LDFLAGS = -shared -lcrypt -lpam -lpam_misc


OBJ = pam_panic_authdevice.o pam_panic_password.o pam_panic_reject.o


pam_panic: $(OBJ)
	cc $(CFLAGS) $(LDFLAGS) -o ../../build/pam_panic.so pam_panic.c $(OBJ)
#	ld $(LDFLAGS) -o ../../build/pam_panic.so $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

all: 
	mkdir -p ../../build

clean:
	rm *.o
	

#	cc $(CFLAGS) -c pam_panic.c -o ../../obj/pam_panic.o
#	ld $(LDFLAGS) -o ../../build/pam_panic.so ../../obj/pam_panic.o

