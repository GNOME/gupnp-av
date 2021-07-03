/*
 * Copyright (C) 2012 Intel Corporation.
 *
 * Authors: Regis Merlino <regis.merlino@intel.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */

#ifndef GUPNP_FEATURE_LIST_PARSER_H
#define GUPNP_FEATURE_LIST_PARSER_H

#include <glib-object.h>
#include "gupnp-feature.h"

G_BEGIN_DECLS

G_DECLARE_FINAL_TYPE (GUPnPFeatureListParser,
                      gupnp_feature_list_parser,
                      GUPNP,
                      FEATURE_LIST_PARSER,
                      GObject)

#define GUPNP_TYPE_FEATURE_LIST_PARSER (gupnp_feature_list_parser_get_type ())

GUPnPFeatureListParser *
gupnp_feature_list_parser_new        (void);

GList *
gupnp_feature_list_parser_parse_text (GUPnPFeatureListParser *parser,
                                      const char             *text,
                                      GError                 **error);

G_END_DECLS

#endif /* __GUPNP_FEATURE_LIST_PARSER_H_ */
