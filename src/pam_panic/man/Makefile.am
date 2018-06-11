cant_believe_its_not_mandir = $(mandir)
securelibdir = $(SECUREDIR)
nobase_dist_cant_believe_its_not_man_DATA = \
	man8/pam_panic.8.gz \
	de/man8/pam_panic.8.gz \
	es/man8/pam_panic.8.gz \
	fr/man8/pam_panic.8.gz \
	ja/man8/pam_panic.8.gz

%.gz: %
	{ sldr=$$(echo "$(securelibdir)" | $(SED) 's/\//\\\//g') ; bdr=$$(echo "$(bindir)" | $(SED) 's/\//\\\//g') ; $(SED) "s/__PAMPANICSO__/$$sldr\/pam_panic\\\\\&.so/; s/__PAMPANICPW__/$$bdr\/pam_panic_pw/" $< >$<.tmp ; }
	gzip -c $<.tmp >$@


clean-local:
	rm ./*/*.tmp ./*/*.gz ./*/*/*.tmp ./*/*/*.gz
