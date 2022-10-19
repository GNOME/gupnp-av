/*
 * Copyright (C) 2012 Intel Corporation
 *
 * Authors: Regis Merlino <regis.merlino@intel.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */

#include <config.h>

#include <libgupnp-av/gupnp-feature-list-parser.h>
#include <libgupnp-av/gupnp-feature-container-shortcuts.h>

#include <stdlib.h>
#include <string.h>

static const char * const names[] = {
        "BOOKMARK",
        "EPG",
        "CONTAINER_SHORTCUTS",
};

static const char * const versions[] = {
        "1",
        "2",
        "1",
};

static const char * const ids[] = {
        "bookmark1,bookmark2,bookmark3",
        "epg1,epg2",
};

static const char *const container_names[] = {
        "MUSIC_GENRES",
        "IMAGES_ALL",
};

static const char *const container_ids[] = {
        "container:genre",
        "container:images_all",
};

//clang-format off
static const char *text =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
        "<Features "
                "xmlns=\"urn:schemas-upnp-org:av:avs\" "
                "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "
                "xsi:schemaLocation=\""
                "urn:schemas-upnp-org:av:avs "
                "http://www.upnp.org/schemas/av/avs-v1-20060531.xsd\">"
                "<Feature name=\"BOOKMARK\" version=\"1\">"
                        "<objectIDs>bookmark1</objectIDs>"
                        "<objectIDs>bookmark2,bookmark3</objectIDs>"
                "</Feature>"
                "<Feature name=\"EPG\" version=\"2\">"
                        "<objectIDs>epg1,epg2</objectIDs>"
                "</Feature>"
                "<Feature name=\"CONTAINER_SHORTCUTS\" version=\"1\">"
                        "<shortcutlist>"
                                "<shortcut>"
                                        "<name>MUSIC_GENRES</name>"
                                        "<objectID>container:genre</objectID>"
                                "</shortcut>"
                                "<shortcut>"
                                        "<name>IMAGES_ALL</name>"
                                        "<objectID>container:images_all</objectID>"
                                "</shortcut>"
                        "</shortcutlist>"
                "</Feature>"
        "</Features>";
// clang-format on

static gboolean
check_feature (GUPnPFeature *feature)
{
        static int index = 0;

        if (strcmp (names[index], gupnp_feature_get_name (feature)))
                        return FALSE;

        if (strcmp (versions[index], gupnp_feature_get_version (feature)))
                return FALSE;

        if (index < 2) {
                if (strcmp (ids[index], gupnp_feature_get_object_ids (feature)))
                        return FALSE;
        } else {
                if (!GUPNP_IS_FEATURE_CONTAINER_SHORTCUTS (feature))
                        return FALSE;

                GUPnPFeatureContainerShortcuts *f =
                        GUPNP_FEATURE_CONTAINER_SHORTCUTS (feature);

                GSList *s = gupnp_feature_container_shortcuts_get_list (f);
        }
        index++;

        return TRUE;
}

int
main (G_GNUC_UNUSED int argc, G_GNUC_UNUSED char **argv)
{
        GUPnPFeatureListParser *parser;
        GError                 *error;
        GList                  *features;
        GList                  *item;
        gboolean               success = TRUE;

        parser = gupnp_feature_list_parser_new ();

        error = NULL;
        features = gupnp_feature_list_parser_parse_text (parser, text, &error);
        if (features == NULL) {
                g_printerr ("Parse error: %s\n", error->message);
                g_error_free (error);
                return EXIT_FAILURE;
        }

        for (item = features; item != NULL; item = g_list_next (item)) {
                success = check_feature ((GUPnPFeature *) item->data);
                if (!success)
                        break;
        }

        g_print ("\n");

        g_list_free_full (features, g_object_unref);
        g_object_unref (parser);

        return (success) ? EXIT_SUCCESS : EXIT_FAILURE;
}
