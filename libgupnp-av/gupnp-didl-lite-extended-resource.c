// SPDX-License-Identifier: LGPL-2.1-or-later

#include <config.h>

#include "gupnp-didl-lite-extended-resource.h"
#include "gupnp-didl-lite-extended-resource-private.h"
#include "xml-util.h"

#include <libxml/tree.h>

typedef struct _GUPnPDIDLLiteExtendedResourcePrivate {
        xmlNode *xml_node;
        xmlDoc *xml_doc;
} GUPnPDIDLLiteExtendedResourcePrivate;

G_DEFINE_ABSTRACT_TYPE_WITH_PRIVATE (GUPnPDIDLLiteExtendedResource,
                                     gupnp_didl_lite_extended_resource,
                                     G_TYPE_OBJECT)

enum
{
        PROP_0,
        PROP_XML_NODE,
        PROP_XML_DOC,
        PROP_ID
};

static void
gupnp_didl_lite_extended_resource_init (GUPnPDIDLLiteExtendedResource *self)
{
        // Nothing
}

static void
gupnp_didl_lite_extended_resource_set_property (GObject *object,
                                                guint property_id,
                                                const GValue *value,
                                                GParamSpec *pspec)
{
        GUPnPDIDLLiteExtendedResource *self =
                GUPNP_DIDL_LITE_EXTENDED_RESOURCE (object);
        GUPnPDIDLLiteExtendedResourcePrivate *priv;

        priv = gupnp_didl_lite_extended_resource_get_instance_private (self);

        switch (property_id) {
        case PROP_XML_NODE:
                priv->xml_node = g_value_get_pointer (value);
                break;
        case PROP_XML_DOC:
                priv->xml_doc = g_value_dup_boxed (value);
                break;
        case PROP_ID:
                gupnp_didl_lite_extended_resource_set_id (
                        self,
                        g_value_get_string (value));
                break;
        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
                break;
        }
}

static void
gupnp_didl_lite_extended_resource_get_property (GObject *object,
                                                guint property_id,
                                                GValue *value,
                                                GParamSpec *pspec)
{
        GUPnPDIDLLiteExtendedResource *self =
                GUPNP_DIDL_LITE_EXTENDED_RESOURCE (object);
        GUPnPDIDLLiteExtendedResourcePrivate *priv;

        priv = gupnp_didl_lite_extended_resource_get_instance_private (self);

        switch (property_id) {
        case PROP_XML_NODE:
                g_value_set_pointer (value, priv->xml_node);
                break;
        case PROP_ID:
                g_value_set_string (
                        value,
                        gupnp_didl_lite_extended_resource_get_id (self));
                break;
        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
                break;
        }
}

static void
gupnp_didl_lite_extended_resource_class_init (
        GUPnPDIDLLiteExtendedResourceClass *klass)
{
        GObjectClass *object_class = G_OBJECT_CLASS (klass);

        object_class->set_property =
                gupnp_didl_lite_extended_resource_set_property;
        object_class->get_property =
                gupnp_didl_lite_extended_resource_get_property;

        /**
         * GUPnPDIDLLiteExtendedResource:xml-node:
         *
         * The pointer to object node in XML document.
         **/
        g_object_class_install_property (
                object_class,
                PROP_XML_NODE,
                g_param_spec_pointer ("xml-node",
                                      "XMLNode",
                                      "The pointer to object node in XML"
                                      " document.",
                                      G_PARAM_READWRITE |
                                              G_PARAM_CONSTRUCT_ONLY |
                                              G_PARAM_STATIC_STRINGS));

        /**
         * GUPnPDIDLLiteExtendedResource:xml-doc:
         *
         * The reference to XML document containing this object.
         *
         * Internal property.
         *
         * Stability: Private
         **/
        g_object_class_install_property (
                object_class,
                PROP_XML_DOC,
                g_param_spec_boxed ("xml-doc",
                                    "XMLDoc",
                                    "The reference to XML document"
                                    " containing this object.",
                                    av_xml_doc_get_type (),
                                    G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY |
                                            G_PARAM_STATIC_STRINGS));

        /**
         * GUPnPDIDLLiteExtendedResource:id:
         *
         * The ID of this object.
         **/
        g_object_class_install_property (
                object_class,
                PROP_ID,
                g_param_spec_string ("id",
                                     "ID",
                                     "The ID of this object.",
                                     NULL,
                                     G_PARAM_READWRITE |
                                             G_PARAM_STATIC_STRINGS));
}

const char *
gupnp_didl_lite_extended_resource_get_id (GUPnPDIDLLiteExtendedResource *self)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_EXTENDED_RESOURCE (self),
                              NULL);
        GUPnPDIDLLiteExtendedResourcePrivate *priv;
        priv = gupnp_didl_lite_extended_resource_get_instance_private (self);

        return av_xml_util_get_attribute_content (priv->xml_node, "id");
}

void
gupnp_didl_lite_extended_resource_set_id (GUPnPDIDLLiteExtendedResource *self,
                                          const char *id)
{
        g_return_if_fail (GUPNP_IS_DIDL_LITE_EXTENDED_RESOURCE (self));
        GUPnPDIDLLiteExtendedResourcePrivate *priv;
        priv = gupnp_didl_lite_extended_resource_get_instance_private (self);

        xmlSetProp (priv->xml_node, BAD_CAST "id", BAD_CAST id);

        g_object_notify (G_OBJECT (self), "id");
}

xmlNode *
gupnp_didl_lite_extended_resource_get_xml_node (
        GUPnPDIDLLiteExtendedResource *self)
{
        GUPnPDIDLLiteExtendedResourcePrivate *priv;
        priv = gupnp_didl_lite_extended_resource_get_instance_private (self);

        return priv->xml_node;
}
