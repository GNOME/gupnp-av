/*
 * Copyright (C) 2008 OpenedHand Ltd.
 *
 * Authors: Jorn Baayen <jorn@openedhand.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */

#include <config.h>

#include <libgupnp-av/gupnp-search-criteria-parser.h>
#include <stdlib.h>

static void
begin_parens_cb (G_GNUC_UNUSED GUPnPSearchCriteriaParser *parser,
                 G_GNUC_UNUSED gpointer                   user_data)
{
        g_print ("(");
}

static void
end_parens_cb (G_GNUC_UNUSED GUPnPSearchCriteriaParser *parser,
               G_GNUC_UNUSED gpointer                   user_data)
{
        g_print (")");
}

static void
conjunction_cb (G_GNUC_UNUSED GUPnPSearchCriteriaParser *parser,
                G_GNUC_UNUSED gpointer                   user_data)
{
        g_print (" and ");
}

static void
disjunction_cb (G_GNUC_UNUSED GUPnPSearchCriteriaParser *parser,
                G_GNUC_UNUSED gpointer                   user_data)
{
        g_print (" or ");
}

static gboolean
expression_cb (G_GNUC_UNUSED GUPnPSearchCriteriaParser *parser,
               const char                              *property,
               GUPnPSearchCriteriaOp                    op,
               const char                              *value,
               G_GNUC_UNUSED GError                   **error,
               G_GNUC_UNUSED gpointer                   user_data)
{
        g_print ("%s %d %s", property, op, value);

        return TRUE;
}

int
main (int argc, char **argv)
{
        GUPnPSearchCriteriaParser *parser;
        GError *error;

        g_assert (argc == 2);

        parser = gupnp_search_criteria_parser_new ();

        g_signal_connect (parser,
                          "begin_parens",
                          G_CALLBACK (begin_parens_cb),
                          NULL);
        g_signal_connect (parser,
                          "end_parens",
                          G_CALLBACK (end_parens_cb),
                          NULL);
        g_signal_connect (parser,
                          "conjunction",
                          G_CALLBACK (conjunction_cb),
                          NULL);
        g_signal_connect (parser,
                          "disjunction",
                          G_CALLBACK (disjunction_cb),
                          NULL);
        g_signal_connect (parser,
                          "expression",
                          G_CALLBACK (expression_cb),
                          NULL);

        error = NULL;
        gupnp_search_criteria_parser_parse_text (parser, argv[1], &error);
        if (error != NULL) {
                g_printerr ("Parse error: %s\n", error->message);
                g_error_free (error);
                return EXIT_FAILURE;
        }

        g_print ("\n");

        g_object_unref (parser);

        return EXIT_SUCCESS;
}
