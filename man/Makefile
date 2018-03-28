install:
	for f in `find . -name pam_panic.8 | sed 's/\/pam_panic.8//'`; do gzip -9 -c $${f}/pam_panic.8 > $(MANDIR)/$${f}/pam_panic.8.gz; done
