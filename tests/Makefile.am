SUBDIRS = gtest

AM_CFLAGS = $(LIBGUPNP_CFLAGS) -I$(top_srcdir)

noinst_PROGRAMS = test-search-criteria-parser
test_search_criteria_parser_SOURCES = test-search-criteria-parser.c
test_search_criteria_parser_LDADD = \
			$(top_builddir)/libgupnp-av/libgupnp-av-1.0.la \
			$(LIBGUPNP_LIBS)

check_PROGRAMS = check-search
check_search_SOURCES = check-search.c
check_search_LDADD = $(top_builddir)/libgupnp-av/libgupnp-av-1.0.la \
		     $(LIBGUPNP_LIBS)

TESTS = $(check_PROGRAMS)
