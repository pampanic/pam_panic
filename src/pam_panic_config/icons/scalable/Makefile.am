appicondir=$(datadir)/icons/hicolor/scalable/apps
appicon_DATA=pampanic.svg
pixmapsdir=$(datadir)/pixmaps

install-exec-hook:
	$(MKDIR_P) $(pixmapsdir)
	$(LN_S) $(appicondir)/pampanic.svg $(pixmapsdir)/pampanic.svg
