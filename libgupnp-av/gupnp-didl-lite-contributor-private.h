/*
 * Copyright (C) 2009 Nokia Corporation.
 *
 * Authors: Zeeshan Ali (Khattak) <zeeshan.ali@nokia.com>
 *                                <zeeshanak@gnome.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */

#ifndef GUPNP_DIDL_LITE_CONTRIBUTOR_PRIVATE_H
#define GUPNP_DIDL_LITE_CONTRIBUTOR_PRIVATE_H

#include "xml-util.h"

#include <glib-object.h>
#include <libxml/tree.h>

#include "gupnp-didl-lite-contributor.h"

G_BEGIN_DECLS

G_GNUC_INTERNAL GUPnPDIDLLiteContributor *
gupnp_didl_lite_contributor_new_from_xml (xmlNode     *xml_node,
                                          GUPnPAVXMLDoc *xml_doc);

G_END_DECLS

#endif /* __GUPNP_DIDL_LITE_CONTRIBUTOR_PRIVATE_H__ */
