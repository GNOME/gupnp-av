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

#include <config.h>

#include "gupnp-av-error.h"

/**
 * GUPNP_PROTOCOL_ERROR:
 *
 * The #GQuark uniquely used by GUPnP AV protocol related errors.
 *
 * Returns: a #GQuark uniquely used by GUPnP AV protocol related errors.
 **/
GQuark
gupnp_protocol_error_quark (void)
{
        static GQuark quark = 0;

        if (!quark)
                quark = g_quark_from_static_string ("gupnp-protocol-error");

        return quark;
}
