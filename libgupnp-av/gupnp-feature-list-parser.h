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

#ifndef __GUPNP_FEATURE_LIST_PARSER_H_
#define __GUPNP_FEATURE_LIST_PARSER_H_

#include <glib-object.h>
#include "gupnp-feature.h"

G_BEGIN_DECLS

G_DECLARE_DERIVABLE_TYPE(GUPnPFeatureListParser,
                          gupnp_feature_list_parser,
                          GUPNP,
                          FEATURE_LIST_PARSER,
                          GObject)

#define GUPNP_TYPE_FEATURE_LIST_PARSER \
                (gupnp_feature_list_parser_get_type ())

struct _GUPnPFeatureListParserClass{
        GObjectClass parent_class;
};

GUPnPFeatureListParser *
gupnp_feature_list_parser_new        (void);

GList *
gupnp_feature_list_parser_parse_text (GUPnPFeatureListParser *parser,
                                      const char             *text,
                                      GError                 **error);

G_END_DECLS

#endif /* __GUPNP_FEATURE_LIST_PARSER_H_ */
