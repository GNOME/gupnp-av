/*
 * Copyright (C) 2008 OpenedHand Ltd.
 *
 * Authors: Jorn Baayen <jorn@openedhand.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */

#ifndef GUPNP_SEARCH_CRITERIA_PARSER_H
#define GUPNP_SEARCH_CRITERIA_PARSER_H

#include <glib-object.h>

G_BEGIN_DECLS

G_DECLARE_DERIVABLE_TYPE(GUPnPSearchCriteriaParser,
                          gupnp_search_criteria_parser,
                          GUPNP,
                          SEARCH_CRITERIA_PARSER,
                          GObject)

#define GUPNP_TYPE_SEARCH_CRITERIA_PARSER                                      \
        (gupnp_search_criteria_parser_get_type ())

/**
 * GUPnPSearchCriteriaOp:
 * @GUPNP_SEARCH_CRITERIA_OP_EQ: '='
 * @GUPNP_SEARCH_CRITERIA_OP_NEQ: '!='
 * @GUPNP_SEARCH_CRITERIA_OP_LESS: '<'
 * @GUPNP_SEARCH_CRITERIA_OP_LEQ: '<='
 * @GUPNP_SEARCH_CRITERIA_OP_GREATER: '>'
 * @GUPNP_SEARCH_CRITERIA_OP_GEQ: '>='
 * @GUPNP_SEARCH_CRITERIA_OP_CONTAINS: 'contains'
 * @GUPNP_SEARCH_CRITERIA_OP_DOES_NOT_CONTAIN: 'doesNotContain'
 * @GUPNP_SEARCH_CRITERIA_OP_DERIVED_FROM: 'derivedFrom'
 * @GUPNP_SEARCH_CRITERIA_OP_EXISTS: 'exists'
 *
 * The possible operators in SearchCriteria strings.
 *
 **/
typedef enum {
        /* G_TYPE_STRING */
        GUPNP_SEARCH_CRITERIA_OP_EQ               = G_TOKEN_LAST + 1,
        GUPNP_SEARCH_CRITERIA_OP_NEQ              = G_TOKEN_LAST + 2,
        GUPNP_SEARCH_CRITERIA_OP_LESS             = G_TOKEN_LAST + 3,
        GUPNP_SEARCH_CRITERIA_OP_LEQ              = G_TOKEN_LAST + 4,
        GUPNP_SEARCH_CRITERIA_OP_GREATER          = G_TOKEN_LAST + 5,
        GUPNP_SEARCH_CRITERIA_OP_GEQ              = G_TOKEN_LAST + 6,
        GUPNP_SEARCH_CRITERIA_OP_CONTAINS         = G_TOKEN_LAST + 7,
        GUPNP_SEARCH_CRITERIA_OP_DOES_NOT_CONTAIN = G_TOKEN_LAST + 8,
        GUPNP_SEARCH_CRITERIA_OP_DERIVED_FROM     = G_TOKEN_LAST + 9,

        /* G_TYPE_BOOLEAN */
        GUPNP_SEARCH_CRITERIA_OP_EXISTS           = G_TOKEN_LAST + 10
} GUPnPSearchCriteriaOp;

GType
gupnp_search_criteria_op_get_type (void) G_GNUC_CONST;

#define GUPNP_TYPE_SEARCH_CRITERIA_OP (gupnp_search_criteria_op_get_type ())

#define GUPNP_SEARCH_CRITERIA_PARSER_ERROR                                     \
        (gupnp_search_criteria_parser_error_quark ())

GQuark
gupnp_search_criteria_parser_error_quark (void);

/**
 * GUPnPSearchCriteriaParserError:
 * @GUPNP_SEARCH_CRITERIA_PARSER_ERROR_FAILED: Parsing the search criteria
 * failed.
 **/
typedef enum {
        GUPNP_SEARCH_CRITERIA_PARSER_ERROR_FAILED
} GUPnPSearchCriteriaParserError;


struct _GUPnPSearchCriteriaParserClass {
        GObjectClass parent_class;

        /* signals */
        void     (* begin_parens) (GUPnPSearchCriteriaParser  *parser);
        void     (* end_parens)   (GUPnPSearchCriteriaParser  *parser);
        void     (* conjunction)  (GUPnPSearchCriteriaParser  *parser);
        void     (* disjunction)  (GUPnPSearchCriteriaParser  *parser);
        gboolean (* expression)   (GUPnPSearchCriteriaParser  *parser,
                                   const char                 *property,
                                   GUPnPSearchCriteriaOp       op,
                                   const char                 *value,
                                   GError                    **error);

        /* future padding */
        void (* _gupnp_reserved1) (void);
        void (* _gupnp_reserved2) (void);
        void (* _gupnp_reserved3) (void);
        void (* _gupnp_reserved4) (void);
};

GUPnPSearchCriteriaParser *
gupnp_search_criteria_parser_new        (void);

gboolean
gupnp_search_criteria_parser_parse_text (GUPnPSearchCriteriaParser *parser,
                                         const char                *text,
                                         GError                   **error);

G_END_DECLS

#endif /* GUPNP_SEARCH_CRITERIA_PARSER_H */
