/*
 * Copyright (C) 2009 Nokia Corporation.
 * Copyright (C) 2007, 2008 OpenedHand Ltd.
 *
 * Authors: Zeeshan Ali (Khattak) <zeeshan.ali@nokia.com>
 *                                <zeeshanak@gnome.org>
 *          Jorn Baayen <jorn@openedhand.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */

#ifndef GUPNP_PROTOCOL_INFO_H
#define GUPNP_PROTOCOL_INFO_H

#include <stdarg.h>
#include <glib-object.h>

#include "gupnp-dlna.h"

G_BEGIN_DECLS

G_DECLARE_DERIVABLE_TYPE (GUPnPProtocolInfo,
                          gupnp_protocol_info,
                          GUPNP,
                          PROTOCOL_INFO,
                          GObject)

#define GUPNP_TYPE_PROTOCOL_INFO (gupnp_protocol_info_get_type ())

struct _GUPnPProtocolInfoClass {
        GObjectClass parent_class;

        /* future padding */
        void (* _gupnp_reserved1) (void);
        void (* _gupnp_reserved2) (void);
        void (* _gupnp_reserved3) (void);
        void (* _gupnp_reserved4) (void);
};

GUPnPProtocolInfo *
gupnp_protocol_info_new                 (void);

GUPnPProtocolInfo *
gupnp_protocol_info_new_from_string     (const char        *protocol_info,
                                         GError           **error);

char *
gupnp_protocol_info_to_string           (GUPnPProtocolInfo *info);

gboolean
gupnp_protocol_info_is_compatible       (GUPnPProtocolInfo *info1,
                                         GUPnPProtocolInfo *info2);

void
gupnp_protocol_info_set_protocol        (GUPnPProtocolInfo *info,
                                         const char        *protocol);

void
gupnp_protocol_info_set_network         (GUPnPProtocolInfo *info,
                                         const char        *network);

void
gupnp_protocol_info_set_mime_type       (GUPnPProtocolInfo *info,
                                         const char        *mime_type);

void
gupnp_protocol_info_set_dlna_profile    (GUPnPProtocolInfo *info,
                                         const char        *profile);

void
gupnp_protocol_info_set_play_speeds     (GUPnPProtocolInfo *info,
                                         const char       **speeds);

void
gupnp_protocol_info_set_dlna_conversion (GUPnPProtocolInfo  *info,
                                         GUPnPDLNAConversion conversion);

void
gupnp_protocol_info_set_dlna_operation  (GUPnPProtocolInfo *info,
                                         GUPnPDLNAOperation operation);

void
gupnp_protocol_info_set_dlna_flags      (GUPnPProtocolInfo *info,
                                         GUPnPDLNAFlags     flags);

const char *
gupnp_protocol_info_get_protocol        (GUPnPProtocolInfo *info);

const char *
gupnp_protocol_info_get_network         (GUPnPProtocolInfo *info);

const char *
gupnp_protocol_info_get_mime_type       (GUPnPProtocolInfo *info);

const char *
gupnp_protocol_info_get_dlna_profile    (GUPnPProtocolInfo *info);

const char **
gupnp_protocol_info_get_play_speeds     (GUPnPProtocolInfo *info);

GUPnPDLNAConversion
gupnp_protocol_info_get_dlna_conversion (GUPnPProtocolInfo *info);

GUPnPDLNAOperation
gupnp_protocol_info_get_dlna_operation  (GUPnPProtocolInfo *info);

GUPnPDLNAFlags
gupnp_protocol_info_get_dlna_flags      (GUPnPProtocolInfo *info);

G_END_DECLS

#endif /* GUPNP_PROTOCOL_INFO_H */
