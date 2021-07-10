// SPDX-License-Identifier: LGPL-2.1-or-later
#pragma once

#include <glib-object.h>
#include <glib.h>

G_BEGIN_DECLS

G_DECLARE_DERIVABLE_TYPE (GUPnPDIDLLiteExtendedResource,
                          gupnp_didl_lite_extended_resource,
                          GUPNP,
                          DIDL_LITE_EXTENDED_RESOURCE,
                          GObject)

struct _GUPnPDIDLLiteExtendedResourceClass {
        GObjectClass parent_class;

        gpointer padding[12];
};

#define GUPNP_TYPE_DIDL_LITE_EXTENDED_RESOURCE \
                (gupnp_didl_lite_extended_resource_get_type ())

const char *
gupnp_didl_lite_extended_resource_get_id (
        GUPnPDIDLLiteExtendedResource *resource);

void
gupnp_didl_lite_extended_resource_set_id (
        GUPnPDIDLLiteExtendedResource *resource,
        const char *id);

G_END_DECLS

