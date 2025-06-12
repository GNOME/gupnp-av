/*
 * Copyright (C) 2012 Intel Corporation.
 *
 * Authors: Regis Merlino <regis.merlino@intel.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */

/**
 * GUPnPFeatureListParser:
 *
 * FeatureList state variable XML parser
 *
 * #GUPnPFeatureListParser parses XML strings from ContentDirectory
 * FeatureList state variable.
 *
 */

#include <config.h>

#include <string.h>
#include "gupnp-feature-list-parser.h"
#include "gupnp-av.h"
#include "xml-util.h"

struct _GUPnPFeatureListParser {
        GObject parent;
};

/* GUPnPFeatureListParser */
G_DEFINE_TYPE (GUPnPFeatureListParser,
               gupnp_feature_list_parser,
               G_TYPE_OBJECT)

static void
gupnp_feature_list_parser_init (G_GNUC_UNUSED GUPnPFeatureListParser *parser)
{
        /* Nothing to do here */
}

static void
gupnp_feature_list_parser_finalize (GObject *object)
{
        GObjectClass *object_class;

        object_class = G_OBJECT_CLASS (gupnp_feature_list_parser_parent_class);
        object_class->finalize (object);
}

static void
gupnp_feature_list_parser_class_init (GUPnPFeatureListParserClass *klass)
{
        GObjectClass *object_class;

        object_class = G_OBJECT_CLASS (klass);

        object_class->finalize = gupnp_feature_list_parser_finalize;
}

/**
 * gupnp_feature_list_parser_new:
 *
 * Return value: A new #GUPnPFeatureListParser object.
 **/
GUPnPFeatureListParser *
gupnp_feature_list_parser_new (void)
{
        return g_object_new (GUPNP_TYPE_FEATURE_LIST_PARSER, NULL);
}

static char *
get_feature_object_ids (xmlNode *feature)
{
        xmlNode    *element;
        const char *content;
        GString *object_ids = g_string_new ("");

        for (element = feature->children; element; element = element->next) {
                const char *name = (const char *) element->name;
                if (g_ascii_strcasecmp (name, "objectIDs") == 0) {
                        content = (const char *) xmlNodeGetContent (element);
                        if (strlen (content) == 0)
                                continue;

                        if (object_ids->len > 0)
                                g_string_append_c (object_ids, ',');

                        g_string_append (object_ids, content);
                }
        }

        return g_string_free (object_ids, FALSE);
}

/**
 * gupnp_feature_list_parser_parse_text:
 * @parser: A #GUPnPFeatureListParser
 * @text: The feature list string to be parsed
 * @error: The location where to store the error information if any, or NULL
 *
 * Parses @text and returns the list of available features.
 * If an error occurred @error will be set.
 *
 * Return value: (transfer full) (element-type GUPnPFeature)(nullable): The list
 *of features or %NULL if an error occurred.
 **/
GList *
gupnp_feature_list_parser_parse_text
                                 (G_GNUC_UNUSED GUPnPFeatureListParser *parser,
                                  const char             *text,
                                  GError                **error)
{
        xmlDoc       *doc;
        xmlNode      *element;
        GList        *feature_list = NULL;

        doc = xmlReadMemory (text,
                             strlen (text),
                             NULL,
                             NULL,
                             XML_PARSE_NONET | XML_PARSE_RECOVER);
        if (doc == NULL) {
                g_set_error (error,
                             G_MARKUP_ERROR,
                             G_MARKUP_ERROR_PARSE,
                             "Could not parse FeatureList XML:\n%s",
                             text);

                return NULL;
        }

        /* Get a pointer to root element */
        element = av_xml_util_get_element ((xmlNode *) doc, "Features", NULL);
        if (element == NULL) {
                g_set_error (error,
                             G_MARKUP_ERROR,
                             G_MARKUP_ERROR_PARSE,
                             "No 'Features' node in the XML:\n%s",
                             text);
                xmlFreeDoc (doc);

                return NULL;
        }

        for (element = element->children; element; element = element->next) {
                GUPnPFeature *feature;
                const char   *name;
                const char   *version;
                char         *object_ids;

                if (g_ascii_strcasecmp ((char *) element->name,
                                        "Feature") == 0) {
                        name = av_xml_util_get_attribute_content (element,
                                                                  "name");
                        version = av_xml_util_get_attribute_content (element,
                                                                     "version");
                        if (!name || !version) {
                                g_set_error (error,
                                             G_MARKUP_ERROR,
                                             G_MARKUP_ERROR_UNKNOWN_ATTRIBUTE,
                                             "Invalid attributes in 'Feature' "
                                             "node in the XML:\n%s",
                                             text);

                                xmlFreeDoc (doc);
                                if (feature_list)
                                        g_list_free_full (feature_list,
                                                          g_object_unref);

                                return NULL;
                        }

                        object_ids = get_feature_object_ids (element);

                        feature = g_object_new (GUPNP_TYPE_FEATURE,
                                                "name", name,
                                                "version", version,
                                                "object-ids", object_ids,
                                                NULL);

                        feature_list = g_list_append (feature_list, feature);

                        g_free (object_ids);
                }
        }

        xmlFreeDoc (doc);

        return feature_list;
}
