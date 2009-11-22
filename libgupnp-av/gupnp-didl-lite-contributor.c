/*
 * Copyright (C) 2009 Nokia Corporation.
 *
 * Authors: Zeeshan Ali (Khattak) <zeeshan.ali@nokia.com>
 *                                <zeeshanak@gnome.org>
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
 * SECTION:gupnp-didl-lite-contributor
 * @short_description: DIDL-Lite Contributor
 *
 * #GUPnPDIDLLiteContributor respresents a contributor (artist, author, actor,
 * producer, director, producer and contributor) property in a DIDL-Lite object.
 */

#include <libgupnp/gupnp.h>

#include "gupnp-didl-lite-contributor.h"
#include "xml-util.h"

G_DEFINE_TYPE (GUPnPDIDLLiteContributor,
               gupnp_didl_lite_contributor,
               G_TYPE_OBJECT);

struct _GUPnPDIDLLiteContributorPrivate {
        xmlNode     *xml_node;
        GUPnPXMLDoc *xml_doc;
};

enum {
        PROP_0,
        PROP_ROLE,
        PROP_NAME
};

static void
gupnp_didl_lite_contributor_init (GUPnPDIDLLiteContributor *contributor)
{
        contributor->priv = G_TYPE_INSTANCE_GET_PRIVATE
                                        (contributor,
                                         GUPNP_TYPE_DIDL_LITE_CONTRIBUTOR,
                                         GUPnPDIDLLiteContributorPrivate);
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
        GUPnPDIDLLiteContributor *contributor;

        contributor = GUPNP_DIDL_LITE_CONTRIBUTOR (object);

        switch (property_id) {
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
        GUPnPDIDLLiteContributorPrivate *priv;

        priv = GUPNP_DIDL_LITE_CONTRIBUTOR (object)->priv;

        if (priv->xml_doc) {
                g_object_unref (priv->xml_doc);
                priv->xml_doc = NULL;
        }

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
         * GUPnPDIDLLiteContributor:role
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
         * GUPnPDIDLLiteContributor:name
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
 * gupnp_didl_lite_contributor_get_role
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

        return xml_util_get_attribute_content (contributor->priv->xml_node,
                                               "role");
}

/**
 * gupnp_didl_lite_contributor_get_name
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

        return (const char *) contributor->priv->xml_node->children->content;
}

/**
 * gupnp_didl_lite_contributor_set_role
 * @contributor: #GUPnPDIDLLiteContributor
 * @role: The role of the @contributor
 *
 * Set the role of the @contributor.
 **/
void
gupnp_didl_lite_contributor_set_role (GUPnPDIDLLiteContributor *contributor,
                                      const char               *role)
{
        g_return_if_fail (contributor != NULL);
        g_return_if_fail (GUPNP_IS_DIDL_LITE_CONTRIBUTOR (contributor));

        xmlSetProp (contributor->priv->xml_node,
                    (unsigned char *) "role",
                    (unsigned char *) role);

        g_object_notify (G_OBJECT (contributor), "role");
}

/**
 * gupnp_didl_lite_contributor_set_name
 * @contributor: A #GUPnPDIDLLiteContributor
 * @name: The name of the contributor
 *
 * Set the name of the @contributor.
 *
 * Return value: None.
 **/
void
gupnp_didl_lite_contributor_set_name (GUPnPDIDLLiteContributor *contributor,
                                      const char               *name)
{
        xmlChar *escaped;

        g_return_if_fail (GUPNP_IS_DIDL_LITE_CONTRIBUTOR (contributor));
        g_return_if_fail (name != NULL);

        escaped = xmlEncodeSpecialChars (contributor->priv->xml_doc->doc,
                                         (const unsigned char *) name);
        xmlNodeSetContent (contributor->priv->xml_node, escaped);
        xmlFree (escaped);

        g_object_notify (G_OBJECT (contributor), "name");
}

/**
 * gupnp_didl_lite_contributor_new_from_xml
 * @xml_node: The pointer to relevant node in XML document
 * @xml_doc: The reference to containing XML document
 *
 * Creates a new #GUPnPDIDLLiteContributor for the @xml_node.
 *
 * Return value: A new #GUPnPDIDLLiteContributor object. Unref after usage.
 **/
GUPnPDIDLLiteContributor *
gupnp_didl_lite_contributor_new_from_xml (xmlNode     *xml_node,
                                          GUPnPXMLDoc *xml_doc)
{
        GUPnPDIDLLiteContributor *contributor;

        return g_object_new (GUPNP_TYPE_DIDL_LITE_CONTRIBUTOR,
                             "xml-node", xml_node,
                             "xml-doc", xml_doc,
                             NULL);

        return contributor;
}

/**
 * gupnp_didl_lite_contributor_get_xml_node
 * @contributor: The #GUPnPDIDLLiteContributor
 *
 * Get the pointer to relavent node in XML document.
 *
 * Return value: The pointer to relavent node in XML document.
 **/
xmlNode *
gupnp_didl_lite_contributor_get_xml_node (GUPnPDIDLLiteContributor *contributor)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_CONTRIBUTOR (contributor),
                              NULL);

        return contributor->priv->xml_node;
}
