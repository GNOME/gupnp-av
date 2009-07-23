/*
 * Copyright (C) 2007 Zeeshan Ali (Khattak) <zeeshanak@gnome.org>
 *
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
 * SECTION:gupnp-didl-lite-object
 * @short_description: Utility functions to deal with DIDL-Lite objects
 *
 * A set of utility functions to ease the handling of DIDL-Lite object nodes.
 * Typically these functions will be used by an application in it's callback
 * function that it passes to gupnp_didl_lite_parser_parse_didl().
 *
 */

#include <string.h>

#include "xml-util.h"
#include "gupnp-didl-lite-object.h"

#define CONTAINER_CLASS_NAME "object.container"
#define ITEM_CLASS_NAME      "object.item"

/**
 * gupnp_didl_lite_object_is_container
 * @object_node: The object node
 *
 * Return value: TRUE if @object_node is a container
 **/
gboolean
gupnp_didl_lite_object_is_container (xmlNode *object_node)
{
        char     *class_name;
        gboolean is_container;

        g_return_val_if_fail (object_node != NULL, FALSE);

        class_name = gupnp_didl_lite_object_get_upnp_class (object_node);
        if (class_name == NULL) {
                return FALSE;
        }

        if (0 == strncmp (class_name,
                          CONTAINER_CLASS_NAME,
                          strlen (CONTAINER_CLASS_NAME))) {
                is_container = TRUE;
        } else {
                is_container = FALSE;
        }

        g_free (class_name);

        return is_container;
}

/**
 * gupnp_didl_lite_object_is_item
 * @object_node: The object node
 *
 * Return value: TRUE if @object_node is an item
 **/
gboolean
gupnp_didl_lite_object_is_item (xmlNode *object_node)
{
        char     *class_name;
        gboolean is_item;

        g_return_val_if_fail (object_node != NULL, FALSE);

        class_name = gupnp_didl_lite_object_get_upnp_class (object_node);
        if (class_name == NULL) {
                return FALSE;
        }

        if (0 == strncmp (class_name,
                          ITEM_CLASS_NAME,
                          strlen (ITEM_CLASS_NAME))) {
                is_item = TRUE;
        } else {
                is_item = FALSE;
        }

        g_free (class_name);

        return is_item;
}

/**
 * gupnp_didl_lite_object_get_upnp_class
 * @object_node: The object node
 *
 * Return value: The class of @object_node, or NULL. g_free() after usage.
 **/
char *
gupnp_didl_lite_object_get_upnp_class (xmlNode *object_node)
{
        g_return_val_if_fail (object_node != NULL, NULL);

        return xml_util_get_child_element_content (object_node, "class");
}

/**
 * gupnp_didl_lite_object_get_upnp_class_name
 * @object_node: The object node
 *
 * Return value: The friendly name of the class of @object_node, or NULL.
 * g_free() after usage.
 **/
char *
gupnp_didl_lite_object_get_upnp_class_name (xmlNode *object_node)
{
        g_return_val_if_fail (object_node != NULL, NULL);

        return xml_util_get_child_attribute_content (object_node,
                                                     "class",
                                                     "name");
}

/**
 * gupnp_didl_lite_object_get_id
 * @object_node: The object node
 *
 * Return value: The ID of the @object_node, or NULL. g_free() after usage.
 **/
char *
gupnp_didl_lite_object_get_id (xmlNode *object_node)
{
        g_return_val_if_fail (object_node != NULL, NULL);

        return xml_util_get_attribute_content (object_node, "id");
}

/**
 * gupnp_didl_lite_object_get_parent_id
 * @object_node: The object node
 *
 * Return value: The ID of parent of the @object_node, or NULL. g_free() after
 * usage.
 **/
char *
gupnp_didl_lite_object_get_parent_id (xmlNode *object_node)
{
        g_return_val_if_fail (object_node != NULL, NULL);

        return xml_util_get_attribute_content (object_node,
                                                "parentID");
}

/**
 * gupnp_didl_lite_object_get_property
 * @object_node: The object node
 * @property_name: name of the property
 *
 * Use this function to retreive property nodes by name.
 *
 * Return value: The list of property nodes by the name @property_name
 * belonging to @object_node, or NULL. g_list_free() the returned list after
 * usage but do not modify the contents.
 **/
GList *
gupnp_didl_lite_object_get_property (xmlNode    *object_node,
                                     const char *property_name)
{
        g_return_val_if_fail (object_node != NULL, NULL);

        return xml_util_get_child_elements_by_name (object_node,
                                                    property_name);
}

/**
 * gupnp_didl_lite_property_get_value
 * @property_node: The object property node
 *
 * Return value: The value of the property node @property_node as a string or
 * NULL. g_free() after usage.
 **/
char *
gupnp_didl_lite_property_get_value (xmlNode *property_node)
{
        return xml_util_get_element_content (property_node);
}

/**
 * gupnp_didl_lite_property_get_attribute
 * @property_node: The object property node
 * @attribute: name of the attribute
 *
 * Use this function to retreive attributes of object properties.
 *
 * Return value: The value of the attribute @attribute of the property node
 * @property_node as a string or NULL. g_free() after usage.
 **/
char *
gupnp_didl_lite_property_get_attribute (xmlNode    *property_node,
                                        const char *attribute)
{
        return xml_util_get_attribute_content (property_node, attribute);
}

/**
 * gupnp_didl_lite_object_get_restricted
 * @object_node: The object node
 *
 * Return value: TRUE if @object_node is restricted.
 **/
gboolean
gupnp_didl_lite_object_get_restricted (xmlNode *object_node)
{
        g_return_val_if_fail (object_node != NULL, FALSE);

        return xml_util_get_boolean_attribute (object_node, "restricted");
}

/**
 * gupnp_didl_lite_object_get_title
 * @object_node: The object node
 *
 * Return value: The title of the @object_node, or NULL. g_free() after usage.
 **/
char *
gupnp_didl_lite_object_get_title (xmlNode *object_node)
{
        g_return_val_if_fail (object_node != NULL, FALSE);

        return xml_util_get_child_element_content (object_node, "title");
}

/**
 * gupnp_didl_lite_object_get_creator
 * @object_node: The object node
 *
 * Return value: The creator of the @object_node, or NULL. g_free() after usage.
 **/
char *
gupnp_didl_lite_object_get_creator (xmlNode *object_node)
{
        g_return_val_if_fail (object_node != NULL, FALSE);

        return xml_util_get_child_element_content (object_node, "creator");
}

/**
 * gupnp_didl_lite_object_get_write_status
 * @object_node: The object node
 *
 * Return value: The write status of the @object_node, or NULL. g_free() after
 * usage.
 **/
char *
gupnp_didl_lite_object_get_write_status (xmlNode *object_node)
{
        g_return_val_if_fail (object_node != NULL, FALSE);

        return xml_util_get_child_element_content (object_node, "writeStatus");
}

/**
 * gupnp_didl_lite_container_get_searchable
 * @container_node: The container node
 *
 * Return value: TRUE if @object_node is searchable
 **/
gboolean
gupnp_didl_lite_container_get_searchable (xmlNode *container_node)
{
        g_return_val_if_fail (container_node != NULL, FALSE);

        return xml_util_get_boolean_attribute (container_node, "searchable");
}

/**
 * gupnp_didl_lite_container_get_child_count
 * @container_node: The container node
 *
 * Return value: The child count of the @container_node.
 **/
guint
gupnp_didl_lite_container_get_child_count (xmlNode *container_node)
{
        g_return_val_if_fail (container_node != NULL, 0);

        return xml_util_get_uint_attribute (container_node, "childCount", 0);
}

/**
 * gupnp_didl_lite_item_get_ref_id
 * @item_node: The item node
 *
 * Return value: The ref ID of the @item, or NULL. g_free() after usage.
 **/
char *
gupnp_didl_lite_item_get_ref_id (xmlNode *item_node)
{
        g_return_val_if_fail (item_node != NULL, NULL);

        return xml_util_get_attribute_content (item_node, "refID");
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

static GUPnPDIDLLiteResource *
parse_resource_xml (xmlNode *res_node)
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

/**
 * gupnp_didl_lite_object_get_resources
 * @object_node: The object node
 * @property_name: name of the property
 *
 * Use this function to retreive resources from the object node.
 *
 * Return value: The list of resources belonging to @object_node, or NULL.
 * g_list_free() the returned list after usage and destroy the contents by
 * calling #g_boxed_free on each resource.
 **/
GList *
gupnp_didl_lite_object_get_resources (xmlNode *object_node)
{
        GList *resources = NULL;
        GList *res = NULL;
        GList *ret = NULL;

        g_return_val_if_fail (object_node != NULL, NULL);

        resources = gupnp_didl_lite_object_get_property (object_node, "res");

        for (res = resources; res; res = res->next) {
                GUPnPDIDLLiteResource *resource;
                xmlNode *res_node;

                res_node = (xmlNode *) res->data;

                /* Create a resource struct out of DIDLLite XML */
                resource = parse_resource_xml (res_node);

                ret = g_list_append (ret, resource);
        }

        g_list_free (resources);

        return ret;
}
