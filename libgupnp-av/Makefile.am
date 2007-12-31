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
			  gupnp-av-util.h \
			  gupnp-av.h

libgupnp_av_1_0_la_SOURCES = gupnp-av-main.c \
			     gupnp-av-protocol.h \
		             gupnp-media-server.c \
		             gupnp-media-server-proxy.c \
			     gupnp-media-renderer-proxy.c \
			     gupnp-didl-lite-object.c \
			     gupnp-didl-lite-parser.c \
			     gupnp-didl-lite-writer.c \
			     gupnp-av-util.c \
			     xml-util.c \
			     xml-util.h \
			     gvalue-util.c \
			     gvalue-util.h

libgupnp_av_1_0_la_LIBADD = $(LIBGUPNP_LIBS)

CLEANFILES = $(BUILT_SOURCES)
DISTCLEANFILES = $(BUILT_SOURCES)
MAINTAINERCLEANFILES = Makefile.in $(BUILT_SOURCES)
