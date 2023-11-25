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

#ifndef XML_UTIL_H
#define XML_UTIL_H

#include <glib.h>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <stdarg.h>
#include <glib-object.h>

typedef enum _GUPnPXMLNamespace
{
        GUPNP_XML_NAMESPACE_DIDL_LITE,
        GUPNP_XML_NAMESPACE_DC,
        GUPNP_XML_NAMESPACE_DLNA,
        GUPNP_XML_NAMESPACE_PV,
        GUPNP_XML_NAMESPACE_UPNP,
        GUPNP_XML_NAMESPACE_COUNT
} GUPnPXMLNamespace;

G_BEGIN_DECLS

typedef struct _GPnPAVXMLDoc
{
    xmlDoc *doc;
} GUPnPAVXMLDoc;

G_GNUC_INTERNAL GUPnPAVXMLDoc *
av_xml_doc_new                             (xmlDoc *doc);

G_GNUC_INTERNAL GUPnPAVXMLDoc *
av_xml_doc_ref                             (GUPnPAVXMLDoc *doc);

G_GNUC_INTERNAL void
av_xml_doc_unref                           (GUPnPAVXMLDoc *doc);

G_GNUC_INTERNAL GType
av_xml_doc_get_type                        (void) G_GNUC_CONST;

/* Misc utilities for inspecting xmlNodes */
G_GNUC_INTERNAL xmlNode *
av_xml_util_get_element                    (xmlNode    *node,
                                             ...) G_GNUC_NULL_TERMINATED;

G_GNUC_INTERNAL GList *
av_xml_util_get_child_elements_by_name     (xmlNode *node,
                                            const char *name);

G_GNUC_INTERNAL const char *
av_xml_util_get_child_element_content      (xmlNode    *node,
                                            const char *child_name);

G_GNUC_INTERNAL guint
av_xml_util_get_uint_child_element         (xmlNode    *node,
                                            const char *child_name,
                                            guint       default_value);

G_GNUC_INTERNAL guint64
av_xml_util_get_uint64_child_element       (xmlNode    *node,
                                            const char *child_name,
                                            guint64     default_value);

G_GNUC_INTERNAL const char *
av_xml_util_get_attribute_content          (xmlNode    *node,
                                            const char *attribute_name);

G_GNUC_INTERNAL gboolean
av_xml_util_get_boolean_attribute          (xmlNode    *node,
                                            const char *attribute_name);

G_GNUC_INTERNAL guint
av_xml_util_get_uint_attribute             (xmlNode    *node,
                                            const char *attribute_name,
                                            guint       default_value);

G_GNUC_INTERNAL gint
av_xml_util_get_int_attribute              (xmlNode    *node,
                                            const char *attribute_name,
                                            gint        default_value);

G_GNUC_INTERNAL glong
av_xml_util_get_long_attribute             (xmlNode    *node,
                                            const char *attribute_name,
                                            glong       default_value);

G_GNUC_INTERNAL gint64
av_xml_util_get_int64_attribute            (xmlNode    *node,
                                            const char *attribute_name,
                                            gint64      default_value);

G_GNUC_INTERNAL xmlNode *
av_xml_util_set_child                      (xmlNode          *parent_node,
                                            GUPnPXMLNamespace ns,
                                            xmlNsPtr         *namespace,
                                            xmlDoc           *doc,
                                            const char       *name,
                                            const char       *value);

G_GNUC_INTERNAL void
av_xml_util_unset_child                    (xmlNode    *parent_node,
                                            const char *name);

G_GNUC_INTERNAL gboolean
av_xml_util_verify_attribute_is_boolean    (xmlNode    *node,
                                            const char *attribute_name);

G_GNUC_INTERNAL char *
av_xml_util_get_child_string               (xmlNode    *parent_node,
                                            xmlDoc     *doc,
                                            const char *name);

G_GNUC_INTERNAL gboolean
av_xml_util_node_deep_equal                (xmlNode *first,
                                            xmlNode *second);

G_GNUC_INTERNAL xmlNode *
av_xml_util_find_node                      (xmlNode *haystack,
                                            xmlNode *needle);

G_GNUC_INTERNAL xmlNode *
av_xml_util_copy_node                      (xmlNode *node);

G_GNUC_INTERNAL GHashTable *
av_xml_util_get_attributes_map             (xmlNode *node);

G_GNUC_INTERNAL xmlNsPtr
av_xml_util_create_namespace               (xmlNodePtr root,
                                            GUPnPXMLNamespace ns);

G_GNUC_INTERNAL xmlNsPtr
av_xml_util_lookup_namespace               (xmlDocPtr doc,
                                            GUPnPXMLNamespace ns);

G_GNUC_INTERNAL xmlNsPtr
av_xml_util_get_ns                         (xmlDocPtr doc,
                                            GUPnPXMLNamespace ns,
                                            xmlNsPtr *ns_out);
G_GNUC_INTERNAL void
av_xml_util_set_int_prop (xmlNodePtr node, const char *name, int value);

G_GNUC_INTERNAL void
av_xml_util_set_prop (xmlNodePtr node, const char *name, const char *format, ...) G_GNUC_PRINTF(3, 4);

G_GNUC_INTERNAL void
av_xml_util_set_ns_prop (xmlNodePtr node, xmlNsPtr ns, const char *name, const char *format, ...) G_GNUC_PRINTF(4, 5);

G_END_DECLS

#endif /* __XML_UTIL_H__ */
