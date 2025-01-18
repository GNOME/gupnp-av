/*
 * Copyright (C) 2012 Intel Corporation.
 * Copyright (C) 2009 Nokia Corporation.
 *
 * Author: Christophe Guiraud <christophe.guiraud@intel.com>
 * Author: Zeeshan Ali (Khattak) <zeeshanak@gnome.org>
 *                               <zeeshan.ali@nokia.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */

/**
 * GUPnPDIDLLiteCreateClass:
 *
 * DIDL-Lite CreateClass
 *
 * [class@GUPnPAV.DIDLLiteCreateClass] respresents a DIDL-Lite create class
 * element.
 */

#include <config.h>

#include <string.h>

#include "gupnp-didl-lite-createclass.h"
#include "gupnp-didl-lite-createclass-private.h"
#include "xml-util.h"

struct _GUPnPDIDLLiteCreateClassPrivate {
        xmlNode     *xml_node;
        GUPnPAVXMLDoc *xml_doc;
};

typedef struct _GUPnPDIDLLiteCreateClassPrivate GUPnPDIDLLiteCreateClassPrivate;

G_DEFINE_TYPE_WITH_PRIVATE (GUPnPDIDLLiteCreateClass,
                            gupnp_didl_lite_create_class,
                            G_TYPE_OBJECT)

enum {
        PROP_0,
        PROP_XML_NODE,
        PROP_XML_DOC,
        PROP_CONTENT,
        PROP_INCLUDE_DERIVED,
        PROP_FRIENDLY_NAME,
};

static void
gupnp_didl_lite_create_class_init (GUPnPDIDLLiteCreateClass *create_class)
{
}

static void
gupnp_didl_lite_create_class_get_property (GObject    *object,
                                           guint       property_id,
                                           GValue     *value,
                                           GParamSpec *pspec)
{
        GUPnPDIDLLiteCreateClass *create_class;

        create_class = GUPNP_DIDL_LITE_CREATE_CLASS (object);

        switch (property_id) {
        case PROP_XML_NODE:
                g_value_set_pointer
                    (value,
                     gupnp_didl_lite_create_class_get_xml_node
                                                            (create_class));
                break;
        case PROP_CONTENT:
                g_value_set_string
                    (value,
                     gupnp_didl_lite_create_class_get_content
                                                            (create_class));
                break;
        case PROP_INCLUDE_DERIVED:
                g_value_set_boolean
                    (value,
                     gupnp_didl_lite_create_class_get_include_derived
                                                            (create_class));
                break;
        case PROP_FRIENDLY_NAME:
                g_value_set_string
                    (value,
                     gupnp_didl_lite_create_class_get_friendly_name
                                                            (create_class));
                break;
        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
                break;
        }
}

static void
gupnp_didl_lite_create_class_set_property (GObject      *object,
                                            guint         property_id,
                                            const GValue *value,
                                            GParamSpec   *pspec)
{
        GUPnPDIDLLiteCreateClass *create_class;

        create_class = GUPNP_DIDL_LITE_CREATE_CLASS (object);
        GUPnPDIDLLiteCreateClassPrivate *priv =
                gupnp_didl_lite_create_class_get_instance_private (
                        create_class);

        switch (property_id) {
        case PROP_XML_NODE:
                priv->xml_node = g_value_get_pointer (value);
                break;
        case PROP_XML_DOC:
                priv->xml_doc = g_value_dup_boxed (value);
                break;
        case PROP_CONTENT:
                gupnp_didl_lite_create_class_set_content
                                                 (create_class,
                                                  g_value_get_string (value));
                break;
        case PROP_INCLUDE_DERIVED:
                gupnp_didl_lite_create_class_set_include_derived
                                                 (create_class,
                                                  g_value_get_boolean (value));
                break;
        case PROP_FRIENDLY_NAME:
                gupnp_didl_lite_create_class_set_friendly_name
                                                 (create_class,
                                                  g_value_get_string (value));
                break;
        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
                break;
        }
}

static void
gupnp_didl_lite_create_class_dispose (GObject *object)
{
        GObjectClass                    *object_class;
        GUPnPDIDLLiteCreateClassPrivate *priv =
                gupnp_didl_lite_create_class_get_instance_private (
                        GUPNP_DIDL_LITE_CREATE_CLASS (object));

        g_clear_pointer (&priv->xml_doc, av_xml_doc_unref);

        object_class = G_OBJECT_CLASS
                                   (gupnp_didl_lite_create_class_parent_class);
        object_class->dispose (object);
}

static void
gupnp_didl_lite_create_class_class_init (GUPnPDIDLLiteCreateClassClass *klass)
{
        GObjectClass *object_class;

        object_class = G_OBJECT_CLASS (klass);

        object_class->get_property = gupnp_didl_lite_create_class_get_property;
        object_class->set_property = gupnp_didl_lite_create_class_set_property;
        object_class->dispose = gupnp_didl_lite_create_class_dispose;

        /**
         * GUPnPDIDLLiteCreateClass:xml-node:
         *
         * The pointer to desc node in XML document.
         **/
        g_object_class_install_property
                (object_class,
                 PROP_XML_NODE,
                 g_param_spec_pointer
                                   ("xml-node",
                                    "XMLNode",
                                    "The pointer to desc node in XML"
                                    " document.",
                                    G_PARAM_READWRITE |
                                    G_PARAM_CONSTRUCT_ONLY |
                                    G_PARAM_STATIC_NAME |
                                    G_PARAM_STATIC_NICK |
                                    G_PARAM_STATIC_BLURB));

        /**
         * GUPnPDIDLLiteCreateClass:xml-doc:
         *
         * The reference to XML document containing this object.
         *
         * Internal property.
         *
         * Stability: Private
         **/
        g_object_class_install_property
                (object_class,
                 PROP_XML_DOC,
                 g_param_spec_boxed
                                   ("xml-doc",
                                    "XMLDoc",
                                    "The reference to XML document"
                                    " containing this object.",
                                    av_xml_doc_get_type (),
                                    G_PARAM_WRITABLE |
                                    G_PARAM_CONSTRUCT_ONLY |
                                    G_PARAM_STATIC_NAME |
                                    G_PARAM_STATIC_NICK |
                                    G_PARAM_STATIC_BLURB));

        /**
         * GUPnPDIDLLiteCreateClass:content:
         *
         * The content of this create Class.
         **/
        g_object_class_install_property
                (object_class,
                 PROP_CONTENT,
                 g_param_spec_string
                                   ("content",
                                    "Content",
                                    "The content of this create Class.",
                                    NULL,
                                    G_PARAM_READWRITE |
                                    G_PARAM_STATIC_NAME |
                                    G_PARAM_STATIC_NICK |
                                    G_PARAM_STATIC_BLURB));

        /**
         * GUPnPDIDLLiteCreateClass:include-derived:
         *
         * Whether this create Class can be derived.
         **/
        g_object_class_install_property
                (object_class,
                 PROP_INCLUDE_DERIVED,
                 g_param_spec_boolean
                                   ("include-derived",
                                    "IncludeDerived",
                                    "Wether this create Class can be derived.",
                                    FALSE,
                                    G_PARAM_READWRITE |
                                    G_PARAM_STATIC_NAME |
                                    G_PARAM_STATIC_NICK |
                                    G_PARAM_STATIC_BLURB));

        /**
         * GUPnPDIDLLiteCreateClass:friendly-name:
         *
         * The friendly name of this create Class.
         **/
        g_object_class_install_property
                (object_class,
                 PROP_FRIENDLY_NAME,
                 g_param_spec_string
                                   ("friendly-name",
                                    "FriendlyName",
                                    "The friendly name of this create Class.",
                                    NULL,
                                    G_PARAM_READWRITE |
                                    G_PARAM_STATIC_NAME |
                                    G_PARAM_STATIC_NICK |
                                    G_PARAM_STATIC_BLURB));
}

/**
 * gupnp_didl_lite_create_class_get_content:
 * @create_class: [class@GUPnPAV.DIDLLiteCreateClass]
 *
 * Get the content of the @create_class.
 *
 * Return value: The Content of the @create_class, or %NULL.
 **/
const char *
gupnp_didl_lite_create_class_get_content
                                    (GUPnPDIDLLiteCreateClass *create_class)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_CREATE_CLASS (create_class),
                              NULL);
        GUPnPDIDLLiteCreateClassPrivate *priv =
                gupnp_didl_lite_create_class_get_instance_private (
                        create_class);

        if (G_UNLIKELY (priv->xml_node->children == NULL))
                return NULL;

        return (const char *) priv->xml_node->children->content;
}

/**
 * gupnp_didl_lite_create_class_set_content:
 * @create_class: [class@GUPnPAV.DIDLLiteCreateClass]
 * @content: The content
 *
 * Set the content of the @create_class.
 **/
void
gupnp_didl_lite_create_class_set_content
                                    (GUPnPDIDLLiteCreateClass *create_class,
                                     const char               *content)
{
        g_return_if_fail (GUPNP_IS_DIDL_LITE_CREATE_CLASS (create_class));
        g_return_if_fail (create_class != NULL);

        GUPnPDIDLLiteCreateClassPrivate *priv =
                gupnp_didl_lite_create_class_get_instance_private (
                        create_class);
        xmlChar *escaped =
                xmlEncodeSpecialChars (priv->xml_doc->doc,
                                       (const unsigned char *) content);
        xmlNodeSetContent (priv->xml_node, escaped);
        xmlFree (escaped);

        g_object_notify (G_OBJECT (create_class), "content");
}

/**
 * gupnp_didl_lite_create_class_get_include_derived:
 * @create_class: [class@GUPnPAV.DIDLLiteCreateClass]
 *
 * Checks whether @create_class can be derived.
 *
 * Return value: #TRUE if @create_class can be derived.
 **/
gboolean
gupnp_didl_lite_create_class_get_include_derived
                                    (GUPnPDIDLLiteCreateClass *create_class)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_CREATE_CLASS (create_class),
                              FALSE);
        GUPnPDIDLLiteCreateClassPrivate *priv =
                gupnp_didl_lite_create_class_get_instance_private (
                        create_class);

        return av_xml_util_get_boolean_attribute (priv->xml_node,
                                                  "includeDerived");
}

/**
 * gupnp_didl_lite_create_class_set_include_derived:
 * @create_class: [class@GUPnPAV.DIDLLiteCreateClass]
 * @include_derived: the derivability
 *
 * (Un)set the derivability of create_class.
 **/
void
gupnp_didl_lite_create_class_set_include_derived
                                    (GUPnPDIDLLiteCreateClass *create_class,
                                     gboolean                 include_derived)
{
        const char *str;

        g_return_if_fail (create_class != NULL);
        g_return_if_fail (GUPNP_IS_DIDL_LITE_CREATE_CLASS (create_class));

        GUPnPDIDLLiteCreateClassPrivate *priv =
                gupnp_didl_lite_create_class_get_instance_private (
                        create_class);

        if (include_derived)
                str = "1";
        else
                str = "0";

        xmlSetProp (priv->xml_node,
                    (unsigned char *) "includeDerived",
                    (unsigned char *) str);

        g_object_notify (G_OBJECT (create_class), "include-derived");
}

/**
 * gupnp_didl_lite_create_class_get_friendly_name:
 * @create_class: [class@GUPnPAV.DIDLLiteCreateClass]
 *
 * Get the friendly name of the @create_class.
 *
 * Return value: The FriendlyName of the @create_class, or %NULL.
 **/
const char *
gupnp_didl_lite_create_class_get_friendly_name
                                    (GUPnPDIDLLiteCreateClass *create_class)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_CREATE_CLASS (create_class),
                              NULL);
        GUPnPDIDLLiteCreateClassPrivate *priv =
                gupnp_didl_lite_create_class_get_instance_private (
                        create_class);

        return av_xml_util_get_attribute_content (priv->xml_node, "name");
}

/**
 * gupnp_didl_lite_create_class_set_friendly_name:
 * @create_class: [class@GUPnPAV.DIDLLiteCreateClass]
 * @friendly_name: The friendly name
 *
 * Set the friendly name of the @create_class.
 **/
void
gupnp_didl_lite_create_class_set_friendly_name
                                    (GUPnPDIDLLiteCreateClass *create_class,
                                     const char               *friendly_name)
{
        g_return_if_fail (create_class != NULL);
        g_return_if_fail (GUPNP_IS_DIDL_LITE_CREATE_CLASS (create_class));

        GUPnPDIDLLiteCreateClassPrivate *priv =
                gupnp_didl_lite_create_class_get_instance_private (
                        create_class);

        xmlSetProp (priv->xml_node,
                    (unsigned char *) "name",
                    (const unsigned char *) friendly_name);

        g_object_notify (G_OBJECT (create_class), "friendly-name");
}

/**
 * gupnp_didl_lite_create_class_new_from_xml:
 * @xml_node: The pointer to relevant node in XML document
 * @xml_doc: The reference to containing XML document
 *
 * Creates a new [class@GUPnPAV.DIDLLiteCreateClass] for the @xml_node.
 *
 * Returns: (transfer full): A new [class@GUPnPAV.DIDLLiteCreateClass] object.
 *Unref after usage.
 **/
GUPnPDIDLLiteCreateClass *
gupnp_didl_lite_create_class_new_from_xml (xmlNode     *xml_node,
                                           GUPnPAVXMLDoc *xml_doc)
{
        return g_object_new (GUPNP_TYPE_DIDL_LITE_CREATE_CLASS,
                             "xml-node", xml_node,
                             "xml-doc", xml_doc,
                             NULL);
}

/**
 * gupnp_didl_lite_create_class_get_xml_node:
 * @create_class: The [class@GUPnPAV.DIDLLiteCreateClass]
 *
 * Get the pointer to relevant node in XML document.
 *
 * Returns: (transfer none): The pointer to relevant node in XML document.
 **/
xmlNode *
gupnp_didl_lite_create_class_get_xml_node
                                      (GUPnPDIDLLiteCreateClass *create_class)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_CREATE_CLASS (create_class),
                              NULL);
        GUPnPDIDLLiteCreateClassPrivate *priv =
                gupnp_didl_lite_create_class_get_instance_private (
                        create_class);

        return priv->xml_node;
}
