SUBDIRS = applications icons/48x48 icons/64x64 icons/scalable
bin_SCRIPTS = pam_panic_config

%: %.prebuild.sh
	{ sldr=$$(echo "$(SECUREDIR)" | $(SED) 's/\//\\\//g'); bdr=$$(echo "$(bindir)" | $(SED) 's/\//\\\//g'); version=$$(echo $(VERSION)); $(SED) "s/__SECURELIBDIR__/$$sldr/;s/__PAMPANICPW__/$$bdr\/pam_panic_pw/;s/__VERSION__/$$version/" $< > $@ ; }
	chmod +x $@

clean-local:
	$(RM) pam_panic_config
