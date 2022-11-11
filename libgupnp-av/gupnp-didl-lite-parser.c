/*
 * Copyright (C) 2007 Zeeshan Ali (Khattak) <zeeshanak@gnome.org>
 *
 * Authors: Zeeshan Ali (Khattak) <zeeshanak@gnome.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */

/**
 * GUPnPDIDLLiteParser:
 *
 * A/V DIDL-Lite XML parser
 *
 * #GUPnPDIDLLiteParser parses DIDL-Lite XML strings.
 *
 */

#include <config.h>

#include <string.h>
#include <ctype.h>
#include "gupnp-av.h"
#include "gupnp-didl-lite-object-private.h"
#include "xml-util.h"
#include "gupnp-didl-lite-parser-private.h"

G_DEFINE_TYPE (GUPnPDIDLLiteParser,
               gupnp_didl_lite_parser,
               G_TYPE_OBJECT)

enum {
        OBJECT_AVAILABLE,
        ITEM_AVAILABLE,
        CONTAINER_AVAILABLE,
        SIGNAL_LAST
};

static guint signals[SIGNAL_LAST];

static gboolean
verify_didl_attributes (xmlNode *node)
{
        const char *content;

        content = av_xml_util_get_child_element_content (node, "date");
        if (content) {
                enum {
                        NONE,
                        YEAR,
                        YEARMONTH_HYPHEN,
                        MONTH,
                        MONTHDAY_HYPHEN,
                        DAY,
                        END
                };
                /* try to roughly verify the passed date with ^\d{4}-\d{2}-\d{2} */
                char *ptr = (char *) content;
                int idx = 0, state = NONE;
                while (*ptr) {
                        switch (*ptr) {
                        case '-':
                                if (state == YEAR && idx == 4)
                                        state = YEARMONTH_HYPHEN;
                                else if (state == MONTH && (idx == 6 || idx == 7))
                                        state = MONTHDAY_HYPHEN;
                                else
                                        return FALSE;
                                break;
                        default:
                                if (!isdigit (*ptr))
                                        return FALSE;
                                if (state == NONE)
                                        state = YEAR;
                                else if (state == YEARMONTH_HYPHEN)
                                        state = MONTH;
                                else if (state == MONTHDAY_HYPHEN)
                                        state = DAY;
                                else if (state == DAY) {
                                        if (*(ptr-1) != '-' && *(ptr-2) != '-')
                                                return FALSE;
                                }
                        }

                        ptr++;
                        idx++;
                        if (*ptr == '\0') {
                                if (state == DAY)
                                        break;
                                else
                                        return FALSE;
                        }
                        if (idx == 10)
                                break;
                }
        }

        if (av_xml_util_get_attribute_content (node, "restricted") != NULL) {
                return av_xml_util_verify_attribute_is_boolean (node,
                                                                "restricted");
        }

        return TRUE;
}

static gboolean
parse_elements (GUPnPDIDLLiteParser *parser,
                xmlNode             *node,
                GUPnPAVXMLDoc       *xml_doc,
                xmlNs               *upnp_ns,
                xmlNs               *dc_ns,
                xmlNs               *dlna_ns,
                xmlNs               *pv_ns,
                gboolean             recursive,
                GError             **error);

static void
gupnp_didl_lite_parser_init (G_GNUC_UNUSED GUPnPDIDLLiteParser *parser)
{
}

static void
gupnp_didl_lite_parser_dispose (GObject *object)
{
        GObjectClass   *gobject_class;

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
         * GUPnPDIDLLiteParser::object-available:
         * @parser: The #GUPnPDIDLLiteParser that received the signal
         * @object: The now available #GUPnPDIDLLiteObject
         *
         * The ::object-available signal is emitted each time an object is
         * found in the DIDL-Lite XML being parsed.
         **/
        signals[OBJECT_AVAILABLE] =
                g_signal_new ("object-available",
                              GUPNP_TYPE_DIDL_LITE_PARSER,
                              G_SIGNAL_RUN_LAST,
                              G_STRUCT_OFFSET (GUPnPDIDLLiteParserClass,
                                               object_available),
                              NULL,
                              NULL,
                              g_cclosure_marshal_VOID__OBJECT,
                              G_TYPE_NONE,
                              1,
                              GUPNP_TYPE_DIDL_LITE_OBJECT);

        /**
         * GUPnPDIDLLiteParser::item-available:
         * @parser: The #GUPnPDIDLLiteParser that received the signal
         * @item: The now available #GUPnPDIDLLiteItem
         *
         * The ::item-available signal is emitted each time an item is found in
         * the DIDL-Lite XML being parsed.
         **/
        signals[ITEM_AVAILABLE] =
                g_signal_new ("item-available",
                              GUPNP_TYPE_DIDL_LITE_PARSER,
                              G_SIGNAL_RUN_LAST,
                              G_STRUCT_OFFSET (GUPnPDIDLLiteParserClass,
                                               item_available),
                              NULL,
                              NULL,
                              g_cclosure_marshal_VOID__OBJECT,
                              G_TYPE_NONE,
                              1,
                              GUPNP_TYPE_DIDL_LITE_ITEM);

        /**
         * GUPnPDIDLLiteParser::container-available:
         * @parser: The #GUPnPDIDLLiteParser that received the signal
         * @container: The now available #GUPnPDIDLLiteContainer
         *
         * The ::container-available signal is emitted each time a container is
         * found in the DIDL-Lite XML being parsed.
         **/
        signals[CONTAINER_AVAILABLE] =
                g_signal_new ("container-available",
                              GUPNP_TYPE_DIDL_LITE_PARSER,
                              G_SIGNAL_RUN_LAST,
                              G_STRUCT_OFFSET (GUPnPDIDLLiteParserClass,
                                               container_available),
                              NULL,
                              NULL,
                              g_cclosure_marshal_VOID__OBJECT,
                              G_TYPE_NONE,
                              1,
                              GUPNP_TYPE_DIDL_LITE_CONTAINER);
}

/**
 * gupnp_didl_lite_parser_new:
 *
 * Return value: A new #GUPnPDIDLLiteParser object.
 **/
GUPnPDIDLLiteParser *
gupnp_didl_lite_parser_new (void)
{
        return g_object_new (GUPNP_TYPE_DIDL_LITE_PARSER, NULL);
}

/**
 * gupnp_didl_lite_parser_parse_didl:
 * @parser: A #GUPnPDIDLLiteParser
 * @didl: The DIDL-Lite XML string to be parsed
 * @error: The location where to store any error, or NULL
 *
 * Parses DIDL-Lite XML string @didl, emitting the ::object-available,
 * ::item-available and ::container-available signals appropriately during the
 * process.
 *
 * Return value: TRUE on success.
 **/
gboolean
gupnp_didl_lite_parser_parse_didl (GUPnPDIDLLiteParser *parser,
                                   const char          *didl,
                                   GError             **error)
{
        return gupnp_didl_lite_parser_parse_didl_recursive (parser,
                                                            didl,
                                                            FALSE,
                                                            error);
}

/**
 * gupnp_didl_lite_parser_parse_didl_recursive:
 * @parser: A #GUPnPDIDLLiteParser
 * @didl: The DIDL-Lite XML string to be parsed
 * @error: The location where to store any error, or %NULL
 *
 * Parses DIDL-Lite XML string @didl, emitting the ::object-available,
 * ::item-available and ::container-available signals appropriately during the
 * process.
 *
 * Return value: TRUE on success.
 **/
gboolean
gupnp_didl_lite_parser_parse_didl_recursive (GUPnPDIDLLiteParser *parser,
                                             const char          *didl,
                                             gboolean             recursive,
                                             GError             **error)
{
        xmlDoc        *doc;
        xmlNode       *element;
        xmlNs         *upnp_ns = NULL;
        xmlNs         *dc_ns   = NULL;
        xmlNs         *dlna_ns = NULL;
        xmlNs         *pv_ns   = NULL;
        GUPnPAVXMLDoc *xml_doc = NULL;
        gboolean       result;

        doc = xmlReadMemory (didl,
                             strlen (didl),
                             NULL,
                             NULL,
                             XML_PARSE_NONET | XML_PARSE_RECOVER);
        if (doc == NULL) {
                g_set_error (error,
                             G_MARKUP_ERROR,
                             G_MARKUP_ERROR_PARSE,
                             "Could not parse DIDL-Lite XML:\n%s",
                             didl);

                return FALSE;
        }

        /* Get a pointer to root element */
        element = av_xml_util_get_element ((xmlNode *) doc,
                                           "DIDL-Lite",
                                           NULL);
        if (element == NULL) {
                g_set_error (error,
                             G_MARKUP_ERROR,
                             G_MARKUP_ERROR_PARSE,
                             "No 'DIDL-Lite' node in the DIDL-Lite XML:\n%s",
                             didl);
                xmlFreeDoc (doc);

                return FALSE;
        }

        if (element->children == NULL) {
                g_set_error (error,
                             G_MARKUP_ERROR,
                             G_MARKUP_ERROR_EMPTY,
                             "Empty 'DIDL-Lite' node in the DIDL-Lite XML:\n%s",
                             didl);
                xmlFreeDoc (doc);

                return FALSE;
        }

        /* Create namespaces if they don't exist */
        upnp_ns = av_xml_util_lookup_namespace (doc, GUPNP_XML_NAMESPACE_UPNP);
        if (! upnp_ns)
                upnp_ns = av_xml_util_create_namespace
                                        (xmlDocGetRootElement (doc),
                                         GUPNP_XML_NAMESPACE_UPNP);

        dc_ns = av_xml_util_lookup_namespace (doc, GUPNP_XML_NAMESPACE_DC);
        if (! dc_ns)
                dc_ns = av_xml_util_create_namespace
                                        (xmlDocGetRootElement (doc),
                                         GUPNP_XML_NAMESPACE_DC);
        dlna_ns = av_xml_util_lookup_namespace (doc, GUPNP_XML_NAMESPACE_DLNA);
        if (! dlna_ns)
                dlna_ns = av_xml_util_create_namespace
                                        (xmlDocGetRootElement (doc),
                                         GUPNP_XML_NAMESPACE_DLNA);

        pv_ns = av_xml_util_lookup_namespace (doc, GUPNP_XML_NAMESPACE_PV);
        if (! pv_ns)
                pv_ns = av_xml_util_create_namespace
                                        (xmlDocGetRootElement (doc),
                                         GUPNP_XML_NAMESPACE_PV);

        xml_doc = av_xml_doc_new (doc);

        result = parse_elements (parser,
                                 element,
                                 xml_doc,
                                 upnp_ns,
                                 dc_ns,
                                 dlna_ns,
                                 pv_ns,
                                 recursive,
                                 error);
        av_xml_doc_unref (xml_doc);

        return result;
}

static gboolean
parse_elements (GUPnPDIDLLiteParser *parser,
                xmlNode             *node,
                GUPnPAVXMLDoc       *xml_doc,
                xmlNs               *upnp_ns,
                xmlNs               *dc_ns,
                xmlNs               *dlna_ns,
                xmlNs               *pv_ns,
                gboolean             recursive,
                GError             **error)
{
        xmlNode *element;

        for (element = node->children; element; element = element->next) {
                GUPnPDIDLLiteObject *object;

                object = gupnp_didl_lite_object_new_from_xml (element, xml_doc,
                                                              upnp_ns, dc_ns,
                                                              dlna_ns, pv_ns);

                if (object == NULL)
                        continue;

                if (GUPNP_IS_DIDL_LITE_CONTAINER (object)) {
                        g_signal_emit (parser,
                                       signals[CONTAINER_AVAILABLE],
                                       0,
                                       object);
                        if (recursive &&
                            !parse_elements (parser,
                                             element,
                                             xml_doc,
                                             upnp_ns,
                                             dc_ns,
                                             dlna_ns,
                                             pv_ns,
                                             recursive,
                                             error)) {
                                g_object_unref (object);

                                return FALSE;
                        }
                } else if (GUPNP_IS_DIDL_LITE_ITEM (object)) {
                        node = gupnp_didl_lite_object_get_xml_node (object);
                        if (!verify_didl_attributes (node)) {
                                g_object_unref (object);
                                g_set_error (error,
                                             G_MARKUP_ERROR,
                                             G_MARKUP_ERROR_PARSE,
                                             "Could not parse DIDL-Lite XML");

                                return FALSE;
                        }

                        g_signal_emit (parser,
                                       signals[ITEM_AVAILABLE],
                                       0,
                                       object);
                }

                g_signal_emit (parser,
                               signals[OBJECT_AVAILABLE],
                               0,
                               object);

                g_object_unref (object);
        }

        return TRUE;
}
