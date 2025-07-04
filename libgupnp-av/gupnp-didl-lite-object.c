/*
 * Copyright (C) 2009 Nokia Corporation.
 * Copyright (C) 2012 Intel Corporation
 *
 * Authors: Zeeshan Ali (Khattak) <zeeshan.ali@nokia.com>
 *                                <zeeshanak@gnome.org>
 *          Krzesimir Nowak <krnowak@openismus.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */

/**
 * GUPnPDIDLLiteObject:
 *
 * DIDL-Lite Object
 *
 * #GUPnPDIDLLiteObject respresent a DIDL-Lite object element.
 */

#include <config.h>

#include <string.h>

#include "gupnp-didl-lite-object.h"
#include "gupnp-didl-lite-object-private.h"
#include "gupnp-didl-lite-resource-private.h"
#include "gupnp-didl-lite-descriptor-private.h"
#include "gupnp-didl-lite-container.h"
#include "gupnp-didl-lite-item.h"
#include "gupnp-didl-lite-contributor-private.h"
#include "xml-util.h"
#include "fragment-util.h"
#include "xsd-data.h"

struct _GUPnPDIDLLiteObjectPrivate {
        xmlNode       *xml_node;
        GUPnPAVXMLDoc *xml_doc;

        xmlNs *upnp_ns;
        xmlNs *dc_ns;
        xmlNs *dlna_ns;
        xmlNs *pv_ns;
};

G_DEFINE_ABSTRACT_TYPE_WITH_PRIVATE (GUPnPDIDLLiteObject,
                                     gupnp_didl_lite_object,
                                     G_TYPE_OBJECT)

static XSDData *didl_lite_xsd;

enum {
        PROP_0,
        PROP_XML_NODE,
        PROP_XML_DOC,
        PROP_UPNP_NAMESPACE,
        PROP_DC_NAMESPACE,
        PROP_DLNA_NAMESPACE,
        PROP_PV_NAMESPACE,
        PROP_ID,
        PROP_PARENT_ID,
        PROP_RESTRICTED,
        PROP_TITLE,
        PROP_UPNP_CLASS,
        PROP_CREATOR,
        PROP_ARTIST,
        PROP_AUTHOR,
        PROP_GENRE,
        PROP_WRITE_STATUS,
        PROP_ALBUM,
        PROP_ALBUM_ART,
        PROP_DESCRIPTION,
        PROP_DATE,
        PROP_TRACK_NUMBER,
        PROP_DLNA_MANAGED,
        PROP_UPDATE_ID
};

static int
is_non_transcoded_resource (GUPnPDIDLLiteResource *resource, gpointer data)
{
        GUPnPProtocolInfo *info;

        info = gupnp_didl_lite_resource_get_protocol_info (resource);
        if (G_UNLIKELY (info == NULL))
                return -1;

        return gupnp_protocol_info_get_dlna_conversion (info) &
               GUPNP_DLNA_CONVERSION_TRANSCODED;
}

static void
gupnp_didl_lite_object_init (GUPnPDIDLLiteObject *object)
{
}

static void
gupnp_didl_lite_object_set_property (GObject      *object,
                                     guint         property_id,
                                     const GValue *value,
                                     GParamSpec   *pspec)

{
        GUPnPDIDLLiteObject *didl_object;
        GUPnPDIDLLiteObjectPrivate *priv;

        didl_object = GUPNP_DIDL_LITE_OBJECT(object);
        priv = gupnp_didl_lite_object_get_instance_private(didl_object);

        switch (property_id) {
        case PROP_XML_NODE:
                priv->xml_node = g_value_get_pointer (value);
                break;
        case PROP_XML_DOC:
                priv->xml_doc = g_value_dup_boxed (value);
                break;
        case PROP_UPNP_NAMESPACE:
                priv->upnp_ns = g_value_get_pointer (value);
                break;
        case PROP_DC_NAMESPACE:
                priv->dc_ns = g_value_get_pointer (value);
                break;
        case PROP_DLNA_NAMESPACE:
                priv->dlna_ns = g_value_get_pointer (value);
                break;
        case PROP_PV_NAMESPACE:
                priv->pv_ns = g_value_get_pointer (value);
                break;
        case PROP_ID:
                gupnp_didl_lite_object_set_id (didl_object,
                                               g_value_get_string (value));
                break;
        case PROP_PARENT_ID:
                gupnp_didl_lite_object_set_parent_id
                                        (didl_object,
                                         g_value_get_string (value));
                break;
        case PROP_RESTRICTED:
                gupnp_didl_lite_object_set_restricted
                                        (didl_object,
                                         g_value_get_boolean (value));
                break;
        case PROP_TITLE:
                gupnp_didl_lite_object_set_title
                                        (didl_object,
                                         g_value_get_string (value));
                break;
        case PROP_UPNP_CLASS:
                gupnp_didl_lite_object_set_upnp_class
                                        (didl_object,
                                         g_value_get_string (value));
                break;
        case PROP_CREATOR:
                gupnp_didl_lite_object_set_creator
                                        (didl_object,
                                         g_value_get_string (value));
                break;
        case PROP_ARTIST:
                gupnp_didl_lite_object_set_artist
                                        (didl_object,
                                         g_value_get_string (value));
                break;
        case PROP_AUTHOR:
                gupnp_didl_lite_object_set_author
                                        (didl_object,
                                         g_value_get_string (value));
                break;
        case PROP_GENRE:
                gupnp_didl_lite_object_set_genre
                                        (didl_object,
                                         g_value_get_string (value));
                break;
        case PROP_WRITE_STATUS:
                gupnp_didl_lite_object_set_write_status
                                        (didl_object,
                                         g_value_get_string (value));
                break;
        case PROP_ALBUM:
                gupnp_didl_lite_object_set_album
                                        (didl_object,
                                         g_value_get_string (value));
                break;
        case PROP_ALBUM_ART:
                gupnp_didl_lite_object_set_album_art
                                        (didl_object,
                                         g_value_get_string (value));
                break;
        case PROP_DESCRIPTION:
                gupnp_didl_lite_object_set_description
                                        (didl_object,
                                         g_value_get_string (value));
                break;
        case PROP_DATE:
                gupnp_didl_lite_object_set_date
                                        (didl_object,
                                         g_value_get_string (value));
                break;
        case PROP_TRACK_NUMBER:
                gupnp_didl_lite_object_set_track_number
                                        (didl_object,
                                         g_value_get_int (value));
                break;
        case PROP_DLNA_MANAGED:
                gupnp_didl_lite_object_set_dlna_managed
                                        (didl_object,
                                         g_value_get_flags (value));
                break;
        case PROP_UPDATE_ID:
                gupnp_didl_lite_object_set_update_id
                                        (didl_object,
                                         g_value_get_uint (value));
                break;
        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
                break;
        }
}

static void
gupnp_didl_lite_object_get_property (GObject    *object,
                                     guint       property_id,
                                     GValue     *value,
                                     GParamSpec *pspec)
{
        GUPnPDIDLLiteObject *didl_object;

        didl_object = GUPNP_DIDL_LITE_OBJECT (object);

        switch (property_id) {
        case PROP_XML_NODE:
                g_value_set_pointer
                        (value,
                         gupnp_didl_lite_object_get_xml_node (didl_object));
                break;
        case PROP_UPNP_NAMESPACE:
                g_value_set_pointer
                        (value,
                         gupnp_didl_lite_object_get_upnp_namespace
                                (didl_object));
                break;
        case PROP_DC_NAMESPACE:
                g_value_set_pointer
                        (value,
                         gupnp_didl_lite_object_get_dc_namespace
                                (didl_object));
                break;
        case PROP_DLNA_NAMESPACE:
                g_value_set_pointer
                        (value,
                         gupnp_didl_lite_object_get_dlna_namespace
                                (didl_object));
                break;
        case PROP_PV_NAMESPACE:
                g_value_set_pointer
                        (value,
                         gupnp_didl_lite_object_get_pv_namespace
                                (didl_object));
                break;
        case PROP_ID:
                g_value_set_string
                        (value,
                         gupnp_didl_lite_object_get_id (didl_object));
                break;
        case PROP_PARENT_ID:
                g_value_set_string
                        (value,
                         gupnp_didl_lite_object_get_parent_id (didl_object));
                break;
        case PROP_RESTRICTED:
                g_value_set_boolean
                        (value,
                         gupnp_didl_lite_object_get_restricted (didl_object));
                break;
        case PROP_TITLE:
                g_value_set_string
                        (value,
                         gupnp_didl_lite_object_get_title (didl_object));
                break;
        case PROP_UPNP_CLASS:
                g_value_set_string
                        (value,
                         gupnp_didl_lite_object_get_upnp_class (didl_object));
                break;
        case PROP_CREATOR:
                g_value_set_string
                        (value,
                         gupnp_didl_lite_object_get_creator (didl_object));
                break;
        case PROP_ARTIST:
                g_value_set_string
                        (value,
                         gupnp_didl_lite_object_get_artist (didl_object));
                break;
        case PROP_AUTHOR:
                g_value_set_string
                        (value,
                         gupnp_didl_lite_object_get_author (didl_object));
                break;
        case PROP_GENRE:
                g_value_set_string
                        (value,
                         gupnp_didl_lite_object_get_genre (didl_object));
                break;
        case PROP_WRITE_STATUS:
                g_value_set_string
                        (value,
                         gupnp_didl_lite_object_get_write_status (didl_object));
                break;
        case PROP_ALBUM:
                g_value_set_string
                        (value,
                         gupnp_didl_lite_object_get_album (didl_object));
                break;
        case PROP_ALBUM_ART:
                g_value_set_string
                        (value,
                         gupnp_didl_lite_object_get_album_art (didl_object));
                break;
        case PROP_DESCRIPTION:
                g_value_set_string
                        (value,
                         gupnp_didl_lite_object_get_description (didl_object));
                break;
        case PROP_DATE:
                g_value_set_string
                        (value,
                         gupnp_didl_lite_object_get_date (didl_object));
                break;
        case PROP_TRACK_NUMBER:
                g_value_set_int
                        (value,
                         gupnp_didl_lite_object_get_track_number (didl_object));
                break;
        case PROP_DLNA_MANAGED:
                g_value_set_flags
                        (value,
                         gupnp_didl_lite_object_get_dlna_managed (didl_object));
                break;
        case PROP_UPDATE_ID:
                g_value_set_uint
                        (value,
                         gupnp_didl_lite_object_get_update_id (didl_object));
                break;
        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
                break;
        }
}

static void
gupnp_didl_lite_object_dispose (GObject *object)
{
        GObjectClass               *object_class;
        GUPnPDIDLLiteObjectPrivate *priv;
        GUPnPDIDLLiteObject *self = GUPNP_DIDL_LITE_OBJECT (object);

        priv = gupnp_didl_lite_object_get_instance_private (self);

        g_clear_pointer (&priv->xml_doc, av_xml_doc_unref);

        object_class = G_OBJECT_CLASS (gupnp_didl_lite_object_parent_class);
        object_class->dispose (object);
}

static void
gupnp_didl_lite_object_class_init (GUPnPDIDLLiteObjectClass *klass)
{
        GObjectClass *object_class;

        object_class = G_OBJECT_CLASS (klass);

        object_class->set_property = gupnp_didl_lite_object_set_property;
        object_class->get_property = gupnp_didl_lite_object_get_property;
        object_class->dispose = gupnp_didl_lite_object_dispose;

        /**
         * GUPnPDIDLLiteObject:xml-node:
         *
         * The pointer to object node in XML document.
         **/
        g_object_class_install_property
                (object_class,
                 PROP_XML_NODE,
                 g_param_spec_pointer ("xml-node",
                                       "XMLNode",
                                       "The pointer to object node in XML"
                                       " document.",
                                       G_PARAM_READWRITE |
                                       G_PARAM_CONSTRUCT_ONLY |
                                       G_PARAM_STATIC_NAME |
                                       G_PARAM_STATIC_NICK |
                                       G_PARAM_STATIC_BLURB));

        /**
         * GUPnPDIDLLiteObject:xml-doc:
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
                                      " containing this object.",
                                      av_xml_doc_get_type (),
                                      G_PARAM_WRITABLE |
                                      G_PARAM_CONSTRUCT_ONLY |
                                      G_PARAM_STATIC_NAME |
                                      G_PARAM_STATIC_NICK |
                                      G_PARAM_STATIC_BLURB));

        /**
         * GUPnPDIDLLiteObject:upnp-namespace:
         *
         * Pointer to the UPnP namespace registered with the XML document
         * containing this object.
         *
         **/
        g_object_class_install_property
                (object_class,
                 PROP_UPNP_NAMESPACE,
                 g_param_spec_pointer ("upnp-namespace",
                                       "XML namespace",
                                       "Pointer to the UPnP XML namespace "
                                       "registered with the XML document "
                                       "containing this object.",
                                       G_PARAM_READWRITE |
                                       G_PARAM_CONSTRUCT_ONLY |
                                       G_PARAM_STATIC_NAME |
                                       G_PARAM_STATIC_NICK |
                                       G_PARAM_STATIC_BLURB));

        /**
         * GUPnPDIDLLiteObject:dc-namespace:
         *
         * Pointer to the DublinCore namespace registered with the XML document
         * containing this object.
         *
         **/
        g_object_class_install_property
                (object_class,
                 PROP_DC_NAMESPACE,
                 g_param_spec_pointer ("dc-namespace",
                                       "XML namespace",
                                       "Pointer to the Dublin Core XML "
                                       "namespace registered with the XML "
                                       "document containing this object.",
                                       G_PARAM_READWRITE |
                                       G_PARAM_CONSTRUCT_ONLY |
                                       G_PARAM_STATIC_NAME |
                                       G_PARAM_STATIC_NICK |
                                       G_PARAM_STATIC_BLURB));

        /**
         * GUPnPDIDLLiteObject:dlna-namespace:
         *
         * Pointer to the DLNA metadata namespace registered with the XML
         * document containing this object.
         *
         **/
        g_object_class_install_property
                (object_class,
                 PROP_DLNA_NAMESPACE,
                 g_param_spec_pointer ("dlna-namespace",
                                       "XML namespace",
                                       "Pointer to the DLNA metadata namespace "
                                       "registered with the XML document "
                                       "containing this object.",
                                       G_PARAM_READWRITE |
                                       G_PARAM_CONSTRUCT_ONLY |
                                       G_PARAM_STATIC_NAME |
                                       G_PARAM_STATIC_NICK |
                                       G_PARAM_STATIC_BLURB));

        /**
         * GUPnPDIDLLiteObject:pv-namespace:
         *
         * Pointer to the PV metadata namespace registered with the XML
         * document containing this object.
         *
         **/
        g_object_class_install_property
                (object_class,
                 PROP_PV_NAMESPACE,
                 g_param_spec_pointer ("pv-namespace",
                                       "XML namespace",
                                       "Pointer to the PV metadata namespace "
                                       "registered with the XML document "
                                       "containing this object.",
                                       G_PARAM_READWRITE |
                                       G_PARAM_CONSTRUCT_ONLY |
                                       G_PARAM_STATIC_STRINGS));

        /**
         * GUPnPDIDLLiteObject:id:
         *
         * The ID of this object.
         **/
        g_object_class_install_property
                (object_class,
                 PROP_ID,
                 g_param_spec_string ("id",
                                      "ID",
                                      "The ID of this object.",
                                      NULL,
                                      G_PARAM_READWRITE |
                                      G_PARAM_STATIC_NAME |
                                      G_PARAM_STATIC_NICK |
                                      G_PARAM_STATIC_BLURB));

        /**
         * GUPnPDIDLLiteObject:parent-id:
         *
         * The ID of the parent container of this object.
         **/
        g_object_class_install_property
                (object_class,
                 PROP_PARENT_ID,
                 g_param_spec_string ("parent-id",
                                      "ParentID",
                                      "The ID of the parent container of"
                                      " this object.",
                                      NULL,
                                      G_PARAM_READWRITE |
                                      G_PARAM_STATIC_NAME |
                                      G_PARAM_STATIC_NICK |
                                      G_PARAM_STATIC_BLURB));

        /**
         * GUPnPDIDLLiteObject:restricted:
         *
         * Whether this object is restricted.
         **/
        g_object_class_install_property
                (object_class,
                 PROP_RESTRICTED,
                 g_param_spec_boolean ("restricted",
                                       "Restricted",
                                       "Whether this object is restricted.",
                                       FALSE,
                                       G_PARAM_READWRITE |
                                       G_PARAM_STATIC_NAME |
                                       G_PARAM_STATIC_NICK |
                                       G_PARAM_STATIC_BLURB));

        /**
         * GUPnPDIDLLiteObject:title:
         *
         * The title of this object.
         **/
        g_object_class_install_property
                (object_class,
                 PROP_TITLE,
                 g_param_spec_string ("title",
                                      "Title",
                                      "The title of this object.",
                                      NULL,
                                      G_PARAM_READWRITE |
                                      G_PARAM_STATIC_NAME |
                                      G_PARAM_STATIC_NICK |
                                      G_PARAM_STATIC_BLURB));

        /**
         * GUPnPDIDLLiteObject:upnp-class:
         *
         * The UPnP class of this object.
         **/
        g_object_class_install_property
                (object_class,
                 PROP_UPNP_CLASS,
                 g_param_spec_string ("upnp-class",
                                      "UPnPClassName",
                                      "The UPnP class of this object.",
                                      NULL,
                                      G_PARAM_READWRITE |
                                      G_PARAM_STATIC_NAME |
                                      G_PARAM_STATIC_NICK |
                                      G_PARAM_STATIC_BLURB));

        /**
         * GUPnPDIDLLiteObject:creator:
         *
         * The creator of this object.
         *
         **/
        g_object_class_install_property
                (object_class,
                 PROP_CREATOR,
                 g_param_spec_string ("creator",
                                      "Creator",
                                      "The creator of this object.",
                                      NULL,
                                      G_PARAM_READWRITE |
                                      G_PARAM_STATIC_NAME |
                                      G_PARAM_STATIC_NICK |
                                      G_PARAM_STATIC_BLURB));

        /**
         * GUPnPDIDLLiteObject:artist:
         *
         * The artist of this object.
         *
         * Deprecated: 0.5.3: Use #gupnp_didl_lite_object_get_artists and
         * #gupnp_didl_lite_object_add_artist instead since unlike this
         * property, they are capable of dealing with multiple artist nodes.
         **/
        g_object_class_install_property
                (object_class,
                 PROP_ARTIST,
                 g_param_spec_string ("artist",
                                      "Artist",
                                      "The artist of this object.",
                                      NULL,
                                      G_PARAM_READWRITE |
                                      G_PARAM_STATIC_NAME |
                                      G_PARAM_STATIC_NICK |
                                      G_PARAM_STATIC_BLURB));

        /**
         * GUPnPDIDLLiteObject:author:
         *
         * The author of this object.
         *
         * Deprecated: 0.5.3: Use #gupnp_didl_lite_object_get_authors and
         * #gupnp_didl_lite_object_add_author instead since unlike this
         * property, they are capable of dealing with multiple author nodes.
         **/
        g_object_class_install_property
                (object_class,
                 PROP_AUTHOR,
                 g_param_spec_string ("author",
                                      "Author",
                                      "The author of this object.",
                                      NULL,
                                      G_PARAM_READWRITE |
                                      G_PARAM_STATIC_NAME |
                                      G_PARAM_STATIC_NICK |
                                      G_PARAM_STATIC_BLURB));

        /**
         * GUPnPDIDLLiteObject:genre:
         *
         * The genre of this object.
         **/
        g_object_class_install_property
                (object_class,
                 PROP_GENRE,
                 g_param_spec_string ("genre",
                                      "Genre",
                                      "The genre of this object.",
                                      NULL,
                                      G_PARAM_READWRITE |
                                      G_PARAM_STATIC_NAME |
                                      G_PARAM_STATIC_NICK |
                                      G_PARAM_STATIC_BLURB));

        /**
         * GUPnPDIDLLiteObject:write-status:
         *
         * The write status of this object.
         **/
        g_object_class_install_property
                (object_class,
                 PROP_WRITE_STATUS,
                 g_param_spec_string ("write-status",
                                      "WriteStatus",
                                      "The write status of this object.",
                                      NULL,
                                      G_PARAM_READWRITE |
                                      G_PARAM_STATIC_NAME |
                                      G_PARAM_STATIC_NICK |
                                      G_PARAM_STATIC_BLURB));

        /**
         * GUPnPDIDLLiteObject:album:
         *
         * The album of this object.
         **/
        g_object_class_install_property
                (object_class,
                 PROP_ALBUM,
                 g_param_spec_string ("album",
                                      "Album",
                                      "The album of this object.",
                                      NULL,
                                      G_PARAM_READWRITE |
                                      G_PARAM_STATIC_NAME |
                                      G_PARAM_STATIC_NICK |
                                      G_PARAM_STATIC_BLURB));

        /**
         * GUPnPDIDLLiteObject:album-art:
         *
         * The URI to album art of this object.
         **/
        g_object_class_install_property
                (object_class,
                 PROP_ALBUM_ART,
                 g_param_spec_string ("album-art",
                                      "AlbumArt",
                                      "The URI to album art of this object.",
                                      NULL,
                                      G_PARAM_READWRITE |
                                      G_PARAM_STATIC_NAME |
                                      G_PARAM_STATIC_NICK |
                                      G_PARAM_STATIC_BLURB));

        /**
         * GUPnPDIDLLiteObject:description:
         *
         * The description of this object.
         **/
        g_object_class_install_property
                (object_class,
                 PROP_DESCRIPTION,
                 g_param_spec_string ("description",
                                      "Description",
                                      "The description of this object.",
                                      NULL,
                                      G_PARAM_READWRITE |
                                      G_PARAM_STATIC_NAME |
                                      G_PARAM_STATIC_NICK |
                                      G_PARAM_STATIC_BLURB));

        /**
         * GUPnPDIDLLiteObject:date:
         *
         * The date of this object.
         **/
        g_object_class_install_property
                (object_class,
                 PROP_DATE,
                 g_param_spec_string ("date",
                                      "Date",
                                      "The date of this object.",
                                      NULL,
                                      G_PARAM_READWRITE |
                                      G_PARAM_STATIC_NAME |
                                      G_PARAM_STATIC_NICK |
                                      G_PARAM_STATIC_BLURB));

        /**
         * GUPnPDIDLLiteObject:track-number:
         *
         * The original track number of this object.
         **/
        g_object_class_install_property
                (object_class,
                 PROP_TRACK_NUMBER,
                 g_param_spec_int ("track-number",
                                   "TrackNumber",
                                   "The original track number of this  object.",
                                   -1, G_MAXINT, -1,
                                   G_PARAM_READWRITE |
                                   G_PARAM_STATIC_NAME |
                                   G_PARAM_STATIC_NICK |
                                   G_PARAM_STATIC_BLURB));

        /**
         * GUPnPDIDLLiteObject:dlna-managed:
         *
         * The 'dlna:dlnaManaged' attribute.
         **/
        g_object_class_install_property
                (object_class,
                 PROP_DLNA_MANAGED,
                 g_param_spec_flags ("dlna-managed",
                                     "DLNAManaged",
                                     "The 'dlna:dlnaManaged' attribute",
                                     GUPNP_TYPE_OCM_FLAGS,
                                     GUPNP_OCM_FLAGS_NONE,
                                     G_PARAM_READWRITE |
                                     G_PARAM_STATIC_NAME |
                                     G_PARAM_STATIC_NICK |
                                     G_PARAM_STATIC_BLURB));

        /**
         * GUPnPDIDLLiteObject:update-id:
         *
         * Update ID of this object.
         **/
        g_object_class_install_property
                                (object_class,
                                 PROP_UPDATE_ID,
                                 g_param_spec_uint ("update-id",
                                                    "UpdateID",
                                                    "Update ID of this object.",
                                                    0,
                                                    G_MAXUINT,
                                                    0,
                                                    G_PARAM_READWRITE |
                                                    G_PARAM_STATIC_NAME |
                                                    G_PARAM_STATIC_NICK |
                                                    G_PARAM_STATIC_BLURB));

        if (didl_lite_xsd == NULL)
                didl_lite_xsd = fragment_util_get_didl_lite_xsd_data ();
}

static gboolean
is_resource_compatible (GUPnPDIDLLiteResource *resource,
                        char                 **protocols)
{
        gboolean ret = FALSE;
        char **it;

        for (it = protocols; *it != NULL && !ret; it++) {
                GUPnPProtocolInfo *info;
                GUPnPProtocolInfo *res_info;

                info = gupnp_protocol_info_new_from_string (*it, NULL);
                if (info == NULL)
                        continue;

                res_info = gupnp_didl_lite_resource_get_protocol_info
                                                        (resource);
                if (res_info == NULL)
                        continue;

                ret = gupnp_protocol_info_is_compatible (info, res_info);

                g_object_unref (info);
        }

        return ret;
}

static GList *
get_contributor_list_by_name (GUPnPDIDLLiteObject *object,
                              const char          *name)
{
        GList *contributors = NULL;
        GList *ret = NULL;
        GList *l;
        GUPnPDIDLLiteObjectPrivate *priv;
        priv = gupnp_didl_lite_object_get_instance_private (object);

        contributors = gupnp_didl_lite_object_get_properties (object, name);

        for (l = contributors; l; l = l->next) {
                GUPnPDIDLLiteContributor *contributor;
                xmlNode *contributor_node;

                contributor_node = (xmlNode *) l->data;
                if (!contributor_node->children)
                        continue;

                contributor = gupnp_didl_lite_contributor_new_from_xml
                                        (contributor_node,
                                         priv->xml_doc);

                ret = g_list_append (ret, contributor);
        }

        g_list_free (contributors);

        return ret;
}

static char *
get_contributors_xml_string_by_name (GUPnPDIDLLiteObject *object,
                                     const char          *name)
{
        GList     *contributors = NULL;
        char      *ret = NULL;
        GList     *l;
        xmlBuffer *buffer;
        GUPnPDIDLLiteObjectPrivate *priv;
        priv = gupnp_didl_lite_object_get_instance_private (object);

        contributors = gupnp_didl_lite_object_get_properties (object, name);
        if (contributors == NULL)
                return NULL;

        buffer = xmlBufferCreate ();

        for (l = contributors; l; l = l->next) {
                xmlNode *node;

                node = (xmlNode *) l->data;
                if (!node->children)
                        continue;

                xmlNodeDump (buffer,
                             priv->xml_doc->doc,
                             node,
                             0,
                             0);
        }

        ret = g_strndup ((char *) xmlBufferContent (buffer),
                         xmlBufferLength (buffer));
        xmlBufferFree (buffer);

        g_list_free (contributors);

        return ret;
}

static void
unset_contributors_by_name (GUPnPDIDLLiteObject *object, const char *name)
{
        GList *contributors = NULL;
        GList *l;

        contributors = gupnp_didl_lite_object_get_properties (object, name);
        if (contributors == NULL)
                return;

        for (l = contributors; l; l = l->next) {
                xmlNode *node;

                node = (xmlNode *) l->data;
                if (!node->children)
                        continue;

                xmlUnlinkNode (node);
                xmlFreeNode (node);
        }

        g_list_free (contributors);

        return;
}

/**
 * gupnp_didl_lite_object_new_from_xml:
 * @xml_node: The pointer to 'res' node in XML document
 * @xml_doc: The reference to XML document containing this object
 * @upnp_ns: The pointer to 'upnp' namespace in XML document
 * @dc_ns: The pointer to 'dc' namespace in XML document
 * @dlna_ns: The pointer to 'dlna' namespace in XML document
 * @pv_ns: The pointer to 'pv' namespace in XML document
 *
 * Creates a new #GUPnPDIDLLiteObject for the @xml_node.
 *
 * Return value: A new #GUPnPDIDLLiteObject object. Unref after usage.
 **/
GUPnPDIDLLiteObject *
gupnp_didl_lite_object_new_from_xml (xmlNode       *xml_node,
                                     GUPnPAVXMLDoc *xml_doc,
                                     xmlNs         *upnp_ns,
                                     xmlNs         *dc_ns,
                                     xmlNs         *dlna_ns,
                                     xmlNs         *pv_ns)
{
        g_return_val_if_fail (xml_node != NULL, NULL);
        g_return_val_if_fail (xml_node->name != NULL, NULL);

        if (g_ascii_strcasecmp ((char *) xml_node->name, "container") == 0)
                return g_object_new (GUPNP_TYPE_DIDL_LITE_CONTAINER,
                                     "xml-node", xml_node,
                                     "xml-doc", xml_doc,
                                     "upnp-namespace", upnp_ns,
                                     "dc-namespace", dc_ns,
                                     "dlna-namespace", dlna_ns,
                                     "pv-namespace", pv_ns,
                                     NULL);
        else if (g_ascii_strcasecmp ((char *) xml_node->name, "item") == 0)
                return g_object_new (GUPNP_TYPE_DIDL_LITE_ITEM,
                                     "xml-node", xml_node,
                                     "xml-doc", xml_doc,
                                     "upnp-namespace", upnp_ns,
                                     "dc-namespace", dc_ns,
                                     "dlna-namespace", dlna_ns,
                                     "pv-namespace", pv_ns,
                                     NULL);
        else
                return NULL;
}

/**
 * gupnp_didl_lite_object_get_gupnp_xml_doc:
 * @object: The #GUPnPDIDLLiteObject
 *
 * Get the pointer to the XML document containing this object.
 *
 * Returns: (transfer none): The pointer to the XML document containing this
 * object.
 **/
GUPnPAVXMLDoc *
gupnp_didl_lite_object_get_gupnp_xml_doc (GUPnPDIDLLiteObject *object)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object), NULL);
        GUPnPDIDLLiteObjectPrivate *priv;
        priv = gupnp_didl_lite_object_get_instance_private (object);

        return priv->xml_doc;
}

/**
 * gupnp_didl_lite_object_get_xml_node:
 * @object: The #GUPnPDIDLLiteObject
 *
 * Get the pointer to object node in XML document.
 *
 * Returns: (transfer none): The pointer to object node in XML document.
 **/
xmlNode *
gupnp_didl_lite_object_get_xml_node (GUPnPDIDLLiteObject *object)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object), NULL);
        GUPnPDIDLLiteObjectPrivate *priv;
        priv = gupnp_didl_lite_object_get_instance_private (object);

        return priv->xml_node;
}

/**
 * gupnp_didl_lite_object_get_upnp_namespace:
 * @object: The #GUPnPDIDLLiteObject
 *
 * Get the pointer to the UPnP namespace registered with the XML document.
 *
 * Returns: (transfer none): The pointer to UPnP namespace in XML document.
 **/
xmlNsPtr
gupnp_didl_lite_object_get_upnp_namespace (GUPnPDIDLLiteObject *object)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object), NULL);
        GUPnPDIDLLiteObjectPrivate *priv;
        priv = gupnp_didl_lite_object_get_instance_private (object);

        return av_xml_util_get_ns (priv->xml_doc->doc,
                                   GUPNP_XML_NAMESPACE_UPNP,
                                   &(priv->upnp_ns));
}

/**
 * gupnp_didl_lite_object_get_dc_namespace:
 * @object: The #GUPnPDIDLLiteObject
 *
 * Get the pointer to the DublinCore namespace registered with the XML document
 * containing this object.
 *
 * Returns: (transfer none): The pointer to DublinCore namespace in XML document.
 **/
xmlNsPtr
gupnp_didl_lite_object_get_dc_namespace (GUPnPDIDLLiteObject *object)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object), NULL);
        GUPnPDIDLLiteObjectPrivate *priv;
        priv = gupnp_didl_lite_object_get_instance_private (object);

        return av_xml_util_get_ns (priv->xml_doc->doc,
                                   GUPNP_XML_NAMESPACE_DC,
                                   &(priv->dc_ns));
}

/**
 * gupnp_didl_lite_object_get_upnp_class:
 * @object: The #GUPnPDIDLLiteObject
 *
 * Get the UPnP class of the @object.
 *
 * Return value: The class of @object, or %NULL.
 **/
const char *
gupnp_didl_lite_object_get_upnp_class (GUPnPDIDLLiteObject *object)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object), NULL);
        GUPnPDIDLLiteObjectPrivate *priv;
        priv = gupnp_didl_lite_object_get_instance_private (object);

        return av_xml_util_get_child_element_content (priv->xml_node,
                                                      "class");
}

/**
 * gupnp_didl_lite_object_get_dlna_namespace:
 * @object: The #GUPnPDIDLLiteObject
 *
 * Get the pointer to the DLNA metadata namespace registered with the XML
 * document containing this object.
 *
 * Returns: (transfer none): The pointer to DLNA namespace in XML document.
 **/
xmlNsPtr
gupnp_didl_lite_object_get_dlna_namespace (GUPnPDIDLLiteObject *object)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object), NULL);
        GUPnPDIDLLiteObjectPrivate *priv;
        priv = gupnp_didl_lite_object_get_instance_private (object);

        return av_xml_util_get_ns (priv->xml_doc->doc,
                                   GUPNP_XML_NAMESPACE_DLNA,
                                   &(priv->dlna_ns));
}

/**
 * gupnp_didl_lite_object_get_pv_namespace:
 * @object: The #GUPnPDIDLLiteObject
 *
 * Get the pointer to the PV metadata namespace registered with the XML
 * document containing this object.
 *
 * Returns: (transfer none): The pointer to PV namespace in XML document.
 **/
xmlNsPtr
gupnp_didl_lite_object_get_pv_namespace (GUPnPDIDLLiteObject *object)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object), NULL);
        GUPnPDIDLLiteObjectPrivate *priv;
        priv = gupnp_didl_lite_object_get_instance_private (object);

        return av_xml_util_get_ns (priv->xml_doc->doc,
                                   GUPNP_XML_NAMESPACE_PV,
                                   &(priv->pv_ns));
}


/**
 * gupnp_didl_lite_object_get_id:
 * @object: #GUPnPDIDLLiteObject
 *
 * Get the ID of the @object.
 *
 * Return value: The ID of the @object, or %NULL.
 **/
const char *
gupnp_didl_lite_object_get_id (GUPnPDIDLLiteObject *object)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object), NULL);
        GUPnPDIDLLiteObjectPrivate *priv;
        priv = gupnp_didl_lite_object_get_instance_private (object);

        return av_xml_util_get_attribute_content (priv->xml_node, "id");
}

/**
 * gupnp_didl_lite_object_get_parent_id:
 * @object: #GUPnPDIDLLiteObject
 *
 * Get the ID of the parent of the @object.
 *
 * Return value: The ID of parent of the @object, or %NULL.
 **/
const char *
gupnp_didl_lite_object_get_parent_id (GUPnPDIDLLiteObject *object)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object), NULL);
        GUPnPDIDLLiteObjectPrivate *priv;
        priv = gupnp_didl_lite_object_get_instance_private (object);

        return av_xml_util_get_attribute_content (priv->xml_node, "parentID");
}

/**
 * gupnp_didl_lite_object_get_properties:
 * @object: #GUPnPDIDLLiteObject
 * @name: name of the properties
 *
 * Use this function to retreive property nodes by name.
 *
 * Return value: (element-type xmlNode*) (transfer container): The list of
 * property nodes by the name @property_name belonging to @object, or %NULL.
 * #g_list_free the returned list after usage but do not modify the contents.
 **/
GList *
gupnp_didl_lite_object_get_properties (GUPnPDIDLLiteObject *object,
                                       const char          *name)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object), NULL);
        g_return_val_if_fail (name != NULL, NULL);
        GUPnPDIDLLiteObjectPrivate *priv;
        priv = gupnp_didl_lite_object_get_instance_private (object);

        return av_xml_util_get_child_elements_by_name (priv->xml_node, name);
}

/**
 * gupnp_didl_lite_object_is_restricted_set:
 * @object: #GUPnPDIDLLiteObject
 *
 * Whehter the restricted attribute exists on @object
 *
 * Return value: #TRUE if restricted exists, #FALSE otherwise.
 **/
gboolean
gupnp_didl_lite_object_is_restricted_set (GUPnPDIDLLiteObject *object)
{
        g_return_val_if_fail (object != NULL, FALSE);
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object), FALSE);
        GUPnPDIDLLiteObjectPrivate *priv;
        priv = gupnp_didl_lite_object_get_instance_private (object);

        return av_xml_util_get_attribute_content (priv->xml_node,
                                                  "restricted") != NULL;
}

/**
 * gupnp_didl_lite_object_get_restricted:
 * @object: #GUPnPDIDLLiteObject
 *
 * Whether the @object is restricted or not.
 *
 * Return value: #TRUE if @object is restricted.
 **/
gboolean
gupnp_didl_lite_object_get_restricted (GUPnPDIDLLiteObject *object)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object), FALSE);
        GUPnPDIDLLiteObjectPrivate *priv;
        priv = gupnp_didl_lite_object_get_instance_private (object);

        return av_xml_util_get_boolean_attribute (priv->xml_node, "restricted");
}

/**
 * gupnp_didl_lite_object_get_title:
 * @object: #GUPnPDIDLLiteObject
 *
 * Get the title of the @object.
 *
 * Return value: The title of the @object, or %NULL.
 **/
const char *
gupnp_didl_lite_object_get_title (GUPnPDIDLLiteObject *object)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object), NULL);
        GUPnPDIDLLiteObjectPrivate *priv;
        priv = gupnp_didl_lite_object_get_instance_private (object);

        return av_xml_util_get_child_element_content (priv->xml_node, "title");
}

/**
 * gupnp_didl_lite_object_get_creator:
 * @object: #GUPnPDIDLLiteObject
 *
 * Get the creator of the @object.
 *
 * Return value: The creator of the @object, or %NULL.
 **/
const char *
gupnp_didl_lite_object_get_creator (GUPnPDIDLLiteObject *object)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object), NULL);
        GUPnPDIDLLiteObjectPrivate *priv;
        priv = gupnp_didl_lite_object_get_instance_private (object);

        return av_xml_util_get_child_element_content (priv->xml_node,
                                                      "creator");
}

/**
 * gupnp_didl_lite_object_get_creators:
 * @object: #GUPnPDIDLLiteObject
 *
 * Get the creators of the @object.
 *
 * Returns: (element-type GUPnPDIDLLiteContributor*) (transfer full): The list
 * of creators belonging to @object, or %NULL.
 * #g_list_free the returned list after usage and unref each object in it.
 **/
GList *
gupnp_didl_lite_object_get_creators (GUPnPDIDLLiteObject *object)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object), NULL);

        return get_contributor_list_by_name (object, "creator");
}

/**
 * gupnp_didl_lite_object_get_artist:
 * @object: #GUPnPDIDLLiteObject
 *
 * Get the artist of the @object. If role is not %NULL, it is set to the role
 * of the artist if available.
 *
 * Return value: The artist of the @object, or %NULL.
 *
 * Deprecated: 0.5.3: Use #gupnp_didl_lite_object_get_artists instead.
 **/
const char *
gupnp_didl_lite_object_get_artist (GUPnPDIDLLiteObject *object)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object), NULL);
        GUPnPDIDLLiteObjectPrivate *priv;
        priv = gupnp_didl_lite_object_get_instance_private (object);

        return av_xml_util_get_child_element_content (priv->xml_node, "artist");
}

/**
 * gupnp_didl_lite_object_get_artists:
 * @object: #GUPnPDIDLLiteObject
 *
 * Get the artists of the @object.
 *
 * Returns: (element-type GUPnPDIDLLiteContributor*) (transfer full): The list
 * of artists belonging to @object, or %NULL.
 * #g_list_free the returned list after usage and unref each object in it.
 **/
GList *
gupnp_didl_lite_object_get_artists (GUPnPDIDLLiteObject *object)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object), NULL);

        return get_contributor_list_by_name (object, "artist");
}

/**
 * gupnp_didl_lite_object_get_author:
 * @object: #GUPnPDIDLLiteObject
 *
 * Get the author of the @object.
 *
 * Return value: The author of the @object, or %NULL.
 *
 * Deprecated: 0.5.3: Use #gupnp_didl_lite_object_get_authors instead.
 **/
const char *
gupnp_didl_lite_object_get_author (GUPnPDIDLLiteObject *object)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object), NULL);
        GUPnPDIDLLiteObjectPrivate *priv;
        priv = gupnp_didl_lite_object_get_instance_private (object);

        return av_xml_util_get_child_element_content (priv->xml_node, "author");
}

/**
 * gupnp_didl_lite_object_get_authors:
 * @object: #GUPnPDIDLLiteObject
 *
 * Get the authors of the @object.
 *
 * Returns: (element-type GUPnPDIDLLiteContributor*) (transfer full): The list
 * of authors belonging to @object, or %NULL.
 * #g_list_free the returned list after usage and unref each object in it.
 **/
GList *
gupnp_didl_lite_object_get_authors (GUPnPDIDLLiteObject *object)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object), NULL);

        return get_contributor_list_by_name (object, "author");
}

/**
 * gupnp_didl_lite_object_get_descriptors:
 * @object: #GUPnPDIDLLiteObject
 *
 * Get the descriptors of the @object.
 *
 * Returns: (element-type GUPnPDIDLLiteDescriptor*) (transfer full): The list of
 * descriptors belonging to @object, or %NULL.
 * #g_list_free the returned list after usage and unref each object in it.
 **/
GList *
gupnp_didl_lite_object_get_descriptors (GUPnPDIDLLiteObject *object)
{
        GList *descriptors = NULL;
        GList *ret = NULL;
        GList *l;
        GUPnPDIDLLiteObjectPrivate *priv;
        priv = gupnp_didl_lite_object_get_instance_private (object);

        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object), NULL);

        descriptors = gupnp_didl_lite_object_get_properties (object, "desc");

        for (l = descriptors; l; l = l->next) {
                GUPnPDIDLLiteDescriptor *descriptor;
                xmlNode *descriptor_node;

                descriptor_node = (xmlNode *) l->data;

                descriptor = gupnp_didl_lite_descriptor_new_from_xml (
                        descriptor_node,
                        priv->xml_doc);

                ret = g_list_append (ret, descriptor);
        }

        g_list_free (descriptors);

        return ret;
}

/**
 * gupnp_didl_lite_object_get_genre:
 * @object: #GUPnPDIDLLiteObject
 *
 * Get the genre of the @object.
 *
 * Return value: The genre of the @object, or %NULL.
 **/
const char *
gupnp_didl_lite_object_get_genre (GUPnPDIDLLiteObject *object)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object), NULL);
        GUPnPDIDLLiteObjectPrivate *priv;
        priv = gupnp_didl_lite_object_get_instance_private (object);

        return av_xml_util_get_child_element_content (priv->xml_node, "genre");
}

/**
 * gupnp_didl_lite_object_get_write_status:
 * @object: #GUPnPDIDLLiteObject
 *
 * Get the write status of the @object.
 *
 * Return value: The write status of the @object, or %NULL.
 **/
const char *
gupnp_didl_lite_object_get_write_status (GUPnPDIDLLiteObject *object)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object), NULL);
        GUPnPDIDLLiteObjectPrivate *priv;
        priv = gupnp_didl_lite_object_get_instance_private (object);

        return av_xml_util_get_child_element_content (priv->xml_node,
                                                      "writeStatus");
}

/**
 * gupnp_didl_lite_object_get_album:
 * @object: #GUPnPDIDLLiteObject
 *
 * Get the album of the @object.
 *
 * Return value: The album of the @object, or %NULL.
 **/
const char *
gupnp_didl_lite_object_get_album (GUPnPDIDLLiteObject *object)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object), NULL);
        GUPnPDIDLLiteObjectPrivate *priv;
        priv = gupnp_didl_lite_object_get_instance_private (object);

        return av_xml_util_get_child_element_content (priv->xml_node, "album");
}

/**
 * gupnp_didl_lite_object_get_album_art:
 * @object: #GUPnPDIDLLiteObject
 *
 * Get the URI to album art of the @object.
 *
 * Return value: The URI to album art of the @object, or %NULL.
 **/
const char *
gupnp_didl_lite_object_get_album_art (GUPnPDIDLLiteObject *object)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object), NULL);
        GUPnPDIDLLiteObjectPrivate *priv;
        priv = gupnp_didl_lite_object_get_instance_private (object);

        return av_xml_util_get_child_element_content (priv->xml_node,
                                                      "albumArtURI");
}

/**
 * gupnp_didl_lite_object_get_description:
 * @object: #GUPnPDIDLLiteObject
 *
 * Get the description of the @object.
 *
 * Return value: The description of the @object, or %NULL.
 **/
const char *
gupnp_didl_lite_object_get_description (GUPnPDIDLLiteObject *object)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object), NULL);
        GUPnPDIDLLiteObjectPrivate *priv;
        priv = gupnp_didl_lite_object_get_instance_private (object);

        return av_xml_util_get_child_element_content (priv->xml_node,
                                                      "description");
}

/**
 * gupnp_didl_lite_object_get_date:
 * @object: #GUPnPDIDLLiteObject
 *
 * Get the date of the @object.
 *
 * Return value: The date of the @object, or %NULL.
 **/
const char *
gupnp_didl_lite_object_get_date (GUPnPDIDLLiteObject *object)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object), NULL);
        GUPnPDIDLLiteObjectPrivate *priv;
        priv = gupnp_didl_lite_object_get_instance_private (object);

        return av_xml_util_get_child_element_content (priv->xml_node, "date");
}

/**
 * gupnp_didl_lite_object_get_track_number:
 * @object: #GUPnPDIDLLiteObject
 *
 * Get the original track number of the @object.
 *
 * Return value: The original track number of the @object, or -1.
 **/
int
gupnp_didl_lite_object_get_track_number (GUPnPDIDLLiteObject *object)
{
        const char *str;

        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object), -1);
        GUPnPDIDLLiteObjectPrivate *priv;
        priv = gupnp_didl_lite_object_get_instance_private (object);

        str = av_xml_util_get_child_element_content (priv->xml_node,
                                                     "originalTrackNumber");
        if (str == NULL)
                return -1;

        return atoi (str);
}

/**
 * gupnp_didl_lite_object_get_dlna_managed:
 * @object: #GUPnPDIDLLiteObject
 *
 * Get the 'dlna:dlnaManaged' attribute of the @object.
 *
 * Return value: The 'dlna:dlnaManaged' attribute of the @object.
 **/
GUPnPOCMFlags
gupnp_didl_lite_object_get_dlna_managed (GUPnPDIDLLiteObject *object)
{
        const char *str;
        GUPnPOCMFlags dlna_managed;

        g_return_val_if_fail (object != NULL, GUPNP_OCM_FLAGS_NONE);
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object),
                              GUPNP_OCM_FLAGS_NONE);
        GUPnPDIDLLiteObjectPrivate *priv;
        priv = gupnp_didl_lite_object_get_instance_private (object);

        str = av_xml_util_get_attribute_content (priv->xml_node,
                                                 "dlnaManaged");
        if (str == NULL)
                return GUPNP_OCM_FLAGS_NONE;

        sscanf (str, "%08x", &dlna_managed);

        return dlna_managed;
}

/**
 * gupnp_didl_lite_object_get_update_id:
 * @object: #GUPnPDIDLLiteObject
 *
 * Get the update ID of the @object.
 *
 * Return value: The update ID of the @object.
 **/
guint
gupnp_didl_lite_object_get_update_id (GUPnPDIDLLiteObject *object)
{
        g_return_val_if_fail (object != NULL, 0);
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object), 0);
        GUPnPDIDLLiteObjectPrivate *priv;
        priv = gupnp_didl_lite_object_get_instance_private (object);

        return av_xml_util_get_uint_child_element (priv->xml_node,
                                                   "objectUpdateID",
                                                   0);
}

/**
 * gupnp_didl_lite_object_update_id_is_set:
 * @object: #GUPnPDIDLLiteObject
 *
 * Get whether the update ID of the @object is set.
 *
 * Return value: %TRUE if update ID is set, otherwise %FALSE
 **/
gboolean
gupnp_didl_lite_object_update_id_is_set (GUPnPDIDLLiteObject *object)
{
        const char *content;

        g_return_val_if_fail (object != NULL, FALSE);
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object), FALSE);
        GUPnPDIDLLiteObjectPrivate *priv;
        priv = gupnp_didl_lite_object_get_instance_private (object);

        content = av_xml_util_get_child_element_content (priv->xml_node,
                                                         "objectUpdateID");
        return content != NULL;
}

/**
 * gupnp_didl_lite_object_get_resources:
 * @object: #GUPnPDIDLLiteObject
 *
 * Use this function to retreive resources from the @object.
 *
 * Return value: (element-type GUPnPDIDLLiteResource*) (transfer full): The list
 *               of resources belonging to  @object, or %NULL. #g_list_free the
 *               returned list after usage and unref each resource in it.
 **/
GList *
gupnp_didl_lite_object_get_resources (GUPnPDIDLLiteObject *object)
{
        GList *resources = NULL;
        GList *res = NULL;
        GList *ret = NULL;

        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object), NULL);
        GUPnPDIDLLiteObjectPrivate *priv;
        priv = gupnp_didl_lite_object_get_instance_private (object);

        resources = gupnp_didl_lite_object_get_properties (object, "res");

        for (res = resources; res; res = res->next) {
                GUPnPDIDLLiteResource *resource;
                xmlNode *res_node;

                res_node = (xmlNode *) res->data;

                /* Create a resource struct out of DIDLLite XML */
                resource = gupnp_didl_lite_resource_new_from_xml (res_node,
                                                                  priv->xml_doc,
                                                                  priv->dlna_ns,
                                                                  priv->pv_ns);

                ret = g_list_append (ret, resource);
        }

        g_list_free (resources);

        return ret;
}

/**
 * gupnp_didl_lite_object_get_compat_resource:
 * @object: #GUPnPDIDLLiteObject
 * @sink_protocol_info: The SinkProtocolInfo string from MediaRenderer
 * @lenient: Enable lenient mode
 *
 * Use this function to get a resource from the @object that is compatible with
 * any of the protocols specified in the @sink_protocol_info. The value of
 * @sink_protocol_info will typically be acquired from 'Sink' argument of
 * 'GetProtocolInfo' action or 'SinkProtocolInfo' state-variable of a
 * ConnectionManager service.
 *
 * If @lenient is #TRUE, the first resource in the list is returned instead of
 * %NULL if none of resources and protocols are found to be compatible.
 *
 * Returns: (transfer full): The resource belonging to @object that is comaptible with
 * any of the protocols specified in @sink_protocol_info, or %NULL. Unref after
 * usage.
 **/
GUPnPDIDLLiteResource *
gupnp_didl_lite_object_get_compat_resource
                                (GUPnPDIDLLiteObject *object,
                                 const char          *sink_protocol_info,
                                 gboolean             lenient)
{
        GUPnPDIDLLiteResource *resource = NULL;
        GList  *resources = NULL;
        GList  *compat_resources = NULL;
        GList  *res;
        char **protocols = NULL;

        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object), NULL);
        g_return_val_if_fail (sink_protocol_info != NULL, NULL);

        resources = gupnp_didl_lite_object_get_resources (object);
        if (resources == NULL)
                return NULL;

        protocols = g_strsplit (sink_protocol_info, ",", -1);
        for (res = resources;
             res != NULL;
             res = res->next) {
                resource = (GUPnPDIDLLiteResource *) res->data;

                if (is_resource_compatible (resource, protocols))
                        compat_resources = g_list_append (compat_resources,
                                                          resource);
        }
        g_strfreev (protocols);
        protocols = NULL;

        resource = NULL;

        if (compat_resources != NULL) {
                /* Try to find non-transcoded resource */
                res = g_list_find_custom (compat_resources,
                                          NULL,
                                          (GCompareFunc)
                                          is_non_transcoded_resource);

                if (res != NULL)
                        resource = (GUPnPDIDLLiteResource *) res->data;
                else
                        /* Just use the first compatible resource */
                        resource = (GUPnPDIDLLiteResource *)
                                   compat_resources->data;

        } else if (lenient)
                /* Just use the first resource */
                resource = (GUPnPDIDLLiteResource *) resources->data;

        /* Unref all resources except for the one we just took */
        for (res = resources; res; res = res->next)
                if (res->data != resource)
                        g_object_unref (res->data);
        g_list_free (resources);
        g_list_free (compat_resources);

        return resource;
}

/**
 * gupnp_didl_lite_object_set_upnp_class:
 * @object: The #GUPnPDIDLLiteObject
 * @upnp_class: The UPnP class as string.
 *
 * Set the UPnP class of the @object to @upnp_class.
 **/
void
gupnp_didl_lite_object_set_upnp_class (GUPnPDIDLLiteObject *object,
                                       const char          *upnp_class)
{
        g_return_if_fail (object != NULL);
        g_return_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object));
        GUPnPDIDLLiteObjectPrivate *priv;
        priv = gupnp_didl_lite_object_get_instance_private (object);

        av_xml_util_set_child (priv->xml_node,
                               GUPNP_XML_NAMESPACE_UPNP,
                               &(priv->upnp_ns),
                               priv->xml_doc->doc,
                               "class",
                               upnp_class);

        g_object_notify (G_OBJECT (object), "upnp-class");
}

/**
 * gupnp_didl_lite_object_set_id:
 * @object: #GUPnPDIDLLiteObject
 * @id: The ID
 *
 * Set the ID of the @object to @id.
 **/
void
gupnp_didl_lite_object_set_id (GUPnPDIDLLiteObject *object,
                               const char          *id)
{
        g_return_if_fail (object != NULL);
        g_return_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object));
        GUPnPDIDLLiteObjectPrivate *priv;
        priv = gupnp_didl_lite_object_get_instance_private (object);

        xmlSetProp (priv->xml_node,
                    (unsigned char *) "id",
                    (unsigned char *) id);

        g_object_notify (G_OBJECT (object), "id");
}

/**
 * gupnp_didl_lite_object_set_parent_id:
 * @object: #GUPnPDIDLLiteObject
 * @parent_id: The parent ID
 *
 * Set the ID of the parent of the @object to @parent_id.
 **/
void
gupnp_didl_lite_object_set_parent_id (GUPnPDIDLLiteObject *object,
                                      const char          *parent_id)
{
        g_return_if_fail (object != NULL);
        g_return_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object));
        GUPnPDIDLLiteObjectPrivate *priv;
        priv = gupnp_didl_lite_object_get_instance_private (object);

        xmlSetProp (priv->xml_node,
                    (unsigned char *) "parentID",
                    (unsigned char *) parent_id);

        g_object_notify (G_OBJECT (object), "parent-id");
}

/**
 * gupnp_didl_lite_object_set_restricted:
 * @object: #GUPnPDIDLLiteObject
 * @restricted: The restricted status
 *
 * Set the restricted status of @object to @restricted.
 **/
void
gupnp_didl_lite_object_set_restricted (GUPnPDIDLLiteObject *object,
                                       gboolean             restricted)
{
        const char *str;

        g_return_if_fail (object != NULL);
        g_return_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object));
        GUPnPDIDLLiteObjectPrivate *priv;
        priv = gupnp_didl_lite_object_get_instance_private (object);

        if (restricted)
                str = "1";
        else
                str = "0";
        xmlSetProp (priv->xml_node,
                    (unsigned char *) "restricted",
                    (unsigned char *) str);

        g_object_notify (G_OBJECT (object), "restricted");
}

/**
 * gupnp_didl_lite_object_set_title:
 * @object: #GUPnPDIDLLiteObject
 * @title: The title
 *
 * Set the title of the @object to @title.
 **/
void
gupnp_didl_lite_object_set_title (GUPnPDIDLLiteObject *object,
                                  const char          *title)
{
        g_return_if_fail (object != NULL);
        g_return_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object));
        GUPnPDIDLLiteObjectPrivate *priv;
        priv = gupnp_didl_lite_object_get_instance_private (object);

        av_xml_util_set_child (priv->xml_node,
                               GUPNP_XML_NAMESPACE_DC,
                               &(priv->dc_ns),
                               priv->xml_doc->doc,
                               "title",
                               title);

        g_object_notify (G_OBJECT (object), "title");
}

/**
 * gupnp_didl_lite_object_set_creator:
 * @object: #GUPnPDIDLLiteObject
 * @creator: The creator
 *
 * Set the creator of the @object to @creator.
 **/
void
gupnp_didl_lite_object_set_creator (GUPnPDIDLLiteObject *object,
                                    const char          *creator)
{
        g_return_if_fail (object != NULL);
        g_return_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object));
        GUPnPDIDLLiteObjectPrivate *priv;
        priv = gupnp_didl_lite_object_get_instance_private (object);

        av_xml_util_set_child (priv->xml_node,
                               GUPNP_XML_NAMESPACE_DC,
                               &(priv->dc_ns),
                               priv->xml_doc->doc,
                               "creator",
                               creator);

        g_object_notify (G_OBJECT (object), "creator");
}

/**
 * gupnp_didl_lite_object_add_creator:
 * @object: The #GUPnPDIDLLiteObject
 *
 * Add a new creator node to the @object and return the associated
 * #GUPnPDIDLLiteContributor object.
 *
 * Returns: (transfer full): A new #GUPnPDIDLLiteContributor object. Unref after usage.
 **/
GUPnPDIDLLiteContributor *
gupnp_didl_lite_object_add_creator (GUPnPDIDLLiteObject *object)
{
        xmlNode *res_node;

        g_return_val_if_fail (object != NULL, NULL);
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object), NULL);
        GUPnPDIDLLiteObjectPrivate *priv;
        priv = gupnp_didl_lite_object_get_instance_private (object);

        res_node = xmlNewChild (priv->xml_node,
                                priv->dc_ns,
                                (unsigned char *) "creator",
                                NULL);

        return gupnp_didl_lite_contributor_new_from_xml (res_node,
                                                         priv->xml_doc);
}


/**
 * gupnp_didl_lite_object_set_artist:
 * @object: The #GUPnPDIDLLiteObject
 * @artist: The Artist
 *
 * Set the Artist of the @object to @artist.
 *
 * Deprecated: 0.5.3: Use #gupnp_didl_lite_object_add_artist instead.
 **/
void
gupnp_didl_lite_object_set_artist (GUPnPDIDLLiteObject *object,
                                   const char          *artist)
{
        g_return_if_fail (object != NULL);
        g_return_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object));
        GUPnPDIDLLiteObjectPrivate *priv;
        priv = gupnp_didl_lite_object_get_instance_private (object);

        av_xml_util_set_child (priv->xml_node,
                               GUPNP_XML_NAMESPACE_UPNP,
                               &(priv->upnp_ns),
                               priv->xml_doc->doc,
                               "artist",
                               artist);

        g_object_notify (G_OBJECT (object), "artist");
}

/**
 * gupnp_didl_lite_object_add_artist:
 * @object: The #GUPnPDIDLLiteObject
 *
 * Add a new Artist node to the @object and return the associated
 * #GUPnPDIDLLiteContributor object.
 *
 * Returns: (transfer full): A new #GUPnPDIDLLiteContributor object. Unref after usage.
 **/
GUPnPDIDLLiteContributor *
gupnp_didl_lite_object_add_artist (GUPnPDIDLLiteObject *object)
{
        xmlNode *res_node;

        g_return_val_if_fail (object != NULL, NULL);
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object), NULL);
        GUPnPDIDLLiteObjectPrivate *priv;
        priv = gupnp_didl_lite_object_get_instance_private (object);

        res_node = xmlNewChild (priv->xml_node,
                                priv->upnp_ns,
                                (unsigned char *) "artist",
                                NULL);

        return gupnp_didl_lite_contributor_new_from_xml (res_node,
                                                         priv->xml_doc);
}

/**
 * gupnp_didl_lite_object_set_author:
 * @object: The #GUPnPDIDLLiteObject
 * @author: The Author
 *
 * Set the Author of the @object to @author.
 *
 * Deprecated: 0.5.3: Use #gupnp_didl_lite_object_add_author instead.
 **/
void
gupnp_didl_lite_object_set_author (GUPnPDIDLLiteObject *object,
                                   const char          *author)
{
        g_return_if_fail (object != NULL);
        g_return_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object));
        GUPnPDIDLLiteObjectPrivate *priv;
        priv = gupnp_didl_lite_object_get_instance_private (object);

        av_xml_util_set_child (priv->xml_node,
                               GUPNP_XML_NAMESPACE_DC,
                               &(priv->upnp_ns),
                               priv->xml_doc->doc,
                               "author",
                               author);

        g_object_notify (G_OBJECT (object), "author");
}

/**
 * gupnp_didl_lite_object_add_author:
 * @object: The #GUPnPDIDLLiteObject
 *
 * Add a new author node to the @object and return the associated
 * #GUPnPDIDLLiteContributor object.
 *
 * Returns: (transfer full): A new #GUPnPDIDLLiteContributor object. Unref after usage.
 **/
GUPnPDIDLLiteContributor *
gupnp_didl_lite_object_add_author (GUPnPDIDLLiteObject *object)
{
        xmlNode *res_node;

        g_return_val_if_fail (object != NULL, NULL);
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object), NULL);
        GUPnPDIDLLiteObjectPrivate *priv;
        priv = gupnp_didl_lite_object_get_instance_private (object);

        res_node = xmlNewChild (priv->xml_node,
                                priv->upnp_ns,
                                (unsigned char *) "author",
                                NULL);

        return gupnp_didl_lite_contributor_new_from_xml (res_node,
                                                         priv->xml_doc);
}

/**
 * gupnp_didl_lite_object_set_genre:
 * @object: The #GUPnPDIDLLiteObject
 * @genre: The Genre
 *
 * Set the genre of the @object to @genre.
 **/
void
gupnp_didl_lite_object_set_genre (GUPnPDIDLLiteObject *object,
                                  const char          *genre)
{
        g_return_if_fail (object != NULL);
        g_return_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object));
        GUPnPDIDLLiteObjectPrivate *priv;
        priv = gupnp_didl_lite_object_get_instance_private (object);

        av_xml_util_set_child (priv->xml_node,
                               GUPNP_XML_NAMESPACE_UPNP,
                               &(priv->upnp_ns),
                               priv->xml_doc->doc,
                               "genre",
                               genre);

        g_object_notify (G_OBJECT (object), "genre");
}

/**
 * gupnp_didl_lite_object_set_write_status:
 * @object: #GUPnPDIDLLiteObject
 * @write_status: The write status string
 *
 * Set the write status of the @object to @write_status.
 **/
void
gupnp_didl_lite_object_set_write_status (GUPnPDIDLLiteObject *object,
                                         const char          *write_status)
{
        g_return_if_fail (object != NULL);
        g_return_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object));
        GUPnPDIDLLiteObjectPrivate *priv;
        priv = gupnp_didl_lite_object_get_instance_private (object);

        av_xml_util_set_child (priv->xml_node,
                               GUPNP_XML_NAMESPACE_DC,
                               &(priv->dc_ns),
                               priv->xml_doc->doc,
                               "writeStatus",
                               write_status);

        g_object_notify (G_OBJECT (object), "write-status");
}

/**
 * gupnp_didl_lite_object_set_album:
 * @object: #GUPnPDIDLLiteObject
 * @album: The album string
 *
 * Set the album of the @object to @album.
 **/
void
gupnp_didl_lite_object_set_album (GUPnPDIDLLiteObject *object,
                                  const char          *album)
{
        g_return_if_fail (object != NULL);
        g_return_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object));
        GUPnPDIDLLiteObjectPrivate *priv;
        priv = gupnp_didl_lite_object_get_instance_private (object);

        av_xml_util_set_child (priv->xml_node,
                               GUPNP_XML_NAMESPACE_UPNP,
                               &(priv->upnp_ns),
                               priv->xml_doc->doc,
                               "album",
                               album);

        g_object_notify (G_OBJECT (object), "album");
}

/**
 * gupnp_didl_lite_object_set_album_art:
 * @object: #GUPnPDIDLLiteObject
 * @album_art: The URI of album art
 *
 * Set the URI to album art of the @object to @album_art.
 **/
void
gupnp_didl_lite_object_set_album_art (GUPnPDIDLLiteObject *object,
                                      const char          *album_art)
{
        xmlNode *node;

        g_return_if_fail (object != NULL);
        g_return_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object));
        GUPnPDIDLLiteObjectPrivate *priv;
        priv = gupnp_didl_lite_object_get_instance_private (object);

        node = av_xml_util_set_child (priv->xml_node,
                                      GUPNP_XML_NAMESPACE_UPNP,
                                      &(priv->upnp_ns),
                                      priv->xml_doc->doc,
                                      "albumArtURI",
                                      album_art);

        av_xml_util_get_ns (priv->xml_doc->doc,
                            GUPNP_XML_NAMESPACE_DLNA,
                            &(priv->dlna_ns));

        xmlSetNsProp (node,
                      priv->dlna_ns,
                      (const unsigned char *) "profileID",
                      (const unsigned char *) "JPEG_TN");

        g_object_notify (G_OBJECT (object), "album-art");
}

/**
 * gupnp_didl_lite_object_set_description:
 * @object: #GUPnPDIDLLiteObject
 * @description: The description string
 *
 * Set the description of the @object to @description.
 **/
void
gupnp_didl_lite_object_set_description (GUPnPDIDLLiteObject *object,
                                        const char          *description)
{
        g_return_if_fail (object != NULL);
        g_return_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object));
        GUPnPDIDLLiteObjectPrivate *priv;
        priv = gupnp_didl_lite_object_get_instance_private (object);

        av_xml_util_set_child (priv->xml_node,
                               GUPNP_XML_NAMESPACE_DC,
                               &(priv->dc_ns),
                               priv->xml_doc->doc,
                               "description",
                               description);

        g_object_notify (G_OBJECT (object), "description");
}

/**
 * gupnp_didl_lite_object_set_date:
 * @object: #GUPnPDIDLLiteObject
 * @date: The date string
 *
 * Set the date of the @object to @date.
 **/
void
gupnp_didl_lite_object_set_date (GUPnPDIDLLiteObject *object,
                                 const char          *date)
{
        g_return_if_fail (object != NULL);
        g_return_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object));
        GUPnPDIDLLiteObjectPrivate *priv;
        priv = gupnp_didl_lite_object_get_instance_private (object);

        av_xml_util_set_child (priv->xml_node,
                               GUPNP_XML_NAMESPACE_DC,
                               &(priv->dc_ns),
                               priv->xml_doc->doc,
                               "date",
                               date);

        g_object_notify (G_OBJECT (object), "date");
}

/**
 * gupnp_didl_lite_object_set_track_number:
 * @object: #GUPnPDIDLLiteObject
 * @track_number: The original track number
 *
 * Set the original track number of the @object to @track_number.
 **/
void
gupnp_didl_lite_object_set_track_number (GUPnPDIDLLiteObject *object,
                                         int                  track_number)
{
        char *str;

        g_return_if_fail (object != NULL);
        g_return_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object));
        GUPnPDIDLLiteObjectPrivate *priv;
        priv = gupnp_didl_lite_object_get_instance_private (object);

        str = g_strdup_printf ("%d", track_number);
        av_xml_util_set_child (priv->xml_node,
                               GUPNP_XML_NAMESPACE_UPNP,
                               &(priv->upnp_ns),
                               priv->xml_doc->doc,
                               "originalTrackNumber",
                               str);
        g_free (str);

        g_object_notify (G_OBJECT (object), "track-number");
}

/**
 * gupnp_didl_lite_object_set_dlna_managed:
 * @object: #GUPnPDIDLLiteObject
 * @dlna_managed: The #GUPnPOCMFlags.
 *
 * Set the 'dlna:dlnaManaged' attribute of the @object to @dlna_managed.
 **/
void
gupnp_didl_lite_object_set_dlna_managed (GUPnPDIDLLiteObject *object,
                                         GUPnPOCMFlags        dlna_managed)
{
        char *str;

        g_return_if_fail (object != NULL);
        g_return_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object));
        GUPnPDIDLLiteObjectPrivate *priv;
        priv = gupnp_didl_lite_object_get_instance_private (object);

        av_xml_util_get_ns (priv->xml_doc->doc,
                            GUPNP_XML_NAMESPACE_DLNA,
                            &(priv->dlna_ns));

        str = g_strdup_printf ("%08x", dlna_managed);
        xmlSetNsProp (priv->xml_node,
                      priv->dlna_ns,
                      (xmlChar *) "dlnaManaged",
                      (xmlChar *) str);
        g_free (str);

        g_object_notify (G_OBJECT (object), "dlna-managed");
}

/**
 * gupnp_didl_lite_object_set_update_id:
 * @object: #GUPnPDIDLLiteObject
 * @update_id: Update ID
 *
 * Set the update ID of the @object.
 **/
void
gupnp_didl_lite_object_set_update_id (GUPnPDIDLLiteObject *object,
                                      guint                update_id)
{
        char *str;

        g_return_if_fail (object != NULL);
        g_return_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object));
        GUPnPDIDLLiteObjectPrivate *priv;
        priv = gupnp_didl_lite_object_get_instance_private (object);

        str = g_strdup_printf ("%u", update_id);
        av_xml_util_set_child (priv->xml_node,
                               GUPNP_XML_NAMESPACE_UPNP,
                               &(priv->upnp_ns),
                               priv->xml_doc->doc,
                               "objectUpdateID",
                               str);
        g_free (str);

        g_object_notify (G_OBJECT (object), "update-id");
}

/**
 * gupnp_didl_lite_object_unset_update_id:
 * @object: #GUPnPDIDLLiteObject
 *
 * Unset the update ID property of the @object.
 **/
void
gupnp_didl_lite_object_unset_update_id (GUPnPDIDLLiteObject *object)
{
        g_return_if_fail (object != NULL);
        g_return_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object));
        GUPnPDIDLLiteObjectPrivate *priv;
        priv = gupnp_didl_lite_object_get_instance_private (object);

        av_xml_util_unset_child (priv->xml_node,
                                 "objectUpdateID");

        g_object_notify (G_OBJECT (object), "update-id");
}

/**
 * gupnp_didl_lite_object_add_resource:
 * @object: A #GUPnPDIDLLiteObject
 *
 * Creates a new resource, attaches it to @object and returns it.
 *
 * Returns: (transfer full): A new #GUPnPDIDLLiteResource object. Unref after usage.
 **/
GUPnPDIDLLiteResource *
gupnp_didl_lite_object_add_resource (GUPnPDIDLLiteObject *object)
{
        xmlNode *res_node;

        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object), NULL);
        GUPnPDIDLLiteObjectPrivate *priv;
        priv = gupnp_didl_lite_object_get_instance_private (object);

        res_node = xmlNewChild (priv->xml_node,
                                NULL,
                                (unsigned char *) "res",
                                NULL);

        return gupnp_didl_lite_resource_new_from_xml (res_node,
                                                      priv->xml_doc,
                                                      priv->dlna_ns,
                                                      priv->pv_ns);
}

/**
 * gupnp_didl_lite_object_add_descriptor:
 * @object: A #GUPnPDIDLLiteObject
 *
 * Creates a new descriptor, attaches it to @object and returns it.
 *
 * Returns: (transfer full): A new #GUPnPDIDLLiteDescriptor object. Unref after usage.
 **/
GUPnPDIDLLiteDescriptor *
gupnp_didl_lite_object_add_descriptor (GUPnPDIDLLiteObject *object)
{
        xmlNode *desc_node;

        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object), NULL);
        GUPnPDIDLLiteObjectPrivate *priv;
        priv = gupnp_didl_lite_object_get_instance_private (object);

        desc_node =
                xmlNewChild (priv->xml_node, NULL, (xmlChar *) "desc", NULL);

        return gupnp_didl_lite_descriptor_new_from_xml (desc_node,
                                                        priv->xml_doc);
}

/**
 * gupnp_didl_lite_object_get_title_xml_string:
 * @object: A #GUPnPDIDLLiteObject
 *
 * Creates a string representation of the DIDL-Lite XML fragment related to the
 * object title.
 *
 * Return value: A DIDL-Lite XML fragment string, or %NULL. #g_free after usage.
 **/
char *
gupnp_didl_lite_object_get_title_xml_string (GUPnPDIDLLiteObject *object)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object), NULL);
        GUPnPDIDLLiteObjectPrivate *priv;
        priv = gupnp_didl_lite_object_get_instance_private (object);

        return av_xml_util_get_child_string (priv->xml_node,
                                             priv->xml_doc->doc,
                                             "title");
}

/**
 * gupnp_didl_lite_object_get_date_xml_string:
 * @object: A #GUPnPDIDLLiteObject
 *
 * Creates a string representation of the DIDL-Lite XML fragment related to the
 * object date.
 *
 * Return value: A DIDL-Lite XML fragment string, or %NULL. #g_free after usage.
 **/
char *
gupnp_didl_lite_object_get_date_xml_string (GUPnPDIDLLiteObject *object)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object), NULL);
        GUPnPDIDLLiteObjectPrivate *priv;
        priv = gupnp_didl_lite_object_get_instance_private (object);

        return av_xml_util_get_child_string (priv->xml_node,
                                             priv->xml_doc->doc,
                                             "date");
}

/**
 * gupnp_didl_lite_object_get_upnp_class_xml_string:
 * @object: A #GUPnPDIDLLiteObject
 *
 * Creates a string representation of the DIDL-Lite XML fragment related to the
 * object UPnP class.
 *
 * Return value: A DIDL-Lite XML fragment string, or %NULL. #g_free after usage.
 **/
char *
gupnp_didl_lite_object_get_upnp_class_xml_string (GUPnPDIDLLiteObject *object)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object), NULL);
        GUPnPDIDLLiteObjectPrivate *priv;
        priv = gupnp_didl_lite_object_get_instance_private (object);

        return av_xml_util_get_child_string (priv->xml_node,
                                             priv->xml_doc->doc,
                                             "class");
}

/**
 * gupnp_didl_lite_object_get_album_xml_string:
 * @object: A #GUPnPDIDLLiteObject
 *
 * Creates a string representation of the DIDL-Lite XML fragment related to the
 * object album.
 *
 * Return value: A DIDL-Lite XML fragment string, or %NULL. #g_free after usage.
 **/
char *
gupnp_didl_lite_object_get_album_xml_string (GUPnPDIDLLiteObject *object)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object), NULL);
        GUPnPDIDLLiteObjectPrivate *priv;
        priv = gupnp_didl_lite_object_get_instance_private (object);

        return av_xml_util_get_child_string (priv->xml_node,
                                             priv->xml_doc->doc,
                                             "album");
}

/**
 * gupnp_didl_lite_object_get_track_number_xml_string:
 * @object: A #GUPnPDIDLLiteObject
 *
 * Creates a string representation of the DIDL-Lite XML fragment related to the
 * object track number.
 *
 * Return value: A DIDL-Lite XML fragment string, or %NULL. #g_free after usage.
 **/
char *
gupnp_didl_lite_object_get_track_number_xml_string (GUPnPDIDLLiteObject *object)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object), NULL);
        GUPnPDIDLLiteObjectPrivate *priv;
        priv = gupnp_didl_lite_object_get_instance_private (object);

        return av_xml_util_get_child_string (priv->xml_node,
                                             priv->xml_doc->doc,
                                             "originalTrackNumber");
}

/**
 * gupnp_didl_lite_object_get_artists_xml_string:
 * @object: A #GUPnPDIDLLiteObject
 *
 * Creates a string representation of the DIDL-Lite XML fragments related to the
 * object artists.
 *
 * Return value: A DIDL-Lite XML fragment string, or %NULL. #g_free after usage.
 **/
char *
gupnp_didl_lite_object_get_artists_xml_string (GUPnPDIDLLiteObject *object)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object), NULL);

        return get_contributors_xml_string_by_name (object, "artist");
}

/**
 * gupnp_didl_lite_object_unset_artists:
 * @object: #GUPnPDIDLLiteObject
 *
 * Unset the artists properties of the @object.
 **/
void
gupnp_didl_lite_object_unset_artists (GUPnPDIDLLiteObject *object)
{
        g_return_if_fail (object != NULL);
        g_return_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object));

        unset_contributors_by_name (object, "artist");

        g_object_notify (G_OBJECT (object), "artist");
}

/* GENERAL DOCS ABOUT FRAGMENT APPLYING.
 *
 * The function applying fragments takes two arrays of fragments. One
 * array contains current fragments and another one contains new
 * fragments. Both arrays have to be of equal length and have more
 * then zero elements. Each of fragments in both arrays make a pair
 * (i.e. first/second/third/... fragment in current array and
 * first/second/third/... fragment in new array form a pair). Each
 * fragment can have zero, one or more XML elements.
 *
 * For each fragment pair first we check if current fragment is indeed
 * a part of this object's document. If it is then we check validity
 * of new fragment for applying. If it is then we replace the current
 * fragment with new fragment in object's document copy and validate
 * the modified document against didl-lite schema. After all fragment
 * pairs are processed we replace a part describing this object in
 * original document with respective one in modified document.
 *
 * Checking if current fragment is a part of object's document is in
 * essence checking for deep equality of document's node and this
 * fragment (i.e. element name and properties have to be equal, same
 * for children).
 *
 * Checking if new fragment is valid for applying is about checking
 * whether element in new fragment is either a context (i.e. both
 * current element and new element are deep equal) or element
 * modification (i.e. changes attributes but element name is still the
 * same). There may be a case when there are more elements in current
 * fragment than in new fragment then those excessive elements are
 * checked whether they can be really removed. The other case is when
 * there are more elements in new fragments than in current fragment -
 * in such situation we check if additions are valid.
 *
 * By checking validity of modification, removals or additions we mean
 * that no read-only properties are changed. Additionaly, for
 * removals, we check if required properties are not removed.
 *
 * This approach may fail in some more twisted cases.
 */

/**
 * gupnp_didl_lite_object_apply_fragments:
 * @object: The #GUPnPDIDLLiteObject
 * @current_fragments: (array length=current_size) (transfer none): XML
 * fragments of @object.
 * @current_size: Size of @current_fragments or -1.
 * @new_fragments: (array length=new_size) (transfer none): Substitutes
 * for @current_fragments.
 * @new_size: Size of @new_fragments or -1.
 *
 * Updates object by applying @new_fragments in places of
 * @current_fragments. For @current_size and @new_size -1 can be
 * passed when respectively @current_fragments and @new_fragments are
 * NULL terminated.
 *
 * Returns: Result of operation.
 */
GUPnPDIDLLiteFragmentResult
gupnp_didl_lite_object_apply_fragments (GUPnPDIDLLiteObject  *object,
                                        gchar               **current_fragments,
                                        gint                  current_size,
                                        gchar               **new_fragments,
                                        gint                  new_size)
{
        DocNode modified;
        DocNode original;
        GUPnPDIDLLiteFragmentResult result;
        gint iter;

        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object),
                              GUPNP_DIDL_LITE_FRAGMENT_RESULT_UNKNOWN_ERROR);
        g_return_val_if_fail (current_fragments != NULL,
                              GUPNP_DIDL_LITE_FRAGMENT_RESULT_CURRENT_INVALID);
        g_return_val_if_fail (new_fragments != NULL,
                              GUPNP_DIDL_LITE_FRAGMENT_RESULT_NEW_INVALID);

        result = GUPNP_DIDL_LITE_FRAGMENT_RESULT_OK;
        modified.doc = NULL;

        if (current_size < 0)
                current_size = g_strv_length (current_fragments);
        if (new_size < 0)
                new_size = g_strv_length (new_fragments);

        if (current_size != new_size) {
                result = GUPNP_DIDL_LITE_FRAGMENT_RESULT_MISMATCH;

                goto out;
        }

        if (!current_size) {
                result = GUPNP_DIDL_LITE_FRAGMENT_RESULT_CURRENT_INVALID;

                goto out;
        }
        GUPnPDIDLLiteObjectPrivate *priv;
        priv = gupnp_didl_lite_object_get_instance_private (object);

        original.doc = priv->xml_doc->doc;
        original.node = priv->xml_node;
        modified.doc = xmlCopyDoc (original.doc, 1);

        if (modified.doc == NULL) {
                result = GUPNP_DIDL_LITE_FRAGMENT_RESULT_UNKNOWN_ERROR;

                goto out;
        }

        modified.node = av_xml_util_find_node (modified.doc->children,
                                               original.node);

        if (modified.node == NULL) {
                result = GUPNP_DIDL_LITE_FRAGMENT_RESULT_UNKNOWN_ERROR;

                goto out;
        }

        for (iter = 0; iter < new_size; ++iter) {
                const gchar *current_fragment = current_fragments[iter];
                const gchar *new_fragment = new_fragments[iter];

                result = fragment_util_check_fragments (&original,
                                                        &modified,
                                                        current_fragment,
                                                        new_fragment,
                                                        didl_lite_xsd);

                if (result != GUPNP_DIDL_LITE_FRAGMENT_RESULT_OK)
                        goto out;
        }

        if (!fragment_util_apply_modification (&priv->xml_node,
                                               &modified))
                result = GUPNP_DIDL_LITE_FRAGMENT_RESULT_UNKNOWN_ERROR;
 out:
        if (modified.doc != NULL)
                xmlFreeDoc (modified.doc);
        return result;
}

/**
 * gupnp_didl_lite_object_get_xml_string:
 * @object: #GUPnPDIDLLiteObject
 *
 * Get the representation of this object as an XML string.
 * Returns: (transfer full): XML representation of this object as string.
 **/
char *
gupnp_didl_lite_object_get_xml_string (GUPnPDIDLLiteObject *object)
{
        xmlBuffer *buffer = NULL;
        char *ret = NULL;
        GUPnPDIDLLiteObjectPrivate *priv;
        priv = gupnp_didl_lite_object_get_instance_private (object);

        buffer = xmlBufferCreate ();
        xmlNodeDump (buffer, priv->xml_doc->doc, priv->xml_node, 0, 0);

        ret = g_strndup ((char *) xmlBufferContent (buffer),
                         xmlBufferLength (buffer));
        xmlBufferFree (buffer);

        return ret;
}

/**
 * gupnp_format_date_time_for_didl_lite:
 * @date_time: DateTime to format
 *
 * Get the representation of DateTime as an ISO8601 string.
 *
 * DLNA requires a specific subset of ISO8601
 * Returns: (transfer full): @date_time formatted as an ISO8601 string
 *
 * Since: 0.14.1
 */
char *
gupnp_format_date_time_for_didl_lite (GDateTime *date_time, gboolean date_only)
{
        g_return_val_if_fail (date_time != NULL, NULL);

        if (date_only) {
                return g_date_time_format (date_time, "%F");
        }

        const char *format = "%FT%H:%M:%S";
        char *base_string = g_date_time_format (date_time, format);
        GString *iso_string = g_string_new (base_string);

        // Check if we have sub-second precision. If so, we use that as well,
        // but cannot use %f since that will use microsecond precision, but DLNA
        // only allows for millisecond so we append the milliseconds manually
        if (g_date_time_get_microsecond (date_time) % G_TIME_SPAN_SECOND != 0) {
                g_string_append_printf (
                        iso_string,
                        ".%03d",
                        g_date_time_get_microsecond (date_time) / 1000);
        }

        GTimeSpan utc_offset = g_date_time_get_utc_offset (date_time);
        if (utc_offset == 0) {
                g_string_append (iso_string, "Z");
        } else {
                char *time_zone = g_date_time_format (date_time, "%:z");
                g_string_append (iso_string, time_zone);
                g_free (time_zone);
        }

        g_free (base_string);

        return g_string_free (iso_string, FALSE);
}
