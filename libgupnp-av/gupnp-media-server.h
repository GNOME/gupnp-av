/*
 * Copyright (C) 2007 OpenedHand Ltd
 *
 * Authors: Jorn Baayen <jorn@openedhand.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef __GUPNP_MEDIA_SERVER_H__
#define __GUPNP_MEDIA_SERVER_H__

#include <libgupnp/gupnp-root-device.h>

G_BEGIN_DECLS

GType
gupnp_media_server_get_type (void) G_GNUC_CONST;

#define GUPNP_TYPE_MEDIA_SERVER \
                (gupnp_media_server_get_type ())
#define GUPNP_MEDIA_SERVER(obj) \
                (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
                 GUPNP_TYPE_MEDIA_SERVER, \
                 GUPnPMediaServer))
#define GUPNP_MEDIA_SERVER_CLASS(obj) \
                (G_TYPE_CHECK_CLASS_CAST ((obj), \
                 GUPNP_TYPE_MEDIA_SERVER, \
                 GUPnPMediaServerClass))
#define GUPNP_IS_MEDIA_SERVER(obj) \
                (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                 GUPNP_TYPE_MEDIA_SERVER))
#define GUPNP_IS_MEDIA_SERVER_CLASS(obj) \
                (G_TYPE_CHECK_CLASS_TYPE ((obj), \
                 GUPNP_TYPE_MEDIA_SERVER))
#define GUPNP_MEDIA_SERVER_GET_CLASS(obj) \
                (G_TYPE_INSTANCE_GET_CLASS ((obj), \
                 GUPNP_TYPE_MEDIA_SERVER, \
                 GUPnPMediaServerClass))

typedef struct _GUPnPMediaServerPrivate GUPnPMediaServerPrivate;

typedef struct {
        GUPnPRootDevice parent;

        GUPnPMediaServerPrivate *priv;
} GUPnPMediaServer;

typedef struct {
        GUPnPRootDeviceClass parent_class;

        /* future padding */
        void (* _gupnp_reserved1) (void);
        void (* _gupnp_reserved2) (void);
        void (* _gupnp_reserved3) (void);
        void (* _gupnp_reserved4) (void);
} GUPnPMediaServerClass;

GUPnPMediaServer *
gupnp_media_server_new (GUPnPContext *context,
                        xmlDoc       *description_doc,
                        const char   *relative_location);

G_END_DECLS

#endif /* __GUPNP_MEDIA_SERVER_H__ */
