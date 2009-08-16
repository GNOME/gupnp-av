/*
 * Copyright (C) 2006, 2007 OpenedHand Ltd.
 * Copyright (C) 2007 Zeeshan Ali.
 *
 * Author: Jorn Baayen <jorn@openedhand.com>
 * Author: Zeeshan Ali (Khattak) <zeeshanak@gnome.org>
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

#include <string.h>

#include "xml-util.h"

xmlNode *
xml_util_get_element (xmlNode *node,
                      ...)
{
        va_list var_args;

        va_start (var_args, node);

        while (TRUE) {
                const char *arg;

                arg = va_arg (var_args, const char *);
                if (!arg)
                        break;

                for (node = node->children; node; node = node->next) {
                        if (node->name == NULL)
                                continue;

                        if (!strcmp (arg, (char *) node->name))
                                break;
                }

                if (!node)
                        break;
        }

        va_end (var_args);

        return node;
}

GList *
xml_util_get_child_elements_by_name (xmlNode *node, const char *name)
{
       GList *children = NULL;

       for (node = node->children; node; node = node->next) {
               if (node->name == NULL) {
                       continue;
               }

               if (strcmp (name, (char *) node->name) == 0) {
                       children = g_list_append (children, node);
               }
       }

       return children;
}

const char *
xml_util_get_child_element_content (xmlNode    *node,
                                    const char *child_name,
                                    ...)
{
        xmlNode *child_node;
        va_list  var_args;
        const char *content;

        child_node = xml_util_get_element (node, child_name, NULL);
        if (!child_node)
                return NULL;

        content = (const char *) child_node->content;
        if (!content)
                return NULL;

        /* Get the attributes */
        va_start (var_args, child_name);
        while (TRUE) {
                const char *name;
                const char **value;

                name = va_arg (var_args, const char *);
                if (!name)
                        break;
                value = va_arg (var_args, const char **);
                if (!value)
                        break;

                *value = xml_util_get_attribute_content (child_node, name);
        }
        va_end (var_args);

        return content;
}

const char *
xml_util_get_attribute_content (xmlNode    *node,
                                const char *attribute_name)
{
        xmlAttr *attribute;

        for (attribute = node->properties;
             attribute;
             attribute = attribute->next) {
                if (attribute->name == NULL)
                        continue;

                if (strcmp (attribute_name, (char *) attribute->name) == 0)
                        break;
        }

        if (attribute)
                return (const char *) attribute->children->content;
        else
                return NULL;
}

gboolean
xml_util_get_boolean_attribute (xmlNode    *node,
                                const char *attribute_name)
{
        const char *content;
        gchar   *str;
        gboolean ret;

        content = xml_util_get_attribute_content (node, attribute_name);
        if (!content)
                return FALSE;

        str = (char *) content;
        if (g_ascii_strcasecmp (str, "true") == 0 ||
            g_ascii_strcasecmp (str, "yes") == 0)
                ret = TRUE;
        else if (g_ascii_strcasecmp (str, "false") == 0 ||
                 g_ascii_strcasecmp (str, "no") == 0)
                ret = FALSE;
        else {
                int i;

                i = atoi (str);
                ret = i ? TRUE : FALSE;
        }

        return ret;
}

guint
xml_util_get_uint_attribute (xmlNode    *node,
                             const char *attribute_name,
                             guint       default_value)
{
        return (guint) xml_util_get_long_attribute (node,
                                                    attribute_name,
                                                    (glong) default_value);
}

glong
xml_util_get_long_attribute (xmlNode    *node,
                             const char *attribute_name,
                             glong       default_value)
{
        const char *content;

        content = xml_util_get_attribute_content (node, attribute_name);
        if (!content)
                return default_value;

        return atoll (content);
}

void
xml_util_set_child (xmlNode    *parent_node,
                    xmlNs      *namespace,
                    const char *name,
                    const char *value,
                    ...)
{
        xmlNode *node;
        va_list  var_args;

        node = xml_util_get_element (parent_node, name, NULL);
        if (node == NULL) {
                node = xmlNewNode (NULL, (unsigned char *) name);
                xmlSetNs (node, namespace);
                xmlAddChild (parent_node, node);
        }

        xmlNodeSetContent (node, (unsigned char *) value);

        /* Set the attributes */
        va_start (var_args, value);
        while (TRUE) {
                unsigned char *name;
                unsigned char *value;

                name = va_arg (var_args, unsigned char *);
                if (!name)
                        break;
                value = va_arg (var_args, unsigned char *);
                if (!value)
                        break;

                xmlSetProp (node, name, value);
        }
        va_end (var_args);
}

