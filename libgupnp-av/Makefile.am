# Version format current:revision:age
# If the library source code has changed at all since the last update, then
# increment revision (‘c:r:a’ becomes ‘c:r+1:a’).
# If any interfaces have been added, removed, or changed since the last update,
# increment current, and set revision to 0.
# If any interfaces have been added since the last public release, then
# increment age.
# If any interfaces have been removed since the last public release, then set
# age to 0.
LTVERSION = 2:0:0

AM_CFLAGS = $(LIBGUPNP_CFLAGS) -I$(top_srcdir)

libgupnp_av_incdir = $(includedir)/gupnp-av-1.0/libgupnp-av

lib_LTLIBRARIES = libgupnp-av-1.0.la

libgupnp_av_inc_HEADERS = gupnp-didl-lite-object.h \
			  gupnp-didl-lite-item.h \
			  gupnp-didl-lite-container.h \
			  gupnp-didl-lite-parser.h \
			  gupnp-didl-lite-resource.h \
			  gupnp-didl-lite-descriptor.h \
			  gupnp-didl-lite-contributor.h \
			  gupnp-didl-lite-writer.h \
			  gupnp-protocol-info.h \
		          gupnp-search-criteria-parser.h \
			  gupnp-last-change-parser.h \
			  gupnp-dlna.h \
			  gupnp-av-error.h \
			  gupnp-av.h


gupnp-av-marshal.c: gupnp-av-marshal.list
	$(GLIB_GENMARSHAL) --prefix=gupnp_av_marshal $(srcdir)/gupnp-av-marshal.list --header --body > gupnp-av-marshal.c

gupnp-av-marshal.h: gupnp-av-marshal.list
	$(GLIB_GENMARSHAL) --prefix=gupnp_av_marshal $(srcdir)/gupnp-av-marshal.list --header > gupnp-av-marshal.h

BUILT_SOURCES = gupnp-av-marshal.c gupnp-av-marshal.h

libgupnp_av_1_0_la_LDFLAGS = -version-info $(LTVERSION) -no-undefined

libgupnp_av_1_0_la_SOURCES = gupnp-didl-lite-object.c \
			     gupnp-didl-lite-object-private.h \
			     gupnp-didl-lite-item.c \
			     gupnp-didl-lite-container.c \
			     gupnp-didl-lite-parser.c \
			     gupnp-didl-lite-resource.c \
			     gupnp-didl-lite-resource-private.h \
			     gupnp-didl-lite-descriptor.c \
			     gupnp-didl-lite-descriptor-private.h \
			     gupnp-didl-lite-contributor.c \
			     gupnp-didl-lite-contributor-private.h \
			     gupnp-didl-lite-writer.c \
			     gupnp-protocol-info.c \
			     gupnp-search-criteria-parser.c \
			     gupnp-last-change-parser.c \
			     gupnp-dlna-private.h \
			     gupnp-dlna.c \
			     gupnp-av-error.c \
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
