/*
 * Copyright (C) 2008 OpenedHand Ltd.
 *
 * Authors: Jorn Baayen <jorn@openedhand.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */

#include <config.h>

#include <libgupnp-av/gupnp-search-criteria-parser.h>
#include <stdlib.h>

static const char * const searches[] = {
        "dc:title contains \"foo\"",
        "dc:title contains 'foo'",
        "upnp:class = \"object.container.person.musicArtist\"",
        "upnp:class = \"object.container.person.musicArtist\" and @refID exists false",
};

int
main (G_GNUC_UNUSED int argc, G_GNUC_UNUSED char **argv)
{
        GUPnPSearchCriteriaParser *parser;
        GError *error;
        guint i;

        parser = gupnp_search_criteria_parser_new ();

        for (i = 0; i < G_N_ELEMENTS (searches); i++) {
                error = NULL;
                gupnp_search_criteria_parser_parse_text (parser, searches[i], &error);
                if (error) {
                        g_printerr ("\n\nCannot parse '%s': %s\n",
                                    searches[i], error->message);
                        g_error_free (error);

                        return EXIT_FAILURE;
                }
                /* TODO: obviously an important next step is to verify that the
                   data was actually parsed correctly */
                g_print (".");
        }

        g_print ("\n");
        return EXIT_SUCCESS;
}
