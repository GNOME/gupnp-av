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

#ifndef __GUPNP_DIDL_LITE_OBJECT_H__
#define __GUPNP_DIDL_LITE_OBJECT_H__

#include <glib.h>
#include <libxml/tree.h>

G_BEGIN_DECLS

/* DIDL-Lite Generic Object related functions */
GList *
gupnp_didl_lite_object_get_property            (xmlNode         *object_node,
                                                const char      *property_name);

char *
gupnp_didl_lite_property_get_value             (xmlNode         *property_node);

char *
gupnp_didl_lite_property_get_attribute         (xmlNode         *property_node,
                                                const char      *attribute);

char *
gupnp_didl_lite_object_get_id                  (xmlNode        *object_node);

char *
gupnp_didl_lite_object_get_parent_id           (xmlNode        *object_node);

gboolean
gupnp_didl_lite_object_get_restricted          (xmlNode        *object_node);

char *
gupnp_didl_lite_object_get_title               (xmlNode        *object_node);

char *
gupnp_didl_lite_object_get_creator             (xmlNode        *object_node);

char *
gupnp_didl_lite_object_get_write_status        (xmlNode        *object_node);

char *
gupnp_didl_lite_object_get_upnp_class          (xmlNode        *object_node);

char *
gupnp_didl_lite_object_get_upnp_class_name     (xmlNode        *object_node);

gboolean
gupnp_didl_lite_object_is_container            (xmlNode        *object_node);

gboolean
gupnp_didl_lite_object_is_item                 (xmlNode        *object_node);

/* DIDL-Lite container Object functions */
gboolean
gupnp_didl_lite_container_get_searchable       (xmlNode     *container_node);

guint
gupnp_didl_lite_container_get_child_count      (xmlNode     *container_node);

/* DIDL-Lite item Object functions */
char *
gupnp_didl_lite_item_get_ref_id                (xmlNode          *item_node);

G_END_DECLS

#endif /* __GUPNP_DIDL_LITE_OBJECT_H__ */
