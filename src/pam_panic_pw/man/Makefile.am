cant_believe_its_not_mandir = $(mandir)
nobase_dist_cant_believe_its_not_man_DATA = \
	man1/pam_panic_pw.1.gz \
	de/man1/pam_panic_pw.1.gz

%.gz: %
	{ ppf=$$(echo "$(PPASSFILE)" | $(SED) 's/\//\\\//g') ; bdr=$$(echo "$(bindir)" | $(SED) 's/\//\\\//g') ; $(SED) "s/__PPASSFILE__/$$ppf/; s/__PAMPANICPW__/$$bdr\/pam_panic\\\\\&.so/" $< >$<.tmp ; }
	gzip -c $<.tmp >$@


clean-local:
	rm ./*/*.tmp ./*/*.gz ./*/*/*.tmp ./*/*/*.g
