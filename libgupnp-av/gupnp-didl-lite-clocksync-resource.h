// SPDX-License-Identifier: LGPL-2.1-or-later
#pragma once

#include "gupnp-didl-lite-extended-resource.h"

#include <glib-object.h>
#include <glib.h>

G_BEGIN_DECLS

G_DECLARE_FINAL_TYPE (GUPnPDIDLLiteClocksyncResource,
                      gupnp_didl_lite_clocksync_resource,
                      GUPNP,
                      DIDL_LITE_CLOCKSYNC_RESOURCE,
                      GUPnPDIDLLiteExtendedResource)

#define GUPNP_TYPE_DIDL_LITE_CLOCKSYNC_RESOURCE \
                (gupnp_didl_lite_clocksync_resource_get_type ())

const char *
gupnp_didl_lite_clocksync_resource_get_device_clock_id (GUPnPDIDLLiteClocksyncResource *self);

void
gupnp_didl_lite_clocksync_resource_set_device_clock_id (GUPnPDIDLLiteClocksyncResource *self, const char *id);

const char *
gupnp_didl_lite_clocksync_resource_get_timestamps_id (GUPnPDIDLLiteClocksyncResource *self);

void
gupnp_didl_lite_clocksync_resource_set_timestamps_id (GUPnPDIDLLiteClocksyncResource *self, const char *id);

G_END_DECLS
