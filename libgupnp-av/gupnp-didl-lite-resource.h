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

#ifndef __GUPNP_DIDL_LITE_RESOURCE_H__
#define __GUPNP_DIDL_LITE_RESOURCE_H__

#include <stdarg.h>
#include <glib-object.h>
#include <libxml/tree.h>

#include "gupnp-dlna.h"

G_BEGIN_DECLS

GType
gupnp_didl_lite_resource_get_type (void) G_GNUC_CONST;

#define GUPNP_TYPE_DIDL_LITE_RESOURCE \
                (gupnp_didl_lite_resource_get_type ())
#define GUPNP_DIDL_LITE_RESOURCE(obj) \
                (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
                 GUPNP_TYPE_DIDL_LITE_RESOURCE, \
                 GUPnPDIDLLiteResource))
#define GUPNP_DIDL_LITE_RESOURCE_CLASS(obj) \
                (G_TYPE_CHECK_CLASS_CAST ((obj), \
                 GUPNP_TYPE_DIDL_LITE_RESOURCE, \
                 GUPnPDIDLLiteResourceClass))
#define GUPNP_IS_DIDL_LITE_RESOURCE(obj) \
                (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                 GUPNP_TYPE_DIDL_LITE_RESOURCE))
#define GUPNP_IS_DIDL_LITE_RESOURCE_CLASS(obj) \
                (G_TYPE_CHECK_CLASS_TYPE ((obj), \
                 GUPNP_TYPE_DIDL_LITE_RESOURCE))
#define GUPNP_DIDL_LITE_RESOURCE_GET_CLASS(obj) \
                (G_TYPE_INSTANCE_GET_CLASS ((obj), \
                 GUPNP_TYPE_DIDL_LITE_RESOURCE, \
                 GUPnPDIDLLiteResourceClass))

typedef struct _GUPnPDIDLLiteResourcePrivate GUPnPDIDLLiteResourcePrivate;

typedef struct {
        GObject parent;

        GUPnPDIDLLiteResourcePrivate *priv;
} GUPnPDIDLLiteResource;

typedef struct {
        GObjectClass parent_class;

        /* future padding */
        void (* _gupnp_reserved1) (void);
        void (* _gupnp_reserved2) (void);
        void (* _gupnp_reserved3) (void);
        void (* _gupnp_reserved4) (void);
} GUPnPDIDLLiteResourceClass;

GUPnPDIDLLiteResource *
gupnp_didl_lite_resource_new            (const char *uri);

GUPnPDIDLLiteResource *
gupnp_didl_lite_resource_new_from_xml   (xmlNode *res_node);

gboolean
gupnp_didl_lite_resource_protocol_info_compatible
                                        (GUPnPDIDLLiteResource *resource,
                                         const char            *protocol_info);

void
gupnp_didl_lite_resource_set_uri        (GUPnPDIDLLiteResource *resource,
                                         const char            *uri);

void
gupnp_didl_lite_resource_set_import_uri (GUPnPDIDLLiteResource *resource,
                                         const char            *import_uri);

void
gupnp_didl_lite_resource_set_protocol   (GUPnPDIDLLiteResource *resource,
                                         const char            *protocol);

void
gupnp_didl_lite_resource_set_network    (GUPnPDIDLLiteResource *resource,
                                         const char            *network);

void
gupnp_didl_lite_resource_set_mime_type  (GUPnPDIDLLiteResource *resource,
                                        const char            *mime_type);

void
gupnp_didl_lite_resource_set_dlna_profile
                                        (GUPnPDIDLLiteResource *resource,
                                         const char            *profile);

void
gupnp_didl_lite_resource_set_play_speeds
                                        (GUPnPDIDLLiteResource *resource,
                                         const char           **speeds);

void
gupnp_didl_lite_resource_set_dlna_conversion
                                        (GUPnPDIDLLiteResource *resource,
                                         GUPnPDLNAConversion    conversion);

void
gupnp_didl_lite_resource_set_dlna_operation
                                        (GUPnPDIDLLiteResource *resource,
                                         GUPnPDLNAOperation     operation);

void
gupnp_didl_lite_resource_set_dlna_flags (GUPnPDIDLLiteResource *resource,
                                         GUPnPDLNAFlags         flags);

void
gupnp_didl_lite_resource_set_size       (GUPnPDIDLLiteResource *resource,
                                         long                   size);

void
gupnp_didl_lite_resource_set_duration   (GUPnPDIDLLiteResource *resource,
                                         long                   duration);

void
gupnp_didl_lite_resource_set_bitrate    (GUPnPDIDLLiteResource *resource,
                                         int                    bitrate);

void
gupnp_didl_lite_resource_set_sample_freq
                                        (GUPnPDIDLLiteResource *resource,
                                         int                    sample_freq);

void
gupnp_didl_lite_resource_set_bits_per_sample
                                        (GUPnPDIDLLiteResource *resource,
                                         int                    sample_size);

void
gupnp_didl_lite_resource_set_protection (GUPnPDIDLLiteResource *resource,
                                         const char            *protection);

void
gupnp_didl_lite_resource_set_audio_channels
                                        (GUPnPDIDLLiteResource *resource,
                                         int                    n_channels);

void
gupnp_didl_lite_resource_set_width      (GUPnPDIDLLiteResource *resource,
                                         int                   width);

void
gupnp_didl_lite_resource_set_height     (GUPnPDIDLLiteResource *resource,
                                         int                    height);

void
gupnp_didl_lite_resource_set_color_depth
                                        (GUPnPDIDLLiteResource *resource,
                                         int                    color_depth);

const char *
gupnp_didl_lite_resource_get_uri        (GUPnPDIDLLiteResource *resource);

const char *
gupnp_didl_lite_resource_get_import_uri (GUPnPDIDLLiteResource *resource);

const char *
gupnp_didl_lite_resource_get_protocol   (GUPnPDIDLLiteResource *resource);

const char *
gupnp_didl_lite_resource_get_network    (GUPnPDIDLLiteResource *resource);

const char *
gupnp_didl_lite_resource_get_mime_type  (GUPnPDIDLLiteResource *resource);

const char *
gupnp_didl_lite_resource_get_dlna_profile
                                        (GUPnPDIDLLiteResource *resource);

const char **
gupnp_didl_lite_resource_get_play_speeds
                                        (GUPnPDIDLLiteResource *resource);

GUPnPDLNAConversion
gupnp_didl_lite_resource_get_dlna_conversion
                                        (GUPnPDIDLLiteResource *resource);

GUPnPDLNAOperation
gupnp_didl_lite_resource_get_dlna_operation
                                        (GUPnPDIDLLiteResource *resource);

GUPnPDLNAFlags
gupnp_didl_lite_resource_get_dlna_flags (GUPnPDIDLLiteResource *resource);

long
gupnp_didl_lite_resource_get_size       (GUPnPDIDLLiteResource *resource);

long
gupnp_didl_lite_resource_get_duration   (GUPnPDIDLLiteResource *resource);

int
gupnp_didl_lite_resource_get_bitrate    (GUPnPDIDLLiteResource *resource);

int
gupnp_didl_lite_resource_get_sample_freq
                                        (GUPnPDIDLLiteResource *resource);

int
gupnp_didl_lite_resource_get_bits_per_sample
                                        (GUPnPDIDLLiteResource *resource);

const char *
gupnp_didl_lite_resource_get_protection (GUPnPDIDLLiteResource *resource);

int
gupnp_didl_lite_resource_get_audio_channels
                                        (GUPnPDIDLLiteResource *resource);

int
gupnp_didl_lite_resource_get_width      (GUPnPDIDLLiteResource *resource);

int
gupnp_didl_lite_resource_get_height     (GUPnPDIDLLiteResource *resource);

int
gupnp_didl_lite_resource_get_color_depth
                                        (GUPnPDIDLLiteResource *resource);

G_END_DECLS

#endif /* __GUPNP_DIDL_LITE_RESOURCE_H__ */
