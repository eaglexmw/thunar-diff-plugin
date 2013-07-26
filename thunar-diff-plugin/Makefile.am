# $Id: Makefile.am,v 1.8 2013/07/21 05:12:44 setup Exp $

SUBDIRS =								\
	icons								\
	po                                                              \
	thunar-diff-plugin

distclean-local:
	rm -rf *.spec *.cache *~

rpm: dist
	rpmbuild -ta $(PACKAGE)-$(VERSION).tar.gz
	@rm -f $(PACKAGE)-$(VERSION).tar.gz

EXTRA_DIST = m4/ChangeLog  m4/ChangeLog  config.rpath  m4/ChangeLog  config.rpath \
	THANKS

# vi:set ts=8 sw=8 noet ai nocindent syntax=automake:

ACLOCAL_AMFLAGS = -I m4
