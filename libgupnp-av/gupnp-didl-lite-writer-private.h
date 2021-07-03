/*
 * Copyright (C) 2012 Intel Corporation.
 *
 * Authors: Jens Georg <jensg@openismus.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */

#ifndef GUPNP_DIDL_LITE_WRITER_PRIVATE_H
#define GUPNP_DIDL_LITE_WRITER_PRIVATE_H

#include "gupnp-didl-lite-writer.h"
#include "gupnp-didl-lite-container.h"

G_BEGIN_DECLS

G_GNUC_INTERNAL GUPnPDIDLLiteItem *
gupnp_didl_lite_writer_add_container_child_item
                                        (GUPnPDIDLLiteWriter    *writer,
                                         GUPnPDIDLLiteContainer *container);

G_GNUC_INTERNAL void
gupnp_didl_lite_writer_filter_tags      (GUPnPDIDLLiteWriter   *writer,
                                         const char            *filter);

G_END_DECLS

#endif /* __GUPNP_DIDL_LITE_WRITER_PRIVATE_H__ */
