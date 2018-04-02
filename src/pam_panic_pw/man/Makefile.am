man1_MANS = man1/pam_panic_pw.1
mande1_MANS = de/man1/pam_panic_pw.1

install-data-local: install-man-de

install-man-de: de/man1/pam_panic_pw.1
	mkdir -p $(mandir)/de/man1
	cp $< $(mandir)/de/man1

.PHONY: install-data-local install-man-de
