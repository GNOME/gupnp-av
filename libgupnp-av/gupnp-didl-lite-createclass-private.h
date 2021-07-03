/*
 * Copyright (C) 2012 Intel Corporation.
 * Copyright (C) 2009 Nokia Corporation.
 *
 * Author: Christophe Guiraud <christophe.guiraud@intel.com>
 * Author: Zeeshan Ali (Khattak) <zeeshanak@gnome.org>
 *                               <zeeshan.ali@nokia.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */

#ifndef GUPNP_DIDL_LITE_CREATE_CLASS_PRIVATE_H
#define GUPNP_DIDL_LITE_CREATE_CLASS_PRIVATE_H

#include "xml-util.h"
#include "gupnp-didl-lite-createclass.h"

#include <glib-object.h>
#include <libxml/tree.h>

G_BEGIN_DECLS

G_GNUC_INTERNAL GUPnPDIDLLiteCreateClass *
gupnp_didl_lite_create_class_new_from_xml (xmlNode     *xml_node,
                                           GUPnPAVXMLDoc *xml_doc);

G_END_DECLS

#endif /* __GUPNP_DIDL_LITE_CREATE_CLASS_PRIVATE_H__ */
