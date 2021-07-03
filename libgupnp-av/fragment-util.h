/*
 * Copyright (C) 2012 Intel Corporation
 *
 * Authors: Krzesimir Nowak <krnowak@openismus.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */

#ifndef GUPNP_AV_FRAGMENT_UTIL_H
#define GUPNP_AV_FRAGMENT_UTIL_H

#include <glib.h>
#include <libxml/tree.h>

#include "xsd-data.h"
#include "gupnp-av-enums.h"

G_BEGIN_DECLS

typedef struct {
        xmlDocPtr doc;
        xmlNodePtr node;
} DocNode;

G_GNUC_INTERNAL XSDData *
fragment_util_get_didl_lite_xsd_data    (void);

G_GNUC_INTERNAL GUPnPDIDLLiteFragmentResult
fragment_util_check_fragments           (DocNode     *original,
                                         DocNode     *modified,
                                         const gchar *current_fragment,
                                         const gchar *new_fragment,
                                         XSDData     *xsd_data);

G_GNUC_INTERNAL gboolean
fragment_util_apply_modification        (xmlNodePtr *node_ptr,
                                         DocNode    *modified);

G_END_DECLS

#endif /* __FRAGMENT_UTIL_H__ */
