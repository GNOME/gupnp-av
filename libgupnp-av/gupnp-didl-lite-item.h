/*
 * Copyright (C) 2009 Nokia Corporation.
 * Copyright (C) 2012 Intel Corporation.
 *
 * Authors: Zeeshan Ali (Khattak) <zeeshan.ali@nokia.com>
 *                                <zeeshanak@gnome.org>
 *          Jens Georg <jensg@openismus.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */

#ifndef GUPNP_DIDL_LITE_ITEM_H
#define GUPNP_DIDL_LITE_ITEM_H

#include <glib-object.h>
#include "gupnp-didl-lite-object.h"

G_BEGIN_DECLS

G_DECLARE_DERIVABLE_TYPE(GUPnPDIDLLiteItem,
                          gupnp_didl_lite_item,
                          GUPNP,
                          DIDL_LITE_ITEM,
                          GUPnPDIDLLiteObject)

#define GUPNP_TYPE_DIDL_LITE_ITEM \
                (gupnp_didl_lite_item_get_type ())

struct _GUPnPDIDLLiteItemClass {
        GUPnPDIDLLiteObjectClass parent_class;

        /* future padding */
        void (* _gupnp_reserved1) (void);
        void (* _gupnp_reserved2) (void);
        void (* _gupnp_reserved3) (void);
        void (* _gupnp_reserved4) (void);
};

const char *
gupnp_didl_lite_item_get_ref_id         (GUPnPDIDLLiteItem *item);

void
gupnp_didl_lite_item_set_ref_id         (GUPnPDIDLLiteItem *item,
                                         const char        *ref_id);

void
gupnp_didl_lite_item_set_lifetime       (GUPnPDIDLLiteItem *item,
                                         glong              lifetime);

glong
gupnp_didl_lite_item_get_lifetime       (GUPnPDIDLLiteItem *item);

G_END_DECLS

#endif /* GUPNP_DIDL_LITE_ITEM_H */
