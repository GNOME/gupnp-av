/*
 * Copyright (C) 2009, Nokia Corporation.
 * Copyright (C) 2006, 2007 OpenedHand Ltd.
 *
 * Authors: Zeeshan Ali (Khattak) <zeeshanak@gnome.org>
 *                                <zeeshan.ali@nokia.com>
 *          Jorn Baayen <jorn@openedhand.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */

#ifndef GUPNP_AV_ERROR_H
#define GUPNP_AV_ERROR_H

#include <glib.h>

G_BEGIN_DECLS

GQuark
gupnp_protocol_error_quark (void) G_GNUC_CONST;

#define GUPNP_PROTOCOL_ERROR (gupnp_protocol_error_quark ())

/**
 * GUPnPProtocolError:
 * @GUPNP_PROTOCOL_ERROR_INVALID_SYNTAX: Invalid syntax.
 * @GUPNP_PROTOCOL_ERROR_OTHER: Unknown/unhandled protocol related errors.
 *
 * #GError codes used for errors in the #GUPNP_PROTOCOL_ERROR domain, upon any
 * protocol related errors.
 */
typedef enum {
        GUPNP_PROTOCOL_ERROR_INVALID_SYNTAX,
        GUPNP_PROTOCOL_ERROR_OTHER
} GUPnPProtocolError;

G_END_DECLS

#endif /* __GUPNP_AV_ERROR_H__ */
