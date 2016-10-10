/*
 * Copyright (C) 2009 Nokia Corporation.
 *
 * Authors: Zeeshan Ali (Khattak) <zeeshan.ali@nokia.com>
 *                                <zeeshanak@gnome.org>
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

#ifndef __GUPNP_DIDL_LITE_RESOURCE_PRIVATE_H__
#define __GUPNP_DIDL_LITE_RESOURCE_PRIVATE_H__

#include "xml-util.h"

#include <stdarg.h>
#include <glib-object.h>
#include <libxml/tree.h>

G_BEGIN_DECLS

GUPnPDIDLLiteResource *
gupnp_didl_lite_resource_new_from_xml   (xmlNode     *xml_node,
                                         GUPnPAVXMLDoc *xml_doc,
                                         xmlNs       *dlna_ns,
                                         xmlNs       *pv_ns);

G_END_DECLS

#endif /* __GUPNP_DIDL_LITE_RESOURCE_PRIVATE_H__ */
