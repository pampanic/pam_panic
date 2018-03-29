POSSIBLE_PAMDIRS = /lib/x86_64-linux-gnu/security /lib/security
PAMDIR = $(shell make detect_pamdir)
MANDIR = /usr/share/man

all:
	@which reboot >/dev/null
	@which poweroff >/dev/null
	@which cryptsetup >/dev/null
	mkdir -p build obj
	gcc -fPIC --std=c99 -DPOWEROFF=\"`which poweroff`\" -DREBOOT=\"`which reboot`\" -DCRYPTSETUP=\"`which cryptsetup`\" -c src/pam_panic.c -o obj/pam_panic.o
	ld -x --shared -o build/pam_panic.so obj/pam_panic.o
	@printf "Done!\n"
clean:
	rm build/pam_panic.so
	rm obj/pam_panic.o
	rmdir build obj
	@printf "Done!\n"

detect_pamdir: 
	@for d in $(POSSIBLE_PAMDIRS); do if [ ! -d $${d} ]; then continue; else printf "$${d}\n"; fi; done

uninstall:
	PAMDIR=$(PAMDIR); rm $${PAMDIR}/pam_panic.so
	rm /usr/share/man/*/pam_panic.8.gz
	@printf "Done!\n"

install:
	PAMDIR=$(PAMDIR); if [ -z $${PAMDIR} ]; then printf "Error: PAM's shared object directory was not detected. If you know where it is, please add to POSSIBLE_PAMDIRS and make a pull request.\n"&& exit 1; else cp build/pam_panic.so $${PAMDIR}/pam_panic.so; fi
	if [ ! -d $(MANDIR) ]; then printf "Error: Where is the manpage directory?\n" && exit 1; else make install -C man -e "MANDIR = $(MANDIR)"; fi
	@printf "Done!\n"
