/*
 * Copyright (C) 2007 OpenedHand Ltd
 *
 * Authors: Jorn Baayen <jorn@openedhand.com>
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
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include <string.h>

#include <libgupnp-av/gupnp-av-search-criteria-scanner.h>

/* XXX operator precedence */

static gboolean
scan_rel_exp (GScanner *scanner)
{
        GTokenValue value;
        gboolean ret;
        guint token;
        char *arg1, *arg2;

        token = g_scanner_get_next_token (scanner);
        g_assert (token == G_TOKEN_IDENTIFIER); /* Already checked */

        value = g_scanner_cur_value (scanner);
        arg1 = g_strdup (value.v_string);

        token = g_scanner_get_next_token (scanner);
        switch (token) {
        case GUPNP_AV_SEARCH_CRITERIA_SYMBOL_EQ:
        case GUPNP_AV_SEARCH_CRITERIA_SYMBOL_NEQ:
        case GUPNP_AV_SEARCH_CRITERIA_SYMBOL_LESS:
        case GUPNP_AV_SEARCH_CRITERIA_SYMBOL_LEQ:
        case GUPNP_AV_SEARCH_CRITERIA_SYMBOL_GREATER:
        case GUPNP_AV_SEARCH_CRITERIA_SYMBOL_GEQ:
        case GUPNP_AV_SEARCH_CRITERIA_SYMBOL_CONTAINS:
        case GUPNP_AV_SEARCH_CRITERIA_SYMBOL_DOES_NOT_CONTAIN:
        case GUPNP_AV_SEARCH_CRITERIA_SYMBOL_DERIVED_FROM:
                token = g_scanner_get_next_token (scanner);
                if (token != G_TOKEN_STRING) {
                        g_scanner_unexp_token (scanner,
                                               G_TOKEN_STRING,
                                               NULL, NULL, NULL,
                                               NULL, FALSE);

                        ret = FALSE;
                }

                value = g_scanner_cur_value (scanner);
                arg2 = value.v_string;

                ret = TRUE;

                break;

        case GUPNP_AV_SEARCH_CRITERIA_SYMBOL_EXISTS:
                token = g_scanner_get_next_token (scanner);
                switch (token) {
                case GUPNP_AV_SEARCH_CRITERIA_SYMBOL_TRUE:
                        ret = TRUE;

                        break;

                case GUPNP_AV_SEARCH_CRITERIA_SYMBOL_FALSE:
                        ret = TRUE;

                        break;

                default:
                        g_scanner_unexp_token (scanner,
                                               G_TOKEN_IDENTIFIER,
                                               NULL, NULL, NULL,
                                               "Expected boolean value",
                                               FALSE);

                        ret = FALSE;
                }

                break;

        default:
                g_scanner_unexp_token (scanner,
                                       G_TOKEN_IDENTIFIER,
                                       NULL, NULL, NULL,
                                       "Expected operator",
                                       FALSE);

                ret = FALSE;
        }

        g_free (arg1);

        return ret;
}

static gboolean
scan_search_exp (GScanner *scanner)
{
        gboolean ret;
        guint token;

        token = g_scanner_peek_next_token (scanner);
        switch (token) {
        case G_TOKEN_LEFT_PAREN:
                g_scanner_get_next_token (scanner);

                ret = scan_search_exp (scanner);
                if (ret == FALSE)
                        break;

                token = g_scanner_get_next_token (scanner);
                if (token != G_TOKEN_RIGHT_PAREN) {
                        g_scanner_unexp_token (scanner,
                                               G_TOKEN_RIGHT_PAREN,
                                               NULL, NULL, NULL, NULL, FALSE);

                        ret = FALSE;
                }

                break;

        case G_TOKEN_IDENTIFIER:
                ret = scan_rel_exp (scanner);
                if (ret == FALSE)
                        break;

                token = g_scanner_peek_next_token (scanner);

                switch (token) {
                case GUPNP_AV_SEARCH_CRITERIA_SYMBOL_AND:
                case GUPNP_AV_SEARCH_CRITERIA_SYMBOL_OR:
                        g_scanner_get_next_token (scanner);
                        ret = scan_search_exp (scanner);
                        break;

                default:
                        break;
                }

                break;

        default:
                g_scanner_get_next_token (scanner);

                g_scanner_unexp_token (scanner,
                                       G_TOKEN_IDENTIFIER,
                                       NULL, NULL, NULL, NULL, FALSE);

                ret = FALSE;
        }

        return ret;
}

int
main (int argc, char **argv)
{
        GScanner *scanner;
        gboolean ret;
        guint token;

        g_assert (argc == 2);

        scanner = gupnp_av_search_criteria_scanner_new ();

        g_scanner_input_text (scanner, argv[1], strlen (argv[1]));

        token = g_scanner_peek_next_token (scanner);
        if (token == GUPNP_AV_SEARCH_CRITERIA_SYMBOL_ASTERISK) {
                g_scanner_get_next_token (scanner);

                ret = TRUE;
        } else
                ret = scan_search_exp (scanner);

        if (ret == TRUE) {
                token = g_scanner_get_next_token (scanner);
                if (token != G_TOKEN_EOF) {
                        g_scanner_unexp_token (scanner,
                                               G_TOKEN_EOF,
                                               NULL, NULL, NULL, NULL, FALSE);
                }
        }

        g_scanner_destroy (scanner);

        return 0;
}
