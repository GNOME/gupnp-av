/*
 * Copyright (C) 2012 Intel Corporation
 *
 * Authors: Krzesimir Nowak <krnowak@openismus.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */

#ifndef GUPNP_AV_ENUMS_H
#define GUPNP_AV_ENUMS_H

#include <glib.h>

G_BEGIN_DECLS

/**
 * GUPnPDIDLLiteFragmentResult:
 * @GUPNP_DIDL_LITE_FRAGMENT_RESULT_OK: Operation succeeded.
 * @GUPNP_DIDL_LITE_FRAGMENT_RESULT_CURRENT_BAD_XML: Current set of fragments
 * is bad XML
 * @GUPNP_DIDL_LITE_FRAGMENT_RESULT_NEW_BAD_XML: New set of fragments is bad
 * XML
 * @GUPNP_DIDL_LITE_FRAGMENT_RESULT_CURRENT_INVALID: Current set of fragments
 * is invalid
 * @GUPNP_DIDL_LITE_FRAGMENT_RESULT_NEW_INVALID: New set of fragments is
 * invalid
 * @GUPNP_DIDL_LITE_FRAGMENT_RESULT_REQUIRED_TAG: Trying to remove a required
 * tag
 * @GUPNP_DIDL_LITE_FRAGMENT_RESULT_READONLY_TAG: Trying to modify or remove a
 * read-only tag
 * @GUPNP_DIDL_LITE_FRAGMENT_RESULT_MISMATCH: Length of the two fragment sets
 * does not match
 * @GUPNP_DIDL_LITE_FRAGMENT_RESULT_UNKNOWN_ERROR: General error sink if none
 * of the others applies.
 *
 * Possible return values of the gupnp_didl_lite_object_apply_fragments() call.
 **/
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
