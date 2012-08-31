/*
 * Copyright (C) 2009, Nokia Corporation.
 * Copyright (C) 2006, 2007 OpenedHand Ltd.
 *
 * Authors: Zeeshan Ali (Khattak) <zeeshanak@gnome.org>
 *                                <zeeshan.ali@nokia.com>
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "gupnp-av-error.h"

/**
 * SECTION:gupnp-av-error
 * @short_description: Error domains and codes.
 */

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
