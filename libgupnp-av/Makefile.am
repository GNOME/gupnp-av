AM_CFLAGS = $(LIBGUPNP_CFLAGS) -I$(top_srcdir)

libgupnp_av_incdir = $(includedir)/gupnp-av-1.0/libgupnp-av

lib_LTLIBRARIES = libgupnp-av-1.0.la

libgupnp_av_inc_HEADERS = gupnp-av-main.h \
		          gupnp-media-server.h \
		      	  gupnp-media-server-proxy.h \
			  gupnp-media-renderer-proxy.h \
			  gupnp-didl-lite-object.h \
			  gupnp-didl-lite-parser.h \
			  gupnp-didl-lite-writer.h \
		          gupnp-search-criteria-parser.h \
			  gupnp-av-util.h \
			  gupnp-av.h


gupnp-av-marshal.c: gupnp-av-marshal.list
	$(GLIB_GENMARSHAL) --prefix=gupnp_av_marshal $(srcdir)/gupnp-av-marshal.list --header --body > gupnp-av-marshal.c

gupnp-av-marshal.h: gupnp-av-marshal.list
	$(GLIB_GENMARSHAL) --prefix=gupnp_av_marshal $(srcdir)/gupnp-av-marshal.list --header > gupnp-av-marshal.h

BUILT_SOURCES = gupnp-av-marshal.c gupnp-av-marshal.h

libgupnp_av_1_0_la_SOURCES = gupnp-av-main.c \
			     gupnp-av-protocol.h \
		             gupnp-media-server.c \
		             gupnp-media-server-proxy.c \
			     gupnp-media-renderer-proxy.c \
			     gupnp-didl-lite-object.c \
			     gupnp-didl-lite-parser.c \
			     gupnp-didl-lite-writer.c \
			     gupnp-search-criteria-parser.c \
			     gupnp-av-util.c \
			     xml-util.c \
			     xml-util.h \
			     gvalue-util.c \
			     gvalue-util.h \
			     $(BUILT_SOURCES)

libgupnp_av_1_0_la_LIBADD = $(LIBGUPNP_LIBS)

EXTRA_DIST = gupnp-av-marshal.list

CLEANFILES = $(BUILT_SOURCES)
DISTCLEANFILES = $(BUILT_SOURCES)
MAINTAINERCLEANFILES = Makefile.in $(BUILT_SOURCES)
