/*
 * Copyright (C) 2009 Nokia Corporation.
 * Copyright (C) 2007, 2008 OpenedHand Ltd.
 *
 * Authors: Zeeshan Ali (Khattak) <zeeshan.ali@nokia.com>
 *                                <zeeshanak@gnome.org>
 *          Jorn Baayen <jorn@openedhand.com>
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
 * SECTION:gupnp-didl-lite-resource
 * @short_description: DIDL-Lite Resource
 *
 * #GUPnPDIDLLiteResource respresent a DIDL-Lite resource (res) element.
 */

#include <string.h>

#include "gupnp-didl-lite-resource.h"
#include "xml-util.h"

#define SEC_PER_MIN 60
#define SEC_PER_HOUR 3600

G_DEFINE_TYPE (GUPnPDIDLLiteResource,
               gupnp_didl_lite_resource,
               G_TYPE_OBJECT);

struct _GUPnPDIDLLiteResourcePrivate {
        char  *uri;
        char  *import_uri;
        char  *protocol;
        char  *network;
        char  *mime_type;
        char  *dlna_profile;
        char **play_speeds;

        GUPnPDLNAConversion dlna_conversion;
        GUPnPDLNAOperation  dlna_operation;
        GUPnPDLNAFlags      dlna_flags;

        /* Stream data */
        long  size;
        long  duration;
        int   bitrate;
        int   sample_freq;
        int   bits_per_sample;
        char *protection;

        /* Audio */
        int n_audio_channels;

        /* Video & Images */
        int width;
        int height;
        int color_depth;
};

enum {
        PROP_0,
        PROP_URI,
        PROP_IMPORT_URI,
        PROP_PROTOCOL,
        PROP_NETWORK,
        PROP_MIME_TYPE,
        PROP_DLNA_PROFILE,
        PROP_PLAY_SPEEDS,

        PROP_DLNA_CONVERSION,
        PROP_DLNA_OPERATION,
        PROP_DLNA_FLAGS,

        PROP_SIZE,
        PROP_DURATION,
        PROP_BITRATE,
        PROP_SAMPLE_FREQ,
        PROP_PROTECTION,

        PROP_AUDIO_CHANNELS,

        PROP_WIDTH,
        PROP_HEIGHT,
        PROP_COLOR_DEPTH
};

static void
parse_additional_info (const char            *additional_info,
                       GUPnPDIDLLiteResource *resource)
{
        GUPnPDIDLLiteResourcePrivate *priv;
        char **tokens = NULL;
        short i;

        priv = resource->priv;

        if (strcmp (additional_info, "*") == 0)
                return;

        tokens = g_strsplit (additional_info, ";", -1);
        if (tokens == NULL) {
                g_warning ("Invalid additional info in DIDL-Lite resource: %s",
                           additional_info);

                return;
        }

        for (i = 0; tokens[i]; i++) {
                char *p;

                p = g_strstr_len (tokens[i],
                                  strlen (tokens[i]),
                                  "DLNA.ORG_PN=");
                if (p != NULL) {
                        p += 12; /* end of "DLNA.ORG_PN=" */
                        gupnp_didl_lite_resource_set_dlna_profile (resource, p);

                        continue;
                }

                p = g_strstr_len (tokens[i],
                                  strlen (tokens[i]),
                                  "DLNA.ORG_PS=");
                if (p != NULL) {
                        char **play_speeds;

                        p += 12; /* end of "DLNA.ORG_PS=" */

                        play_speeds = g_strsplit (p, ",", -1);
                        gupnp_didl_lite_resource_set_play_speeds
                                                (resource,
                                                 (const char **) play_speeds);
                        g_strfreev (play_speeds);

                        continue;
                }

                p = g_strstr_len (tokens[i],
                                  strlen (tokens[i]),
                                  "DLNA.ORG_CI=");
                if (p != NULL) {
                        p += 12; /* end of "DLNA.ORG_CI=" */

                        gupnp_didl_lite_resource_set_dlna_conversion (resource,
                                                                      atoi (p));

                        continue;
                }

                p = g_strstr_len (tokens[i],
                                  strlen (tokens[i]),
                                  "DLNA.ORG_OP=");
                if (p != NULL) {
                        p += 12; /* end of "DLNA.ORG_OP=" */

                        gupnp_didl_lite_resource_set_dlna_operation
                                                        (resource,
                                                         strtol (p, NULL, 16));

                        continue;
                }

                p = g_strstr_len (tokens[i],
                                  strlen (tokens[i]),
                                  "DLNA.ORG_FLAGS=");
                if (p != NULL) {
                        p += 15; /* end of "DLNA.ORG_FLAGS=" */

                        p[8] = '\0';
                        gupnp_didl_lite_resource_set_dlna_flags
                                                        (resource,
                                                         strtol (p, NULL, 16));

                        continue;
                }
        }

        g_strfreev (tokens);
}

static void
parse_protocol_info (xmlNode               *res_node,
                     GUPnPDIDLLiteResource *resource)
{
        char **tokens = NULL;
        char *protocol_info;

        protocol_info = xml_util_get_attribute_content (res_node,
                                                        "protocolInfo");
        if (protocol_info == NULL) {
                g_warning ("Attribute 'protocolInfo' missing in DIDL-Lite "
                           "resource");

                goto return_point;
        }

        tokens = g_strsplit (protocol_info, ":", 4);
        if (tokens == NULL ||
            tokens[0] == NULL ||
            tokens[1] == NULL ||
            tokens[2] == NULL ||
            tokens[3] == NULL)
                goto return_point;

        gupnp_didl_lite_resource_set_protocol (resource, tokens[0]);
        gupnp_didl_lite_resource_set_network (resource, tokens[1]);
        gupnp_didl_lite_resource_set_mime_type (resource, tokens[2]);

        parse_additional_info (tokens[3], resource);

return_point:
        g_free (protocol_info);
        g_strfreev (tokens);
}

static void
parse_resolution_info (xmlNode               *res_node,
                       GUPnPDIDLLiteResource *resource)
{
        char *resolution;
        char **tokens;

        resolution = xml_util_get_attribute_content (res_node, "resolution");
        if (resolution == NULL)
                return;

        tokens = g_strsplit (resolution, "x", -1);
        if (tokens == NULL || tokens[0] == NULL || tokens[1] == NULL) {
                g_warning ("Failed to resolution string '%s'\n", resolution);

                return;
        }

        gupnp_didl_lite_resource_set_width (resource, atoi (tokens[0]));
        gupnp_didl_lite_resource_set_height (resource, atoi (tokens[1]));

        g_free (resolution);
        g_strfreev (tokens);
}

static gchar *
get_dlna_pn (const char *additional_info)
{
        gchar **tokens;
        gchar *pn = NULL;
        gint i;

        tokens = g_strsplit (additional_info, ";", -1);
        if (tokens == NULL)
                return NULL;

        for (i = 0; tokens[i]; i++) {
                pn = g_strstr_len (tokens[i],
                                   strlen (tokens[i]),
                                   "DLNA.ORG_PN=");
                if (pn != NULL) {
                        pn = g_strdup (pn + 12); /* end of "DLNA.ORG_PN=" */

                        break;
                }
        }

        g_strfreev (tokens);

        return pn;
}

static gboolean
is_transport_compat (GUPnPDIDLLiteResource *resource,
                     const char            *protocol,
                     const char            *network)
{
        if (protocol[0] != '*' &&
            g_ascii_strcasecmp
                        (gupnp_didl_lite_resource_get_protocol (resource),
                         protocol) != 0)
                return FALSE;
        else if (g_ascii_strcasecmp ("internal", protocol) == 0 &&
                 strcmp (gupnp_didl_lite_resource_get_network (resource),
                         network) != 0)
                /* Host must be the same in case of INTERNAL protocol */
                return FALSE;
        else
                return TRUE;
}

static gboolean
is_content_format_compat (GUPnPDIDLLiteResource *resource,
                          const char            *mime_type)
{
        if (mime_type[0] != '*' &&
            g_ascii_strcasecmp
                        (gupnp_didl_lite_resource_get_mime_type (resource),
                         mime_type) != 0)
                return FALSE;
        else
                return TRUE;
}

static gboolean
is_additional_info_compat (GUPnPDIDLLiteResource *resource,
                           const char            *additional_info)
{
        gchar *dlna_profile;
        gboolean ret = FALSE;

        if (additional_info[0] == '*')
                return TRUE;

        dlna_profile = get_dlna_pn (additional_info);
        if (dlna_profile == NULL)
                goto no_profile;

        if (g_ascii_strcasecmp
                        (gupnp_didl_lite_resource_get_dlna_profile (resource),
                         dlna_profile) == 0)
                ret = TRUE;

        g_free (dlna_profile);

no_profile:
        return ret;
}

static long
seconds_from_time (const char *time_str)
{
        char **tokens;
        gdouble seconds = -1;

        if (time_str == NULL)
                return -1;

        tokens = g_strsplit (time_str, ":", -1);
        if (tokens[0] == NULL ||
            tokens[1] == NULL ||
            tokens[2] == NULL)
                goto return_point;

        seconds = g_strtod (tokens[2], NULL);
        seconds += g_strtod (tokens[1], NULL) * SEC_PER_MIN;
        seconds += g_strtod (tokens[0], NULL) * SEC_PER_HOUR;

return_point:
        g_strfreev (tokens);

        return (long) seconds;
}

static void
gupnp_didl_lite_resource_init (GUPnPDIDLLiteResource *resource)
{
        resource->priv = G_TYPE_INSTANCE_GET_PRIVATE
                                        (resource,
                                         GUPNP_TYPE_DIDL_LITE_RESOURCE,
                                         GUPnPDIDLLiteResourcePrivate);

        resource->priv->dlna_conversion = GUPNP_DLNA_CONVERSION_NONE;
        resource->priv->dlna_operation  = GUPNP_DLNA_OPERATION_NONE;
        resource->priv->dlna_flags      = GUPNP_DLNA_FLAGS_DLNA_V15;

        resource->priv->size             = -1;
        resource->priv->duration         = -1;
        resource->priv->bitrate          = -1;
        resource->priv->sample_freq      = -1;
        resource->priv->bits_per_sample  = -1;
        resource->priv->n_audio_channels = -1;
        resource->priv->width            = -1;
        resource->priv->height           = -1;
        resource->priv->color_depth      = -1;
}

static void
gupnp_didl_lite_resource_set_property (GObject      *object,
                                       guint         property_id,
                                       const GValue *value,
                                       GParamSpec   *pspec)
{
        GUPnPDIDLLiteResource *resource;

        resource = GUPNP_DIDL_LITE_RESOURCE (object);

        switch (property_id) {
        case PROP_URI:
                gupnp_didl_lite_resource_set_uri (resource,
                                                  g_value_get_string (value));
                break;
        case PROP_IMPORT_URI:
                gupnp_didl_lite_resource_set_import_uri
                                (resource,
                                 g_value_get_string (value));
                break;
        case PROP_PROTOCOL:
                gupnp_didl_lite_resource_set_protocol
                                (resource,
                                 g_value_get_string (value));
                break;
        case PROP_NETWORK:
                gupnp_didl_lite_resource_set_network
                                (resource,
                                 g_value_get_string (value));
                break;
        case PROP_MIME_TYPE:
                gupnp_didl_lite_resource_set_mime_type
                                (resource,
                                 g_value_get_string (value));
                break;
        case PROP_DLNA_PROFILE:
                gupnp_didl_lite_resource_set_dlna_profile
                                (resource,
                                 g_value_get_string (value));
                break;
        case PROP_PLAY_SPEEDS:
                gupnp_didl_lite_resource_set_play_speeds
                                (resource,
                                 g_value_get_boxed (value));
                break;
        case PROP_DLNA_CONVERSION:
                gupnp_didl_lite_resource_set_dlna_conversion
                                (resource,
                                 g_value_get_flags (value));
                break;
        case PROP_DLNA_OPERATION:
                gupnp_didl_lite_resource_set_dlna_operation
                                (resource,
                                 g_value_get_flags (value));
                break;
        case PROP_DLNA_FLAGS:
                gupnp_didl_lite_resource_set_dlna_flags
                                (resource,
                                 g_value_get_flags (value));
                break;
        case PROP_SIZE:
                gupnp_didl_lite_resource_set_size (resource,
                                                   g_value_get_long (value));
                break;
        case PROP_DURATION:
                gupnp_didl_lite_resource_set_duration
                                (resource,
                                 g_value_get_long (value));
                break;
        case PROP_BITRATE:
                gupnp_didl_lite_resource_set_bitrate (resource,
                                                      g_value_get_int (value));
                break;
        case PROP_SAMPLE_FREQ:
                gupnp_didl_lite_resource_set_sample_freq
                                (resource,
                                 g_value_get_int (value));
                break;
        case PROP_PROTECTION:
                gupnp_didl_lite_resource_set_protection
                                (resource,
                                 g_value_get_string (value));
                break;
        case PROP_AUDIO_CHANNELS:
                gupnp_didl_lite_resource_set_audio_channels
                                (resource,
                                 g_value_get_int (value));
                break;
        case PROP_WIDTH:
                gupnp_didl_lite_resource_set_width (resource,
                                                    g_value_get_int (value));
                break;
        case PROP_HEIGHT:
                gupnp_didl_lite_resource_set_height (resource,
                                                     g_value_get_int (value));
                break;
        case PROP_COLOR_DEPTH:
                gupnp_didl_lite_resource_set_color_depth
                                (resource,
                                 g_value_get_int (value));
                break;
        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
                break;
        }
}

static void
gupnp_didl_lite_resource_get_property (GObject    *object,
                                       guint       property_id,
                                       GValue     *value,
                                       GParamSpec *pspec)
{
        GUPnPDIDLLiteResource *resource;

        resource = GUPNP_DIDL_LITE_RESOURCE (object);

        switch (property_id) {
        case PROP_URI:
                g_value_set_string
                        (value,
                         gupnp_didl_lite_resource_get_uri (resource));
                break;
        case PROP_IMPORT_URI:
                g_value_set_string
                        (value,
                         gupnp_didl_lite_resource_get_import_uri (resource));
                break;
        case PROP_PROTOCOL:
                g_value_set_string
                        (value,
                         gupnp_didl_lite_resource_get_protocol (resource));
                break;
        case PROP_NETWORK:
                g_value_set_string
                        (value,
                         gupnp_didl_lite_resource_get_network (resource));
                break;
        case PROP_MIME_TYPE:
                g_value_set_string
                        (value,
                         gupnp_didl_lite_resource_get_mime_type (resource));
                break;
        case PROP_DLNA_PROFILE:
                g_value_set_string
                        (value,
                         gupnp_didl_lite_resource_get_dlna_profile (resource));
                break;
        case PROP_PLAY_SPEEDS:
                g_value_set_boxed
                        (value,
                         gupnp_didl_lite_resource_get_play_speeds (resource));
                break;
        case PROP_DLNA_CONVERSION:
                g_value_set_flags
                        (value,
                         gupnp_didl_lite_resource_get_dlna_conversion
                                                                (resource));
                break;
        case PROP_DLNA_OPERATION:
                g_value_set_flags
                        (value,
                         gupnp_didl_lite_resource_get_dlna_operation
                                                                (resource));
                break;
        case PROP_DLNA_FLAGS:
                g_value_set_flags
                        (value,
                         gupnp_didl_lite_resource_get_dlna_flags (resource));
                break;
        case PROP_SIZE:
                g_value_set_long (value,
                                  gupnp_didl_lite_resource_get_size (resource));
                break;
        case PROP_DURATION:
                g_value_set_long
                        (value,
                         gupnp_didl_lite_resource_get_duration (resource));
                break;
        case PROP_BITRATE:
                g_value_set_int
                        (value,
                         gupnp_didl_lite_resource_get_bitrate (resource));
                break;
        case PROP_SAMPLE_FREQ:
                g_value_set_int
                        (value,
                         gupnp_didl_lite_resource_get_sample_freq (resource));
                break;
        case PROP_PROTECTION:
                g_value_set_string
                        (value,
                         gupnp_didl_lite_resource_get_protection (resource));
                break;
        case PROP_AUDIO_CHANNELS:
                g_value_set_int
                        (value,
                         gupnp_didl_lite_resource_get_audio_channels
                                                                (resource));
                break;
        case PROP_WIDTH:
                g_value_set_int (value,
                                 gupnp_didl_lite_resource_get_width (resource));
                break;
        case PROP_HEIGHT:
                g_value_set_int
                        (value,
                         gupnp_didl_lite_resource_get_height (resource));
                break;
        case PROP_COLOR_DEPTH:
                g_value_set_int
                        (value,
                         gupnp_didl_lite_resource_get_color_depth (resource));
                break;
        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
                break;
        }
}

static void
gupnp_didl_lite_resource_finalize (GObject *object)
{
        GObjectClass                 *object_class;
        GUPnPDIDLLiteResourcePrivate *priv;

        priv = GUPNP_DIDL_LITE_RESOURCE (object)->priv;

        if (priv->uri)
                g_free (priv->uri);
        if (priv->import_uri)
                g_free (priv->import_uri);
        if (priv->protocol)
                g_free (priv->protocol);
        if (priv->network)
                g_free (priv->network);
        if (priv->mime_type)
                g_free (priv->mime_type);
        if (priv->dlna_profile)
                g_free (priv->dlna_profile);
        if (priv->protection)
                g_free (priv->protection);
        if (priv->play_speeds)
                g_strfreev (priv->play_speeds);

        object_class = G_OBJECT_CLASS (gupnp_didl_lite_resource_parent_class);
        object_class->finalize (object);
}

static void
gupnp_didl_lite_resource_class_init (GUPnPDIDLLiteResourceClass *klass)
{
        GObjectClass *object_class;

        object_class = G_OBJECT_CLASS (klass);

        object_class->set_property = gupnp_didl_lite_resource_set_property;
        object_class->get_property = gupnp_didl_lite_resource_get_property;
        object_class->finalize = gupnp_didl_lite_resource_finalize;

        g_type_class_add_private (klass, sizeof (GUPnPDIDLLiteResourcePrivate));

        /**
         * GUPnPDIDLLiteResource:uri
         *
         * The URI associated with this resource.
         **/
        g_object_class_install_property
                (object_class,
                 PROP_URI,
                 g_param_spec_string ("uri",
                                      "URI",
                                      "The URI associated with this resource",
                                      NULL,
                                      G_PARAM_READWRITE |
                                      G_PARAM_CONSTRUCT |
                                      G_PARAM_STATIC_NAME |
                                      G_PARAM_STATIC_NICK |
                                      G_PARAM_STATIC_BLURB));

        /**
         * GUPnPDIDLLiteResource:import-uri
         *
         * The Import URI associated with this resource.
         **/
        g_object_class_install_property
                (object_class,
                 PROP_IMPORT_URI,
                 g_param_spec_string ("import-uri",
                                      "ImportURI",
                                      "The import URI associated with this"
                                      " resource",
                                      NULL,
                                      G_PARAM_READWRITE |
                                      G_PARAM_STATIC_NAME |
                                      G_PARAM_STATIC_NICK |
                                      G_PARAM_STATIC_BLURB));

        /**
         * GUPnPDIDLLiteResource:protocol
         *
         * The protocol of this resource.
         **/
        g_object_class_install_property
                (object_class,
                 PROP_PROTOCOL,
                 g_param_spec_string ("protocol",
                                      "Protocol",
                                      "The protocol of this resource.",
                                      NULL,
                                      G_PARAM_READWRITE |
                                      G_PARAM_STATIC_NAME |
                                      G_PARAM_STATIC_NICK |
                                      G_PARAM_STATIC_BLURB));

        /**
         * GUPnPDIDLLiteResource:network
         *
         * The network this resource is associated with.
         **/
        g_object_class_install_property
                (object_class,
                 PROP_NETWORK,
                 g_param_spec_string ("network",
                                      "Network",
                                      "The network this resource is associated"
                                      " with.",
                                      NULL,
                                      G_PARAM_READWRITE |
                                      G_PARAM_STATIC_NAME |
                                      G_PARAM_STATIC_NICK |
                                      G_PARAM_STATIC_BLURB));

        /**
         * GUPnPDIDLLiteResource:mime-type
         *
         * The MIME-type of this resource.
         **/
        g_object_class_install_property
                (object_class,
                 PROP_MIME_TYPE,
                 g_param_spec_string ("mime-type",
                                      "MIMEType",
                                      "The MIME-type of this resource.",
                                      NULL,
                                      G_PARAM_READWRITE |
                                      G_PARAM_STATIC_NAME |
                                      G_PARAM_STATIC_NICK |
                                      G_PARAM_STATIC_BLURB));

        /**
         * GUPnPDIDLLiteResource:dlna-profile
         *
         * The DLNA profile of this resource.
         **/
        g_object_class_install_property
                (object_class,
                 PROP_DLNA_PROFILE,
                 g_param_spec_string ("dlna-profile",
                                      "DLNAProfile",
                                      "The DLNA profile of this resource.",
                                      NULL,
                                      G_PARAM_READWRITE |
                                      G_PARAM_STATIC_NAME |
                                      G_PARAM_STATIC_NICK |
                                      G_PARAM_STATIC_BLURB));

        /**
         * GUPnPDIDLLiteResource:play-speeds
         *
         * The allowed play speeds on this resource in the form of array of
         * strings.
         **/
        g_object_class_install_property
                (object_class,
                 PROP_PLAY_SPEEDS,
                 g_param_spec_boxed ("play-speeds",
                                     "PlaySpeeds",
                                     "The allowed play speeds on this"
                                     " resource in the form of array of"
                                     " strings.",
                                     G_TYPE_STRV,
                                     G_PARAM_READWRITE |
                                     G_PARAM_STATIC_NAME |
                                     G_PARAM_STATIC_NICK |
                                     G_PARAM_STATIC_BLURB));

        /**
         * GUPnPDIDLLiteResource:dlna-conversion
         *
         * The DLNA conversion flags.
         **/
        g_object_class_install_property
                (object_class,
                 PROP_DLNA_CONVERSION,
                 g_param_spec_flags ("dlna-conversion",
                                     "DLNAConversion",
                                     "The DLNA conversion flags.",
                                     GUPNP_TYPE_DLNA_CONVERSION,
                                     GUPNP_DLNA_CONVERSION_NONE,
                                     G_PARAM_READWRITE |
                                     G_PARAM_STATIC_NAME |
                                     G_PARAM_STATIC_NICK |
                                     G_PARAM_STATIC_BLURB));

        /**
         * GUPnPDIDLLiteResource:dlna-operation
         *
         * The DLNA operation flags.
         **/
        g_object_class_install_property
                (object_class,
                 PROP_DLNA_OPERATION,
                 g_param_spec_flags ("dlna-operation",
                                     "DLNAOperation",
                                     "The DLNA operation flags.",
                                     GUPNP_TYPE_DLNA_OPERATION,
                                     GUPNP_DLNA_OPERATION_NONE,
                                     G_PARAM_READWRITE |
                                     G_PARAM_STATIC_NAME |
                                     G_PARAM_STATIC_NICK |
                                     G_PARAM_STATIC_BLURB));

        /**
         * GUPnPDIDLLiteResource:dlna-flags
         *
         * Various generic DLNA flags.
         **/
        g_object_class_install_property
                (object_class,
                 PROP_DLNA_FLAGS,
                 g_param_spec_flags ("dlna-flags",
                                     "DLNAFlags",
                                     "Various generic DLNA flags.",
                                     GUPNP_TYPE_DLNA_FLAGS,
                                     GUPNP_DLNA_FLAGS_DLNA_V15,
                                     G_PARAM_READWRITE |
                                     G_PARAM_STATIC_NAME |
                                     G_PARAM_STATIC_NICK |
                                     G_PARAM_STATIC_BLURB));

        /**
         * GUPnPDIDLLiteResource:size
         *
         * The size (in bytes) of this resource.
         **/
        g_object_class_install_property
                (object_class,
                 PROP_SIZE,
                 g_param_spec_long ("size",
                                    "Size",
                                    "The size (in bytes) of this resource.",
                                    -1,
                                    G_MAXLONG,
                                    -1,
                                    G_PARAM_READWRITE |
                                    G_PARAM_STATIC_NAME |
                                    G_PARAM_STATIC_NICK |
                                    G_PARAM_STATIC_BLURB));

        /**
         * GUPnPDIDLLiteResource:duration
         *
         * The duration (in seconds) of this resource.
         **/
        g_object_class_install_property
                (object_class,
                 PROP_DURATION,
                 g_param_spec_long ("duration",
                                    "Duration",
                                    "The duration (in seconds) of this"
                                    " resource.",
                                    -1,
                                    G_MAXLONG,
                                    -1,
                                    G_PARAM_READWRITE |
                                    G_PARAM_STATIC_NAME |
                                    G_PARAM_STATIC_NICK |
                                    G_PARAM_STATIC_BLURB));

        /**
         * GUPnPDIDLLiteResource:bitrate
         *
         * The bitrate of this resource.
         **/
        g_object_class_install_property
                (object_class,
                 PROP_BITRATE,
                 g_param_spec_int ("bitrate",
                                   "Bitrate",
                                   "The bitrate of this resource.",
                                   -1,
                                   G_MAXINT,
                                   -1,
                                   G_PARAM_READWRITE |
                                   G_PARAM_STATIC_NAME |
                                   G_PARAM_STATIC_NICK |
                                   G_PARAM_STATIC_BLURB));

        /**
         * GUPnPDIDLLiteResource:sample-freq
         *
         * The sample frequency of this resource.
         **/
        g_object_class_install_property
                (object_class,
                 PROP_SAMPLE_FREQ,
                 g_param_spec_int ("sample-freq",
                                   "SampleFrequency",
                                   "The sample frequency of this resource.",
                                   -1,
                                   G_MAXINT,
                                   -1,
                                   G_PARAM_READWRITE |
                                   G_PARAM_STATIC_NAME |
                                   G_PARAM_STATIC_NICK |
                                   G_PARAM_STATIC_BLURB));

        /**
         * GUPnPDIDLLiteResource:protection
         *
         * The protection system used for this resource.
         **/
        g_object_class_install_property
                (object_class,
                 PROP_PROTECTION,
                 g_param_spec_string ("protection",
                                      "Protection",
                                      "The protection system used by this"
                                      " resource.",
                                      NULL,
                                      G_PARAM_READWRITE |
                                      G_PARAM_STATIC_NAME |
                                      G_PARAM_STATIC_NICK |
                                      G_PARAM_STATIC_BLURB));

        /**
         * GUPnPDIDLLiteResource:audio-channels
         *
         * The number of audio channels in this resource.
         **/
        g_object_class_install_property
                (object_class,
                 PROP_AUDIO_CHANNELS,
                 g_param_spec_int ("audio-channels",
                                   "AudioChannels",
                                   "The number of audio channels in this"
                                   " resource.",
                                   -1,
                                   G_MAXINT,
                                   -1,
                                   G_PARAM_READWRITE |
                                   G_PARAM_STATIC_NAME |
                                   G_PARAM_STATIC_NICK |
                                   G_PARAM_STATIC_BLURB));

        /**
         * GUPnPDIDLLiteResource:width
         *
         * The width of this image/video resource.
         **/
        g_object_class_install_property
                (object_class,
                 PROP_WIDTH,
                 g_param_spec_int ("width",
                                   "Width",
                                   "The width of this image/video resource.",
                                   -1,
                                   G_MAXINT,
                                   -1,
                                   G_PARAM_READWRITE |
                                   G_PARAM_STATIC_NAME |
                                   G_PARAM_STATIC_NICK |
                                   G_PARAM_STATIC_BLURB));

        /**
         * GUPnPDIDLLiteResource:height
         *
         * The height of this image/video resource.
         **/
        g_object_class_install_property
                (object_class,
                 PROP_HEIGHT,
                 g_param_spec_int ("height",
                                   "Height",
                                   "The height of this image/video resource.",
                                   -1,
                                   G_MAXINT,
                                   -1,
                                   G_PARAM_READWRITE |
                                   G_PARAM_STATIC_NAME |
                                   G_PARAM_STATIC_NICK |
                                   G_PARAM_STATIC_BLURB));

        /**
         * GUPnPDIDLLiteResource:color-depth
         *
         * The color-depth of this image/video resource.
         **/
        g_object_class_install_property
                (object_class,
                 PROP_COLOR_DEPTH,
                 g_param_spec_int ("color-depth",
                                   "ColorDepth",
                                   "The color-depth of this image/video"
                                   " resource.",
                                   -1,
                                   G_MAXINT,
                                   -1,
                                   G_PARAM_READWRITE |
                                   G_PARAM_STATIC_NAME |
                                   G_PARAM_STATIC_NICK |
                                   G_PARAM_STATIC_BLURB));
}

/**
 * gupnp_didl_lite_resource_new
 *
 * Return value: A new #GUPnPDIDLLiteResource object. Unref after usage.
 **/
GUPnPDIDLLiteResource *
gupnp_didl_lite_resource_new (const char *uri)
{
        return g_object_new (GUPNP_TYPE_DIDL_LITE_RESOURCE,
                             "uri", uri,
                             NULL);
}

/**
 * gupnp_didl_lite_resource_new_from_xml
 * @res_node: The pointer to 'res' node in XML document
 *
 * Parses the @res_node and creates a new #GUPnPDIDLLiteResource as a result.
 *
 * Return value: A new #GUPnPDIDLLiteResource object. Unref after usage.
 **/
GUPnPDIDLLiteResource *
gupnp_didl_lite_resource_new_from_xml (xmlNode *res_node)
{
        GUPnPDIDLLiteResource *resource;
        char *uri;
        char *duration_str;

        uri = xml_util_get_element_content (res_node);
        resource = gupnp_didl_lite_resource_new (uri);
        g_free (uri);

        gupnp_didl_lite_resource_set_import_uri
                        (resource,
                         xml_util_get_attribute_content (res_node,
                                                         "importUri"));

        duration_str = xml_util_get_attribute_content (res_node, "duration");
        gupnp_didl_lite_resource_set_duration
                        (resource,
                         seconds_from_time (duration_str));
        g_free (duration_str);

        gupnp_didl_lite_resource_set_size
                        (resource,
                         xml_util_get_long_attribute (res_node, "size", -1));
        gupnp_didl_lite_resource_set_bitrate
                        (resource,
                         xml_util_get_long_attribute (res_node, "bitrate", -1));
        gupnp_didl_lite_resource_set_sample_freq
                        (resource,
                         xml_util_get_long_attribute (res_node,
                                                      "sampleFrequency",
                                                      -1));
        gupnp_didl_lite_resource_set_bits_per_sample
                        (resource,
                         xml_util_get_long_attribute (res_node,
                                                      "bitsPerSample",
                                                      -1));
        gupnp_didl_lite_resource_set_protection
                        (resource,
                         xml_util_get_attribute_content (res_node,
                                                         "protection"));
        gupnp_didl_lite_resource_set_audio_channels
                        (resource,
                         xml_util_get_long_attribute (res_node,
                                                      "nrAudioChannels",
                                                      -1));
        gupnp_didl_lite_resource_set_color_depth
                        (resource,
                         xml_util_get_long_attribute (res_node,
                                                      "colorDepth",
                                                      -1));

        parse_protocol_info (res_node, resource);
        parse_resolution_info (res_node, resource);

        return resource;
}

/**
 * gupnp_didl_lite_resource_get_uri
 * @resource: A #GUPnPDIDLLiteResource
 *
 * Get the URI associated with this resource.
 *
 * Return value: The of URI this resource. This string should not be freed.
 **/
const char *
gupnp_didl_lite_resource_get_uri (GUPnPDIDLLiteResource *resource)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_RESOURCE (resource), NULL);

        return resource->priv->uri;
}

/**
 * gupnp_didl_lite_resource_get_import_uri
 * @resource: A #GUPnPDIDLLiteResource
 *
 * Get the import URI associated with this resource.
 *
 * Return value: The import URI or %NULL. This string should not be freed.
 **/
const char *
gupnp_didl_lite_resource_get_import_uri (GUPnPDIDLLiteResource *resource)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_RESOURCE (resource), NULL);

        return resource->priv->import_uri;
}

/**
 * gupnp_didl_lite_resource_get_protocol
 * @resource: A #GUPnPDIDLLiteResource
 *
 * Get the protocol of this resource.
 *
 * Return value: The protocol of this resource or %NULL. This string should not
 * be freed.
 **/
const char *
gupnp_didl_lite_resource_get_protocol (GUPnPDIDLLiteResource *resource)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_RESOURCE (resource), NULL);

        return resource->priv->protocol;
}

/**
 * gupnp_didl_lite_resource_get_network
 * @resource: A #GUPnPDIDLLiteResource
 *
 * Get the network this resource is associated with.
 *
 * Return value: The network string or %NULL. This string should not be freed.
 **/
const char *
gupnp_didl_lite_resource_get_network (GUPnPDIDLLiteResource *resource)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_RESOURCE (resource), NULL);

        return resource->priv->network;
}

/**
 * gupnp_didl_lite_resource_get_mime_type
 * @resource: A #GUPnPDIDLLiteResource
 *
 * Get the MIME-type of this resource.
 *
 * Return value: The MIME-type of this resource or %NULL. This string should not
 * be freed.
 **/
const char *
gupnp_didl_lite_resource_get_mime_type (GUPnPDIDLLiteResource *resource)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_RESOURCE (resource), NULL);

        return resource->priv->mime_type;
}

/**
 * gupnp_didl_lite_resource_get_dlna_profile
 * @resource: A #GUPnPDIDLLiteResource
 *
 * Get the DLNA profile of this resource.
 *
 * Return value: The DLNA profile of this resource or %NULL. This string should
 * not be freed.
 **/
const char *
gupnp_didl_lite_resource_get_dlna_profile (GUPnPDIDLLiteResource *resource)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_RESOURCE (resource), NULL);

        return resource->priv->dlna_profile;
}

/**
 * gupnp_didl_lite_resource_get_play_speeds
 * @resource: A #GUPnPDIDLLiteResource
 *
 * Get the allowed play speeds on this resource in the form of array of strings.
 *
 * Return value: The allowed play speeds as array of strings or %NULL. This
 * return array and it's content must not be modified or freed.
 **/
const char **
gupnp_didl_lite_resource_get_play_speeds (GUPnPDIDLLiteResource *resource)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_RESOURCE (resource), NULL);

        return (const char **) resource->priv->play_speeds;
}

/**
 * gupnp_didl_lite_resource_get_dlna_conversion
 * @resource: A #GUPnPDIDLLiteResource
 *
 * Get the DLNA conversion flags.
 *
 * Return value: The DLNA conversion flags.
 **/
GUPnPDLNAConversion
gupnp_didl_lite_resource_get_dlna_conversion (GUPnPDIDLLiteResource *resource)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_RESOURCE (resource),
                              GUPNP_DLNA_CONVERSION_NONE);

        return resource->priv->dlna_conversion;
}

/**
 * gupnp_didl_lite_resource_get_dlna_operation
 * @resource: A #GUPnPDIDLLiteResource
 *
 * Get the DLNA operation flags.
 *
 * Return value: The DLNA operation flags.
 **/
GUPnPDLNAOperation
gupnp_didl_lite_resource_get_dlna_operation (GUPnPDIDLLiteResource *resource)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_RESOURCE (resource),
                              GUPNP_DLNA_OPERATION_NONE);

        return resource->priv->dlna_operation;
}

/**
 * gupnp_didl_lite_resource_get_dlna_flags
 * @resource: A #GUPnPDIDLLiteResource
 *
 * Get the gereric DLNA flags.
 *
 * Return value: The generic DLNA flags.
 **/
GUPnPDLNAFlags
gupnp_didl_lite_resource_get_dlna_flags (GUPnPDIDLLiteResource *resource)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_RESOURCE (resource),
                              GUPNP_DLNA_FLAGS_DLNA_V15);

        return resource->priv->dlna_flags;
}

/**
 * gupnp_didl_lite_resource_get_size
 * @resource: A #GUPnPDIDLLiteResource
 *
 * Get the size (in bytes) of this resource.
 *
 * Return value: The size (in bytes) of this resource or -1.
 **/
long
gupnp_didl_lite_resource_get_size (GUPnPDIDLLiteResource *resource)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_RESOURCE (resource), -1);

        return resource->priv->size;
}

/**
 * gupnp_didl_lite_resource_get_duration
 * @resource: A #GUPnPDIDLLiteResource
 *
 * Get the duration (in seconds) of this resource.
 *
 * Return value: The duration (in seconds) of this resource or -1.
 **/
long
gupnp_didl_lite_resource_get_duration (GUPnPDIDLLiteResource *resource)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_RESOURCE (resource), -1);

        return resource->priv->duration;
}

/**
 * gupnp_didl_lite_resource_get_bitrate
 * @resource: A #GUPnPDIDLLiteResource
 *
 * Get the bitrate (in bytes per second) of this resource.
 *
 * Return value: The bitrate (in bytes per second) of this resource or -1.
 **/
int
gupnp_didl_lite_resource_get_bitrate (GUPnPDIDLLiteResource *resource)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_RESOURCE (resource), -1);

        return resource->priv->bitrate;
}

/**
 * gupnp_didl_lite_resource_get_sample_freq
 * @resource: A #GUPnPDIDLLiteResource
 *
 * Get the sample frequency of this resource.
 *
 * Return value: The sample frequency of this resource or -1.
 **/
int
gupnp_didl_lite_resource_get_sample_freq (GUPnPDIDLLiteResource *resource)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_RESOURCE (resource), -1);

        return resource->priv->sample_freq;
}

/**
 * gupnp_didl_lite_resource_get_bits_per_sample
 * @resource: A #GUPnPDIDLLiteResource
 *
 * Set the sample size of this resource.
 *
 * Return value: The number of bits per sample of this resource or -1.
 **/
int
gupnp_didl_lite_resource_get_bits_per_sample
                                        (GUPnPDIDLLiteResource *resource)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_RESOURCE (resource), -1);

        return resource->priv->bits_per_sample;
}

/**
 * gupnp_didl_lite_resource_get_protection
 * @resource: A #GUPnPDIDLLiteResource
 *
 * Get the protection system used by this resource.
 *
 * Return value: The protection system in use by this resource or %NULL. This
 * string should not be freed.
 **/
const char *
gupnp_didl_lite_resource_get_protection (GUPnPDIDLLiteResource *resource)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_RESOURCE (resource), NULL);

        return resource->priv->protection;
}

/**
 * gupnp_didl_lite_resource_get_audio_channels
 * @resource: A #GUPnPDIDLLiteResource
 *
 * Get the number of audio channels in this resource.
 *
 * Return value: The number of audio channels in this resource or -1.
 **/
int
gupnp_didl_lite_resource_get_audio_channels (GUPnPDIDLLiteResource *resource)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_RESOURCE (resource), -1);

        return resource->priv->n_audio_channels;
}

/**
 * gupnp_didl_lite_resource_get_width
 * @resource: A #GUPnPDIDLLiteResource
 *
 * Get the width of this image/video resource.
 *
 * Return value: The width of this image/video resource or -1.
 **/
int
gupnp_didl_lite_resource_get_width (GUPnPDIDLLiteResource *resource)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_RESOURCE (resource), -1);

        return resource->priv->width;
}

/**
 * gupnp_didl_lite_resource_get_height
 * @resource: A #GUPnPDIDLLiteResource
 *
 * Get the height of this image/video resource.
 *
 * Return value: The height of this resource or -1.
 **/
int
gupnp_didl_lite_resource_get_height (GUPnPDIDLLiteResource *resource)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_RESOURCE (resource), -1);

        return resource->priv->height;
}

/**
 * gupnp_didl_lite_resource_get_color_depth
 * @resource: A #GUPnPDIDLLiteResource
 *
 * Get the color-depth of this image/video resource.
 *
 * Return value: The color depth of this resource or -1.
 **/
int
gupnp_didl_lite_resource_get_color_depth (GUPnPDIDLLiteResource *resource)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_RESOURCE (resource), -1);

        return resource->priv->color_depth;
}

/**
 * gupnp_didl_lite_resource_set_uri
 * @resource: A #GUPnPDIDLLiteResource
 * @uri: The URI as string
 *
 * Set the URI associated with this resource.
 *
 * Return value: None.
 **/
void
gupnp_didl_lite_resource_set_uri (GUPnPDIDLLiteResource *resource,
                                  const char            *uri)
{
        g_return_if_fail (GUPNP_IS_DIDL_LITE_RESOURCE (resource));
        g_return_if_fail (uri != NULL);

        if (resource->priv->uri)
                g_free (resource->priv->uri);
        resource->priv->uri = g_strdup (uri);

        g_object_notify (G_OBJECT (resource), "uri");
}

/**
 * gupnp_didl_lite_resource_set_import_uri
 * @resource: A #GUPnPDIDLLiteResource
 * @import_uri: The URI as string
 *
 * Set the import URI associated with this resource.
 *
 * Return value: None.
 **/
void
gupnp_didl_lite_resource_set_import_uri (GUPnPDIDLLiteResource *resource,
                                         const char            *import_uri)
{
        g_return_if_fail (GUPNP_IS_DIDL_LITE_RESOURCE (resource));

        if (resource->priv->import_uri)
                g_free (resource->priv->import_uri);
        resource->priv->import_uri = g_strdup (import_uri);

        g_object_notify (G_OBJECT (resource), "import-uri");
}

/**
 * gupnp_didl_lite_resource_set_protocol
 * @resource: A #GUPnPDIDLLiteResource
 * @protocol: The protocol string
 *
 * Set the protocol of this resource.
 *
 * Return value: None.
 **/
void
gupnp_didl_lite_resource_set_protocol (GUPnPDIDLLiteResource *resource,
                                       const char            *protocol)
{
        g_return_if_fail (GUPNP_IS_DIDL_LITE_RESOURCE (resource));

        if (resource->priv->protocol)
                g_free (resource->priv->protocol);
        resource->priv->protocol = g_strdup (protocol);

        g_object_notify (G_OBJECT (resource), "protocol");
}

/**
 * gupnp_didl_lite_resource_set_network
 * @resource: A #GUPnPDIDLLiteResource
 * @network: The network string
 *
 * Set the network this resource is associated with.
 *
 * Return value: None.
 **/
void
gupnp_didl_lite_resource_set_network (GUPnPDIDLLiteResource *resource,
                                      const char            *network)
{
        g_return_if_fail (GUPNP_IS_DIDL_LITE_RESOURCE (resource));

        if (resource->priv->network)
                g_free (resource->priv->network);
        resource->priv->network = g_strdup (network);

        g_object_notify (G_OBJECT (resource), "network");
}

/**
 * gupnp_didl_lite_resource_set_mime_type
 * @resource: A #GUPnPDIDLLiteResource
 * @mime_type: The MIME-type string
 *
 * Set the MIME-type of this resource.
 *
 * Return value: None.
 **/
void
gupnp_didl_lite_resource_set_mime_type (GUPnPDIDLLiteResource *resource,
                                        const char            *mime_type)
{
        g_return_if_fail (GUPNP_IS_DIDL_LITE_RESOURCE (resource));

        if (resource->priv->mime_type)
                g_free (resource->priv->mime_type);
        resource->priv->mime_type = g_strdup (mime_type);

        g_object_notify (G_OBJECT (resource), "mime-type");
}

/**
 * gupnp_didl_lite_resource_set_dlna_profile
 * @resource: A #GUPnPDIDLLiteResource
 * @profile: The DLNA profile string
 *
 * Set the DLNA profile of this resource.
 *
 * Return value: None.
 **/
void
gupnp_didl_lite_resource_set_dlna_profile (GUPnPDIDLLiteResource *resource,
                                           const char            *profile)
{
        g_return_if_fail (GUPNP_IS_DIDL_LITE_RESOURCE (resource));

        if (resource->priv->dlna_profile)
                g_free (resource->priv->dlna_profile);
        resource->priv->dlna_profile = g_strdup (profile);

        g_object_notify (G_OBJECT (resource), "dlna-profile");
}

/**
 * gupnp_didl_lite_resource_set_play_speeds
 * @resource: A #GUPnPDIDLLiteResource
 * @speeds: The allowed play speeds
 *
 * Set the allowed play speeds on this resource in the form of array of strings.
 *
 * Return value: None.
 **/
void
gupnp_didl_lite_resource_set_play_speeds (GUPnPDIDLLiteResource *resource,
                                          const char           **speeds)
{
        g_return_if_fail (GUPNP_IS_DIDL_LITE_RESOURCE (resource));

        if (resource->priv->play_speeds)
                g_strfreev (resource->priv->play_speeds);
        resource->priv->play_speeds = (char **) g_boxed_copy (G_TYPE_STRV,
                                                              speeds);

        g_object_notify (G_OBJECT (resource), "play-speeds");
}

/**
 * gupnp_didl_lite_resource_set_dlna_conversion
 * @resource: A #GUPnPDIDLLiteResource
 * @conversion: The bitwise OR of one or more DLNA conversion flags
 *
 * Set the DLNA conversion flags.
 *
 * Return value: None.
 **/
void
gupnp_didl_lite_resource_set_dlna_conversion (GUPnPDIDLLiteResource *resource,
                                              GUPnPDLNAConversion    conversion)
{
        g_return_if_fail (GUPNP_IS_DIDL_LITE_RESOURCE (resource));

        resource->priv->dlna_conversion = conversion;

        g_object_notify (G_OBJECT (resource), "dlna-conversion");
}

/**
 * gupnp_didl_lite_resource_set_dlna_operation
 * @resource: A #GUPnPDIDLLiteResource
 * @operation: The bitwise OR of one or more DLNA operation flags
 *
 * Set the DLNA operation flags.
 *
 * Return value: None.
 **/
void
gupnp_didl_lite_resource_set_dlna_operation (GUPnPDIDLLiteResource *resource,
                                             GUPnPDLNAOperation     operation)
{
        g_return_if_fail (GUPNP_IS_DIDL_LITE_RESOURCE (resource));

        resource->priv->dlna_operation = operation;

        g_object_notify (G_OBJECT (resource), "dlna-operation");
}

/**
 * gupnp_didl_lite_resource_set_dlna_flags
 * @resource: A #GUPnPDIDLLiteResource
 * @flags: The bitwise OR of one or more generic DLNA flags
 *
 * Set the gereric DLNA flags.
 *
 * Return value: None.
 **/
void
gupnp_didl_lite_resource_set_dlna_flags (GUPnPDIDLLiteResource *resource,
                                         GUPnPDLNAFlags         flags)
{
        g_return_if_fail (GUPNP_IS_DIDL_LITE_RESOURCE (resource));

        resource->priv->dlna_flags = flags;

        g_object_notify (G_OBJECT (resource), "dlna-flags");
}

/**
 * gupnp_didl_lite_resource_set_size
 * @resource: A #GUPnPDIDLLiteResource
 * @size: The size (in bytes)
 *
 * Set the size (in bytes) of this resource.
 *
 * Return value: None.
 **/
void
gupnp_didl_lite_resource_set_size (GUPnPDIDLLiteResource *resource,
                                   long                   size)
{
        g_return_if_fail (GUPNP_IS_DIDL_LITE_RESOURCE (resource));

        resource->priv->size = size;

        g_object_notify (G_OBJECT (resource), "size");
}

/**
 * gupnp_didl_lite_resource_set_duration
 * @resource: A #GUPnPDIDLLiteResource
 * @duration: The duration (in seconds)
 *
 * Set the duration (in seconds) of this resource.
 *
 * Return value: None.
 **/
void
gupnp_didl_lite_resource_set_duration (GUPnPDIDLLiteResource *resource,
                                       long                   duration)
{
        g_return_if_fail (GUPNP_IS_DIDL_LITE_RESOURCE (resource));

        resource->priv->duration = duration;

        g_object_notify (G_OBJECT (resource), "duration");
}

/**
 * gupnp_didl_lite_resource_set_bitrate
 * @resource: A #GUPnPDIDLLiteResource
 * @bitrate: The bitrate
 *
 * Set the bitrate (in bytes per second) of this resource.
 *
 * Return value: None.
 **/
void
gupnp_didl_lite_resource_set_bitrate (GUPnPDIDLLiteResource *resource,
                                      int                    bitrate)
{
        g_return_if_fail (GUPNP_IS_DIDL_LITE_RESOURCE (resource));

        resource->priv->bitrate = bitrate;

        g_object_notify (G_OBJECT (resource), "bitrate");
}

/**
 * gupnp_didl_lite_resource_set_sample_freq
 * @resource: A #GUPnPDIDLLiteResource
 * @sample_freq: The sample frequency
 *
 * Set the sample frequency of this resource.
 *
 * Return value: None.
 **/
void
gupnp_didl_lite_resource_set_sample_freq (GUPnPDIDLLiteResource *resource,
                                          int                    sample_freq)
{
        g_return_if_fail (GUPNP_IS_DIDL_LITE_RESOURCE (resource));

        resource->priv->sample_freq = sample_freq;

        g_object_notify (G_OBJECT (resource), "sample-freq");
}

/**
 * gupnp_didl_lite_resource_set_bits_per_sample
 * @resource: A #GUPnPDIDLLiteResource
 * @sample_size: The number of bits per sample
 *
 * Set the sample size of this resource.
 *
 * Return value: None.
 **/
void
gupnp_didl_lite_resource_set_bits_per_sample
                                        (GUPnPDIDLLiteResource *resource,
                                         int                    sample_size)
{
        g_return_if_fail (GUPNP_IS_DIDL_LITE_RESOURCE (resource));

        resource->priv->bits_per_sample = sample_size;

        g_object_notify (G_OBJECT (resource), "bits-per-sample");
}

/**
 * gupnp_didl_lite_resource_set_protection
 * @resource: A #GUPnPDIDLLiteResource
 * @protection: The protection system identifier as string
 *
 * Set the protection system used by this resource.
 *
 * Return value: None.
 **/
void
gupnp_didl_lite_resource_set_protection (GUPnPDIDLLiteResource *resource,
                                         const char            *protection)
{
        g_return_if_fail (GUPNP_IS_DIDL_LITE_RESOURCE (resource));

        if (resource->priv->protection)
                g_free (resource->priv->protection);
        resource->priv->protection = g_strdup (protection);

        g_object_notify (G_OBJECT (resource), "protection");
}

/**
 * gupnp_didl_lite_resource_set_audio_channels
 * @resource: A #GUPnPDIDLLiteResource
 * @n_channels: The number of channels
 *
 * Set the number of audio channels in this resource.
 *
 * Return value: The number of audio channels in this resource or -1.
 **/
void
gupnp_didl_lite_resource_set_audio_channels (GUPnPDIDLLiteResource *resource,
                                             int                    n_channels)
{
        g_return_if_fail (GUPNP_IS_DIDL_LITE_RESOURCE (resource));

        resource->priv->n_audio_channels = n_channels;

        g_object_notify (G_OBJECT (resource), "height");
}

/**
 * gupnp_didl_lite_resource_set_width
 * @resource: A #GUPnPDIDLLiteResource
 * @width: The width
 *
 * Set the width of this image/video resource.
 *
 * Return value: None.
 **/
void
gupnp_didl_lite_resource_set_width (GUPnPDIDLLiteResource *resource,
                                     int                   width)
{
        g_return_if_fail (GUPNP_IS_DIDL_LITE_RESOURCE (resource));

        resource->priv->width = width;

        g_object_notify (G_OBJECT (resource), "width");
}

/**
 * gupnp_didl_lite_resource_set_height
 * @resource: A #GUPnPDIDLLiteResource
 * @height: The height
 *
 * Set the height of this image/video resource.
 *
 * Return value: None.
 **/
void
gupnp_didl_lite_resource_set_height (GUPnPDIDLLiteResource *resource,
                                     int                    height)
{
        g_return_if_fail (GUPNP_IS_DIDL_LITE_RESOURCE (resource));

        resource->priv->height = height;

        g_object_notify (G_OBJECT (resource), "height");
}

/**
 * gupnp_didl_lite_resource_set_color_depth
 * @resource: A #GUPnPDIDLLiteResource
 * @height: The color-depth
 *
 * Set the color-depth of this image/video resource.
 *
 * Return value: None.
 **/
void
gupnp_didl_lite_resource_set_color_depth (GUPnPDIDLLiteResource *resource,
                                          int                    color_depth)
{
        g_return_if_fail (GUPNP_IS_DIDL_LITE_RESOURCE (resource));

        resource->priv->color_depth = color_depth;

        g_object_notify (G_OBJECT (resource), "color-depth");
}

/**
 * gupnp_didl_lite_resource_protocol_info_compatible
 * @resource: The #GUPnPDIDLLiteResource
 * @protocol_info: The protocolInfo string
 *
 * Checks if the given protocolInfo string is compatible with @resource.
 *
 * Return value: #TRUE if @protocol_info is compatible with @resource, otherwise
 * #FALSE.
 **/
gboolean
gupnp_didl_lite_resource_protocol_info_compatible
                                        (GUPnPDIDLLiteResource *resource,
                                         const char            *protocol_info)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_RESOURCE (resource), FALSE);
        g_return_val_if_fail (protocol_info != NULL, FALSE);

        gchar **tokens;
        gboolean ret = FALSE;

        tokens = g_strsplit (protocol_info, ":", 4);
        if (tokens[0] == NULL ||
            tokens[1] == NULL ||
            tokens[2] == NULL ||
            tokens[3] == NULL) {
                goto return_point;
        }

        if (is_transport_compat (resource, tokens[0], tokens[1]) &&
            is_content_format_compat (resource, tokens[2]) &&
            is_additional_info_compat (resource, tokens[3])) {
                ret = TRUE;
        }

return_point:
        g_strfreev (tokens);

        return ret;
}

