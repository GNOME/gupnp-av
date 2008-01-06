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

#ifndef __GUPNP_AV_SEARCH_CRITERIA_SCANNER_H__
#define __GUPNP_AV_SEARCH_CRITERIA_SCANNER_H__

#include <glib.h>

G_BEGIN_DECLS

typedef enum {
        GUPNP_AV_SEARCH_CRITERIA_SYMBOL_ASTERISK         = G_TOKEN_LAST + 1,

        GUPNP_AV_SEARCH_CRITERIA_SYMBOL_AND              = G_TOKEN_LAST + 2,
        GUPNP_AV_SEARCH_CRITERIA_SYMBOL_OR               = G_TOKEN_LAST + 3,

        GUPNP_AV_SEARCH_CRITERIA_SYMBOL_EQ               = G_TOKEN_LAST + 4,
        GUPNP_AV_SEARCH_CRITERIA_SYMBOL_NEQ              = G_TOKEN_LAST + 5,
        GUPNP_AV_SEARCH_CRITERIA_SYMBOL_LESS             = G_TOKEN_LAST + 6,
        GUPNP_AV_SEARCH_CRITERIA_SYMBOL_LEQ              = G_TOKEN_LAST + 7,
        GUPNP_AV_SEARCH_CRITERIA_SYMBOL_GREATER          = G_TOKEN_LAST + 8,
        GUPNP_AV_SEARCH_CRITERIA_SYMBOL_GEQ              = G_TOKEN_LAST + 9,

        GUPNP_AV_SEARCH_CRITERIA_SYMBOL_CONTAINS         = G_TOKEN_LAST + 10,
        GUPNP_AV_SEARCH_CRITERIA_SYMBOL_DOES_NOT_CONTAIN = G_TOKEN_LAST + 11,
        GUPNP_AV_SEARCH_CRITERIA_SYMBOL_DERIVED_FROM     = G_TOKEN_LAST + 12,

        GUPNP_AV_SEARCH_CRITERIA_SYMBOL_EXISTS           = G_TOKEN_LAST + 13,

        GUPNP_AV_SEARCH_CRITERIA_SYMBOL_TRUE             = G_TOKEN_LAST + 14,
        GUPNP_AV_SEARCH_CRITERIA_SYMBOL_FALSE            = G_TOKEN_LAST + 15
} GUPnPAVSearchCriteriaSymbol;

GScanner *
gupnp_av_search_criteria_scanner_new (void);

G_END_DECLS

#endif /* __GUPNP_AV_SEARCH_CRITERIA_SCANNER_H__ */
