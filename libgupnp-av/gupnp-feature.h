/*
 * Copyright (C) 2012 Intel Corporation.
 *
 * Authors: Regis Merlino <regis.merlino@intel.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */

#ifndef GUPNP_FEATURE_H
#define GUPNP_FEATURE_H

#include <glib-object.h>

G_BEGIN_DECLS

G_DECLARE_DERIVABLE_TYPE(GUPnPFeature,
                          gupnp_feature,
                          GUPNP,
                          FEATURE,
                          GObject)

#define GUPNP_TYPE_FEATURE (gupnp_feature_get_type ())

struct _GUPnPFeatureClass {
        GObjectClass parent_class;
};

const char *
gupnp_feature_get_name       (GUPnPFeature *feature);

const char *
gupnp_feature_get_version    (GUPnPFeature *feature);

const char *
gupnp_feature_get_object_ids (GUPnPFeature *feature);

G_END_DECLS

#endif /* GUPNP_FEATURE_H */
