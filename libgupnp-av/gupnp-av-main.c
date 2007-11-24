/*
 * Copyright (C) 2007 Zeeshan Ali <zeenix@gstreamer.net>
 * Copyright (C) 2006, 2007 OpenedHand Ltd.
 *
 * Author: Zeeshan Ali <zeenix@gstreamer.net>
 *         Jorn Baayen <jorn@openedhand.com>
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

#include "gupnp-av-main.h"
#include "gupnp-media-renderer-proxy.h"
#include "gupnp-media-server-proxy.h"
#include "gupnp-av-protocol.h"

void
gupnp_av_init (void)
{
        GUPnPResourceFactory *factory;

        g_type_init ();

        factory = gupnp_resource_factory_get_default ();
        g_assert (factory != NULL);

        gupnp_resource_factory_register_resource_proxy_type
                                        (factory,
                                         MEDIA_RENDERER_V1,
                                         GUPNP_TYPE_MEDIA_RENDERER_PROXY);
        gupnp_resource_factory_register_resource_proxy_type
                                        (factory,
                                         MEDIA_RENDERER_V2,
                                         GUPNP_TYPE_MEDIA_RENDERER_PROXY);
        gupnp_resource_factory_register_resource_proxy_type
                                        (factory,
                                         MEDIA_SERVER_V1,
                                         GUPNP_TYPE_MEDIA_SERVER_PROXY);
        gupnp_resource_factory_register_resource_proxy_type
                                        (factory,
                                         MEDIA_SERVER_V2,
                                         GUPNP_TYPE_MEDIA_SERVER_PROXY);
}

