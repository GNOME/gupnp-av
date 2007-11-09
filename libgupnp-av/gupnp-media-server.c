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

#include <string.h>

#include <libgupnp/gupnp-service.h>

#include "gupnp-media-server.h"

#define CONNECTION_MANAGER_V1 \
                "urn:schemas-upnp-org:service:ConnectionManager:1"
#define CONNECTION_MANAGER_V2 \
                "urn:schemas-upnp-org:service:ConnectionManager:2"
#define CONTENT_DIR_V1        "urn:schemas-upnp-org:service:ContentDirectory:1"
#define CONTENT_DIR_V2        "urn:schemas-upnp-org:service:ContentDirectory:2"
#define AV_TRANSPORT_V1       "urn:schemas-upnp-org:service:AVTransport:1"
#define AV_TRANSPORT_V2       "urn:schemas-upnp-org:service:AVTransport:2"

G_DEFINE_TYPE (GUPnPMediaServer,
               gupnp_media_server,
               GUPNP_TYPE_ROOT_DEVICE);

struct _GUPnPMediaServerPrivate {
        GUPnPService *connection_manager;
        GUPnPService *content_dir;
        GUPnPService *av_transport;
};

static void
browse_cb (GUPnPService       *service,
           GUPnPServiceAction *action,
           gpointer            user_data)
{
        GUPnPMediaServer *server;

        server = GUPNP_MEDIA_SERVER (user_data);

        gupnp_service_action_set (action,
                                  "Result", G_TYPE_STRING, "Hello world",
                                  "NumberReturned", G_TYPE_INT, 0,
                                  "TotalMatches", G_TYPE_INT, 0,
                                  "UpdateID", G_TYPE_INT, 31415927,
                                  NULL);

        gupnp_service_action_return (action);
}

static void
gupnp_media_server_init (GUPnPMediaServer *server)
{
        GUPnPDeviceInfo *info;
        GUPnPServiceInfo *content_dir;

        server->priv =
                G_TYPE_INSTANCE_GET_PRIVATE (server,
                                             GUPNP_TYPE_MEDIA_SERVER,
                                             GUPnPMediaServerPrivate);

        info = GUPNP_DEVICE_INFO (server);

        content_dir = gupnp_device_info_get_service (info,
                                                     CONTENT_DIR_V1);
        if (content_dir == NULL) {
                content_dir = gupnp_device_info_get_service
                                                      (info,
                                                       CONTENT_DIR_V2);
        }

        if (content_dir != NULL) {
                server->priv->content_dir = GUPNP_SERVICE (content_dir);

                g_signal_connect (content_dir,
                                  "action-invoked::browse",
                                  G_CALLBACK (browse_cb),
                                  server);
        } else
                g_warning ("ContentDirectory service not found!");
}

static void
gupnp_media_server_dispose (GObject *object)
{
        GObjectClass       *gobject_class;
        GUPnPMediaServer *server;

        server = GUPNP_MEDIA_SERVER (object);

        if (server->priv->connection_manager) {
                g_object_unref (server->priv->connection_manager);
                server->priv->connection_manager = NULL;
        }

        if (server->priv->content_dir) {
                g_object_unref (server->priv->content_dir);
                server->priv->content_dir = NULL;
        }

        if (server->priv->av_transport) {
                g_object_unref (server->priv->av_transport);
                server->priv->av_transport = NULL;
        }

        gobject_class = G_OBJECT_CLASS (gupnp_media_server_parent_class);
        gobject_class->dispose (object);
}

static void
gupnp_media_server_class_init (GUPnPMediaServerClass *klass)
{
        GObjectClass *object_class;

        object_class = G_OBJECT_CLASS (klass);

        object_class->dispose = gupnp_media_server_dispose;

        g_type_class_add_private (klass,
                                  sizeof (GUPnPMediaServerPrivate));

}

GUPnPMediaServer *
gupnp_media_server_new (GUPnPContext *context,
                        xmlDoc       *description_doc,
                        const char   *relative_location)
{
        g_return_val_if_fail (GUPNP_IS_CONTEXT (context), NULL);

        return g_object_new (GUPNP_TYPE_MEDIA_SERVER,
                             "context", context,
                             "description-doc", description_doc,
                             "relative-location", relative_location,
                             NULL);
}
