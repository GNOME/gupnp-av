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

#include <libxml/tree.h>

G_BEGIN_DECLS

/* DIDL-Lite Generic Object related functions */
char *
gupnp_didl_lite_object_get_value               (xmlNode        *object_node,
                                                const char     *value_id);

char *
gupnp_didl_lite_object_get_upnp_class          (xmlNode        *object_node);

char *
gupnp_didl_lite_object_get_id                  (xmlNode        *object_node);

char *
gupnp_didl_lite_object_get_parent_id           (xmlNode        *object_node);

gboolean
gupnp_didl_lite_object_get_restricted          (xmlNode        *object_node);

gboolean
gupnp_didl_lite_object_get_never_playable      (xmlNode        *object_node);

char *
gupnp_didl_lite_object_get_title               (xmlNode        *object_node);

GList *
gupnp_didl_lite_object_get_descriptors         (xmlNode        *object_node);

GList *
gupnp_didl_lite_object_get_resources           (xmlNode        *object_node);

gboolean
gupnp_didl_lite_object_is_container            (xmlNode        *object_node);

gboolean
gupnp_didl_lite_object_is_item                 (xmlNode        *object_node);

GHashTable *
gupnp_didl_lite_object_get_resource_hash       (xmlNode        *object_node);

/* DIDL-Lite container Object functions */
gboolean
gupnp_didl_lite_container_get_searchable       (xmlNode     *container_node);

guint
gupnp_didl_lite_container_get_child_count      (xmlNode     *container_node);

/* DIDL-Lite item Object functions */
char *
gupnp_didl_lite_item_get_ref_id                (xmlNode          *item_node);

/* DIDL-Lite desc Object functions */
char *
gupnp_didl_lite_descriptor_get_type            (xmlNode          *desc_node);

char *
gupnp_didl_lite_descriptor_get_name_space      (xmlNode          *desc_node);

char *
gupnp_didl_lite_descriptor_get_contents        (xmlNode          *desc_node);

/* DIDL-Lite res Object functions */
char *
gupnp_didl_lite_resource_get_contents          (xmlNode          *res_node);

char *
gupnp_didl_lite_resource_get_import_uri        (xmlNode          *res_node);

char *
gupnp_didl_lite_resource_get_protocol_info     (xmlNode          *res_node);

guint
gupnp_didl_lite_resource_get_size              (xmlNode          *res_node);

char *
gupnp_didl_lite_resource_get_duration          (xmlNode          *res_node);

guint
gupnp_didl_lite_resource_get_bitrate           (xmlNode          *res_node);

guint
gupnp_didl_lite_resource_get_sample_frequency  (xmlNode          *res_node);

guint
gupnp_didl_lite_resource_get_bits_per_sample   (xmlNode          *res_node);

guint
gupnp_didl_lite_resource_get_nr_audio_channels (xmlNode          *res_node);

char *
gupnp_didl_lite_resource_get_resolution        (xmlNode          *res_node);

guint
gupnp_didl_lite_resource_get_color_depth       (xmlNode          *res_node);

char *
gupnp_didl_lite_resource_get_protection        (xmlNode          *res_node);

G_END_DECLS

#endif /* __GUPNP_DIDL_LITE_OBJECT_H__ */
