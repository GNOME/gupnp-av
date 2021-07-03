/*
 * Copyright (C) 2012 Intel Corporation.
 *
 * Authors: Jens Georg <jensg@openismus.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */

#ifndef TIME_UTILS_H
#define TIME_UTILS_H

G_BEGIN_DECLS

G_GNUC_INTERNAL long
seconds_from_time (const char *time_string);

G_GNUC_INTERNAL char *
seconds_to_time (long seconds);

G_END_DECLS

#endif /* __TIME_UTILS_H__ */
