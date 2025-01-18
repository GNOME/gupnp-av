/*
 * Copyright (C) 2009 Nokia Corporation.
 *
 * Authors: Zeeshan Ali (Khattak) <zeeshan.ali@nokia.com>
 *                                <zeeshanak@gnome.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */

/**
 * GUPnPDIDLLiteContributor:
 *
 * Contributor attached to a DIDL-Lite object
 *
 * This represents a contributor (artist, author, actor,
 * producer, director, producer and contributor) property in a DIDL-Lite object.
 */

#include <config.h>

#include "gupnp-didl-lite-contributor.h"
#include "gupnp-didl-lite-contributor-private.h"
#include "xml-util.h"

typedef struct _GUPnPDIDLLiteContributorPrivate {
        xmlNode     *xml_node;
        GUPnPAVXMLDoc *xml_doc;
} GUPnPDIDLLiteContributorPrivate;

G_DEFINE_TYPE_WITH_PRIVATE (GUPnPDIDLLiteContributor,
                            gupnp_didl_lite_contributor,
                            G_TYPE_OBJECT)

enum {
        PROP_0,
        PROP_XML_NODE,
        PROP_XML_DOC,
        PROP_ROLE,
        PROP_NAME
};

static void
gupnp_didl_lite_contributor_init (GUPnPDIDLLiteContributor *contributor)
{
}

static void
gupnp_didl_lite_contributor_get_property (GObject    *object,
                                          guint       property_id,
                                          GValue     *value,
                                          GParamSpec *pspec)
{
        GUPnPDIDLLiteContributor *contributor;

        contributor = GUPNP_DIDL_LITE_CONTRIBUTOR (object);

        switch (property_id) {
        case PROP_XML_NODE:
                g_value_set_pointer
                        (value,
                         gupnp_didl_lite_contributor_get_xml_node
                                        (contributor));
                break;
        case PROP_ROLE:
                g_value_set_string
                        (value,
                         gupnp_didl_lite_contributor_get_role (contributor));
                break;
        case PROP_NAME:
                g_value_set_string
                        (value,
                         gupnp_didl_lite_contributor_get_name (contributor));
                break;
        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
                break;
        }
}

static void
gupnp_didl_lite_contributor_set_property (GObject      *object,
                                          guint         property_id,
                                          const GValue *value,
                                          GParamSpec   *pspec)

{
        GUPnPDIDLLiteContributor *contributor =
                GUPNP_DIDL_LITE_CONTRIBUTOR (object);
        GUPnPDIDLLiteContributorPrivate *priv =
                gupnp_didl_lite_contributor_get_instance_private (contributor);

        switch (property_id) {
        case PROP_XML_NODE:
                priv->xml_node = g_value_get_pointer (value);
                break;
        case PROP_XML_DOC:
                priv->xml_doc = g_value_dup_boxed (value);
                break;
        case PROP_ROLE:
                gupnp_didl_lite_contributor_set_role
                        (contributor,
                         g_value_get_string (value));
                break;
        case PROP_NAME:
                gupnp_didl_lite_contributor_set_name
                        (contributor,
                         g_value_get_string (value));
                break;
        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
                break;
        }
}

static void
gupnp_didl_lite_contributor_dispose (GObject *object)
{
        GObjectClass                    *object_class;

        GUPnPDIDLLiteContributor *contributor =
                GUPNP_DIDL_LITE_CONTRIBUTOR (object);
        GUPnPDIDLLiteContributorPrivate *priv =
                gupnp_didl_lite_contributor_get_instance_private (contributor);

        g_clear_pointer (&priv->xml_doc, av_xml_doc_unref);

        object_class = G_OBJECT_CLASS
                        (gupnp_didl_lite_contributor_parent_class);
        object_class->dispose (object);
}

static void
gupnp_didl_lite_contributor_class_init (GUPnPDIDLLiteContributorClass *klass)
{
        GObjectClass *object_class;

        object_class = G_OBJECT_CLASS (klass);

        object_class->get_property = gupnp_didl_lite_contributor_get_property;
        object_class->set_property = gupnp_didl_lite_contributor_set_property;
        object_class->dispose = gupnp_didl_lite_contributor_dispose;

        /**
         * GUPnPDIDLLiteContributor:xml-node:(attributes org.gtk.Property.get=gupnp_didl_lite_contributor_get_xml_node):
         *
         * The pointer to object node in XML document.
         **/
        g_object_class_install_property
                (object_class,
                 PROP_XML_NODE,
                 g_param_spec_pointer ("xml-node",
                                       "XMLNode",
                                       "The pointer to contributor node in XML"
                                       " document.",
                                       G_PARAM_READWRITE |
                                       G_PARAM_CONSTRUCT_ONLY |
                                       G_PARAM_STATIC_NAME |
                                       G_PARAM_STATIC_NICK |
                                       G_PARAM_STATIC_BLURB));

        /**
         * GUPnPDIDLLiteContributor:xml-doc:
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
                 g_param_spec_boxed ("xml-doc",
                                      "XMLDoc",
                                      "The reference to XML document"
                                      " containing this contributor.",
                                      av_xml_doc_get_type (),
                                      G_PARAM_WRITABLE |
                                      G_PARAM_CONSTRUCT_ONLY |
                                      G_PARAM_STATIC_NAME |
                                      G_PARAM_STATIC_NICK |
                                      G_PARAM_STATIC_BLURB));

        /**
         * GUPnPDIDLLiteContributor:role:(attributes org.gtk.Property.get=gupnp_didl_lite_contributor_get_role org.gtk.Property.set=gupnp_didl_lite_contributor_set_role):
         *
         * The role of this contributor.
         **/
        g_object_class_install_property
                (object_class,
                 PROP_ROLE,
                 g_param_spec_string ("role",
                                      "Role",
                                      "The role of this contributor.",
                                      NULL,
                                      G_PARAM_READWRITE |
                                      G_PARAM_STATIC_NAME |
                                      G_PARAM_STATIC_NICK |
                                      G_PARAM_STATIC_BLURB));

        /**
         * GUPnPDIDLLiteContributor:name:(attributes org.gtk.Property.get=gupnp_didl_lite_contributor_get_role org.gtk.Property.set=gupnp_didl_lite_contributor_set_role):
         *
         * The name of this contributor.
         **/
        g_object_class_install_property
                (object_class,
                 PROP_NAME,
                 g_param_spec_string ("name",
                                      "Name",
                                      "The name of this contributor.",
                                      NULL,
                                      G_PARAM_READWRITE |
                                      G_PARAM_STATIC_NAME |
                                      G_PARAM_STATIC_NICK |
                                      G_PARAM_STATIC_BLURB));
}

/**
 * gupnp_didl_lite_contributor_get_role:(attributes org.gtk.Method.get_property=role):
 * @contributor: #GUPnPDIDLLiteContributor
 *
 * Get the role of the @contributor.
 *
 * Return value: The role of the @contributor, or %NULL.
 **/
const char *
gupnp_didl_lite_contributor_get_role (GUPnPDIDLLiteContributor *contributor)
{
        g_return_val_if_fail (contributor != NULL, NULL);
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_CONTRIBUTOR (contributor),
                              NULL);
        GUPnPDIDLLiteContributorPrivate *priv =
                gupnp_didl_lite_contributor_get_instance_private (contributor);

        return av_xml_util_get_attribute_content (priv->xml_node, "role");
}

/**
 * gupnp_didl_lite_contributor_get_name:(attributes org.gtk.Method.get_property=name):
 * @contributor: A #GUPnPDIDLLiteContributor
 *
 * Get the name of the @contributor.
 *
 * Return value: The name of the @contributor or %NULL.
 **/
const char *
gupnp_didl_lite_contributor_get_name (GUPnPDIDLLiteContributor *contributor)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_CONTRIBUTOR (contributor),
                              NULL);
        GUPnPDIDLLiteContributorPrivate *priv =
                gupnp_didl_lite_contributor_get_instance_private (contributor);

        return (const char *) priv->xml_node->children->content;
}

/**
 * gupnp_didl_lite_contributor_set_role:(attributes org.gtk.Method.set_property=role):
 * @contributor: #GUPnPDIDLLiteContributor
 * @role: The role of the @contributor
 *
 * Set the role of the @contributor to @role.
 **/
void
gupnp_didl_lite_contributor_set_role (GUPnPDIDLLiteContributor *contributor,
                                      const char               *role)
{
        g_return_if_fail (contributor != NULL);
        g_return_if_fail (GUPNP_IS_DIDL_LITE_CONTRIBUTOR (contributor));

        GUPnPDIDLLiteContributorPrivate *priv =
                gupnp_didl_lite_contributor_get_instance_private (contributor);

        xmlSetProp (priv->xml_node,
                    (unsigned char *) "role",
                    (unsigned char *) role);

        g_object_notify (G_OBJECT (contributor), "role");
}

/**
 * gupnp_didl_lite_contributor_set_name:(attributes org.gtk.Method.set_property=name):
 * @contributor: A #GUPnPDIDLLiteContributor
 * @name: The name of the contributor
 *
 * Set the name of the @contributor to @name.
 **/
void
gupnp_didl_lite_contributor_set_name (GUPnPDIDLLiteContributor *contributor,
                                      const char               *name)
{
        xmlChar *escaped;

        g_return_if_fail (GUPNP_IS_DIDL_LITE_CONTRIBUTOR (contributor));
        g_return_if_fail (name != NULL);

        GUPnPDIDLLiteContributorPrivate *priv =
                gupnp_didl_lite_contributor_get_instance_private (contributor);

        escaped = xmlEncodeSpecialChars (priv->xml_doc->doc,
                                         (const unsigned char *) name);
        xmlNodeSetContent (priv->xml_node, escaped);
        xmlFree (escaped);

        g_object_notify (G_OBJECT (contributor), "name");
}

/**
 * gupnp_didl_lite_contributor_new_from_xml:
 * @xml_node: The pointer to relevant node in XML document
 * @xml_doc: The reference to containing XML document
 *
 * Creates a new #GUPnPDIDLLiteContributor for the @xml_node.
 *
 * Return value: A new #GUPnPDIDLLiteContributor object. Unref after usage.
 **/
GUPnPDIDLLiteContributor *
gupnp_didl_lite_contributor_new_from_xml (xmlNode       *xml_node,
                                          GUPnPAVXMLDoc *xml_doc)
{
        return g_object_new (GUPNP_TYPE_DIDL_LITE_CONTRIBUTOR,
                             "xml-node", xml_node,
                             "xml-doc", xml_doc,
                             NULL);
}

/**
 * gupnp_didl_lite_contributor_get_xml_node:(attributes org.gtk.Method.get_property=xml-node):
 * @contributor: The #GUPnPDIDLLiteContributor
 *
 * Get the pointer to relevant node in XML document.
 *
 * Returns: (transfer none): The pointer to relevant node in XML document.
 **/
xmlNode *
gupnp_didl_lite_contributor_get_xml_node (GUPnPDIDLLiteContributor *contributor)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_CONTRIBUTOR (contributor),
                              NULL);
        GUPnPDIDLLiteContributorPrivate *priv =
                gupnp_didl_lite_contributor_get_instance_private (contributor);

        return priv->xml_node;
}
