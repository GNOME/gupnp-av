/*
 * Copyright (C) 2006, 2007 OpenedHand Ltd.
 * Copyright (C) 2007 Zeeshan Ali.
 * Copyright (C) 2012 Intel Corporation
 *
 * Author: Jorn Baayen <jorn@openedhand.com>
 * Author: Zeeshan Ali (Khattak) <zeeshanak@gnome.org>
 * Author: Krzesimir Nowak <krnowak@openismus.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */

#include <config.h>

#include <string.h>
#include <glib/gprintf.h>

#include "xml-util.h"

typedef struct _GUPnPXMLNamespaceDescription
{
        const char *uri;
        const char *prefix;
} GUPnPXMLNamespaceDescription;


static GUPnPXMLNamespaceDescription gupnp_xml_namespaces[] =
{
        { "urn:schemas-upnp-org:metadata-1-0/DIDL-Lite/", NULL },
        { "http://purl.org/dc/elements/1.1/", "dc" },
        { "urn:schemas-dlna-org:metadata-1-0/", "dlna" },
        { "http://www.pv.com/pvns/", "pv" },
        { "urn:schemas-upnp-org:metadata-1-0/upnp/", "upnp" },
        { NULL, NULL }
};

GUPnPAVXMLDoc *
av_xml_doc_new (xmlDoc *doc)
{
        GUPnPAVXMLDoc *ret = NULL;

        g_return_val_if_fail (doc, NULL);

        ret = g_rc_box_new0(GUPnPAVXMLDoc);
        ret->doc = doc;

        return ret;
}

static void
av_xml_doc_free (GUPnPAVXMLDoc *doc)
{
        g_clear_pointer (&doc->doc, xmlFreeDoc);
}

void
av_xml_doc_unref (GUPnPAVXMLDoc *doc)
{
        g_rc_box_release_full (doc, (GFreeFunc) av_xml_doc_free);
}

xmlNode *
av_xml_util_get_element (xmlNode *node,
                         ...)
{
        va_list var_args;

        va_start (var_args, node);

        while (TRUE) {
                const char *arg;

                arg = va_arg (var_args, const char *);
                if (!arg)
                        break;

                for (node = node->children; node; node = node->next) {
                        if (node->name == NULL)
                                continue;

                        if (!g_ascii_strcasecmp (arg, (char *) node->name))
                                break;
                }

                if (!node)
                        break;
        }

        va_end (var_args);

        return node;
}

GList *
av_xml_util_get_child_elements_by_name (xmlNode *node, const char *name)
{
       GList *children = NULL;

       for (node = node->children; node; node = node->next) {
               if (node->name == NULL)
                       continue;

               if (strcmp (name, (char *) node->name) == 0) {
                       children = g_list_append (children, node);
               }
       }

       return children;
}

const char *
av_xml_util_get_child_element_content (xmlNode    *node,
                                       const char *child_name)
{
        xmlNode *child_node;
        const char *content;

        child_node = av_xml_util_get_element (node, child_name, NULL);
        if (!child_node || !(child_node->children))
                return NULL;

        content = (const char *) child_node->children->content;
        if (!content)
                return NULL;

        return content;
}

guint
av_xml_util_get_uint_child_element (xmlNode    *node,
                                    const char *child_name,
                                    guint       default_value)
{
        const char *content;

        content = av_xml_util_get_child_element_content (node, child_name);
        if (!content)
                return default_value;

        return strtoul (content, NULL, 0);
}

guint64
av_xml_util_get_uint64_child_element (xmlNode    *node,
                                      const char *child_name,
                                      guint64     default_value)
{
        const char *content;

        content = av_xml_util_get_child_element_content (node, child_name);
        if (!content)
                return default_value;

        return g_ascii_strtoull (content, NULL, 0);
}

const char *
av_xml_util_get_attribute_content (xmlNode    *node,
                                   const char *attribute_name)
{
        xmlAttr *attribute;

        for (attribute = node->properties;
             attribute;
             attribute = attribute->next) {
                if (attribute->name == NULL)
                        continue;

                if (strcmp (attribute_name, (char *) attribute->name) == 0)
                        break;
        }

        if (attribute)
                return (const char *) attribute->children->content;
        else
                return NULL;
}

gboolean
av_xml_util_get_boolean_attribute (xmlNode    *node,
                                   const char *attribute_name)
{
        const char *content;
        gchar   *str;
        gboolean ret;

        content = av_xml_util_get_attribute_content (node, attribute_name);
        if (!content)
                return FALSE;

        str = (char *) content;
        if (g_ascii_strcasecmp (str, "true") == 0 ||
            g_ascii_strcasecmp (str, "yes") == 0)
                ret = TRUE;
        else if (g_ascii_strcasecmp (str, "false") == 0 ||
                 g_ascii_strcasecmp (str, "no") == 0)
                ret = FALSE;
        else {
                int i;

                i = atoi (str);
                ret = i ? TRUE : FALSE;
        }

        return ret;
}

guint
av_xml_util_get_uint_attribute (xmlNode    *node,
                                const char *attribute_name,
                                guint       default_value)
{
        return (guint) av_xml_util_get_long_attribute (node,
                                                    attribute_name,
                                                    (glong) default_value);
}

gint
av_xml_util_get_int_attribute (xmlNode    *node,
                               const char *attribute_name,
                               gint        default_value)
{
        return (gint) av_xml_util_get_long_attribute (node,
                                                   attribute_name,
                                                   (glong) default_value);
}

glong
av_xml_util_get_long_attribute (xmlNode    *node,
                                const char *attribute_name,
                                glong       default_value)
{
    return (glong) av_xml_util_get_int64_attribute (node,
                                                 attribute_name,
                                                 (gint64) default_value);
}

gint64
av_xml_util_get_int64_attribute (xmlNode    *node,
                                 const char *attribute_name,
                                 gint64      default_value)
{
        const char *content;

        content = av_xml_util_get_attribute_content (node, attribute_name);
        if (!content)
                return default_value;

        return g_ascii_strtoll (content, NULL, 0);
}

xmlNode *
av_xml_util_set_child (xmlNode    *parent_node,
                       GUPnPXMLNamespace ns,
                       xmlNsPtr   *xmlns,
                       xmlDoc     *doc,
                       const char *name,
                       const char *value)
{
        xmlNode *node;
        xmlChar *escaped;

        node = av_xml_util_get_element (parent_node, name, NULL);
        if (node == NULL) {
                xmlNsPtr ns_ptr = NULL;

                ns_ptr = av_xml_util_get_ns (doc, ns, xmlns);
                node = xmlNewChild (parent_node,
                                    ns_ptr,
                                    (unsigned char *) name,
                                    NULL);
        }

        escaped = xmlEncodeSpecialChars (doc, (const unsigned char *) value);
        xmlNodeSetContent (node, escaped);
        xmlFree (escaped);

        return node;
}

void
av_xml_util_unset_child (xmlNode    *parent_node,
                         const char *name)
{
        xmlNode *node;

        node = av_xml_util_get_element (parent_node, name, NULL);
        if (node != NULL) {
                xmlUnlinkNode (node);
                xmlFreeNode (node);
        }
}

gboolean
av_xml_util_verify_attribute_is_boolean (xmlNode    *node,
                                         const char *attribute_name)
{
        const char *content;
        char *str;

        content = av_xml_util_get_attribute_content (node, attribute_name);
        if (content == NULL)
            return FALSE;

        str = (char *) content;

        return g_ascii_strcasecmp (str, "true") == 0 ||
               g_ascii_strcasecmp (str, "yes") == 0 ||
               g_ascii_strcasecmp (str, "false") == 0 ||
               g_ascii_strcasecmp (str, "no") == 0 ||
               g_ascii_strcasecmp (str, "0") == 0 ||
               g_ascii_strcasecmp (str, "1") == 0;
}

char *
av_xml_util_get_child_string (xmlNode    *parent_node,
                              xmlDoc     *doc,
                              const char *name)
{
        xmlBuffer *buffer;
        char      *ret;
        xmlNode   *node;

        node = av_xml_util_get_element (parent_node, name, NULL);
        if (!node)
                return NULL;

        buffer = xmlBufferCreate ();
        xmlNodeDump (buffer,
                     doc,
                     node,
                     0,
                     0);
        ret = g_strndup ((char *) xmlBufferContent (buffer),
                         xmlBufferLength (buffer));
        xmlBufferFree (buffer);

        return ret;
}

gboolean
av_xml_util_node_deep_equal (xmlNode *first,
                             xmlNode *second)
{
        GHashTable *first_attributes;
        xmlAttr *attribute;
        gboolean equal;

        if (first == NULL && second == NULL)
                return TRUE;
        if (first == NULL || second == NULL)
                return FALSE;

        if (xmlStrcmp (first->name, second->name))
                return FALSE;

        equal = FALSE;
        first_attributes = av_xml_util_get_attributes_map (first);
        /* compare attributes */
        for (attribute = second->properties;
             attribute != NULL;
             attribute = attribute->next) {
                const xmlChar *value = NULL;
                const xmlChar *key = attribute->name;

                if (g_hash_table_lookup_extended (first_attributes,
                                                  key,
                                                  NULL,
                                                  (gpointer *) &value))
                        if (!xmlStrcmp (value, attribute->children->content)) {
                                g_hash_table_remove (first_attributes, key);

                                continue;
                        }

                goto out;
        }

        if (g_hash_table_size (first_attributes))
                goto out;

        /* compare content */
        if (xmlStrcmp (first->content, second->content))
                goto out;
        equal = TRUE;
 out:
        g_hash_table_unref (first_attributes);
        if (equal) {
                xmlNode *first_child;
                xmlNode *second_child;

                for (first_child = first->children,
                     second_child = second->children;
                     first_child != NULL && second_child != NULL;
                     first_child = first_child->next,
                     second_child = second_child->next)
                        if (!av_xml_util_node_deep_equal (first_child,
                                                          second_child))
                                return FALSE;
                if (first_child != NULL || second_child != NULL)
                        return FALSE;
        }

        return equal;
}

xmlNode *
av_xml_util_find_node (xmlNode *haystack,
                       xmlNode *needle)
{
        xmlNodePtr iter;

        if (av_xml_util_node_deep_equal (haystack, needle))
                return haystack;

        for (iter = haystack->children; iter != NULL; iter = iter->next) {
                xmlNodePtr found_node = av_xml_util_find_node (iter, needle);

                if (found_node != NULL)
                        return found_node;
        }

        return NULL;
}

xmlNode *
av_xml_util_copy_node (xmlNode *node)
{
        xmlNode *dup = xmlCopyNode (node, 1);

        /* TODO: remove useless namespace definition. */

        return dup;
}

GHashTable *
av_xml_util_get_attributes_map (xmlNode *node)
{
        xmlAttr *attribute;
        GHashTable *attributes_map = g_hash_table_new (g_str_hash,
                                                       g_str_equal);

        for (attribute = node->properties;
             attribute != NULL;
             attribute = attribute->next)
                g_hash_table_insert (attributes_map,
                                     (gpointer) attribute->name,
                                     (gpointer) attribute->children->content);

        return attributes_map;
}

/**
 * av_xml_util_create_namespace:
 * @root: (allow-none): Document root node or %NULL for anonymous ns.
 * @ns: Namespace
 * @returns: Newly created namespace on root node
 */
xmlNsPtr
av_xml_util_create_namespace (xmlNodePtr root, GUPnPXMLNamespace ns)
{
        g_return_val_if_fail (ns < GUPNP_XML_NAMESPACE_COUNT, NULL);

        return xmlNewNs (root,
                         (const xmlChar *) gupnp_xml_namespaces[ns].uri,
                         (const xmlChar *) gupnp_xml_namespaces[ns].prefix);
}

/**
 * av_xml_util_lookup_namespace:
 * @doc: #xmlDoc
 * @ns: namespace to look up (except DIDL-Lite, which doesn't have a prefix)
 * @returns: %NULL if namespace does not exist, a pointer to the namespace
 * otherwise.
 */
xmlNsPtr
av_xml_util_lookup_namespace (xmlDocPtr doc, GUPnPXMLNamespace ns)
{
        xmlNsPtr *ns_list, *it, retval = NULL;
        const char *ns_prefix = NULL;
        const char *ns_uri = NULL;

        g_return_val_if_fail (ns < GUPNP_XML_NAMESPACE_COUNT, NULL);

        ns_prefix = gupnp_xml_namespaces[ns].prefix;
        ns_uri = gupnp_xml_namespaces[ns].uri;

        ns_list = xmlGetNsList (doc, xmlDocGetRootElement (doc));
        if (ns_list == NULL)
                return NULL;

        for (it = ns_list; *it != NULL; it++) {
                const char *it_prefix = (const char *) (*it)->prefix;
                const char *it_uri = (const char *) (*it)->href;

                if (it_prefix == NULL) {
                        if (ns_prefix != NULL)
                                continue;

                        if (g_ascii_strcasecmp (it_uri, ns_uri) == 0) {
                                retval = *it;
                                break;
                        }

                        continue;
                }

                if (g_ascii_strcasecmp (it_prefix, ns_prefix) == 0) {
                        retval = *it;
                        break;
                }
        }

        xmlFree (ns_list);

        return retval;
}

/**
 * av_xml_util_get_ns:
 * @doc: A #xmlDoc.
 * @ns: A #GUPnPXMLNamespace.
 * @ns_out: (out) (allow-none): return location for the namespace or %NULL.
 *
 * Lazy-create a XML namespace on @doc.
 *
 * If @ns_out is non-%NULL, the function will return @ns_out immediately.
 * @returns: either the existing #xmlNsPtr or a newly created one.
 */
xmlNsPtr
av_xml_util_get_ns (xmlDocPtr doc, GUPnPXMLNamespace ns, xmlNsPtr *ns_out)
{
        xmlNsPtr tmp_ns;

        /* User supplied namespace, just return that */
        if (ns_out != NULL && *ns_out != NULL)
                return *ns_out;

        tmp_ns = av_xml_util_lookup_namespace (doc, ns);
        if (!tmp_ns)
                tmp_ns = av_xml_util_create_namespace
                                        (xmlDocGetRootElement (doc),
                                         ns);

        if (ns_out != NULL)
                *ns_out = tmp_ns;

        return tmp_ns;
}

void
av_xml_util_set_int_prop (xmlNodePtr node, const char *name, int value)
{
        char *str;

        str = g_strdup_printf ("%d", value);
        xmlSetProp (node, (unsigned char *) name, (unsigned char *) str);
        g_free (str);
}

void
av_xml_util_set_prop (xmlNodePtr node,
                      const char *name,
                      const char *format,
                      ...)
{
        va_list args;

        va_start (args, format);
        char *str = NULL;
        g_vasprintf (&str, format, args);
        xmlSetProp (node, (xmlChar *) name, (xmlChar *) str);
        g_free (str);
        va_end (args);
}

void
av_xml_util_set_ns_prop (xmlNodePtr node,
                         xmlNsPtr ns,
                         const char *name,
                         const char *format,
                         ...)
{
        va_list args;

        va_start (args, format);
        char *str = NULL;
        g_vasprintf(&str, format, args);
        xmlSetNsProp (node, ns, (xmlChar *) name, (xmlChar *) str);
        g_free (str);
        va_end (args);
}

G_DEFINE_BOXED_TYPE (GUPnPAVXMLDoc, av_xml_doc, g_rc_box_acquire, av_xml_doc_unref)
