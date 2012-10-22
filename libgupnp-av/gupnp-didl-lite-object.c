/*
 * Copyright (C) 2009 Nokia Corporation.
 * Copyright (C) 2012 Intel Corporation
 *
 * Authors: Zeeshan Ali (Khattak) <zeeshan.ali@nokia.com>
 *                                <zeeshanak@gnome.org>
 *          Krzesimir Nowak <krnowak@openismus.com>
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

/**
 * SECTION:gupnp-didl-lite-object
 * @short_description: DIDL-Lite Object
 *
 * #GUPnPDIDLLiteObject respresent a DIDL-Lite object element.
 */

#include <string.h>
#include <libgupnp/gupnp.h>
#include <libxml/parser.h>
#include <libxml/xmlschemas.h>

#include "gupnp-didl-lite-object.h"
#include "gupnp-didl-lite-object-private.h"
#include "gupnp-didl-lite-resource-private.h"
#include "gupnp-didl-lite-descriptor-private.h"
#include "gupnp-didl-lite-container.h"
#include "gupnp-didl-lite-item.h"
#include "gupnp-didl-lite-contributor-private.h"
#include "xml-util.h"

G_DEFINE_ABSTRACT_TYPE (GUPnPDIDLLiteObject,
                        gupnp_didl_lite_object,
                        G_TYPE_OBJECT);

struct _GUPnPDIDLLiteObjectPrivate {
        xmlNode     *xml_node;
        GUPnPXMLDoc *xml_doc;

        xmlNs *upnp_ns;
        xmlNs *dc_ns;
        xmlNs *dlna_ns;
};

enum {
        PROP_0,
        PROP_XML_NODE,
        PROP_XML_DOC,
        PROP_UPNP_NAMESPACE,
        PROP_DC_NAMESPACE,
        PROP_DLNA_NAMESPACE,
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
is_non_transcoded_resource (GUPnPDIDLLiteResource *resource)
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
        object->priv = G_TYPE_INSTANCE_GET_PRIVATE
                                        (object,
                                         GUPNP_TYPE_DIDL_LITE_OBJECT,
                                         GUPnPDIDLLiteObjectPrivate);
}

static void
gupnp_didl_lite_object_set_property (GObject      *object,
                                     guint         property_id,
                                     const GValue *value,
                                     GParamSpec   *pspec)

{
        GUPnPDIDLLiteObject *didl_object;

        didl_object = GUPNP_DIDL_LITE_OBJECT (object);

        switch (property_id) {
        case PROP_XML_NODE:
                didl_object->priv->xml_node = g_value_get_pointer (value);
                break;
        case PROP_XML_DOC:
                didl_object->priv->xml_doc = g_value_dup_object (value);
                break;
        case PROP_UPNP_NAMESPACE:
                didl_object->priv->upnp_ns = g_value_get_pointer (value);
                break;
        case PROP_DC_NAMESPACE:
                didl_object->priv->dc_ns = g_value_get_pointer (value);
                break;
        case PROP_DLNA_NAMESPACE:
                didl_object->priv->dlna_ns = g_value_get_pointer (value);
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

        priv = GUPNP_DIDL_LITE_OBJECT (object)->priv;

        if (priv->xml_doc) {
                g_object_unref (priv->xml_doc);
                priv->xml_doc = NULL;
        }

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

        g_type_class_add_private (klass, sizeof (GUPnPDIDLLiteObjectPrivate));

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
                 g_param_spec_object ("xml-doc",
                                      "XMLDoc",
                                      "The reference to XML document"
                                      " containing this object.",
                                      GUPNP_TYPE_XML_DOC,
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
         * Deprecated: 0.5.3: Use #gupnp_didl_lite_object_get_creators and
         * #gupnp_didl_lite_object_add_creator instead since unlike this
         * property, they are capable of dealing with multiple creator nodes.
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
}

static gboolean
is_resource_compatible (GUPnPDIDLLiteResource *resource,
                        const char            *sink_protocol_info)
{
        gboolean ret = FALSE;
        char **protocols, **it;

        protocols = g_strsplit (sink_protocol_info, ",", 0);


        for (it = protocols; *it != NULL && !ret; it++) {
                GUPnPProtocolInfo *info;
                GUPnPProtocolInfo *res_info;

                info = gupnp_protocol_info_new_from_string (*it, NULL);
                if (info == NULL)
                        continue;

                res_info = gupnp_didl_lite_resource_get_protocol_info
                                                        (resource);
                ret = gupnp_protocol_info_is_compatible (info, res_info);

                g_object_unref (info);
        }

        g_strfreev (protocols);

        return ret;
}

static GList *
get_contributor_list_by_name (GUPnPDIDLLiteObject *object,
                              const char          *name)
{
        GList *contributors = NULL;
        GList *ret = NULL;
        GList *l;

        contributors = gupnp_didl_lite_object_get_properties (object, name);

        for (l = contributors; l; l = l->next) {
                GUPnPDIDLLiteContributor *contributor;
                xmlNode *contributor_node;

                contributor_node = (xmlNode *) l->data;
                if (!contributor_node->children)
                        continue;

                contributor = gupnp_didl_lite_contributor_new_from_xml
                                        (contributor_node,
                                         object->priv->xml_doc);

                ret = g_list_append (ret, contributor);
        }

        g_list_free (contributors);

        return ret;
}

/**
 * gupnp_didl_lite_object_new_from_xml:
 * @xml_node: The pointer to 'res' node in XML document
 * @xml_doc: The reference to XML document containing this object
 * @upnp_ns: The pointer to 'upnp' namespace in XML document
 * @dc_ns: The pointer to 'dc' namespace in XML document
 * @dlna_ns: The pointer to 'dlna' namespace in XML document
 *
 * Creates a new #GUPnPDIDLLiteObject for the @xml_node.
 *
 * Return value: A new #GUPnPDIDLLiteObject object. Unref after usage.
 **/
GUPnPDIDLLiteObject *
gupnp_didl_lite_object_new_from_xml (xmlNode     *xml_node,
                                     GUPnPXMLDoc *xml_doc,
                                     xmlNs       *upnp_ns,
                                     xmlNs       *dc_ns,
                                     xmlNs       *dlna_ns)
{
        g_return_val_if_fail (xml_node != NULL, NULL);
        g_return_val_if_fail (xml_node->name != NULL, NULL);
        g_return_val_if_fail (upnp_ns != NULL, NULL);
        g_return_val_if_fail (dc_ns != NULL, NULL);
        g_return_val_if_fail (dlna_ns != NULL, NULL);

        if (g_ascii_strcasecmp ((char *) xml_node->name, "container") == 0)
                return g_object_new (GUPNP_TYPE_DIDL_LITE_CONTAINER,
                                     "xml-node", xml_node,
                                     "xml-doc", xml_doc,
                                     "upnp-namespace", upnp_ns,
                                     "dc-namespace", dc_ns,
                                     "dlna-namespace", dlna_ns,
                                     NULL);
        else if (g_ascii_strcasecmp ((char *) xml_node->name, "item") == 0)
                return g_object_new (GUPNP_TYPE_DIDL_LITE_ITEM,
                                     "xml-node", xml_node,
                                     "xml-doc", xml_doc,
                                     "upnp-namespace", upnp_ns,
                                     "dc-namespace", dc_ns,
                                     "dlna-namespace", dlna_ns,
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
GUPnPXMLDoc *
gupnp_didl_lite_object_get_gupnp_xml_doc (GUPnPDIDLLiteObject *object)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object), NULL);

        return object->priv->xml_doc;
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

        return object->priv->xml_node;
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

        return object->priv->upnp_ns;
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

        return object->priv->dc_ns;
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

        return xml_util_get_child_element_content (object->priv->xml_node,
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

        return object->priv->dlna_ns;
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

        return xml_util_get_attribute_content (object->priv->xml_node, "id");
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

        return xml_util_get_attribute_content (object->priv->xml_node,
                                               "parentID");
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

        return xml_util_get_child_elements_by_name (object->priv->xml_node,
                                                    name);
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

        return xml_util_get_boolean_attribute (object->priv->xml_node,
                                               "restricted");
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

        return xml_util_get_child_element_content (object->priv->xml_node,
                                                   "title");
}

/**
 * gupnp_didl_lite_object_get_creator:
 * @object: #GUPnPDIDLLiteObject
 *
 * Get the creator of the @object.
 *
 * Return value: The creator of the @object, or %NULL.
 *
 * Deprecated: 0.5.3: Use #gupnp_didl_lite_object_get_creators instead.
 **/
const char *
gupnp_didl_lite_object_get_creator (GUPnPDIDLLiteObject *object)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object), NULL);

        return xml_util_get_child_element_content (object->priv->xml_node,
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

        return xml_util_get_child_element_content (object->priv->xml_node,
                                                   "artist");
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

        return xml_util_get_child_element_content (object->priv->xml_node,
                                                   "author");
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

        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object), NULL);

        descriptors = gupnp_didl_lite_object_get_properties (object, "desc");

        for (l = descriptors; l; l = l->next) {
                GUPnPDIDLLiteDescriptor *descriptor;
                xmlNode *descriptor_node;

                descriptor_node = (xmlNode *) l->data;

                descriptor = gupnp_didl_lite_descriptor_new_from_xml
                                        (descriptor_node,
                                         object->priv->xml_doc);

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

        return xml_util_get_child_element_content (object->priv->xml_node,
                                                   "genre");
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

        return xml_util_get_child_element_content (object->priv->xml_node,
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

        return xml_util_get_child_element_content (object->priv->xml_node,
                                                   "album");
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

        return xml_util_get_child_element_content (object->priv->xml_node,
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

        return xml_util_get_child_element_content (object->priv->xml_node,
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

        return xml_util_get_child_element_content (object->priv->xml_node,
                                                   "date");
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

        str = xml_util_get_child_element_content (object->priv->xml_node,
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

        str = xml_util_get_attribute_content (object->priv->xml_node,
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

        return xml_util_get_uint_child_element (object->priv->xml_node,
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

        content = xml_util_get_child_element_content (object->priv->xml_node,
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

        resources = gupnp_didl_lite_object_get_properties (object, "res");

        for (res = resources; res; res = res->next) {
                GUPnPDIDLLiteResource *resource;
                xmlNode *res_node;

                res_node = (xmlNode *) res->data;

                /* Create a resource struct out of DIDLLite XML */
                resource = gupnp_didl_lite_resource_new_from_xml
                                        (res_node,
                                         object->priv->xml_doc);

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

        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object), NULL);
        g_return_val_if_fail (sink_protocol_info != NULL, NULL);

        resources = gupnp_didl_lite_object_get_resources (object);
        if (resources == NULL)
                return NULL;

        for (res = resources;
             res != NULL;
             res = res->next) {
                resource = (GUPnPDIDLLiteResource *) res->data;

                if (is_resource_compatible (resource, sink_protocol_info))
                        compat_resources = g_list_append (compat_resources,
                                                          resource);
        }

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

        xml_util_set_child (object->priv->xml_node,
                            object->priv->upnp_ns,
                            object->priv->xml_doc->doc,
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

        xmlSetProp (object->priv->xml_node,
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

        xmlSetProp (object->priv->xml_node,
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

        if (restricted)
                str = "1";
        else
                str = "0";
        xmlSetProp (object->priv->xml_node,
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

        xml_util_set_child (object->priv->xml_node,
                            object->priv->dc_ns,
                            object->priv->xml_doc->doc,
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
 *
 * Deprecated: 0.5.3: Use #gupnp_didl_lite_object_add_creator instead.
 **/
void
gupnp_didl_lite_object_set_creator (GUPnPDIDLLiteObject *object,
                                    const char          *creator)
{
        g_return_if_fail (object != NULL);
        g_return_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object));

        xml_util_set_child (object->priv->xml_node,
                            object->priv->dc_ns,
                            object->priv->xml_doc->doc,
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

        res_node = xmlNewChild (object->priv->xml_node,
                                object->priv->dc_ns,
                                (unsigned char *) "creator",
                                NULL);

        return gupnp_didl_lite_contributor_new_from_xml (res_node,
                                                         object->priv->xml_doc);
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

        xml_util_set_child (object->priv->xml_node,
                            object->priv->upnp_ns,
                            object->priv->xml_doc->doc,
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

        res_node = xmlNewChild (object->priv->xml_node,
                                object->priv->upnp_ns,
                                (unsigned char *) "artist",
                                NULL);

        return gupnp_didl_lite_contributor_new_from_xml (res_node,
                                                         object->priv->xml_doc);
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

        xml_util_set_child (object->priv->xml_node,
                            object->priv->upnp_ns,
                            object->priv->xml_doc->doc,
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

        res_node = xmlNewChild (object->priv->xml_node,
                                object->priv->upnp_ns,
                                (unsigned char *) "author",
                                NULL);

        return gupnp_didl_lite_contributor_new_from_xml (res_node,
                                                         object->priv->xml_doc);
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

        xml_util_set_child (object->priv->xml_node,
                            object->priv->upnp_ns,
                            object->priv->xml_doc->doc,
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

        xml_util_set_child (object->priv->xml_node,
                            object->priv->dc_ns,
                            object->priv->xml_doc->doc,
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

        xml_util_set_child (object->priv->xml_node,
                            object->priv->upnp_ns,
                            object->priv->xml_doc->doc,
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

        node = xml_util_set_child (object->priv->xml_node,
                                   object->priv->upnp_ns,
                                   object->priv->xml_doc->doc,
                                   "albumArtURI",
                                   album_art);
        xmlSetNsProp (node,
                      object->priv->dlna_ns,
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

        xml_util_set_child (object->priv->xml_node,
                            object->priv->dc_ns,
                            object->priv->xml_doc->doc,
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

        xml_util_set_child (object->priv->xml_node,
                            object->priv->dc_ns,
                            object->priv->xml_doc->doc,
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

        str = g_strdup_printf ("%d", track_number);
        xml_util_set_child (object->priv->xml_node,
                            object->priv->upnp_ns,
                            object->priv->xml_doc->doc,
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

        str = g_strdup_printf ("%08x", dlna_managed);
        xmlSetNsProp (object->priv->xml_node,
                      object->priv->dlna_ns,
                      (const unsigned char *) "dlnaManaged",
                      (const unsigned char *) str);
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

        str = g_strdup_printf ("%u", update_id);
        xml_util_set_child (object->priv->xml_node,
                            object->priv->upnp_ns,
                            object->priv->xml_doc->doc,
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

        xml_util_unset_child (object->priv->xml_node,
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

        res_node = xmlNewChild (object->priv->xml_node,
                                NULL,
                                (unsigned char *) "res",
                                NULL);

        return gupnp_didl_lite_resource_new_from_xml (res_node,
                                                      object->priv->xml_doc);
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

        desc_node = xmlNewChild (object->priv->xml_node,
                                NULL,
                                (unsigned char *) "desc",
                                NULL);

        return gupnp_didl_lite_descriptor_new_from_xml (desc_node,
                                                        object->priv->xml_doc);
}

typedef struct {
        gchar *node_name;
        gchar *attribute_name;
} NodeDiff;

static NodeDiff *
node_diff_new (const xmlChar *node_name,
               const xmlChar *attribute_name)
{
        NodeDiff *diff = g_slice_new (NodeDiff);

        diff->node_name = g_strdup ((gchar *) node_name);
        diff->attribute_name = g_strdup ((gchar *) attribute_name);

        return diff;
}

static void
node_diff_free (NodeDiff *diff)
{
        if (diff) {
                g_free (diff->node_name);
                g_free (diff->attribute_name);
                g_slice_free (NodeDiff, diff);
        }
}

static gboolean
node_deep_equal (xmlNodePtr first,
                 xmlNodePtr second);

static gboolean
node_deep_equal (xmlNodePtr first,
                 xmlNodePtr second)
{
        GHashTable *first_attributes;
        xmlAttrPtr attribute;
        gboolean equal;

        if (!first && !second)
                return TRUE;
        if (!first || !second)
                return FALSE;

        if (xmlStrcmp (first->name, second->name))
                return FALSE;

        equal = FALSE;
        first_attributes = g_hash_table_new (g_str_hash, g_str_equal);
        /* compare attributes */
        for (attribute = first->properties;
             attribute;
             attribute = attribute->next) {
                g_hash_table_insert (first_attributes,
                                     (gpointer) attribute->name,
                                     attribute->children->content);
        }
        for (attribute = second->properties;
             attribute;
             attribute = attribute->next) {
                const xmlChar *value = NULL;
                const xmlChar *key = attribute->name;

                if (g_hash_table_lookup_extended (first_attributes, key, NULL, (gpointer *) &value)) {
                        if (!xmlStrcmp (value, attribute->children->content)) {
                                g_hash_table_remove (first_attributes, key);
                                continue;
                        }
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
                xmlNodePtr first_child;
                xmlNodePtr second_child;

                for (first_child = first->children,
                     second_child = second->children;
                     first_child && second_child;
                     first_child = first_child->next,
                     second_child = second_child->next) {
                        if (!node_deep_equal (first_child, second_child)) {
                                return FALSE;
                        }
                }
                if (first_child || second_child)
                        return FALSE;
        }

        return equal;
}

static xmlNodePtr
find_node (xmlNodePtr haystack,
           xmlNodePtr needle);

static xmlNodePtr
find_node (xmlNodePtr haystack,
           xmlNodePtr needle)
{
        xmlNodePtr iter;

        if (node_deep_equal (haystack, needle))
                return haystack;

        for (iter = haystack->children;
             iter;
             iter = iter->next) {
                xmlNodePtr found_node = find_node (iter, needle);

                if (found_node)
                        return found_node;
        }

        return NULL;
}

typedef struct {
        xmlDocPtr doc;
        xmlNodePtr node;
} DocNode;

static gboolean
is_current_doc_part_of_original_doc (DocNode   *original,
                                     xmlDocPtr  current_doc)
{
        xmlNodePtr current_node = current_doc->children->children;
        xmlNodePtr this_node;

        /* No current node means that we want to add new elements to
           the document. */
        if (!current_node)
                return TRUE;

        this_node = find_node (original->node, current_node);

        if (!this_node)
                return FALSE;

        for (current_node = current_node->next, this_node = this_node->next;
             current_node && this_node;
             current_node = current_node->next, this_node = this_node->next) {
                if (!node_deep_equal (current_node, this_node))
                        return FALSE;
        }

        return TRUE;
}

static gboolean
is_read_only (const gchar *changed_element,
              const gchar *changed_attribute)
{
        static GHashTable *readonly_props = NULL;
        static gsize readonly_props_loaded = 0;

        if (g_once_init_enter (&readonly_props_loaded)) {
                readonly_props = g_hash_table_new (g_str_hash,
                                                   g_str_equal);

                g_hash_table_add (readonly_props, "@id");
                g_hash_table_add (readonly_props, "@parentID");
                g_hash_table_add (readonly_props, "@refID");
                g_hash_table_add (readonly_props, "@restricted");
                g_hash_table_add (readonly_props, "@searchable");
                g_hash_table_add (readonly_props, "@childCount");
                g_hash_table_add (readonly_props, "searchClass");
                g_hash_table_add (readonly_props, "searchClass@name");
                g_hash_table_add (readonly_props, "searchClass@includeDerived");
                g_hash_table_add (readonly_props, "createClass");
                g_hash_table_add (readonly_props, "createClass@name");
                g_hash_table_add (readonly_props, "createClass@includeDerived");
                g_hash_table_add (readonly_props, "writeStatus");
                g_hash_table_add (readonly_props, "res@importUri");
                g_hash_table_add (readonly_props, "storageTotal");
                g_hash_table_add (readonly_props, "storageUsed");
                g_hash_table_add (readonly_props, "storageFree");
                g_hash_table_add (readonly_props, "storageMaxPartition");
                g_hash_table_add (readonly_props, "storageMedium");
                g_hash_table_add (readonly_props, "playbackCount");
                g_hash_table_add (readonly_props, "srsRecordScheduleID");
                g_hash_table_add (readonly_props, "srsRecordTaskID");
                g_hash_table_add (readonly_props, "price");
                g_hash_table_add (readonly_props, "price@currency");
                g_hash_table_add (readonly_props, "payPerView");
                g_hash_table_add (readonly_props, "dateTimeRange");
                g_hash_table_add (readonly_props, "dateTimeRange@daylightSaving");
                g_hash_table_add (readonly_props, "signalStrength");
                g_hash_table_add (readonly_props, "signalLocked");
                g_hash_table_add (readonly_props, "tuned");
                g_hash_table_add (readonly_props, "containerUpdateID");
                g_hash_table_add (readonly_props, "objectUpdateID");
                g_hash_table_add (readonly_props, "totalDeletedChildCount");
                g_hash_table_add (readonly_props, "res@updateCount");
                g_once_init_leave (&readonly_props_loaded, 1);
        }
        if (changed_element) {

                if (changed_attribute) {
                        gchar *test_prop = g_strdup_printf ("%s@%s",
                                                            changed_element,
                                                            changed_attribute);
                        gboolean result = g_hash_table_contains (readonly_props,
                                                                 test_prop);

                        g_free (test_prop);
                        if (result)
                                return TRUE;
                        test_prop = g_strdup_printf ("@%s", changed_attribute);
                        result = g_hash_table_contains (readonly_props,
                                                        test_prop);
                        g_free (test_prop);
                        if (result)
                                return TRUE;
                }
                return g_hash_table_contains (readonly_props, changed_element);
        }
        return FALSE;
}

typedef struct {
        gboolean required;
        GHashTable* required_dep_props; /* string set */
        GHashTable* required_indep_props; /* string to indep prop */
} IndependentProperty;

static void
independent_property_free (IndependentProperty *indep)
{
        if (indep) {
                g_hash_table_unref (indep->required_dep_props);
                g_hash_table_unref (indep->required_indep_props);
                g_slice_free (IndependentProperty, indep);
        }
}

static IndependentProperty *
independent_property_new (gboolean required)
{
        IndependentProperty *indep = g_slice_new (IndependentProperty);

        indep->required = required;
        indep->required_dep_props = g_hash_table_new_full (g_str_hash,
                                                           g_str_equal,
                                                           g_free,
                                                           NULL);
        indep->required_indep_props = g_hash_table_new_full
                                   (g_str_hash,
                                    g_str_equal,
                                    g_free,
                                    (GDestroyNotify) independent_property_free);

        return indep;
}

static void
insert_indep_prop (GHashTable *props,
                   gchar *name,
                   IndependentProperty *prop)
{
        g_hash_table_insert (props, g_strdup (name), prop);
}

static void
insert_indep_prop_to_indep (IndependentProperty *prop,
                            gchar *name,
                            IndependentProperty *req_prop)
{
        insert_indep_prop (prop->required_indep_props, name, req_prop);
}

static void
add_dep_prop (IndependentProperty *indep,
              gchar *name)
{
        g_hash_table_add (indep->required_dep_props, g_strdup (name));
}

static IndependentProperty *
create_prop_with_required_dep_props (gboolean required,
                                     gchar *dep_prop,
                                     ...)
{
        IndependentProperty *indep = independent_property_new (required);

        if (dep_prop) {
                va_list var_args;
                gchar *name = dep_prop;

                va_start (var_args, dep_prop);
                do {
                        add_dep_prop (indep, name);
                        name = va_arg (var_args, gchar *);
                } while (name);
                va_end (var_args);
        }

        return indep;
}

static IndependentProperty *
create_foreign_metadata_props (void)
{
        IndependentProperty *fm = independent_property_new (FALSE);
        IndependentProperty *other;

        add_dep_prop (fm, "type");

        other = independent_property_new (TRUE);
        insert_indep_prop_to_indep (fm, "fmId", other);

        other = independent_property_new (TRUE);
        insert_indep_prop_to_indep (fm, "fmClass", other);

        other = independent_property_new (TRUE);
        insert_indep_prop_to_indep (fm, "fmProvider", other);

        other = independent_property_new (TRUE);
        add_dep_prop (other, "xmlFlag");
        insert_indep_prop_to_indep (fm, "fmBody", other);

        return fm;
}

static GHashTable *
get_required_properties (void)
{
        static GHashTable *required_props = NULL;
        static gsize required_props_loaded = 0;

        if (g_once_init_enter (&required_props_loaded)) {
                required_props = g_hash_table_new_full
                                   (g_str_hash,
                                    g_str_equal,
                                    g_free,
                                    (GDestroyNotify) independent_property_free);

                insert_indep_prop (required_props,
                                   "",
                                   create_prop_with_required_dep_props
                                        (FALSE,
                                         "id",
                                         "parentID",
                                         "restricted",
                                         NULL));

                insert_indep_prop (required_props,
                                   "title",
                                   independent_property_new (TRUE));
                insert_indep_prop (required_props,
                                   "class",
                                   independent_property_new (TRUE));

                insert_indep_prop (required_props,
                                   "res",
                                   create_prop_with_required_dep_props
                                        (FALSE,
                                         "protocolInfo",
                                         NULL));
                insert_indep_prop (required_props,
                                   "programID",
                                   create_prop_with_required_dep_props
                                        (FALSE,
                                         "type",
                                         NULL));
                insert_indep_prop (required_props,
                                   "seriesID",
                                   create_prop_with_required_dep_props
                                        (FALSE,
                                         "type",
                                         NULL));
                insert_indep_prop (required_props,
                                   "channelID",
                                   create_prop_with_required_dep_props
                                        (FALSE,
                                         "type",
                                         NULL));
                insert_indep_prop (required_props,
                                   "programCode",
                                   create_prop_with_required_dep_props
                                        (FALSE,
                                         "type",
                                         NULL));
                insert_indep_prop (required_props,
                                   "channelGroupName",
                                   create_prop_with_required_dep_props
                                        (FALSE,
                                         "id",
                                         NULL));
                insert_indep_prop (required_props,
                                   "price",
                                   create_prop_with_required_dep_props
                                        (FALSE,
                                         "currency",
                                         NULL));
                insert_indep_prop (required_props,
                                   "desc",
                                   create_prop_with_required_dep_props
                                        (FALSE,
                                         "nameSpace",
                                         NULL));
                insert_indep_prop (required_props,
                                   "deviceUDN",
                                   create_prop_with_required_dep_props
                                        (FALSE,
                                         "serviceType",
                                         "serviceId",
                                         NULL));
                insert_indep_prop (required_props,
                                   "stateVariableCollection",
                                   create_prop_with_required_dep_props
                                        (FALSE,
                                         "serviceName",
                                         "rcsInstanceType",
                                         NULL));
                insert_indep_prop (required_props,
                                   "foreignMetadata",
                                   create_foreign_metadata_props ());
                g_once_init_leave (&required_props_loaded, 1);
        }

        return required_props;
}

static gboolean
is_required (const xmlChar *changed_element,
             const xmlChar *changed_attribute)
{
        GHashTable *required_props = get_required_properties ();

        if (changed_element) {
                IndependentProperty *toplevel_prop = g_hash_table_lookup
                                        (required_props,
                                         "");
                IndependentProperty *this_prop = g_hash_table_lookup
                                        (required_props,
                                         (gpointer) changed_element);

                if (changed_attribute) {
                        if (g_hash_table_contains (toplevel_prop->required_dep_props,
                                                   changed_attribute))
                                return TRUE;
                        if (g_hash_table_contains (this_prop->required_dep_props,
                                                   changed_attribute))
                                return TRUE;
                }
                if (g_hash_table_contains (toplevel_prop->required_indep_props,
                                           changed_element))
                                return TRUE;
                /* TODO: check if changed element is not a required
                 * property of its parent element. That needs some
                 * additions in IndepependentProperty.
                 */
        }
        return FALSE;
}

static GList *
get_toplevel_changes (xmlNodePtr current_node,
                      xmlNodePtr new_node)
{
        xmlAttrPtr attribute;
        GHashTable *current_attributes = g_hash_table_new (g_str_hash,
                                                           g_str_equal);
        GList *changes = NULL;
        const xmlChar *name = new_node->name;

        /* compare attributes */
        for (attribute = current_node->properties;
             attribute;
             attribute = attribute->next) {
                g_hash_table_insert (current_attributes,
                                     (gpointer) attribute->name,
                                     attribute->children->content);
        }
        for (attribute = new_node->properties;
             attribute;
             attribute = attribute->next) {
                const xmlChar *value = NULL;
                const xmlChar *key = attribute->name;
                gboolean differs = FALSE;

                if (g_hash_table_lookup_extended (current_attributes,
                                                  key,
                                                  NULL,
                                                  (gpointer *) &value)) {
                        if (xmlStrcmp (value, attribute->children->content)) {
                                differs = TRUE;
                        }
                        g_hash_table_remove (current_attributes, key);
                } else
                        differs = TRUE;
                if (differs)
                        changes = g_list_prepend (changes,
                                                  node_diff_new (name,
                                                                 key));
        }

        if (g_hash_table_size (current_attributes) > 0) {
                GHashTableIter iter;
                xmlChar *key = NULL;

                g_hash_table_iter_init (&iter, current_attributes);
                while (g_hash_table_iter_next (&iter,
                                               (gpointer *) &key,
                                               NULL)) {
                        changes = g_list_prepend (changes, node_diff_new (name,
                                                                          key));
                }
        }

        g_hash_table_unref (current_attributes);

        return changes;
}

static gboolean
is_any_change_read_only (xmlNodePtr current_node,
                         xmlNodePtr new_node)
{
        GList *changes = get_toplevel_changes (current_node, new_node);
        GList *iter;
        gboolean read_only = FALSE;

        for (iter = changes; iter; iter = iter->next) {
                NodeDiff *diff = (NodeDiff *) iter->data;

                if (is_read_only (diff->node_name,
                                  diff->attribute_name)) {
                        read_only = TRUE;
                        break;
                }
        }

        if (changes)
                g_list_free_full (changes, (GDestroyNotify) node_diff_free);
        return read_only;
}

typedef struct {
        xmlDocPtr schema_doc;
        xmlSchemaParserCtxtPtr parser_context;
        xmlSchemaPtr schema;
        xmlSchemaValidCtxtPtr valid_context;
} XSDValidateData;

static void
xsd_validate_data_free (XSDValidateData *data)
{
        if (!data)
                return;
        if (data->valid_context)
                xmlSchemaFreeValidCtxt (data->valid_context);
        if (data->schema)
                xmlSchemaFree (data->schema);
        if (data->parser_context)
                xmlSchemaFreeParserCtxt (data->parser_context);
        if (data->schema_doc)
                xmlFreeDoc (data->schema_doc);
        g_slice_free (XSDValidateData, data);
}

static XSDValidateData *
xsd_validate_data_new (const gchar *xsd_file)
{
        XSDValidateData *data = g_slice_new0 (XSDValidateData);
        gboolean failed = TRUE;

        return data;

        data->schema_doc = xmlReadFile (xsd_file, NULL, XML_PARSE_NONET);
        if (!data->schema_doc) {
                /* the schema cannot be loaded or is not well-formed */
                goto out;
        }
        data->parser_context = xmlSchemaNewDocParserCtxt (data->schema_doc);
        if (!data->parser_context) {
                /* unable to create a parser context for the schema */
                goto out;
        }
        data->schema = xmlSchemaParse (data->parser_context);
        if (!data->schema) {
                /* the schema itself is not valid */
                goto out;
        }
        data->valid_context = xmlSchemaNewValidCtxt (data->schema);
        if (!data->valid_context) {
                /* unable to create a validation context for the schema */
                goto out;
        }
        failed = FALSE;
 out:
        if (failed) {
                xsd_validate_data_free (data);
                data = NULL;
        }
        
        return data;
}

static gboolean
validate_temporary_modification (xmlDocPtr        modified_doc,
                                 XSDValidateData *vdata)
{
        xmlChar *dump = NULL;

        xmlDocDumpMemory (modified_doc, &dump, NULL);
        g_debug ("Modified doc dump:\n%s", dump);
        xmlFree (dump);

        return TRUE;

        return (xmlSchemaValidateDoc (vdata->valid_context, modified_doc) == 0);
}

static xmlNodePtr
copy_node (xmlNodePtr node)
{
        xmlNodePtr dup = xmlCopyNode (node, 1);

        /* TODO: remove useless namespace definition. */

        return dup;
}

static GUPnPDIDLLiteFragmentResult
apply_temporary_modification (DocNode         *modified,
                              xmlNodePtr       current_node,
                              xmlNodePtr       new_node,
                              XSDValidateData *vdata)
{
        xmlNodePtr mod_cur_node = find_node (modified->node,
                                             current_node);
        xmlNodePtr new_node_copy = copy_node (new_node);

        if (!mod_cur_node) {
                return GUPNP_DIDL_LITE_FRAGMENT_RESULT_UNKNOWN_ERROR;
        }

        xmlUnlinkNode (new_node_copy);
        mod_cur_node = xmlReplaceNode (mod_cur_node, new_node_copy);
        xmlUnlinkNode (mod_cur_node);
        xmlFreeNode (mod_cur_node);

        if (!validate_temporary_modification (modified->doc, vdata))
                return GUPNP_DIDL_LITE_FRAGMENT_RESULT_NEW_INVALID;

        return GUPNP_DIDL_LITE_FRAGMENT_RESULT_OK;
}

static GUPnPDIDLLiteFragmentResult
apply_temporary_addition (DocNode         *modified,
                          xmlNodePtr       sibling,
                          xmlNodePtr       new_node,
                          XSDValidateData *vdata)
{
        xmlNodePtr mod_sibling;
        xmlNodePtr new_node_copy = copy_node (new_node);

        if (sibling->doc == modified->doc)
                mod_sibling = sibling;
        else
                mod_sibling = find_node (modified->node, sibling);

        if (!mod_sibling)
                return GUPNP_DIDL_LITE_FRAGMENT_RESULT_UNKNOWN_ERROR;

        xmlUnlinkNode (new_node_copy);

        if (!xmlAddSibling (mod_sibling, new_node_copy)) {
                xmlFreeNode (new_node_copy);
                return GUPNP_DIDL_LITE_FRAGMENT_RESULT_UNKNOWN_ERROR;
        }

        if (!validate_temporary_modification (modified->doc, vdata))
                return GUPNP_DIDL_LITE_FRAGMENT_RESULT_NEW_INVALID;

        return GUPNP_DIDL_LITE_FRAGMENT_RESULT_OK;
}

static GUPnPDIDLLiteFragmentResult
apply_temporary_removal (DocNode         *modified,
                         xmlNodePtr       current_node,
                         XSDValidateData *vdata)
{
        xmlNodePtr mod_cur_node = find_node (modified->node,
                                             current_node);

        if (!mod_cur_node)
                return GUPNP_DIDL_LITE_FRAGMENT_RESULT_UNKNOWN_ERROR;

        xmlUnlinkNode (mod_cur_node);
        xmlFreeNode (mod_cur_node);
        if (!validate_temporary_modification (modified->doc, vdata))
                /* not sure if this is correct */
                return GUPNP_DIDL_LITE_FRAGMENT_RESULT_REQUIRED_TAG;

        return GUPNP_DIDL_LITE_FRAGMENT_RESULT_OK;
}

static GUPnPDIDLLiteFragmentResult
new_doc_is_valid_modification (DocNode         *modified,
                               xmlDocPtr        current_doc,
                               xmlDocPtr        new_doc,
                               XSDValidateData *vdata) {
        xmlNodePtr current_node = current_doc->children->children;
        xmlNodePtr new_node = new_doc->children->children;
        xmlNodePtr last_sibling = NULL;

        while (current_node && new_node) {
                GUPnPDIDLLiteFragmentResult result;
                xmlNodePtr temp_current_node = current_node;
                xmlNodePtr temp_new_node = new_node;

                last_sibling = new_node;
                /* We can't put this line into for instruction,
                 * because new_node could be unlinked from its
                 * document and put into another one in
                 * apply_temporary_modification. We have to get its
                 * sibling before that could happen.
                 */
                new_node = new_node->next;
                current_node = current_node->next;
                if (node_deep_equal (temp_current_node, temp_new_node))
                        /* This is just a context, skip the checks. */
                        continue;
                if (xmlStrcmp (temp_current_node->name, temp_new_node->name))
                        return GUPNP_DIDL_LITE_FRAGMENT_RESULT_NEW_INVALID;
                if (is_any_change_read_only (temp_current_node, temp_new_node))
                        return GUPNP_DIDL_LITE_FRAGMENT_RESULT_READONLY_TAG;
                result = apply_temporary_modification (modified,
                                                       temp_current_node,
                                                       temp_new_node,
                                                       vdata);
                if (result != GUPNP_DIDL_LITE_FRAGMENT_RESULT_OK)
                        return result;
        }
        if (!last_sibling) {
                if (modified->node->children)
                        last_sibling = modified->node->children;
                else
                        /* We expect that modified object has some
                         * required tags like <upnp:class> or
                         * <dc:title>.
                         */
                        return GUPNP_DIDL_LITE_FRAGMENT_RESULT_UNKNOWN_ERROR;
        }
        /* If there are some more nodes in current fragment then it
         * means they are going to be removed. Check against required
         * or read-only tag removal.
         */
        while (current_node) {
                GUPnPDIDLLiteFragmentResult result;
                xmlNodePtr temp_node = current_node;

                current_node = current_node->next;
                /* TODO: should we check if there are some readonly
                 * attributes when we remove whole element?
                 */
                if (is_read_only ((gchar *) temp_node->name, NULL)) {
                        return GUPNP_DIDL_LITE_FRAGMENT_RESULT_READONLY_TAG;
                }
                /* We don't check for required attributes or
                 * subelements, because most of them are required only
                 * when the element exists. And we are removing this
                 * one.
                 */
                if (is_required (temp_node->name, NULL)) {
                        return GUPNP_DIDL_LITE_FRAGMENT_RESULT_REQUIRED_TAG;
                }
                result = apply_temporary_removal (modified,
                                                  temp_node,
                                                  vdata);

                if (result != GUPNP_DIDL_LITE_FRAGMENT_RESULT_OK)
                        return result;
        }
        /* If there are some more nodes in new fragment then it means
         * they are going to be added. Check against read-only tags
         * addition and general sanity check.
         */
        while (new_node) {
                GUPnPDIDLLiteFragmentResult result;
                xmlNodePtr temp_node;

                if (is_read_only ((gchar *) new_node->name, NULL)) {
                        return GUPNP_DIDL_LITE_FRAGMENT_RESULT_READONLY_TAG;
                }
                /* TODO: We probably should check if newly added node
                 * has all required properties. Maybe XSD check could
                 * do that for us.
                 */
                temp_node = new_node;
                new_node = new_node->next;
                result = apply_temporary_addition (modified,
                                                   last_sibling,
                                                   temp_node,
                                                   vdata);
                if (result != GUPNP_DIDL_LITE_FRAGMENT_RESULT_OK)
                        return result;
        }

        return GUPNP_DIDL_LITE_FRAGMENT_RESULT_OK;
}

static gchar *
fix_fragment (const gchar *fragment)
{
        return g_strdup_printf
                    ("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                     "<DIDLLiteFragment\n"
                     "xmlns:dc=\"http://purl.org/dc/elements/1.1/\"\n"
                     "xmlns=\"urn:schemas-upnp-org:metadata-1-0/DIDL-Lite/\"\n"
                     "xmlns:upnp=\"urn:schemas-upnp-org:metadata-1-0/upnp/\"\n"
                     "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
                     ">%s</DIDLLiteFragment>\n",
                     fragment);
}

static GUPnPDIDLLiteFragmentResult
check_fragments (DocNode         *original,
                 DocNode         *modified,
                 const gchar     *current_fragment,
                 const gchar     *new_fragment,
                 XSDValidateData *vdata)
{
        gchar *fixed_current_fragment = fix_fragment (current_fragment);
        gchar *fixed_new_fragment = fix_fragment (new_fragment);
        xmlDocPtr current_doc = xmlReadDoc (BAD_CAST (fixed_current_fragment),
                                            NULL,
                                            NULL,
                                            XML_PARSE_NONET);
        xmlDocPtr new_doc = xmlReadDoc (BAD_CAST (fixed_new_fragment),
                                        NULL,
                                        NULL,
                                        XML_PARSE_NONET);
        GUPnPDIDLLiteFragmentResult result;

        if (!current_doc) {
                result = GUPNP_DIDL_LITE_FRAGMENT_RESULT_CURRENT_BAD_XML;
                goto out;
        }
        if (!new_doc) {
                result = GUPNP_DIDL_LITE_FRAGMENT_RESULT_NEW_BAD_XML;
                goto out;
        }

        if (!is_current_doc_part_of_original_doc (original, current_doc)) {
                result = GUPNP_DIDL_LITE_FRAGMENT_RESULT_CURRENT_INVALID;
                goto out;
        }

        result = new_doc_is_valid_modification (modified,
                                                current_doc,
                                                new_doc,
                                                vdata);

 out:
        if (new_doc)
                xmlFreeDoc (new_doc);
        if (current_doc)
                xmlFreeDoc (current_doc);
        g_free (fixed_new_fragment);
        g_free (fixed_current_fragment);

        return result;
}

static gboolean
apply_modification (GUPnPDIDLLiteObject *object,
                    DocNode             *modified)
{
        GUPnPDIDLLiteObjectPrivate *priv = object->priv;
        xmlNodePtr old = xmlReplaceNode (priv->xml_node, modified->node);

        if (!old)
                return FALSE;

        priv->xml_node = modified->node;
        xmlFreeNode (old);

        return TRUE;
}

static const gchar *
get_data_dir (void)
{
        const gchar *datadir = g_getenv ("GUPNP_AV_DATADIR");

        if (!datadir)
                /* that's a macro defined by -DDATADIR=foo */
                datadir = DATADIR;

        return datadir;
}

static XSDValidateData *
get_xsd_validate_data (void)
{
        gchar *path = g_strdup_printf
                                     ("%s" G_DIR_SEPARATOR_S "didl-lite-v2.xsd",
                                      get_data_dir ());
        XSDValidateData *vdata = xsd_validate_data_new (path);

        g_free (path);

        return vdata;
}

/**
 * gupnp_didl_lite_object_apply_fragments:
 * @object: The #GUPnPDIDLLiteObject
 * @current_fragments: (array) (transfer none): XML fragments of @objects
 * @current_size: Size of @current_fragments or -1.
 * @new_fragments: (array) (transfer none): Substitutes for @current_fragments.
 * @new_size: Size of @new_fragments or -1.
 *
 * Updates object by applying @new_fragments in places of @current_fragments.
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
        XSDValidateData *vdata;
        gint iter;

        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object),
                              GUPNP_DIDL_LITE_FRAGMENT_RESULT_UNKNOWN_ERROR);
        g_return_val_if_fail (current_fragments != NULL,
                              GUPNP_DIDL_LITE_FRAGMENT_RESULT_CURRENT_INVALID);
        g_return_val_if_fail (new_fragments != NULL,
                              GUPNP_DIDL_LITE_FRAGMENT_RESULT_NEW_INVALID);

        vdata = get_xsd_validate_data ();

        g_return_val_if_fail (vdata != NULL,
                              GUPNP_DIDL_LITE_FRAGMENT_RESULT_UNKNOWN_ERROR);

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

        original.doc = object->priv->xml_doc->doc;
        original.node = object->priv->xml_node;
        modified.doc = xmlCopyDoc (original.doc, 1);

        if (!modified.doc) {
                result = GUPNP_DIDL_LITE_FRAGMENT_RESULT_UNKNOWN_ERROR;
                goto out;
        }

        modified.node = find_node (modified.doc->children, original.node);

        if (!modified.node) {
                result = GUPNP_DIDL_LITE_FRAGMENT_RESULT_UNKNOWN_ERROR;
                goto out;
        }

        for (iter = 0; iter < new_size; ++iter) {
                const gchar *current_fragment = current_fragments[iter];
                const gchar *new_fragment = new_fragments[iter];

                result = check_fragments (&original,
                                          &modified,
                                          current_fragment,
                                          new_fragment,
                                          vdata);

                if (result != GUPNP_DIDL_LITE_FRAGMENT_RESULT_OK) {
                        goto out;
                }
        }

        apply_modification (object, &modified);
 out:
        if (modified.doc)
                xmlFreeDoc (modified.doc);
        xsd_validate_data_free (vdata);

        return result;
}
