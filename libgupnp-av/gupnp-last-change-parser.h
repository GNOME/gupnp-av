/*
 * Copyright (C) 2007 Zeeshan Ali (Khattak) <zeeshanak@gnome.org>
 * Copyright (C) 2007 OpenedHand Ltd
 *
 * Authors: Zeeshan Ali Khattak <zeenix@gstreamer.net>
 *          Jorn Baayen <jorn@openedhand.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */

#ifndef GUPNP_LAST_CHANGE_PARSER_H
#define GUPNP_LAST_CHANGE_PARSER_H

#include <glib-object.h>

G_BEGIN_DECLS

G_DECLARE_DERIVABLE_TYPE (GUPnPLastChangeParser,
                          gupnp_last_change_parser,
                          GUPNP,
                          LAST_CHANGE_PARSER,
                          GObject)

#define GUPNP_TYPE_LAST_CHANGE_PARSER (gupnp_last_change_parser_get_type ())

struct _GUPnPLastChangeParserClass {
        GObjectClass parent_class;

        /* future padding */
        void (* _gupnp_reserved1) (void);
        void (* _gupnp_reserved2) (void);
        void (* _gupnp_reserved3) (void);
        void (* _gupnp_reserved4) (void);
};

GUPnPLastChangeParser *
gupnp_last_change_parser_new           (void);

gboolean
gupnp_last_change_parser_parse_last_change_valist
                                       (GUPnPLastChangeParser *parser,
                                        guint                  instance_id,
                                        const char            *last_change_xml,
                                        GError               **error,
                                        va_list                var_args);

gboolean
gupnp_last_change_parser_parse_last_change
                                       (GUPnPLastChangeParser *parser,
                                        guint                  instance_id,
                                        const char            *last_change_xml,
                                        GError               **error,
                                        ...) G_GNUC_NULL_TERMINATED;

G_END_DECLS

#endif /* __GUPNP_LAST_CHANGE_PARSER_H__ */

