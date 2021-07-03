/*
 * Copyright (C) 2007 OpenedHand Ltd.
 *
 * Author: Jorn Baayen <jorn@openedhand.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */

#ifndef GVALUE_UTIL_H
#define GVALUE_UTIL_H

#include <glib-object.h>

G_GNUC_INTERNAL gboolean
av_gvalue_util_set_value_from_string (GValue *value, const char *str);

#endif /* __GVALUE_UTIL_H__ */

