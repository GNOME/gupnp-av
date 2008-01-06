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

#include "gupnp-av-search-criteria-scanner.h"

#define NUM_SYMBOLS 15

struct {
        const char                 *name;
        GUPnPAVSearchCriteriaSymbol token;
} symbols[NUM_SYMBOLS] = {
        { "*",
          GUPNP_AV_SEARCH_CRITERIA_SYMBOL_ASTERISK },

        { "and",
          GUPNP_AV_SEARCH_CRITERIA_SYMBOL_AND },
        { "or",
          GUPNP_AV_SEARCH_CRITERIA_SYMBOL_OR },

        { "=",
          GUPNP_AV_SEARCH_CRITERIA_SYMBOL_EQ },
        { "!=",
          GUPNP_AV_SEARCH_CRITERIA_SYMBOL_NEQ },
        { "<",
          GUPNP_AV_SEARCH_CRITERIA_SYMBOL_LESS },
        { "<=",
          GUPNP_AV_SEARCH_CRITERIA_SYMBOL_LEQ },
        { ">",
          GUPNP_AV_SEARCH_CRITERIA_SYMBOL_GREATER },
        { ">=",
          GUPNP_AV_SEARCH_CRITERIA_SYMBOL_GEQ },

        { "contains",
          GUPNP_AV_SEARCH_CRITERIA_SYMBOL_CONTAINS },
        { "doesNotContain",
          GUPNP_AV_SEARCH_CRITERIA_SYMBOL_DOES_NOT_CONTAIN },
        { "derivedfrom",
          GUPNP_AV_SEARCH_CRITERIA_SYMBOL_DERIVED_FROM },

        { "exists",
          GUPNP_AV_SEARCH_CRITERIA_SYMBOL_EXISTS },

        { "true",
          GUPNP_AV_SEARCH_CRITERIA_SYMBOL_TRUE },
        { "false",
          GUPNP_AV_SEARCH_CRITERIA_SYMBOL_FALSE }
};

GScanner *
gupnp_av_search_criteria_scanner_new (void)
{
        GScanner *scanner;
        int i;

        scanner = g_scanner_new (NULL);

        scanner->config->cset_skip_characters  = " \t\n\r\012\013\014\015";
        scanner->config->scan_identifier_1char = TRUE;
        scanner->config->cset_identifier_first = G_CSET_a_2_z
                                                 "_*<>=!"
                                                 G_CSET_A_2_Z;
        scanner->config->symbol_2_token        = TRUE;

        /* Add symbols */
        for (i = 0; i < NUM_SYMBOLS; i++) {
                g_scanner_scope_add_symbol (scanner,
                                            0,
                                            symbols[i].name,
                                            GINT_TO_POINTER (symbols[i].token));
        }

        return scanner;
}
