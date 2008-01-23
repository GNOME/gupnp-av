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

#include <string.h>
#include "gupnp-didl-lite-parser.h"
#include "gupnp-didl-lite-object.h"
#include "xml-util.h"

G_DEFINE_TYPE (GUPnPDIDLLiteParser,
               gupnp_didl_lite_parser,
               G_TYPE_OBJECT);

enum {
        DIDL_OBJECT_AVAILABLE,
        SIGNAL_LAST
};

static guint signals[SIGNAL_LAST];

static void
gupnp_didl_lite_parser_init (GUPnPDIDLLiteParser *didl)
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

        /**
         * GUPnPDIDLLiteParser::didl-object-available
         * @didl: The #GUPnPGUPnPDIDLLiteParser that received the signal
         * @object_node: The now available object node
         *
         * The ::didl-object-available signal is emitted whenever a new
         * DIDL-Lite object node is available.
         **/
        signals[DIDL_OBJECT_AVAILABLE] =
                g_signal_new ("didl-object-available",
                              GUPNP_TYPE_DIDL_LITE_PARSER,
                              G_SIGNAL_RUN_LAST,
                              G_STRUCT_OFFSET (GUPnPDIDLLiteParserClass,
                                               didl_object_available),
                              NULL,
                              NULL,
                              g_cclosure_marshal_VOID__POINTER,
                              G_TYPE_NONE,
                              1,
                              G_TYPE_POINTER);
}

GUPnPDIDLLiteParser *
gupnp_didl_lite_parser_new (void)
{
        return g_object_new (GUPNP_TYPE_DIDL_LITE_PARSER, NULL);
}

void
gupnp_didl_lite_parser_parse_didl (GUPnPDIDLLiteParser *parser,
                                   const char          *didl)
{
        xmlDoc  *doc;
        xmlNode *element;

        doc = xmlParseMemory (didl, strlen (didl));
	if (doc == NULL) {
	        g_warning ("Parse error on XML DIDL-Light:\n'%s'", didl);

                return;
        }

        /* Get a pointer to root element */
        element = xml_util_get_element ((xmlNode *) doc,
                                        "DIDL-Lite",
                                        NULL);
        if (element == NULL)
                return;

        for (element = element->children; element; element = element->next) {
                g_signal_emit (parser,
                               signals[DIDL_OBJECT_AVAILABLE],
                               0,
                               element);
        }

        xmlFreeDoc (doc);
}

