fuckyou_automake_mandir = $(mandir)
nobase_dist_fuckyou_automake_man_DATA = \
	man8/pam_panic.8.gz \
	de/man8/pam_panic.8.gz \
	es/man8/pam_panic.8.gz \
	fr/man8/pam_panic.8.gz

%.gz: %
	gzip -c $< >$@
