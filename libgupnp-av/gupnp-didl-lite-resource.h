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

#ifndef __GUPNP_DIDL_LITE_RESOURCE_H__
#define __GUPNP_DIDL_LITE_RESOURCE_H__

#include <stdarg.h>
#include <glib-object.h>
#include <libxml/tree.h>

#include "gupnp-dlna.h"

G_BEGIN_DECLS

/**
 * GUPNP_TYPE_DIDL_LITE_RESOURCE:
 *
 * #GType for GUPnPDIDLLiteResource struct.
 */
#define GUPNP_TYPE_DIDL_LITE_RESOURCE \
                (gupnp_didl_lite_resource_get_type ())

typedef struct {
        char               *uri;           /* Required */
        char               *import_uri;
        char               *protocol;      /* Required */
        char               *network;
        char               *mime_type;     /* Required */
        char               *dlna_profile;
        GList              *play_speeds;   /* GList of strings indicating
                                            * allowed play speeds
                                            */
        GUPnPDLNAConversion dlna_conversion;
        GUPnPDLNAOperation  dlna_operation;
        GUPnPDLNAFlags      dlna_flags;

        /* Stream data */
        long    size;
        long    duration;
        int     bitrate;
        int     sample_freq;
        int     bits_per_sample;
        char   *protection;

        /* Audio */
        int     n_audio_channels;

        /* Video */
        int     width;
        int     height;
        int     color_depth;
} GUPnPDIDLLiteResource;

GType
gupnp_didl_lite_resource_get_type     (void);

void
gupnp_didl_lite_resource_reset        (GUPnPDIDLLiteResource *res);

void
gupnp_didl_lite_resource_destroy      (GUPnPDIDLLiteResource *res);

GUPnPDIDLLiteResource*
gupnp_didl_lite_resource_copy         (const GUPnPDIDLLiteResource *source_res,
                                       GUPnPDIDLLiteResource       *dest_res);

GUPnPDIDLLiteResource *
gupnp_didl_lite_resource_create_from_xml
                                      (xmlNode *res_node);

gboolean
gupnp_didl_lite_resource_protocol_info_compatible
                                      (GUPnPDIDLLiteResource *res,
                                       const char            *protocol_info);

G_END_DECLS

#endif /* __GUPNP_DIDL_LITE_RESOURCE_H__ */
