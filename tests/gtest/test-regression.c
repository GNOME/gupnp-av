/*
 * Copyright (C) 2012 Openismus GmbH
 *
 * Author: Jens Georg <jensg@openismus.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */

#include <config.h>

#include <libgupnp-av/gupnp-didl-lite-parser.h>
#include <libgupnp-av/gupnp-didl-lite-writer.h>

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

#define TEST_DIDL_BGO705564 \
"<DIDL-Lite xmlns=\"urn:schemas-upnp-org:metadata-1-0/DIDL-Lite/\" " \
           "xmlns:upnp=\"urn:schemas-upnp-org:metadata-1-0/upnp/\" " \
           "xmlns:dlna=\"urn:schemas-dlna-org:metadata-1-0/\">" \
        "<item>" \
                "<upnp:albumArtURI dlna:profileID=\"JPEG_TN\">" \
                "http://example.com/album.jpg"\
                "</upnp:albumArtURI>"\
        "</item>" \
"</DIDL-Lite>"

#define TEST_DIDL_BGO770174 \
"<DIDL-Lite xmlns:dc=\"http://purl.org/dc/elements/1.1/\" " \
           "xmlns:upnp=\"urn:schemas-upnp-org:metadata-1-0/upnp/\" " \
           "xmlns=\"urn:schemas-upnp-org:metadata-1-0/DIDL-Lite/\" " \
           "xmlns:dlna=\"urn:schemas-dlna-org:metadata-1-0/\">\n" \
    "<container id=\"0\" parentID=\"-1\" restricted=\"1\" childCount=\"4\" searchable=\"1\">" \
        "<upnp:searchClass includeDerived=\"1\">object.item.audioItem</upnp:searchClass>" \
        "<upnp:searchClass includeDerived=\"1\">object.item.imageItem</upnp:searchClass>" \
        "<upnp:searchClass includeDerived=\"1\">object.item.videoItem</upnp:searchClass>" \
        "<dc:title>root</dc:title>" \
        "<upnp:class>object.container.storageFolder</upnp:class>" \
        "<upnp:storageUsed>-1</upnp:storageUsed>" \
    "</container>" \
"</DIDL-Lite>"

#define TEST_DIDL_GGO11 \
"<DIDL-Lite xmlns=\"urn:schemas-upnp-org:metadata-1-0/DIDL-Lite/\" " \
           "xmlns:dc=\"http://purl.org/dc/elements/1.1/\" " \
           "xmlns:upnp=\"urn:schemas-upnp-org:metadata-1-0/upnp/\">" \
               "<item id=\"0/1/Disque dur/Vidéos/MEGAFORCE.avi\" " \
                "parentID=\"0/1/Disque dur/Vidéos\">" \
                "<dc:title>MEGAFORCE.avi</dc:title>" \
                "<upnp:class>object.item.videoItem</upnp:class>" \
                "<dc:date>%s</dc:date>" \
                    "<res protocolInfo=\"http-get:*:video/x-msvideo:DLNA.ORG_OP=01;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=21F00000000000000000000000000000\" size=\"880839598\">" \
                         "http://192.168.1.254:52424/files/Disque%%20dur/Vid%%C3%%A9os/MEGAFORCE.avi" \
                    "</res>" \
                "</item>" \
"</DIDL-Lite>"

static void
test_ggo11 (void)
{
        GUPnPDIDLLiteParser *parser;
        guint i;
        struct {
                const char *date;
                gboolean success;
        } date_tests[] = {
                { "2008-2-28", TRUE },
                { "200-2-28", FALSE },
                { "2008-02-28", TRUE },
                { "2008-2--28", FALSE },
                { "2008-02-8", TRUE },
                { "2008-102-8", FALSE },
                { "2008-2-128", FALSE },
                { "", TRUE },
                { "0-1-2", FALSE },
                { "2008-2-", FALSE },
                { "2021-04-23T18:18:42Z", TRUE },
                { ":", FALSE }
        };

        parser = gupnp_didl_lite_parser_new ();
        for (i = 0; i < G_N_ELEMENTS (date_tests); i++) {
                g_autofree char *didl = g_strdup_printf (TEST_DIDL_GGO11, date_tests[i].date);
                g_test_message ("Testing validity of date '%s', expecting %s",
                                date_tests[i].date,
                                date_tests[i].success ? "success" : "failure");
                g_assert_true (gupnp_didl_lite_parser_parse_didl (parser, didl, NULL) == date_tests[i].success);
        }
        g_clear_object (&parser);
}

static void
test_bgo674319_on_object_available (G_GNUC_UNUSED GUPnPDIDLLiteParser *parser,
                                    GUPnPDIDLLiteObject               *object,
                                    gpointer                           user_data)
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

static void
test_bgo687462 (void)
{
        GUPnPDIDLLiteWriter *writer;
        GUPnPDIDLLiteObject *object;

        writer = gupnp_didl_lite_writer_new (NULL);
        object = (GUPnPDIDLLiteObject *)
                                gupnp_didl_lite_writer_add_item (writer);
        gupnp_didl_lite_object_set_album (object, "Test");
        g_assert_cmpstr (gupnp_didl_lite_object_get_album (object), ==, "Test");

        gupnp_didl_lite_object_set_album_art (object, "AlbumArt");
        g_assert_cmpstr (gupnp_didl_lite_object_get_album_art (object), ==, "AlbumArt");

        gupnp_didl_lite_writer_filter (writer, "upnp:album");

        g_assert_cmpstr (gupnp_didl_lite_object_get_album (object), ==, "Test");
        g_assert (gupnp_didl_lite_object_get_album_art (object) == NULL);
}

static void
test_bgo705564 (void)
{
        GUPnPDIDLLiteWriter *writer;
        GUPnPDIDLLiteObject *object;
        char *xml;

        writer = gupnp_didl_lite_writer_new (NULL);
        object = (GUPnPDIDLLiteObject *)
                        gupnp_didl_lite_writer_add_item (writer);
        gupnp_didl_lite_object_set_album_art (object,
                                              "http://example.com/album.jpg");

        xml = gupnp_didl_lite_writer_get_string (writer);
        g_assert_cmpstr (xml, ==, TEST_DIDL_BGO705564);
        g_free (xml);
        g_object_unref (object);
        g_object_unref (writer);
}

#if 0
static void
test_bgo753314 (void)
{
        GUPnPDIDLLiteObject *object;
        GUPnPDIDLLiteWriter *writer;
        GUPnPDIDLLiteResource *resource;
        GUPnPProtocolInfo *pi;
        GList *list = NULL;
        GUPnPDLNAFlags flags = GUPNP_DLNA_FLAGS_NONE;

        flags = GUPNP_DLNA_FLAGS_BYTE_BASED_SEEK |
                GUPNP_DLNA_FLAGS_STREAMING_TRANSFER_MODE |
                GUPNP_DLNA_FLAGS_BACKGROUND_TRANSFER_MODE;

        writer = gupnp_didl_lite_writer_new (NULL);
        object = (GUPnPDIDLLiteObject *)
                        gupnp_didl_lite_writer_add_item (writer);
        resource = gupnp_didl_lite_object_add_resource (object);
        pi = gupnp_protocol_info_new ();
        gupnp_protocol_info_set_protocol (pi, "http");
        gupnp_protocol_info_set_mime_type (pi, "video/mp4");
        gupnp_protocol_info_set_dlna_flags (pi, flags);
        gupnp_didl_lite_resource_set_protocol_info (resource, pi);
        g_object_unref (pi);
        g_object_unref (resource);

        list = gupnp_didl_lite_object_get_resources (object);
        g_assert_cmpint (g_list_length (list), ==, 1);

        resource = list->data;
        pi = gupnp_didl_lite_resource_get_protocol_info (resource);

        g_assert_cmpint (gupnp_protocol_info_get_dlna_flags (pi), ==, flags);
        g_object_unref (resource);
        g_object_unref (object);
        g_object_unref (writer);
}
#endif

static void
test_bgo770174_on_object_available (G_GNUC_UNUSED GUPnPDIDLLiteParser *parser,
                                    GUPnPDIDLLiteObject               *object,
                                    gpointer                           user_data)
{
        GUPnPDIDLLiteObject **out = (GUPnPDIDLLiteObject **) user_data;
        *out = g_object_ref (object);
}

static void
test_bgo770174 (void)
{
        GUPnPDIDLLiteParser *parser;
        GUPnPDIDLLiteObject *object = NULL;
        gint storage_used = 0;

        parser = gupnp_didl_lite_parser_new ();
        g_signal_connect (parser,
                          "object-available",
                          G_CALLBACK (test_bgo770174_on_object_available),
                          &object);
        g_assert (gupnp_didl_lite_parser_parse_didl (parser, TEST_DIDL_BGO770174, NULL));
        g_assert (object != NULL);

        storage_used = gupnp_didl_lite_container_get_storage_used
                                        (GUPNP_DIDL_LITE_CONTAINER (object));
        g_assert_cmpint (storage_used, ==, -1);
}

int main (int argc, char *argv[])
{
        g_test_init (&argc, &argv, NULL);

        g_test_add_func ("/bugs/gnome/674319", test_bgo674319);
        g_test_add_func ("/bugs/gnome/687462", test_bgo687462);
        g_test_add_func ("/bugs/gnome/705564", test_bgo705564);
/*        g_test_add_func ("/bugs/gnome/753314", test_bgo753314); */
        g_test_add_func ("/bugs/gnome/770174", test_bgo770174);
        g_test_add_func ("/bugs/gnome/11", test_ggo11);

        g_test_run ();


        return 0;
}
