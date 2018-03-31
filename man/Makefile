.PHONY: man1 man8

man1:
	for f in `find . -name pam_panic_pw.1 | sed 's/\/pam_panic_pw.1//'`; do gzip -9 -c $${f}/pam_panic_pw.1 > $(MANDIR)/$${f}/pam_panic_pw.1.gz; done

man8:
	for f in `find . -name pam_panic.8 | sed 's/\/pam_panic.8//'`; do gzip -9 -c $${f}/pam_panic.8 > $(MANDIR)/$${f}/pam_panic.8.gz; done

install:
	make man1
	make man8
