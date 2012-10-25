/*
 * Copyright (C) 2012 Intel Corporation
 *
 * Authors: Krzesimir Nowak <krnowak@openismus.com>
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

#ifndef __XSD_DATA_H__
#define __XSD_DATA_H__

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
