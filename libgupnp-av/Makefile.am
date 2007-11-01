AM_CFLAGS = $(LIBGUPNP_CFLAGS) -I$(top_srcdir)

libgupnpincdir = $(includedir)/gupnp-av-1.0/libgupnp

lib_LTLIBRARIES = libgupnp-av-1.0.la

libgupnpinc_HEADERS = gupnp-av-main.h \
		      gupnp-av.h

libgupnp_av_1_0_la_SOURCES = gupnp-av-main.c

libgupnp_av_1_0_la_LIBADD = $(LIBGUPNP_LIBS)

CLEANFILES = $(BUILT_SOURCES)
DISTCLEANFILES = $(BUILT_SOURCES)
MAINTAINERCLEANFILES = Makefile.in $(BUILT_SOURCES)
