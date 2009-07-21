#! /bin/sh
gtkdocize --flavour no-tmpl || exit 1
autoreconf -v --install || exit 1
glib-gettextize --force --copy || exit 1
./configure --enable-maintainer-mode --enable-debug "$@"
