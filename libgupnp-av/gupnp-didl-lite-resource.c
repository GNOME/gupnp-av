/*
 * Copyright (C) 2007, 2008 OpenedHand Ltd.
 * Copyright (C) 2009 Zeeshan Ali (Khattak) <zeeshanak@gnome.org>
 *
 * Authors: Jorn Baayen <jorn@openedhand.com>
 * Authors: Zeeshan Ali (Khattak) <zeeshanak@gnome.org>
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
 * @short_description: DIDL-Lite Resource.
 *
 * #GUPnPDIDLLiteResource represents 'res' nodes in DIDL-Lite fragments. Please
 * refer to UPnP AV specifications for further details.
 */

#include <string.h>

#include "xml-util.h"
#include "gupnp-didl-lite-resource.h"
#include "gupnp-didl-lite-object.h"

/**
 * gupnp_didl_lite_resource_reset
 * @res: A #GUPnPDIDLLiteResource
 *
 * Resets all fields of @res: strings to NULL and numbers to -1.
 **/
void
gupnp_didl_lite_resource_reset (GUPnPDIDLLiteResource *res)
{
        res->uri           = NULL;
        res->import_uri    = NULL;
        res->protocol      = NULL;
        res->network       = NULL;
        res->protection    = NULL;

        res->mime_type = NULL;
        res->dlna_profile = NULL;

        res->play_speeds = NULL;
        res->dlna_conversion = GUPNP_DLNA_CONVERSION_NONE;
        res->dlna_operation  = GUPNP_DLNA_OPERATION_NONE;
        res->dlna_flags      = GUPNP_DLNA_FLAG_DLNA_V15;

        res->size             = -1;
        res->duration         = -1;
        res->bitrate          = -1;
        res->sample_freq      = -1;
        res->bits_per_sample  = -1;
        res->n_audio_channels = -1;
        res->width            = -1;
        res->height           = -1;
        res->color_depth      = -1;
}

/**
 * gupnp_didl_lite_resource_destroy
 * @res: A #GUPnPDIDLLiteResource
 *
 * Frees the string fields of @res.
 *
 * Mainly intended for bindings, avoid using it in applications.
 **/
void
gupnp_didl_lite_resource_destroy (GUPnPDIDLLiteResource *res)
{
        GList *lst;

        g_return_if_fail (res);

        g_free (res->uri);
        g_free (res->import_uri);
        g_free (res->protocol);
        g_free (res->network);
        g_free (res->mime_type);
        g_free (res->dlna_profile);
        g_free (res->protection);

        for (lst = res->play_speeds; lst; lst = lst->next) {
                g_free (lst->data);
        }

        if (res->play_speeds != NULL)
                g_list_free (res->play_speeds);
}

/**
 * gupnp_didl_lite_resource_copy
 * @source_res: The source #GUPnPDIDLLiteResource struct pointer
 * @dest_res: The destincation #GUPnPDIDLLiteResource struct pointer
 *
 * Makes @dest_res a copy of @source_res. Call
 * #gupnp_didl_lite_resource_destroy on the @dest_res before freeing the
 * struct itself.
 *
 * Mainly intended for bindings, avoid using it in applications.
 *
 * Return value: @dest_res.
 **/
GUPnPDIDLLiteResource*
gupnp_didl_lite_resource_copy (const GUPnPDIDLLiteResource *source_res,
                               GUPnPDIDLLiteResource       *dest_res)
{
        GList *lst;

        g_return_val_if_fail (source_res, NULL);
        g_return_val_if_fail (dest_res, NULL);

        g_memmove (dest_res, source_res, sizeof (GUPnPDIDLLiteResource));

        /* Create a copy of the string fields */
        dest_res->uri = g_strdup (dest_res->uri);
        dest_res->import_uri = g_strdup (dest_res->import_uri);
        dest_res->protocol = g_strdup (dest_res->protocol);
        dest_res->network = g_strdup (dest_res->network);
        dest_res->mime_type = g_strdup (dest_res->mime_type);
        dest_res->dlna_profile = g_strdup (dest_res->dlna_profile);
        dest_res->protection = g_strdup (dest_res->protection);

        for (lst = source_res->play_speeds; lst; lst = lst->next) {
                char *speed = g_strdup (lst->data);
                dest_res->play_speeds = g_list_append (dest_res->play_speeds,
                                                       speed);
        }

        return dest_res;
}

static void
parse_additional_info (const char            *additional_info,
                       GUPnPDIDLLiteResource *res)
{
        char **tokens = NULL;
        short i;

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
                short j;

                p = g_strstr_len (tokens[i],
                                  strlen (tokens[i]),
                                  "DLNA.ORG_PN=");
                if (p != NULL) {
                        p += 12; /* end of "DLNA.ORG_PN=" */
                        res->dlna_profile = g_strdup (p);

                        continue;
                }

                p = g_strstr_len (tokens[i],
                                  strlen (tokens[i]),
                                  "DLNA.ORG_PS=");
                if (p != NULL) {
                        char **speeds;

                        p += 12; /* end of "DLNA.ORG_PS=" */

                        speeds = g_strsplit (p, ",", -1);

                        for (j = 0; speeds[j] != NULL; j++) {
                                res->play_speeds =
                                        g_list_append (res->play_speeds,
                                                       speeds[j]);
                        }

                        g_free (speeds); /* No need to free the strings as we
                                            already took them. */

                        continue;
                }

                p = g_strstr_len (tokens[i],
                                  strlen (tokens[i]),
                                  "DLNA.ORG_CI=");
                if (p != NULL) {
                        p += 12; /* end of "DLNA.ORG_CI=" */

                        res->dlna_conversion = atol (p);

                        continue;
                }

                p = g_strstr_len (tokens[i],
                                  strlen (tokens[i]),
                                  "DLNA.ORG_OP=");
                if (p != NULL) {
                        p += 12; /* end of "DLNA.ORG_OP=" */

                        res->dlna_operation = strtol (p, NULL, 16);

                        continue;
                }

                p = g_strstr_len (tokens[i],
                                  strlen (tokens[i]),
                                  "DLNA.ORG_FLAGS=");
                if (p != NULL) {
                        p += 15; /* end of "DLNA.ORG_FLAGS=" */

                        p[8] = '\0';
                        res->dlna_flags = strtol (p, NULL, 16);

                        continue;
                }
        }

        g_strfreev (tokens);
}

static void
parse_protocol_info (xmlNode               *res_node,
                     GUPnPDIDLLiteResource *res)
{
        char **tokens = NULL;
        char *protocol_info;

        protocol_info = gupnp_didl_lite_property_get_attribute (res_node,
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
            tokens[3] == NULL) {
                goto return_point;
        }

        res->protocol = tokens[0];
        res->network = tokens[1];
        res->mime_type = tokens[2];

        parse_additional_info (tokens[4], res);
        g_free (tokens[4]);

        /* So that g_strfreev doesn't free strings that we took */
        tokens[0] = NULL;

return_point:
        g_free (protocol_info);
        g_strfreev (tokens);
}

static void
parse_resolution_info (xmlNode               *res_node,
                       GUPnPDIDLLiteResource *resource)
{
        int width;
        int height;
        char *resolution;
        char **tokens;

        resolution = gupnp_didl_lite_property_get_attribute (res_node,
                                                             "resolution");
        if (resolution == NULL)
                return;

        tokens = g_strsplit (resolution, "x", -1);
        if (tokens == NULL || tokens[0] == NULL || tokens[1] == NULL) {
                g_warning ("Failed to resolution string '%s'\n", resolution);

                return;
        }

        width = atol (tokens[0]);
        height = atol (tokens[1]);

        g_free (resolution);
        g_strfreev (tokens);
}

/**
 * gupnp_didl_lite_resource_create_from_xml
 * @res_node: The pointer to 'res' node in XML document.
 *
 * Parses the @res_node and creates a #GUPnPDIDLLiteResource as a result.
 *
 * Return value: A new #GUPnPDIDLLiteResource structure. Free it after usage by
 * calling #g_boxed_free on it.
 **/
GUPnPDIDLLiteResource *
gupnp_didl_lite_resource_create_from_xml (xmlNode *res_node)
{
        GUPnPDIDLLiteResource *res;

        res = g_slice_new (GUPnPDIDLLiteResource);
        gupnp_didl_lite_resource_reset (res);

        res->uri = gupnp_didl_lite_property_get_value (res_node);
        res->import_uri = gupnp_didl_lite_property_get_attribute (res_node,
                                                                  "importUri");

        parse_protocol_info (res_node, res);

        res->size = xml_util_get_long_attribute (res_node, "size", -1);
        res->duration = xml_util_get_long_attribute (res_node, "duration", -1);
        res->bitrate = xml_util_get_long_attribute (res_node, "bitrate", -1);
        res->sample_freq = xml_util_get_long_attribute (res_node,
                                                        "sampleFrequency",
                                                        -1);
        res->bits_per_sample = xml_util_get_long_attribute (res_node,
                                                            "bitsPerSample",
                                                            -1);
        res->protection= gupnp_didl_lite_property_get_attribute (res_node,
                                                                 "protection");
        res->n_audio_channels = xml_util_get_long_attribute (res_node,
                                                             "nrAudioChannels",
                                                             -1);

        parse_resolution_info (res_node, res);

        res->color_depth = xml_util_get_long_attribute (res_node,
                                                        "colorDepth",
                                                        -1);

        return res;
}

static GUPnPDIDLLiteResource*
boxed_copy (const GUPnPDIDLLiteResource *res)
{
        GUPnPDIDLLiteResource *copy = g_slice_new (GUPnPDIDLLiteResource);

        return gupnp_didl_lite_resource_copy (res, copy);
}

static void
boxed_free (GUPnPDIDLLiteResource *res)
{
        gupnp_didl_lite_resource_destroy (res);

        g_slice_free (GUPnPDIDLLiteResource, res);
}

GType
gupnp_didl_lite_resource_get_type (void)
{
        static GType type = 0;

        if (!type) {
                const char *name =
                        g_intern_static_string ("GUPnPDIDLLiteResource");

                type = g_boxed_type_register_static (
                                        name,
                                        (GBoxedCopyFunc) boxed_copy,
                                        (GBoxedFreeFunc) boxed_free);
        }

        return type;
}

