/*
 * Copyright (C) 2007 OpenedHand Ltd.
 *
 * Author: Jorn Baayen <jorn@openedhand.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */

#include <config.h>

#include <string.h>
#include <stdlib.h>

#include "gvalue-util.h"

gboolean
av_gvalue_util_set_value_from_string (GValue *value, const char *str)
{
        GValue tmp_value = {0, };
        int i;
        long l;
        double d;

        g_return_val_if_fail (str != NULL, FALSE);

        switch (G_VALUE_TYPE (value)) {
        case G_TYPE_STRING:
                g_value_set_string (value, str);

                break;

        case G_TYPE_CHAR:
                g_value_set_schar (value, *str);

                break;

        case G_TYPE_UCHAR:
                g_value_set_uchar (value, *str);

                break;

        case G_TYPE_INT:
                i = atoi (str);
                g_value_set_int (value, i);

                break;

        case G_TYPE_UINT:
                i = atoi (str);
                g_value_set_uint (value, (guint) i);

                break;

        case G_TYPE_INT64:
                i = atoi (str);
                g_value_set_int64 (value, (gint64) i);

                break;

        case G_TYPE_UINT64:
                i = atoi (str);
                g_value_set_uint64 (value, (guint64) i);

                break;

        case G_TYPE_LONG:
                l = atol (str);
                g_value_set_long (value, l);

                break;

        case G_TYPE_ULONG:
                l = atol (str);
                g_value_set_ulong (value, (gulong) l);

                break;

        case G_TYPE_FLOAT:
                d = atof (str);
                g_value_set_float (value, (float) d);

                break;

        case G_TYPE_DOUBLE:
                d = atof (str);
                g_value_set_float (value, d);

                break;

        case G_TYPE_BOOLEAN:
                if (g_ascii_strcasecmp (str, "true") == 0 ||
                    g_ascii_strcasecmp (str, "yes") == 0)
                        g_value_set_boolean (value, TRUE);
                else if (g_ascii_strcasecmp (str, "false") == 0 ||
                         g_ascii_strcasecmp (str, "no") == 0)
                        g_value_set_boolean (value, FALSE);
                else {
                        i = atoi (str);
                        g_value_set_boolean (value, i ? TRUE : FALSE);
                }

                break;

        default:
                /* Try to convert */
                if (g_value_type_transformable (G_TYPE_STRING,
                                                G_VALUE_TYPE (value))) {
                        g_value_init (&tmp_value, G_TYPE_STRING);
                        g_value_set_static_string (&tmp_value, str);

                        g_value_transform (&tmp_value, value);

                        g_value_unset (&tmp_value);

                } else if (g_value_type_transformable (G_TYPE_INT,
                                                       G_VALUE_TYPE (value))) {
                        i = atoi (str);

                        g_value_init (&tmp_value, G_TYPE_INT);
                        g_value_set_int (&tmp_value, i);

                        g_value_transform (&tmp_value, value);

                        g_value_unset (&tmp_value);

                } else {
                        g_warning ("Failed to transform integer "
                                   "value to type %s",
                                   G_VALUE_TYPE_NAME (value));

                        return FALSE;
                }

                break;
        }

        return TRUE;
}

