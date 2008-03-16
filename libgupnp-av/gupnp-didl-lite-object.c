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
 * gupnp_didl_lite_object_get_searchable
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

