/*
 * Copyright (C) 2012 Intel Corporation
 *
 * Authors: Regis Merlino <regis.merlino@intel.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */

/**
 * SECTION:gupnp-feature
 * @short_description: ContentDirectory feature
 *
 * #GUPnPFeature respresent a Feature element.
 */

#include <config.h>

#include "gupnp-feature.h"
#include "gupnp-feature-private.h"

#include "xml-util.h"

#include <libxml/tree.h>

struct _GUPnPFeaturePrivate {
        char *name;
        char *version;
        char *object_ids;
        GUPnPAVXMLDoc *doc;
        xmlNode *node;
};
typedef struct _GUPnPFeaturePrivate GUPnPFeaturePrivate;

G_DEFINE_TYPE_WITH_PRIVATE (GUPnPFeature,
                            gupnp_feature,
                            G_TYPE_OBJECT)

enum {
        PROP_0,
        PROP_NAME,
        PROP_VERSION,
        PROP_OBJECT_IDS,
        PROP_DOC,
        PROP_NODE
};

static void
gupnp_feature_init (GUPnPFeature *object)
{
}

static void
gupnp_feature_finalize (GObject *object)
{
        GObjectClass        *object_class;
        GUPnPFeaturePrivate *priv;

        priv = gupnp_feature_get_instance_private (GUPNP_FEATURE (object));

        g_free (priv->name);
        g_free (priv->version);
        g_free (priv->object_ids);

        object_class = G_OBJECT_CLASS (gupnp_feature_parent_class);
        object_class->finalize (object);
}

static void
gupnp_feature_get_property (GObject    *object,
                            guint       property_id,
                            GValue     *value,
                            GParamSpec *pspec)
{
        GUPnPFeature *feature;

        feature = GUPNP_FEATURE (object);

        switch (property_id) {
        case PROP_NAME:
                g_value_set_string (value, gupnp_feature_get_name (feature));
                break;
        case PROP_VERSION:
                g_value_set_string (value, gupnp_feature_get_version (feature));
                break;
        case PROP_OBJECT_IDS:
                g_value_set_string (value,
                                    gupnp_feature_get_object_ids (feature));
                break;
        case PROP_DOC:
                g_value_set_boxed (value, gupnp_feature_get_doc (feature));
                break;
        case PROP_NODE:
                g_value_set_pointer (value, gupnp_feature_get_node (feature));
                break;
        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
                break;
        }
}

static void
gupnp_feature_set_property (GObject      *object,
                            guint         property_id,
                            const GValue *value,
                            GParamSpec   *pspec)
{
        GUPnPFeaturePrivate *priv =
                gupnp_feature_get_instance_private (GUPNP_FEATURE (object));

        switch (property_id) {
        case PROP_NAME:
                priv->name = g_value_dup_string (value);
                break;
        case PROP_VERSION:
                priv->version = g_value_dup_string (value);
                break;
        case PROP_OBJECT_IDS:
                if (G_VALUE_HOLDS(value, G_TYPE_STRING))
                        priv->object_ids = g_value_dup_string (value);
                else
                        priv->object_ids = NULL;
                break;
        case PROP_DOC:
                priv->doc = g_value_dup_boxed (value);
                break;
        case PROP_NODE:
                priv->node = g_value_get_pointer (value);
                break;
        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
                break;
        }
}

static void
gupnp_feature_class_init (GUPnPFeatureClass *klass)
{
        GObjectClass *object_class;

        object_class = G_OBJECT_CLASS (klass);

        object_class->get_property = gupnp_feature_get_property;
        object_class->set_property = gupnp_feature_set_property;
        object_class->finalize = gupnp_feature_finalize;

        /**
         * GUPnPFeature:name:
         *
         * The name of this feature.
         **/
        g_object_class_install_property
                (object_class,
                 PROP_NAME,
                 g_param_spec_string ("name",
                                      "Name",
                                      "The name of this feature.",
                                      NULL,
                                      G_PARAM_READWRITE |
                                      G_PARAM_CONSTRUCT_ONLY |
                                      G_PARAM_STATIC_NAME |
                                      G_PARAM_STATIC_NICK |
                                      G_PARAM_STATIC_BLURB));

        /**
         * GUPnPFeature:version:
         *
         * The version of this feature.
         **/
        g_object_class_install_property
                (object_class,
                 PROP_VERSION,
                 g_param_spec_string ("version",
                                      "Version",
                                      "The version of this feature.",
                                      NULL,
                                      G_PARAM_READWRITE |
                                      G_PARAM_CONSTRUCT_ONLY |
                                      G_PARAM_STATIC_NAME |
                                      G_PARAM_STATIC_NICK |
                                      G_PARAM_STATIC_BLURB));

        /**
         * GUPnPFeature:object-ids:
         *
         * The object IDs related to this feature.
         **/
        g_object_class_install_property
                (object_class,
                 PROP_OBJECT_IDS,
                 g_param_spec_string ("object-ids",
                                      "Object IDs",
                                      "The object IDs of this feature.",
                                      NULL,
                                      G_PARAM_READWRITE |
                                      G_PARAM_CONSTRUCT_ONLY |
                                      G_PARAM_STATIC_NAME |
                                      G_PARAM_STATIC_NICK |
                                      G_PARAM_STATIC_BLURB));

        /**
         * GUPnPFeature:doc:
         *
         * The XML Doc of the feature list that contains this feature.
         **/
        g_object_class_install_property (
                object_class,
                PROP_DOC,
                g_param_spec_boxed ("doc",
                                    "XML document",
                                    "XML document for the feature list that "
                                    "contains this feature",
                                    av_xml_doc_get_type (),
                                    G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY |
                                            G_PARAM_STATIC_STRINGS));

        /**
         * GUPnPFeature:node:
         *
         * The XML node for this feature
         **/
        g_object_class_install_property (
                object_class,
                PROP_OBJECT_IDS,
                g_param_spec_pointer ("node",
                                      "XML node",
                                      "XML node of this feature",
                                      G_PARAM_READWRITE |
                                              G_PARAM_CONSTRUCT_ONLY |
                                              G_PARAM_STATIC_STRINGS));
}

/**
 * gupnp_feature_get_name:
 * @feature: #GUPnPFeature
 *
 * Get the name of the @feature.
 *
 * Return value: The name of the @feature.
 **/
const char *
gupnp_feature_get_name (GUPnPFeature *feature)
{
        GUPnPFeaturePrivate *priv =
                gupnp_feature_get_instance_private (GUPNP_FEATURE (feature));

        return priv->name;
}

/**
 * gupnp_feature_get_version:
 * @feature: #GUPnPFeature
 *
 * Get the version of the @feature.
 *
 * Return value: The version of the @feature.
 **/
const char *
gupnp_feature_get_version (GUPnPFeature *feature)
{
        GUPnPFeaturePrivate *priv =
                gupnp_feature_get_instance_private (GUPNP_FEATURE (feature));

        return priv->version;
}

GUPnPAVXMLDoc *
gupnp_feature_get_doc (GUPnPFeature *feature)
{
        GUPnPFeaturePrivate *priv =
                gupnp_feature_get_instance_private (GUPNP_FEATURE (feature));

        return av_xml_doc_ref (priv->doc);
}

xmlNode *
gupnp_feature_get_node (GUPnPFeature *feature)
{
        GUPnPFeaturePrivate *priv =
                gupnp_feature_get_instance_private (GUPNP_FEATURE (feature));

        return priv->node;
}

/**
 * gupnp_feature_get_object_ids:
 * @feature: #GUPnPFeature
 *
 * Get the object IDs related to the @feature.
 *
 * Return value: The object IDs related to the @feature.
 *
 * Deprecated. Use gupnp_feature_bookmarks_get_object_ids() or
 * gupnp_feature_epg_get_object_ids() instead
 **/
G_GNUC_DEPRECATED const char *
gupnp_feature_get_object_ids (GUPnPFeature *feature)
{
        GUPnPFeaturePrivate *priv =
                gupnp_feature_get_instance_private (GUPNP_FEATURE (feature));

        return priv->object_ids;
}
