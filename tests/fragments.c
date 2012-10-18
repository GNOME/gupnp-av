/*
 * Copyright (C) 2012 Intel Corporation
 *
 * Authors: Krzesimir Nowak <krnowak@openismus.com>
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include <glib-object.h>
#include "gupnp-didl-lite-object.h"
#include "gupnp-didl-lite-object-private.h"

/* creates an item described by:
static const gchar * const didllite =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
        "<DIDL-Lite"
        "xmlns:dc=\"http://purl.org/dc/elements/1.1/\""
        "xmlns=\"urn:schemas-upnp-org:metadata-1-0/DIDL-Lite/\""
        "xmlns:upnp=\"urn:schemas-upnp-org:metadata-1-0/upnp/\""
        "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\""
        "xsi:schemaLocation=\""
        "urn:schemas-upnp-org:metadata-1-0/DIDL-Lite/"
        "http://www.upnp.org/schemas/av/didl-lite.xsd"
        "urn:schemas-upnp-org:metadata-1-0/upnp/"
        "http://www.upnp.org/schemas/av/upnp.xsd\">"
        "<item id=\"18\" parentID=\"13\" restricted=\"0\">"
        "<dc:title>Try a little tenderness</dc:title>"
        "<upnp:class>object.item.audioItem.musicTrack</upnp:class>"
        "<res protocolInfo=\"http-get:*:audio/mpeg:*\" size=\"3558000\">"
        "http://168.192.1.1/audio197.mp3"
        "</res>"
        "<upnp:artist>Unknown</upnp:artist>"
        "</item>"
        "</DIDL-Lite>";
*/
static GUPnPDIDLLiteObject *
get_item (void)
{
        GUPnPDIDLiteWriter *writer = gupnp_didl_lite_writer_new ();
        GUPnPDIDLiteItem *item = gupnp_didl_lite_writer_add_item (writer);
        GUPnPDIDLLiteObject *object = GUPNP_DIDL_LITE_OBJECT (item);
        GUPnPDIDLLiteContributor *artist;
        GUPnPDIDLLiteResource *resource;
        GUPnPProtocolInfo *info;

        g_object_unref (writer);
        gupnp_didl_lite_object_set_id (object, "18");
        gupnp_didl_lite_object_set_parent_id (object, "13");
        gupnp_didl_lite_object_set_restricted (object, FALSE);
        gupnp_didl_lite_object_set_title (object, "Try a little tenderness");
        gupnp_didl_lite_object_set_upnp_class
                                        (object,
                                         "object.item.audioItem.musicTrack");
        artist = gupnp_didl_lite_object_add_artist (object);
        gupnp_didl_lite_contributor_set_name (artist, "Unknown");
        g_object_unref (artist);
        resource = gupnp_didl_lite_object_add_resource (object);
        info = gupnp_protocol_info_new ();
        gupnp_protocol_info_set_protocol (info, "http-get");
        gupnp_protocol_info_set_network (info, "*");
        gupnp_protocol_info_set_mime_type (info, "audio/mpeg");
        gupnp_didl_lite_resource_set_protocol_info (resource, info);
        g_object_unref (info);
        gupnp_didl_lite_resource_set_size (resource, 3558000);
        gupnp_didl_lite_resource_set_uri (resource,
                                          "http://168.192.1.1/audio197.mp3");
        g_object_unref (resource);

        return object;
}

static const gchar * const current_fragments[] = {
        "<upnp:class>object.item.audioItem.musicTrack</upnp:class>",
        "",
        "<upnp:artist>Unknown</upnp:artist>",
        "<dc:title>Try a little tenderness</dc:title>"
};

static const gchar * const new_fragments[] = {
        "<upnp:class>object.item.audioItem.musicTrack</upnp:class>"
        "<upnp:genre>Obscure</upnp:genre>",
        "<upnp:genre>Even more obscure</upnp:genre>",
        "",
        "<dc:title>Cthulhu fhtagn</dc:title>"
};

static GList*
create_fragment_list (const gchar * const * const fragments,
                      guint count)
{
        GList *list = NULL;
        guint iter;

        for (iter = 0; iter < count; ++iter) {
                list = g_list_prepend (list, fragments[iter]);
        }

        return g_list_reverse (list);
}

int main (void)
{
        GUPnPDIDLLiteObject *obj;
        GList *current = create_fragment_list
                                        (current_fragments,
                                         G_N_ELEMENTS (current_fragments));
        GList *new = create_fragment_list (new_fragments,
                                           G_N_ELEMENTS (new_fragments));
        GUPnPDIDLLiteFragmentResult result;

        g_type_init ();
        g_setenv ("GUPNP_AV_DATADIR", TOP_SRCDIR G_DIR_SEPARATOR_S "data", FALSE);

        obj = get_item ();
        result = gupnp_didl_lite_object_apply_fragments (obj, current, new);

        if (result != GUPNP_DIDL_LITE_FRAGMENT_RESULT_OK)
                return 1;
        return 0;
}
