/*
 * Copyright (C) 2012 Intel Corporation.
 *
 * Authors: Jens Georg <jensg@openismus.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */

#ifndef GUPNP_DIDL_LITE_MEDIA_COLLECTION_H
#define GUPNP_DIDL_LITE_MEDIA_COLLECTION_H

#include <glib-object.h>

#include "gupnp-didl-lite-item.h"

G_BEGIN_DECLS

G_DECLARE_DERIVABLE_TYPE (GUPnPMediaCollection,
                          gupnp_media_collection,
                          GUPNP,
                          MEDIA_COLLECTION,
                          GObject)

#define GUPNP_TYPE_MEDIA_COLLECTION (gupnp_media_collection_get_type ())

struct _GUPnPMediaCollectionClass {
        GObjectClass parent_class;

        /* padding */
        void (* _gupnp_reserved1) (void);
        void (* _gupnp_reserved2) (void);
        void (* _gupnp_reserved3) (void);
        void (* _gupnp_reserved4) (void);
};

GUPnPMediaCollection *
gupnp_media_collection_new (void);

GUPnPMediaCollection *
gupnp_media_collection_new_from_string (const char *data);

void
gupnp_media_collection_set_title       (GUPnPMediaCollection *collection,
                                        const char           *title);

const char *
gupnp_media_collection_get_title       (GUPnPMediaCollection *collection);

void
gupnp_media_collection_set_author      (GUPnPMediaCollection *collection,
                                        const char           *author);

const char *
gupnp_media_collection_get_author      (GUPnPMediaCollection *collection);

GUPnPDIDLLiteItem *
gupnp_media_collection_add_item        (GUPnPMediaCollection *collection);

char *
gupnp_media_collection_get_string      (GUPnPMediaCollection *collection);

GList *
gupnp_media_collection_get_items       (GUPnPMediaCollection *collection);

gboolean
gupnp_media_collection_get_mutable     (GUPnPMediaCollection *collection);

G_END_DECLS

#endif /* GUPNP_DIDL_LITE_MEDIA_COLLECTION_H */
