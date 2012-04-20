AC_PREREQ([2.63])
AC_INIT([gupnp-av],
        [0.10.2],
        [https://bugzilla.gnome.org/enter_bug.cgi?product=GUPnP],
        [gupnp-av],
        [http://www.gupnp.org/])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_SRCDIR(libgupnp-av/gupnp-av.h)
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])

AM_INIT_AUTOMAKE([1.11 tar-ustar no-dist-gzip dist-xz])

AM_SILENT_RULES([yes])

# Check for programs
AC_PROG_CC
AC_HEADER_STDC
AC_FUNC_MMAP

# Initialize libtool
LT_PREREQ([2.2])
LT_INIT([win32-dll])

PKG_CHECK_MODULES(LIBGUPNP, gupnp-1.0 >= 0.17.0)

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

GTK_DOC_CHECK([1.10],[--flavour no-tmpl])

AC_CONFIG_FILES([
Makefile
libgupnp-av/Makefile
tests/Makefile
tests/gtest/Makefile
doc/Makefile
doc/version.xml
gupnp-av-1.0.pc
gupnp-av-1.0-uninstalled.pc
])
AC_OUTPUT
