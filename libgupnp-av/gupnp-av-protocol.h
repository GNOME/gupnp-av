/*
 * Copyright (C) 2007 Zeeshan Ali <zeenix@gstreamer.net>
 * Copyright (C) 2006, 2007 OpenedHand Ltd.
 *
 * Author: Zeeshan Ali Khattak <zeenix@gstreamer.net>
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

#ifndef __GUPNP_AV_PROTOCOL_H__
#define __GUPNP_AV_PROTOCOL_H__

#include <libgupnp/gupnp.h>

G_BEGIN_DECLS

#define MEDIA_RENDERER_V1 "urn:schemas-upnp-org:device:MediaRenderer:1"
#define MEDIA_RENDERER_V2 "urn:schemas-upnp-org:device:MediaRenderer:2"
#define MEDIA_SERVER_V1 "urn:schemas-upnp-org:device:MediaServer:1"
#define MEDIA_SERVER_V2 "urn:schemas-upnp-org:device:MediaServer:2"

#define CONNECTION_MANAGER_V1 \
                "urn:schemas-upnp-org:service:ConnectionManager:1"
#define CONNECTION_MANAGER_V2 \
                "urn:schemas-upnp-org:service:ConnectionManager:2"
#define CONTENT_DIR_V1        "urn:schemas-upnp-org:service:ContentDirectory:1"
#define CONTENT_DIR_V2        "urn:schemas-upnp-org:service:ContentDirectory:2"
#define AV_TRANSPORT_V1       "urn:schemas-upnp-org:service:AVTransport:1"
#define AV_TRANSPORT_V2       "urn:schemas-upnp-org:service:AVTransport:2"
#define RENDERING_CONTROL_V1  "urn:schemas-upnp-org:service:RenderingControl:1"
#define RENDERING_CONTROL_V2  "urn:schemas-upnp-org:service:RenderingControl:2"

G_END_DECLS

#endif /* __GUPNP_AV_PROTOCOL_H__ */

