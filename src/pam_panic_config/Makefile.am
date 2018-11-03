SUBDIRS = applications icons/48x48 icons/64x64 icons/scalable
bin_SCRIPTS = pam_panic_config

%: %.prebuild
	{ sldr=$$(echo "$(SECUREDIR)" | $(SED) 's/\//\\\//g');	$(SED) "s/__SECURELIBDIR__/$$sldr/" $< > $@ ; }

clean-local:
	$(RM) *.tmp
