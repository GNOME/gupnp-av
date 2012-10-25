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

#ifndef __GUPNP_AV_ENUMS_H__
#define __GUPNP_AV_ENUMS_H__

#include <glib.h>

G_BEGIN_DECLS

typedef enum {
      GUPNP_DIDL_LITE_FRAGMENT_RESULT_OK,
      GUPNP_DIDL_LITE_FRAGMENT_RESULT_CURRENT_BAD_XML,
      GUPNP_DIDL_LITE_FRAGMENT_RESULT_NEW_BAD_XML,
      GUPNP_DIDL_LITE_FRAGMENT_RESULT_CURRENT_INVALID,
      GUPNP_DIDL_LITE_FRAGMENT_RESULT_NEW_INVALID,
      GUPNP_DIDL_LITE_FRAGMENT_RESULT_REQUIRED_TAG,
      GUPNP_DIDL_LITE_FRAGMENT_RESULT_READONLY_TAG,
      GUPNP_DIDL_LITE_FRAGMENT_RESULT_MISMATCH,
      GUPNP_DIDL_LITE_FRAGMENT_RESULT_UNKNOWN_ERROR
} GUPnPDIDLLiteFragmentResult;

G_END_DECLS

#endif /* __GUPNP_AV_ENUMS_H__ */
