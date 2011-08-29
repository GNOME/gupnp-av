AC_PREREQ(2.53)
AC_INIT(gupnp-av, 0.10.0, http://www.gupnp.org/)
AM_INIT_AUTOMAKE()
AC_CONFIG_SRCDIR(libgupnp-av/gupnp-av.h)
AM_CONFIG_HEADER(config.h)

m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])],)

AC_ISC_POSIX
AC_PROG_CC
AC_STDC_HEADERS
AC_LIBTOOL_WIN32_DLL
AC_PROG_LIBTOOL
AC_CONFIG_MACRO_DIR([m4])
AC_FUNC_MMAP

PKG_CHECK_MODULES(LIBGUPNP, gupnp-1.0 >= 0.13)

# glib-genmarshal
GLIB_GENMARSHAL=`pkg-config --variable=glib_genmarshal glib-2.0`
AC_SUBST(GLIB_GENMARSHAL)

# Debugging
AC_ARG_ENABLE(debug,
	[  --enable-debug          enable debugging],,
        enable_debug=no)
if test "x$enable_debug" = "xyes"; then
        CFLAGS="$CFLAGS -g -Wall"
fi

GOBJECT_INTROSPECTION_CHECK([0.9.5])

GTK_DOC_CHECK([1.0])

AC_OUTPUT([
Makefile
libgupnp-av/Makefile
tests/Makefile
doc/Makefile
doc/version.xml
gupnp-av-1.0.pc
gupnp-av-1.0-uninstalled.pc
])
