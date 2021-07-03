/*
 * Copyright (C) 2009 Nokia Corporation.
 * Copyright (C) 2012 Intel Corporation
 *
 * Authors: Zeeshan Ali (Khattak) <zeeshan.ali@nokia.com>
 *                                <zeeshanak@gnome.org>
 *          Krzesimir Nowak <krnowak@openismus.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */

#ifndef GUPNP_DIDL_LITE_OBJECT_PRIVATE_H
#define GUPNP_DIDL_LITE_OBJECT_PRIVATE_H

#include "xml-util.h"
#include "gupnp-didl-lite-object.h"

#include <glib-object.h>
#include <libxml/tree.h>

G_BEGIN_DECLS

G_GNUC_INTERNAL GUPnPDIDLLiteObject *
gupnp_didl_lite_object_new_from_xml     (xmlNode     *xml_node,
                                         GUPnPAVXMLDoc *xml_doc,
                                         xmlNs       *upnp_ns,
                                         xmlNs       *dc_ns,
                                         xmlNs       *dlna_ns,
                                         xmlNs       *pv_ns);

G_GNUC_INTERNAL GUPnPAVXMLDoc *
gupnp_didl_lite_object_get_gupnp_xml_doc
                                        (GUPnPDIDLLiteObject *object);

G_END_DECLS

#endif /* __GUPNP_DIDL_LITE_OBJECT_PRIVATE_H__ */
