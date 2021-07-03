/*
 * Copyright (C) 2009 Nokia Corporation.
 * Copyright (C) 2007, 2008 OpenedHand Ltd.
 * Copyright (C) 2012 Intel Corporation
 *
 * Authors: Zeeshan Ali (Khattak) <zeeshan.ali@nokia.com>
 *                                <zeeshanak@gnome.org>
 *          Jorn Baayen <jorn@openedhand.com>
 *          Krzesimir Nowak <krnowak@openismus.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */

#ifndef GUPNP_DIDL_LITE_RESOURCE_H
#define GUPNP_DIDL_LITE_RESOURCE_H

#include <stdarg.h>
#include <glib-object.h>
#include <glib.h>
#include <libxml/tree.h>

#include "gupnp-dlna.h"
#include "gupnp-protocol-info.h"

G_BEGIN_DECLS


G_DECLARE_DERIVABLE_TYPE(GUPnPDIDLLiteResource,
                          gupnp_didl_lite_resource,
                          GUPNP,
                          DIDL_LITE_RESOURCE,
                          GObject)

#define GUPNP_TYPE_DIDL_LITE_RESOURCE \
                (gupnp_didl_lite_resource_get_type ())


struct _GUPnPDIDLLiteResourceClass {
        GObjectClass parent_class;

        /* future padding */
        void (* _gupnp_reserved1) (void);
        void (* _gupnp_reserved2) (void);
        void (* _gupnp_reserved3) (void);
        void (* _gupnp_reserved4) (void);
};

void
gupnp_didl_lite_resource_set_uri        (GUPnPDIDLLiteResource *resource,
                                         const char            *uri);

void
gupnp_didl_lite_resource_set_import_uri (GUPnPDIDLLiteResource *resource,
                                         const char            *import_uri);

void
gupnp_didl_lite_resource_set_protocol_info
                                        (GUPnPDIDLLiteResource *resource,
                                         GUPnPProtocolInfo     *info);

void
gupnp_didl_lite_resource_set_size       (GUPnPDIDLLiteResource *resource,
                                         glong                  size);

void
gupnp_didl_lite_resource_set_size64     (GUPnPDIDLLiteResource *resource,
                                         gint64                 size);

void
gupnp_didl_lite_resource_set_cleartext_size
                                        (GUPnPDIDLLiteResource *resource,
                                         gint64                 cleartext_size);

void
gupnp_didl_lite_resource_set_duration   (GUPnPDIDLLiteResource *resource,
                                         glong                  duration);

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

void
gupnp_didl_lite_resource_set_update_count
                                        (GUPnPDIDLLiteResource *resource,
                                         guint                  update_count);

void
gupnp_didl_lite_resource_set_track_total
                                        (GUPnPDIDLLiteResource *resource,
                                         guint                  track_total);
void
gupnp_didl_lite_resource_unset_track_total
                                        (GUPnPDIDLLiteResource *resource);

void
gupnp_didl_lite_resource_unset_update_count
                                        (GUPnPDIDLLiteResource *resource);

void
gupnp_didl_lite_resource_set_subtitle_file_uri
                                        (GUPnPDIDLLiteResource *resource,
                                         const char            *uri);

void
gupnp_didl_lite_resource_set_subtitle_file_type
                                        (GUPnPDIDLLiteResource *resource,
                                         const char            *type);

xmlNode *
gupnp_didl_lite_resource_get_xml_node   (GUPnPDIDLLiteResource *resource);

xmlNsPtr
gupnp_didl_lite_resource_get_dlna_namespace
                                        (GUPnPDIDLLiteResource *resource);

xmlNsPtr
gupnp_didl_lite_resource_get_pv_namespace
                                        (GUPnPDIDLLiteResource *resource);

const char *
gupnp_didl_lite_resource_get_uri        (GUPnPDIDLLiteResource *resource);

const char *
gupnp_didl_lite_resource_get_import_uri (GUPnPDIDLLiteResource *resource);

GUPnPProtocolInfo *
gupnp_didl_lite_resource_get_protocol_info
                                        (GUPnPDIDLLiteResource *resource);

long
gupnp_didl_lite_resource_get_size       (GUPnPDIDLLiteResource *resource);

gint64
gupnp_didl_lite_resource_get_size64     (GUPnPDIDLLiteResource *resource);

gint64
gupnp_didl_lite_resource_get_cleartext_size
                                        (GUPnPDIDLLiteResource *resource);

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

guint
gupnp_didl_lite_resource_get_update_count
                                        (GUPnPDIDLLiteResource *resource);

gboolean
gupnp_didl_lite_resource_update_count_is_set
                                        (GUPnPDIDLLiteResource *resource);

guint
gupnp_didl_lite_resource_get_track_total
                                        (GUPnPDIDLLiteResource *resource);

gboolean
gupnp_didl_lite_resource_track_total_is_set
                                        (GUPnPDIDLLiteResource *resource);

const char *
gupnp_didl_lite_resource_get_subtitle_file_uri
                                        (GUPnPDIDLLiteResource *resource);

const char *
gupnp_didl_lite_resource_get_subtitle_file_type
                                        (GUPnPDIDLLiteResource *resource);
G_END_DECLS

#endif /* __GUPNP_DIDL_LITE_RESOURCE_H__ */
