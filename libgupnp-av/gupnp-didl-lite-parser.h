/*
 * Copyright (C) 2007 Zeeshan Ali (Khattak) <zeeshanak@gnome.org>
 *
 * Authors: Zeeshan Ali (Khattak) <zeeshanak@gnome.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */

#ifndef __GUPNP_DIDL_LITE_PARSER_H__
#define __GUPNP_DIDL_LITE_PARSER_H__

#include "gupnp-didl-lite-container.h"
#include "gupnp-didl-lite-item.h"

G_BEGIN_DECLS

G_DECLARE_DERIVABLE_TYPE(GUPnPDIDLLiteParser,
                          gupnp_didl_lite_parser,
                          GUPNP,
                          DIDL_LITE_PARSER,
                          GObject)

#define GUPNP_TYPE_DIDL_LITE_PARSER \
                (gupnp_didl_lite_parser_get_type ())

struct _GUPnPDIDLLiteParserClass {
        GObjectClass parent_class;

        /* signals */
        void (* object_available)    (GUPnPDIDLLiteParser    *parser,
                                      GUPnPDIDLLiteObject    *object);
        void (* item_available)      (GUPnPDIDLLiteParser    *parser,
                                      GUPnPDIDLLiteItem      *item);
        void (* container_available) (GUPnPDIDLLiteParser    *parser,
                                      GUPnPDIDLLiteContainer *container);

        /* future padding */
        void (* _gupnp_reserved1) (void);
        void (* _gupnp_reserved2) (void);
        void (* _gupnp_reserved3) (void);
        void (* _gupnp_reserved4) (void);
        void (* _gupnp_reserved5) (void);
};

GUPnPDIDLLiteParser *
gupnp_didl_lite_parser_new              (void);

gboolean
gupnp_didl_lite_parser_parse_didl       (GUPnPDIDLLiteParser *parser,
                                         const char          *didl,
                                         GError             **error);

G_END_DECLS

#endif /* __GUPNP_DIDL_LITE_PARSER_H__ */
