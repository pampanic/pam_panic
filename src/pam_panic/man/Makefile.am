cant_believe_its_not_mandir = $(mandir)
nobase_dist_cant_believe_its_not_man_DATA = \
	man8/pam_panic.8.gz \
	de/man8/pam_panic.8.gz \
	es/man8/pam_panic.8.gz \
	fr/man8/pam_panic.8.gz

%.gz: %
	gzip -c $< >$@
