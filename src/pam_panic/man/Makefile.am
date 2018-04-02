dist_man8_MANS = man8/pam_panic.8
dist_data_DATA = \
	de/man8/pam_panic.8 \
	es/man8/pam_panic.8 \
	fr/man8/pam_panic.8

install-data-local: install-man-de install-man-es install-man-fr

install-man-de: de/man8/pam_panic.8
	mkdir -p $(mandir)/de/man8
	cp $< $(mandir)/de/man8

install-man-es: es/man8/pam_panic.8
	mkdir -p $(mandir)/es/man8
	cp $< $(mandir)/es/man8

install-man-fr: fr/man8/pam_panic.8
	mkdir -p $(mandir)/fr/man8
	cp $< $(mandir)/fr/man8

.PHONY: install-data-local install-man-de install-man-es install-man-fr
