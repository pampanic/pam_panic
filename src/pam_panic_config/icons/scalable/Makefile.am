appicondir=$(datadir)/icons/hicolor/scalable/apps
appicon_DATA=pampanic.svg
pixmapsdir=$(datadir)/pixmaps

install-exec-hook:
	test -L $(pixmapsdir)/pampanic.svg && echo "Symlink exists" || { $(MKDIR_P) $(pixmapsdir) ; $(LN_S) $(appicondir)/pampanic.svg $(pixmapsdir)/pampanic.svg ; }
