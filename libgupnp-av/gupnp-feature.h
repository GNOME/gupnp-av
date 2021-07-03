/*
 * Copyright (C) 2012 Intel Corporation.
 *
 * Authors: Regis Merlino <regis.merlino@intel.com>
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
