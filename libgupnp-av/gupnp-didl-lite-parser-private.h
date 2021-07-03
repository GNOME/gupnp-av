/*
 * Copyright (C) 2012 Intel Corporation.
 *
 * Authors: Jens Georg <jensg@openismus.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */

#ifndef GUPNP_DIDL_LITE_PARSER_PRIVATE_H
#define GUPNP_DIDL_LITE_PARSER_PRIVATE_H

#include "gupnp-didl-lite-parser.h"

G_BEGIN_DECLS

G_GNUC_INTERNAL gboolean
gupnp_didl_lite_parser_parse_didl_recursive (GUPnPDIDLLiteParser *parser,
                                             const char          *didl,
                                             gboolean             recursive,
                                             GError             **error);
G_END_DECLS

#endif /* __GUPNP_DIDL_LITE_PARSER_PRIVATE_H__ */
