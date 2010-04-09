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
	$(AM_V_GEN) \
	$(GLIB_GENMARSHAL) --prefix=gupnp_av_marshal $(srcdir)/gupnp-av-marshal.list --header --body > gupnp-av-marshal.c

gupnp-av-marshal.h: gupnp-av-marshal.list
	$(AM_V_GEN) \
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

if HAVE_INTROSPECTION

GUPnPAV-1.0.gir: libgupnp-av-1.0.la $(INTROSPECTION_SCANNER)
	$(AM_V_GEN) \
	$(INTROSPECTION_SCANNER) -v \
            --namespace GUPnPAV --nsversion=1.0 \
            --output $@ \
            --strip-prefix= gupnp \
            --libtool="$(LIBTOOL)" \
            --library=gupnp-av-1.0 \
            --include=GObject-2.0 \
            --include=GSSDP-1.0 \
            --include=Soup-2.4 \
            --include=libxml2-2.0 \
	    --include=GUPnP-1.0 \
            --pkg libxml-2.0 \
	    --pkg gobject-2.0 \
	    --pkg gssdp-1.0 \
	    --pkg libsoup-2.4 \
	    -I$(top_srcdir) \
	    $(addprefix $(top_srcdir)/libgupnp-av/, \
			$(libgupnp_av_inc_HEADERS)) \
	    $(top_srcdir)/libgupnp-av/gupnp-didl-lite-object.c \
	    $(top_srcdir)/libgupnp-av/gupnp-didl-lite-item.c \
	    $(top_srcdir)/libgupnp-av/gupnp-didl-lite-container.c \
	    $(top_srcdir)/libgupnp-av/gupnp-didl-lite-parser.c \
	    $(top_srcdir)/libgupnp-av/gupnp-didl-lite-resource.c \
	    $(top_srcdir)/libgupnp-av/gupnp-didl-lite-descriptor.c \
	    $(top_srcdir)/libgupnp-av/gupnp-didl-lite-contributor.c \
	    $(top_srcdir)/libgupnp-av/gupnp-didl-lite-writer.c \
	    $(top_srcdir)/libgupnp-av/gupnp-protocol-info.c \
	    $(top_srcdir)/libgupnp-av/gupnp-search-criteria-parser.c \
	    $(top_srcdir)/libgupnp-av/gupnp-last-change-parser.c \
	    $(top_srcdir)/libgupnp-av/gupnp-dlna.c \
	    $(top_srcdir)/libgupnp-av/gupnp-av-error.c

girdir = $(datadir)/gir-1.0
gir_DATA = GUPnPAV-1.0.gir

typelibdir = $(libdir)/girepository-1.0/
typelib_DATA = $(gir_DATA:.gir=.typelib)

%.typelib: %.gir $(INTROSPECTION_COMPILER)
	$(AM_V_GEN) \
	$(INTROSPECTION_COMPILER) \
		--includedir=$(srcdir) \
		--includedir=. \
		$(INTROSPECTION_COMPILER_OPTS) $< -o $(@F)

endif

CLEANFILES = $(BUILT_SOURCES) $(gir_DATA) $(typelib_DATA)
DISTCLEANFILES = $(BUILT_SOURCES)
MAINTAINERCLEANFILES = Makefile.in $(BUILT_SOURCES)

