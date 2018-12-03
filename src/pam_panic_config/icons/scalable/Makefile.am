appicondir=$(datadir)/icons/hicolor/scalable/apps
pixmapsdir=$(datadir)/pixmaps

install:
	$(MKDIR_P) $(appicondir)
	$(install_sh_DATA) pampanic.svg $(appicondir)/
	test -L $(pixmapsdir)/pampanic.svg && echo "Symlink exists" || { $(MKDIR_P) $(pixmapsdir) ; $(LN_S) $(appicondir)/pampanic.svg $(pixmapsdir)/pampanic.svg ; }
