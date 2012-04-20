/*
 * Copyright (C) 2012 Openismus GmbH
 *
 * Author: Jens Georg <jensg@openismus.com>
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

#include <libgupnp-av/gupnp-didl-lite-parser.h>

#define TEST_DIDL_BGO674319 \
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>" \
"<DIDL-Lite" \
"    xmlns:dc=\"http://purl.org/dc/elements/1.1/\"" \
"    xmlns=\"urn:schemas-upnp-org:metadata-1-0/DIDL-Lite/\"" \
"    xmlns:upnp=\"urn:schemas-upnp-org:metadata-1-0/upnp/\"" \
"    xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"" \
"    xsi:schemaLocation=\"urn:schemas-upnp-org:metadata-1-0/DIDL-Lite/" \
"        http://www.upnp.org/schemas/av/didl-lite-v2-20060531.xsd" \
"      urn:schemas-upnp-org:metadata-1-0/upnp/" \
"        http://www.upnp.org/schemas/av/upnp-v2-20060531.xsd\">" \
"    <item id=\"\" parentID=\"0\" restricted=\"0\">" \
"        <dc:title>New Song</dc:title>" \
"        <upnp:class>object.item.audioItem</upnp:class>" \
"        <res protocolInfo=\"*:*:*:*\" />" \
"        <upnp:artist></upnp:artist>" \
"    </item>" \
"</DIDL-Lite>"

static void
test_bgo674319_on_object_available (GUPnPDIDLLiteParser *parser,
                                    GUPnPDIDLLiteObject *object,
                                    gpointer             user_data)
{
        GUPnPDIDLLiteObject **out = (GUPnPDIDLLiteObject **) user_data;
        *out = g_object_ref (object);
}

static void
test_bgo674319 (void)
{
        GUPnPDIDLLiteParser *parser;
        GUPnPDIDLLiteObject *object = NULL;
        GList *artists;

        parser = gupnp_didl_lite_parser_new ();
        g_signal_connect (parser,
                          "object-available",
                          G_CALLBACK (test_bgo674319_on_object_available),
                          &object);
        g_assert (gupnp_didl_lite_parser_parse_didl (parser, TEST_DIDL_BGO674319, NULL));
        g_assert (object != NULL);

        artists = gupnp_didl_lite_object_get_artists (object);
        g_assert (artists == NULL);
}

int main (int argc, char *argv[])
{
        g_type_init ();
        g_test_init (&argc, &argv, NULL);

        g_test_add_func ("/bugs/gnome/674319", test_bgo674319);

        g_test_run ();


        return 0;
}
