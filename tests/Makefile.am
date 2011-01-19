AM_CFLAGS = $(LIBGUPNP_CFLAGS) -I$(top_srcdir)
AM_LDFLAGS = $(top_builddir)/libgupnp-av/libgupnp-av-1.0.la \
	     $(LIBGUPNP_LIBS)

noinst_PROGRAMS = test-search-criteria-parser
test_search_criteria_parser_SOURCES = test-search-criteria-parser.c

check_PROGRAMS = check-search
check_search_SOURCES = check-search.c

TESTS = $(check_PROGRAMS)
