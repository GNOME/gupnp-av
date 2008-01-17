/*
 * Copyright (C) 2008 OpenedHand Ltd.
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

#include <libgupnp-av/gupnp-search-criteria-parser.h>

static const char * const searches[] = {
        "dc:title contains \"foo\"",
        "dc:title contains 'foo'",
        "upnp:class = \"object.container.person.musicArtist\"",
        "upnp:class = \"object.container.person.musicArtist\" and @refID exists false",
};

int
main (int argc, char **argv)
{
        GUPnPSearchCriteriaParser *parser;
        GError *error;
        int i;

        g_type_init ();

        parser = gupnp_search_criteria_parser_new ();

        for (i = 0; i < G_N_ELEMENTS (searches); i++) {
                error = NULL;
                gupnp_search_criteria_parser_parse_text (parser, searches[i], &error);
                if (error) {
                        g_print ("\n\nCannot parse '%s'", searches[i]);
                        g_error (error->message);
                        return 1;
                }
                /* TODO: obviously an important next step is to verify that the
                   data was actually parsed correctly */
                g_print (".");
        }

        g_print ("\n");
        return 0;
}
