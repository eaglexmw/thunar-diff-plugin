# $Id: Makefile.am,v 1.3 2013/06/02 14:20:01 setup Exp $

INCLUDES =								\
	-I$(top_builddir)						\
	-I$(top_srcdir)							\
	-DG_LOG_DOMAIN=\"thunar-diff-plugin\"				\
	-DEXO_API_SUBJECT_TO_CHANGE					\
	-DLIBEXECDIR=\"$(libexecdir)\"					\
	-DPACKAGE_LOCALE_DIR=\"$(localedir)\"				\
	$(PLATFORM_CPPFLAGS)

extensionsdir = $(libdir)/thunarx-2
extensions_LTLIBRARIES =						\
	thunar-diff-plugin.la

thunar_diff_plugin_la_SOURCES =						\
	tdp_preference.c						\
	tdp_preference.h						\
	tdp-provider.c							\
	tdp-provider.h							\
	thunar-diff-plugin.c

thunar_diff_plugin_la_CFLAGS =						\
	$(PLATFORM_CFLAGS)						\
	$(THUNARX_CFLAGS)						\
	$(THUNAR_VFS_CFLAGS)

thunar_diff_plugin_la_LDFLAGS =						\
	-avoid-version							\
	-export-dynamic							\
	-module								\
	$(PLATFORM_LDFLAGS)

# vi:set ts=8 sw=8 noet ai nocindent syntax=automake:
