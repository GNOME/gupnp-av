/*
 * Copyright (C) 2007 Zeeshan Ali <zeenix@gstreamer.net>
 * Copyright (C) 2007 OpenedHand Ltd
 *
 * Authors: Zeeshan Ali Khattak <zeenix@gstreamer.net>
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

#ifndef __GUPNP_AV_UTIL_H__
#define __GUPNP_AV_UTIL_H__

#include <libgupnp/gupnp.h>

G_BEGIN_DECLS

gboolean
gupnp_av_util_parse_last_change_valist (const char *last_change_xml,
                                        guint       instance_id,
                                        GError    **error,
                                        va_list     var_args);

gboolean
gupnp_av_util_parse_last_change        (const char *last_change_xml,
                                        guint       instance_id,
                                        GError    **error,
                                        ...) G_GNUC_NULL_TERMINATED;

G_END_DECLS

#endif /* __GUPNP_AV_UTIL_H__ */

