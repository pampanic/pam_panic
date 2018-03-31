LDFLAGS = -lc -lcrypt 
CFLAGS = $(LDFLAGS) --std=gnu11 -Wall -O2 -DPPASSFILE=\"$(PPASSFILE)\"

all:
	mkdir -p ../../build/
	gcc $(CFLAGS) pam_panic_pw.c -o ../../build/pam_panic_pw
