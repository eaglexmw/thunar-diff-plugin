# $Id: Makefile.am,v 1.5 2013/07/22 13:34:05 setup Exp $

SUBDIRS =	    \
	16x16	    \
	scalable

gtk_update_icon_cache = gtk-update-icon-cache -f -t $(datadir)/icons/hicolor

install-data-hook:
	@-if test -z "$(DESTDIR)"; then					\
                echo "Updating Gtk icon cache.";			\
                $(gtk_update_icon_cache);				\
        else								\
                echo "*** Icon cache not updated. Remember to run:";	\
		echo "***";						\
                echo "***   $(gtk_update_icon_cache)";			\
		echo "***";						\
        fi

# vi:set ts=8 sw=8 noet ai nocindent syntax=automake:
