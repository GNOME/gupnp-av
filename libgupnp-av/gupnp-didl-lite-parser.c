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

/**
 * SECTION:gupnp-didl-lite-parser
 * @short_description: A/V DIDL-Lite XML parser
 *
 * #GUPnPDIDLLiteParser parses DIDL-Lite XML strings.
 *
 */

#include <string.h>
#include "gupnp-didl-lite-parser.h"
#include "gupnp-didl-lite-object.h"
#include "xml-util.h"

G_DEFINE_TYPE (GUPnPDIDLLiteParser,
               gupnp_didl_lite_parser,
               G_TYPE_OBJECT);

static void
gupnp_didl_lite_parser_init (GUPnPDIDLLiteParser *parser)
{
}

static void
gupnp_didl_lite_parser_dispose (GObject *object)
{
        GObjectClass   *gobject_class;
        GUPnPDIDLLiteParser *parser;

        parser = GUPNP_DIDL_LITE_PARSER (object);

        gobject_class = G_OBJECT_CLASS (gupnp_didl_lite_parser_parent_class);
        gobject_class->dispose (object);
}

static void
gupnp_didl_lite_parser_class_init (GUPnPDIDLLiteParserClass *klass)
{
        GObjectClass *object_class;

        object_class = G_OBJECT_CLASS (klass);

        object_class->dispose = gupnp_didl_lite_parser_dispose;
}

/**
 * gupnp_didl_lite_parser_new
 *
 * Return value: A new #GUPnPDIDLLiteParser object.
 **/
GUPnPDIDLLiteParser *
gupnp_didl_lite_parser_new (void)
{
        return g_object_new (GUPNP_TYPE_DIDL_LITE_PARSER, NULL);
}

/**
 * gupnp_didl_lite_parser_parse_didl
 * @parser: A #GUPnPDIDLLiteParser
 * @didl: The DIDL-Lite XML string to be parsed
 * @callback: The callback to call for each DIDL-Lite object found
 * @user_data: User data for callback
 * @error: The location where to store any error, or NULL
 *
 * Parses DIDL-Lite XML string @didl, calling the supplied callback for each
 * DIDL-Lite object it finds in the process.
 *
 * Return value: TRUE on success.
 **/
gboolean
gupnp_didl_lite_parser_parse_didl
                              (GUPnPDIDLLiteParser              *parser,
                               const char                       *didl,
                               GUPnPDIDLLiteParserObjectCallback callback,
                               gpointer                          user_data,
                               GError                          **error)
{
        xmlDoc  *doc;
        xmlNode *element;

        doc = xmlParseMemory (didl, strlen (didl));
	if (doc == NULL) {
                g_set_error (error,
                             GUPNP_SERVER_ERROR,
                             GUPNP_SERVER_ERROR_INVALID_RESPONSE,
                             "Could not parse DIDL-Lite XML:\n%s", didl);

                return FALSE;
        }

        /* Get a pointer to root element */
        element = xml_util_get_element ((xmlNode *) doc,
                                        "DIDL-Lite",
                                        NULL);
        if (element == NULL) {
                g_set_error (error,
                             GUPNP_SERVER_ERROR,
                             GUPNP_SERVER_ERROR_INVALID_RESPONSE,
                             "No 'DIDL-Lite' node in the DIDL-Lite XML:\n%s",
                             didl);

                return FALSE;
        }

        for (element = element->children; element; element = element->next) {
                callback (parser, element, user_data);
        }

        xmlFreeDoc (doc);

        return TRUE;
}

