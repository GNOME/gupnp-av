/*
 * Copyright (C) 2012 Intel Corporation.
 *
 * Authors: Jens Georg <jensg@openismus.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */

#include <config.h>

#include <glib.h>

#include "time-utils.h"

#define SEC_PER_MIN 60
#define SEC_PER_HOUR 3600

long
seconds_from_time (const char *time_str)
{
        char **tokens;
        gdouble seconds = -1;

        if (time_str == NULL)
                return -1;

        tokens = g_strsplit (time_str, ":", -1);
        if (tokens[0] == NULL ||
            tokens[1] == NULL ||
            tokens[2] == NULL)
                goto return_point;

        seconds = g_strtod (tokens[2], NULL);
        seconds += g_strtod (tokens[1], NULL) * SEC_PER_MIN;
        seconds += g_strtod (tokens[0], NULL) * SEC_PER_HOUR;

return_point:
        g_strfreev (tokens);

        return (long) seconds;
}

char *
seconds_to_time (long seconds)
{
        char *str;

        if (seconds < 0)
                return NULL;

        str = g_strdup_printf ("%ld:%.2ld:%.2ld.000",
                               seconds / (60 * 60),
                               (seconds / 60) % 60,
                               seconds % 60);

        return str;
}
