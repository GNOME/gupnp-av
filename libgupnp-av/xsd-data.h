/*
 * Copyright (C) 2012 Intel Corporation
 *
 * Authors: Krzesimir Nowak <krnowak@openismus.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */

#ifndef XSD_DATA_H
#define XSD_DATA_H

#include <glib.h>
#include <libxml/tree.h>

G_BEGIN_DECLS

typedef struct _XSDData XSDData;

G_GNUC_INTERNAL XSDData *
xsd_data_new                            (const gchar *xsd_file);

G_GNUC_INTERNAL void
xsd_data_free                           (XSDData *data);

G_GNUC_INTERNAL gboolean
xsd_data_validate_doc                   (XSDData *data,
                                         xmlDoc  *doc);

G_END_DECLS

#endif /* __XSD_DATA_H__ */
