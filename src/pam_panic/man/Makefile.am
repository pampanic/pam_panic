cant_believe_its_not_mandir = $(mandir)
securelibdir = $(SECUREDIR)
nobase_dist_cant_believe_its_not_man_DATA = \
	man8/pam_panic.8.gz \
	de/man8/pam_panic.8.gz \
	es/man8/pam_panic.8.gz \
	fr/man8/pam_panic.8.gz \
	ja/man8/pam_panic.8.gz \
	nl/man8/pam_panic.8.gz \
	pl/man8/pam_panic.8.gz

%.gz: %
	{ sldr=$$(echo "$(securelibdir)" | $(SED) 's/\//\\\//g') ; bdr=$$(echo "$(bindir)" | $(SED) 's/\//\\\//g') ; $(SED) "s/__PAMPANICSO__/$$sldr\/pam_panic\\\\\&.so/; s/__PAMPANICPW__/$$bdr\/pam_panic_pw/" $< >$<.tmp ; }
	gzip -c $<.tmp >$@
	rm $<.tmp


clean-local:
	$(RM) ./*/*.gz
	$(FIND) . -type d -not -name man8 -not -path . -exec rm -rf {} +
