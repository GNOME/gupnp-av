TESTS_ENVIRONMENT = G_SLICE=debug-blocks \
		    LD_LIBRARY_PATH=$(top_builddir)/libgssdp/.libs

TESTS=$(check_PROGRAMS)

check_PROGRAMS = \
	test-regression \
	test-media-collection

test_regression_SOURCES = test-regression.c

test_media_collection_SOURCES =  \
	test-media-collection.c

LDADD = \
	$(top_builddir)/libgupnp-av/libgupnp-av-1.0.la \
	$(LIBGUPNP_LIBS)

AM_CFLAGS = \
	    $(LIBGUPNP_CFLAGS) \
	    -I $(top_srcdir) \
	    -DDATA_PATH="\"$(srcdir)\""

