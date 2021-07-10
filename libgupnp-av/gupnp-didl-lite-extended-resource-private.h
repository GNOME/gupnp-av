// SPDX-License-Identifier: LGPL-2.1-or-later
#pragma once

#include "gupnp-didl-lite-extended-resource.h"

#include <glib-object.h>
#include <glib.h>

#include <libxml/tree.h>

G_BEGIN_DECLS

G_GNUC_INTERNAL xmlNode *
gupnp_didl_lite_extended_resource_get_xml_node (
        GUPnPDIDLLiteExtendedResource *self);

G_END_DECLS
