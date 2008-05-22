/*
 * Copyright (C) 2007, 2008 OpenedHand Ltd.
 *
 * Authors: Jorn Baayen <jorn@openedhand.com>
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

#ifndef __GUPNP_DLNA_H__
#define __GUPNP_DLNA_H__

#include <glib.h>

G_BEGIN_DECLS

typedef enum {
        GUPNP_DLNA_PLAY_SPEED_INVALID = 0,
        GUPNP_DLNA_PLAY_SPEED_NORMAL  = 1
} GUPnPDLNAPlaySpeed;

typedef enum {
        GUPNP_DLNA_CONVERSION_NONE       = 0,
        GUPNP_DLNA_CONVERSION_TRANSCODED = 1
} GUPnPDLNAConversion;

typedef enum {
        GUPNP_DLNA_OPERATION_NONE     = 0x00,
        GUPNP_DLNA_OPERATION_RANGE    = 0x01,
        GUPNP_DLNA_OPERATION_TIMESEEK = 0x10
} GUPnPDLNAOperation;

typedef enum {
        GUPNP_DLNA_FLAG_SENDER_PACED              = (1 << 31),
        GUPNP_DLNA_FLAG_TIME_BASED_SEEK           = (1 << 30),
        GUPNP_DLNA_FLAG_BYTE_BASED_SEEK           = (1 << 29),
        GUPNP_DLNA_FLAG_PLAY_CONTAINER            = (1 << 28),
        GUPNP_DLNA_FLAG_S0_INCREASE               = (1 << 27),
        GUPNP_DLNA_FLAG_SN_INCREASE               = (1 << 26),
        GUPNP_DLNA_FLAG_RTSP_PAUSE                = (1 << 25),
        GUPNP_DLNA_FLAG_STREAMING_TRANSFER_MODE   = (1 << 24),
        GUPNP_DLNA_FLAG_INTERACTIVE_TRANSFER_MODE = (1 << 23),
        GUPNP_DLNA_FLAG_BACKGROUND_TRANSFER_MODE  = (1 << 22),
        GUPNP_DLNA_FLAG_CONNECTION_STALL          = (1 << 21),
        GUPNP_DLNA_FLAG_DLNA_V15                  = (1 << 20)
} GUPnPDLNAFlags;

G_END_DECLS

#endif /* __GUPNP_DLNA_H__ */