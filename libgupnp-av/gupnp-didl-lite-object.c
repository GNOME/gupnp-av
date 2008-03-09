/*
 * Copyright (C) 2007 Zeeshan Ali <zeenix@gstreamer.net>
 *
 * Authors: Zeeshan Ali <zeenix@gstreamer.net>
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

        class_name = gupnp_didl_lite_object_get_property (object_node,
                                                          "class");
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

        class_name = gupnp_didl_lite_object_get_property (object_node,
                                                          "class");
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
                                                     "id");
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
 * Use this function to retreive a property by name.
 *
 * Return value: The value of property @property_name belonging to @object_node
 * as a string, or NULL. g_free() after usage.
 **/
char *
gupnp_didl_lite_object_get_property (xmlNode    *object_node,
                                     const char *property_name)
{
        g_return_val_if_fail (object_node != NULL, NULL);

        return xml_util_get_child_element_content (object_node, property_name);
}

/**
 * gupnp_didl_lite_object_get_properties
 * @object_node: The object node
 * @properties_name: name of the properties
 *
 * Use this function to retreive a list of properties by the same name. Useful
 * for dealing with multi-valued properties.
 *
 * Return value: The list of property nodes by the name @properties_name
 * belonging to @object_node, or NULL. g_list_free() the returned list after
 * usage but do not modify the contents.
 **/
GList *
gupnp_didl_lite_object_get_properties (xmlNode    *object_node,
                                       const char *properties_name)
{
        g_return_val_if_fail (object_node != NULL, NULL);

        return xml_util_get_child_elements_by_name (object_node,
                                                    properties_name);
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
 * gupnp_didl_lite_object_get_never_playable
 * @object_node: The object node
 *
 * Return value: TRUE if @object_node is never playable.
 **/
gboolean
gupnp_didl_lite_object_get_never_playable (xmlNode *object_node)
{
        g_return_val_if_fail (object_node != NULL, FALSE);

        return xml_util_get_boolean_attribute (object_node, "neverPlayable");
}

/**
 * gupnp_didl_lite_object_get_descriptors
 * @object_node: The object node
 *
 * Return value: The list of descriptor nodes of the @object_node, or NULL.
 * g_list_free() the returned list after usage but do not modify the contents.
 **/
GList *
gupnp_didl_lite_object_get_descriptors (xmlNode *object_node)
{
       return gupnp_didl_lite_object_get_properties (object_node, "desc");
}

/**
 * gupnp_didl_lite_object_get_resources
 * @object_node: The object node
 *
 * Return value: The list of resource nodes of the @object_node, or NULL.
 * g_list_free() the returned list after usage but do not modify the contents.
 **/
GList *
gupnp_didl_lite_object_get_resources (xmlNode *object_node)
{
        return gupnp_didl_lite_object_get_properties (object_node, "res");
}

/**
 * gupnp_didl_lite_object_get_resource_hash
 * @object_node: The object node
 *
 * Use this function to create a hashtable that maps protocolInfo to
 * corresponding URIs.
 *
 * Return value: A newly created hashtable, or NULL. g_hash_table_unref () the
 * returned hash table after usage but do not modify the contents.
 **/
GHashTable *
gupnp_didl_lite_object_get_resource_hash (xmlNode *object_node)
{
   GHashTable *resource_hash;
   GList *resources;
   GList *iter;

   resources = gupnp_didl_lite_object_get_resources (object_node);

   if (resources == NULL)
           return NULL;

   resource_hash = g_hash_table_new_full (g_str_hash,
                                          g_str_equal,
                                          g_free,
                                          g_free);

   for (iter = resources; iter; iter = iter->next) {
           xmlNode *res_node;
           char *uri;
           char *proto_info;

           res_node = (xmlNode *) iter->data;
           proto_info = gupnp_didl_lite_resource_get_protocol_info (res_node);
           if (proto_info == NULL)
                   continue;

           uri = gupnp_didl_lite_resource_get_contents (res_node);
           if (uri == NULL) {
                   g_free (proto_info);
                   continue;
           }

           g_hash_table_insert (resource_hash, proto_info, uri);
   }
   g_list_free (resources);

   if (g_hash_table_size (resource_hash) == 0) {
           /* No point in keeping empty hash tables here */
           g_hash_table_destroy (resource_hash);
           resource_hash = NULL;
   }

   return resource_hash;
}

/**
 * gupnp_didl_lite_object_get_searchable
 * @object_node: The object node
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

        return xml_util_get_uint_attribute (container_node, "childCount");
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

/**
 * gupnp_didl_lite_descriptor_get_type
 * @desc_node: The descriptor node
 *
 * Return value: The type of the @desc_node, or NULL. g_free() after usage.
 **/
char *
gupnp_didl_lite_descriptor_get_type (xmlNode *desc_node)
{
        g_return_val_if_fail (desc_node != NULL, NULL);

        return xml_util_get_attribute_content (desc_node, "type");
}

/**
 * gupnp_didl_lite_descriptor_get_name_space
 * @desc_node: The descriptor node
 *
 * Return value: The name space of the @desc_node, or NULL. g_free() after
 * usage.
 **/
char *
gupnp_didl_lite_descriptor_get_name_space (xmlNode *desc_node)
{
        g_return_val_if_fail (desc_node != NULL, NULL);

        return xml_util_get_attribute_content (desc_node, "nameSpace");
}

/**
 * gupnp_didl_lite_resource_get_contents
 * @res_node: The resource node
 *
 * Return value: The contents of the @res_node, or NULL. g_free() after usage.
 **/
char *
gupnp_didl_lite_resource_get_contents (xmlNode *res_node)
{
        g_return_val_if_fail (res_node != NULL, NULL);

        return xml_util_get_element_content (res_node);
}

/**
 * gupnp_didl_lite_resource_get_import_uri
 * @res_node: The resource node
 *
 * Return value: The import UI of the @res_node, or NULL. g_free() after usage.
 **/
char *
gupnp_didl_lite_resource_get_import_uri (xmlNode *res_node)
{
        g_return_val_if_fail (res_node != NULL, NULL);

        return xml_util_get_attribute_content (res_node, "importUri");
}

/**
 * gupnp_didl_lite_resource_get_protocol_info
 * @res_node: The resource node
 *
 * Return value: The protocol info of the @res_node, or NULL. g_free() after
 * usage.
 **/
char *
gupnp_didl_lite_resource_get_protocol_info (xmlNode *res_node)
{
        g_return_val_if_fail (res_node != NULL, NULL);

        return xml_util_get_attribute_content (res_node, "protocolInfo");
}

/**
 * gupnp_didl_lite_resource_get_size
 * @res_node: The resource node
 *
 * Return value: The size of the @res_node.
 **/
guint
gupnp_didl_lite_resource_get_size (xmlNode *res_node)
{
        g_return_val_if_fail (res_node != NULL, 0);

        return xml_util_get_uint_attribute (res_node, "size");
}

/**
 * gupnp_didl_lite_resource_get_duration
 * @res_node: The resource node
 *
 * Return value: The duration of the @res_node, or NULL. g_free() after usage.
 **/
char *
gupnp_didl_lite_resource_get_duration (xmlNode *res_node)
{
        g_return_val_if_fail (res_node != NULL, NULL);

        return xml_util_get_attribute_content (res_node, "duration");
}

/**
 * gupnp_didl_lite_resource_get_bitrate
 * @res_node: The resource node
 *
 * Return value: The bitrate of the @res_node.
 **/
guint
gupnp_didl_lite_resource_get_bitrate (xmlNode *res_node)
{
        g_return_val_if_fail (res_node != NULL, 0);

        return xml_util_get_uint_attribute (res_node, "bitrate");
}

/**
 * gupnp_didl_lite_resource_get_sample_frequency
 * @res_node: The resource node
 *
 * Return value: The sample frequency of the @res_node.
 **/
guint
gupnp_didl_lite_resource_get_sample_frequency (xmlNode *res_node)
{
        g_return_val_if_fail (res_node != NULL, 0);

        return xml_util_get_uint_attribute (res_node, "frequency");
}

/**
 * gupnp_didl_lite_resource_get_bits_per_sample
 * @res_node: The resource node
 *
 * Return value: The bits-per-sample of the @res_node.
 **/
guint
gupnp_didl_lite_resource_get_bits_per_sample (xmlNode *res_node)
{
        g_return_val_if_fail (res_node != NULL, 0);

        return xml_util_get_uint_attribute (res_node, "bitsPerSample");
}

/**
 * gupnp_didl_lite_resource_get_nr_audio_channels
 * @res_node: The resource node
 *
 * Return value: The number of audio channel of the @res_node.
 **/
guint
gupnp_didl_lite_resource_get_nr_audio_channels (xmlNode *res_node)
{
        g_return_val_if_fail (res_node != NULL, 0);

        return xml_util_get_uint_attribute (res_node, "nrAudioChannels");
}

/**
 * gupnp_didl_lite_resource_get_resolution
 * @res_node: The resource node
 *
 * Return value: The resolution of the @res_node, or NULL. g_free() after
 * usage.
 **/
char *
gupnp_didl_lite_resource_get_resolution (xmlNode *res_node)
{
        g_return_val_if_fail (res_node != NULL, NULL);

        return xml_util_get_attribute_content (res_node, "resolution");
}

/**
 * gupnp_didl_lite_resource_get_color_depth
 * @res_node: The resource node
 *
 * Return value: The color depth of the @res_node.
 **/
guint
gupnp_didl_lite_resource_get_color_depth (xmlNode *res_node)
{
        g_return_val_if_fail (res_node != NULL, 0);

        return xml_util_get_uint_attribute (res_node, "colorDepth");
}

/**
 * gupnp_didl_lite_resource_get_protection
 * @res_node: The resource node
 *
 * Return value: The protection field of the @res_node, or NULL. g_free() after
 * usage.
 **/
char *
gupnp_didl_lite_resource_get_protection (xmlNode *res_node)
{
        g_return_val_if_fail (res_node != NULL, NULL);

        return xml_util_get_attribute_content (res_node, "protection");
}

/**
 * gupnp_didl_lite_resource_get_allowed_use
 * @res_node: The resource node
 *
 * Retreives allowedUse strings of the resource @res_node.
 *
 * Return value: A newly-allocated NULL-terminated array of strings, or NULL.
 * g_strfreev() after usage.
 **/
char **
gupnp_didl_lite_resource_get_allowed_use (xmlNode *res_node)
{
        char *allowed_use;

        g_return_val_if_fail (res_node != NULL, NULL);

        allowed_use = xml_util_get_attribute_content (res_node, "allowedUse");
        if (allowed_use == NULL) {
                return NULL;
        } else {
                return g_strsplit (allowed_use, ",", 0);
        }
}

