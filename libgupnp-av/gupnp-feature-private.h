// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: 2022 Jens Georg <mail@jensge.org>

#pragma once

#include "gupnp-feature.h"
#include "xml-util.h"

G_BEGIN_DECLS

G_GNUC_INTERNAL
GUPnPAVXMLDoc *
gupnp_feature_get_doc (GUPnPFeature *feature);

G_GNUC_INTERNAL
xmlNode *
gupnp_feature_get_node (GUPnPFeature *feature);

G_END_DECLS
