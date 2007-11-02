/*
 * Copyright (C) 2007 Zeeshan Ali <zeenix@gstreamer.net>
 *
 * Authors: Zeeshan Ali <zeenix@gstreamer.net>
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

#ifndef __GUPNP_MEDIA_RENDERER_PROXY_H__
#define __GUPNP_MEDIA_RENDERER_PROXY_H__

#include <libgupnp/gupnp-device-proxy.h>

G_BEGIN_DECLS

GType
gupnp_media_renderer_proxy_get_type (void) G_GNUC_CONST;

#define GUPNP_TYPE_MEDIA_RENDERER_PROXY \
                (gupnp_media_renderer_proxy_get_type ())
#define GUPNP_MEDIA_RENDERER_PROXY(obj) \
                (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
                 GUPNP_TYPE_MEDIA_RENDERER_PROXY, \
                 GUPnPMediaRendererProxy))
#define GUPNP_MEDIA_RENDERER_PROXY_CLASS(obj) \
                (G_TYPE_CHECK_CLASS_CAST ((obj), \
                 GUPNP_TYPE_MEDIA_RENDERER_PROXY, \
                 GUPnPMediaRendererProxyClass))
#define IS_GUPNP_MEDIA_RENDERER_PROXY(obj) \
                (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                 GUPNP_TYPE_MEDIA_RENDERER_PROXY))
#define IS_GUPNP_MEDIA_RENDERER_PROXY_CLASS(obj) \
                (G_TYPE_CHECK_CLASS_TYPE ((obj), \
                 GUPNP_TYPE_MEDIA_RENDERER_PROXY))
#define GUPNP_MEDIA_RENDERER_PROXY_GET_CLASS(obj) \
                (G_TYPE_INSTANCE_GET_CLASS ((obj), \
                 GUPNP_TYPE_MEDIA_RENDERER_PROXY, \
                 GUPnPMediaRendererProxyClass))

typedef struct _GUPnPMediaRendererProxyPrivate GUPnPMediaRendererProxyPrivate;

typedef struct {
        GUPnPDeviceProxy parent;

        GUPnPMediaRendererProxyPrivate *priv;
} GUPnPMediaRendererProxy;

typedef struct {
        GUPnPDeviceProxyClass parent_class;

        /* future padding */
        void (* _gupnp_reserved1) (void);
        void (* _gupnp_reserved2) (void);
        void (* _gupnp_reserved3) (void);
        void (* _gupnp_reserved4) (void);
} GUPnPMediaRendererProxyClass;

G_END_DECLS

#endif /* __GUPNP_MEDIA_RENDERER_PROXY_H__ */
