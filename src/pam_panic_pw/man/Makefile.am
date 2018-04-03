cant_believe_its_not_mandir = $(mandir)
nobase_dist_cant_believe_its_not_man_DATA = \
	man1/pam_panic_pw.1.gz \
	de/man1/pam_panic_pw.1.gz

%.gz: %
	gzip -c $< >$@
