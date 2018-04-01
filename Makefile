PREFIX = /usr/local/bin

PPASSPATH = /var/lib/pam_panic
PPASSFILE = $(PPASSPATH)/auth

POSSIBLE_PAMDIRS = /lib/x86_64-linux-gnu/security /lib/security
PAMDIR = $(shell make detect_pamdir)
MANDIR = /usr/share/man

all:
	@which reboot >/dev/null
	@which poweroff >/dev/null
	@which cryptsetup >/dev/null
	mkdir -p build
	make -C src -e "PPASSFILE = $(PPASSFILE)"
	@printf "Done!\n"

clean:
	rm build/pam_panic.so
	rm build/pam_panic_pw
	make -C src/pam_panic clean
	rmdir build
	@printf "Done!\n"

detect_pamdir: 
	@for d in $(POSSIBLE_PAMDIRS); do if [ ! -d $${d} ]; then continue; else printf "$${d}\n"; fi; done

purge:
	rm /var/lib/pam_panic/*
	rmdir /var/lib/pam_panic
	@printf "Done!\n"

uninstall:
	PAMDIR=$(PAMDIR); rm $${PAMDIR}/pam_panic.so
	rm /usr/share/man/*/pam_panic.8.gz
	rm /usr/share/man/*/pam_panic_pw.1.gz
	rm $(PREFIX)/pam_panic_pw
	@printf "Done!\n"

install:
	PAMDIR=$(PAMDIR); if [ -z $${PAMDIR} ]; then printf "Error: PAM's shared object directory was not detected. If you know where it is, please add to POSSIBLE_PAMDIRS and make a pull request.\n"&& exit 1; else cp build/pam_panic.so $${PAMDIR}/pam_panic.so; fi
	if [ ! -d $(MANDIR) ]; then printf "Error: Where is the manpage directory?\n" && exit 1; else make install -C man -e "MANDIR = $(MANDIR)"; fi
	mkdir -p $(PPASSPATH)
	cp build/pam_panic_pw $(PREFIX)/pam_panic_pw
	@printf "Done!\n"

test:
	@printf "We only can test it in production, yet.\n"
