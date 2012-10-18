SUBDIRS = gtest

common_cflags = $(LIBGUPNP_CFLAGS) -I$(top_srcdir)

noinst_PROGRAMS = test-search-criteria-parser
test_search_criteria_parser_SOURCES = test-search-criteria-parser.c
test_search_criteria_parser_LDADD = \
			$(top_builddir)/libgupnp-av/libgupnp-av-1.0.la \
			$(LIBGUPNP_LIBS)
test_search_criteria_parser_CFLAGS = $(common_cflags)

check_PROGRAMS = check-search fragments
check_search_SOURCES = check-search.c
check_search_LDADD = $(top_builddir)/libgupnp-av/libgupnp-av-1.0.la \
		     $(LIBGUPNP_LIBS)
check_search_CFLAGS = $(common_cflags)

fragments_SOURCES = fragments.c
fragments_LDADD = $(top_builddir)/libgupnp-av/libgupnp-av-1.0.la \
		  $(LIBGUPNP_LIBS)
fragments_CFLAGS = $(common_cflags) -DABS_TOP_SRCDIR="\"$(abs_top_srcdir)\""

TESTS = $(check_PROGRAMS)
