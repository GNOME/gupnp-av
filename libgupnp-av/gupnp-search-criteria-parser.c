/*
 * Copyright (C) 2008 OpenedHand Ltd.
 *
 * Authors: Jorn Baayen <jorn@openedhand.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */

/**
 * GUPnPSearchCriteriaParser:
 *
 * A/V search criteria parser
 *
 * #GUPnPSearchCriteriaParser parses ContentDirectory search criteria
 * strings.
 *
 * Note that no signals will be emitted if a wildcard is specified,
 * and that the user is responsible for ensuring precedence of conjunction
 * over disjunction.
 */

#include <config.h>

#include <string.h>

#include "gupnp-search-criteria-parser.h"
#include "gupnp-av-marshal.h"

/* GType for GUPNPSearchCriteriaOp */
GType
gupnp_search_criteria_op_get_type (void)
{
        static GType type = 0;

        if (type == 0) {
                static const GEnumValue values[] = {
                        { GUPNP_SEARCH_CRITERIA_OP_EQ,
                          "GUPNP_SEARCH_CRITERIA_OP_EQ",
                          "EQ" },
                        { GUPNP_SEARCH_CRITERIA_OP_NEQ,
                          "GUPNP_SEARCH_CRITERIA_OP_NEQ",
                          "NEQ" },
                        { GUPNP_SEARCH_CRITERIA_OP_LESS,
                          "GUPNP_SEARCH_CRITERIA_OP_LESS",
                          "LESS" },
                        { GUPNP_SEARCH_CRITERIA_OP_LEQ,
                          "GUPNP_SEARCH_CRITERIA_OP_LEQ",
                          "LEQ" },
                        { GUPNP_SEARCH_CRITERIA_OP_GREATER,
                          "GUPNP_SEARCH_CRITERIA_OP_GREATER",
                          "GREATER" },
                        { GUPNP_SEARCH_CRITERIA_OP_GEQ,
                          "GUPNP_SEARCH_CRITERIA_OP_GEQ",
                          "GEQ" },
                        { GUPNP_SEARCH_CRITERIA_OP_CONTAINS,
                          "GUPNP_SEARCH_CRITERIA_OP_CONTAINS",
                          "CONTAINS" },
                        { GUPNP_SEARCH_CRITERIA_OP_DOES_NOT_CONTAIN,
                          "GUPNP_SEARCH_CRITERIA_OP_DOES_NOT_CONTAIN",
                          "DOES_NOT_CONTAIN" },
                        { GUPNP_SEARCH_CRITERIA_OP_DERIVED_FROM,
                          "GUPNP_SEARCH_CRITERIA_OP_DERIVED_FROM",
                          "DERIVED_FROM" },
                        { GUPNP_SEARCH_CRITERIA_OP_EXISTS,
                          "GUPNP_SEARCH_CRITERIA_OP_EXISTS",
                          "EXISTS" },
                        { 0, NULL, NULL }
                };

                type = g_enum_register_static
                                (g_intern_static_string (
                                 "GUPnPSearchCriteriaOp"),
                                 values);
        }

        return type;
}

/* GUPnPSearchCriteriaParserError */
GQuark
gupnp_search_criteria_parser_error_quark (void)
{
        return g_quark_from_static_string
                ("gupnp-search-criteria-parser-error-quark");
}

struct _GUPnPSearchCriteriaParserPrivate {
        GScanner *scanner;
};

typedef struct _GUPnPSearchCriteriaParserPrivate
        GUPnPSearchCriteriaParserPrivate;

/* GUPnPSearchCriteriaParser */
G_DEFINE_TYPE_WITH_PRIVATE (GUPnPSearchCriteriaParser,
                         gupnp_search_criteria_parser,
                         G_TYPE_OBJECT)

enum {
        BEGIN_PARENS,
        END_PARENS,
        CONJUNCTION,
        DISJUNCTION,
        EXPRESSION,
        SIGNAL_LAST
};

static guint signals[SIGNAL_LAST];

/* Additional parsable symbols */
enum {
        SYMBOL_ASTERISK = G_TOKEN_LAST + 11,
        SYMBOL_AND      = G_TOKEN_LAST + 12,
        SYMBOL_OR       = G_TOKEN_LAST + 13,
        SYMBOL_TRUE     = G_TOKEN_LAST + 14,
        SYMBOL_FALSE    = G_TOKEN_LAST + 15
};

#define NUM_SYMBOLS 15

struct {
        const char *name;
        int         token;
} symbols[NUM_SYMBOLS] = {
        { "*",
          SYMBOL_ASTERISK },

        { "and",
          SYMBOL_AND },
        { "or",
          SYMBOL_OR },

        { "=",
          GUPNP_SEARCH_CRITERIA_OP_EQ },
        { "!=",
          GUPNP_SEARCH_CRITERIA_OP_NEQ },
        { "<",
          GUPNP_SEARCH_CRITERIA_OP_LESS },
        { "<=",
          GUPNP_SEARCH_CRITERIA_OP_LEQ },
        { ">",
          GUPNP_SEARCH_CRITERIA_OP_GREATER },
        { ">=",
          GUPNP_SEARCH_CRITERIA_OP_GEQ },

        { "contains",
          GUPNP_SEARCH_CRITERIA_OP_CONTAINS },
        { "doesNotContain",
          GUPNP_SEARCH_CRITERIA_OP_DOES_NOT_CONTAIN },
        { "derivedfrom",
          GUPNP_SEARCH_CRITERIA_OP_DERIVED_FROM },

        { "exists",
          GUPNP_SEARCH_CRITERIA_OP_EXISTS },

        { "true",
          SYMBOL_TRUE },
        { "false",
          SYMBOL_FALSE }
};

static void
gupnp_search_criteria_parser_init (GUPnPSearchCriteriaParser *parser)
{
        GUPnPSearchCriteriaParserPrivate *priv =
                gupnp_search_criteria_parser_get_instance_private (parser);

        /* Set up GScanner */
        priv->scanner = g_scanner_new (NULL);

        priv->scanner->config->cset_skip_characters  = (char *)" \t\n\r\012"
                                                               "\013\014\015";
        priv->scanner->config->scan_identifier_1char = TRUE;
        priv->scanner->config->cset_identifier_first = (char *) G_CSET_a_2_z
                                                               "_*<>=!@"
                                                               G_CSET_A_2_Z;
        priv->scanner->config->cset_identifier_nth =   (char *)G_CSET_a_2_z
                                                               "_0123456789=:@"
                                                               G_CSET_A_2_Z
                                                               G_CSET_LATINS
                                                               G_CSET_LATINC;
        priv->scanner->config->symbol_2_token        = TRUE;

        /* Add symbols */
        for (int i = 0; i < NUM_SYMBOLS; i++) {
                g_scanner_scope_add_symbol (priv->scanner,
                                            0,
                                            symbols[i].name,
                                            GINT_TO_POINTER (symbols[i].token));
        }
}

static void
gupnp_search_criteria_parser_finalize (GObject *object)
{
        GObjectClass *gobject_class;
        GUPnPSearchCriteriaParser *parser;

        parser = GUPNP_SEARCH_CRITERIA_PARSER (object);
        GUPnPSearchCriteriaParserPrivate *priv =
                gupnp_search_criteria_parser_get_instance_private (parser);

        /* Destroy GScanner */
        g_scanner_destroy (priv->scanner);

        gobject_class =
                G_OBJECT_CLASS (gupnp_search_criteria_parser_parent_class);
        gobject_class->dispose (object);
}

static void
gupnp_search_criteria_parser_class_init
                                    (GUPnPSearchCriteriaParserClass *klass)
{
        GObjectClass *object_class;

        object_class = G_OBJECT_CLASS (klass);

        object_class->finalize = gupnp_search_criteria_parser_finalize;

        /**
         * GUPnPSearchCriteriaParser::begin-parens:
         * @parser: The #GUPnPSearchCriteriaParser that received the signal
         *
         * The ::begin_parens signal is emitted to mark the beginning of a
         * parenthetical expression.
         **/
        signals[BEGIN_PARENS] =
                g_signal_new ("begin-parens",
                              GUPNP_TYPE_SEARCH_CRITERIA_PARSER,
                              G_SIGNAL_RUN_LAST,
                              G_STRUCT_OFFSET (GUPnPSearchCriteriaParserClass,
                                               begin_parens),
                              NULL,
                              NULL,
                              g_cclosure_marshal_VOID__VOID,
                              G_TYPE_NONE,
                              0);

        /**
         * GUPnPSearchCriteriaParser::end-parens:
         * @parser: The #GUPnPSearchCriteriaParser that received the signal
         *
         * The ::end_parens signal is emitted to mark the end of a parenthetical
         * expression.
         **/
        signals[END_PARENS] =
                g_signal_new ("end-parens",
                              GUPNP_TYPE_SEARCH_CRITERIA_PARSER,
                              G_SIGNAL_RUN_LAST,
                              G_STRUCT_OFFSET (GUPnPSearchCriteriaParserClass,
                                               end_parens),
                              NULL,
                              NULL,
                              g_cclosure_marshal_VOID__VOID,
                              G_TYPE_NONE,
                              0);

        /**
         * GUPnPSearchCriteriaParser::conjunction:
         * @parser: The #GUPnPSearchCriteriaParser that received the signal
         *
         * The ::conjuction signal is emitted whenever a conjuction marker
         * &lpar;and&rpar; is parsed.
         **/
        signals[CONJUNCTION] =
                g_signal_new ("conjunction",
                              GUPNP_TYPE_SEARCH_CRITERIA_PARSER,
                              G_SIGNAL_RUN_LAST,
                              G_STRUCT_OFFSET (GUPnPSearchCriteriaParserClass,
                                               conjunction),
                              NULL,
                              NULL,
                              g_cclosure_marshal_VOID__VOID,
                              G_TYPE_NONE,
                              0);

        /**
         * GUPnPSearchCriteriaParser::disjunction:
         * @parser: The #GUPnPSearchCriteriaParser that received the signal
         *
         * The ::disjuction signal is emitted whenever a disjuction marker
         * &lpar;or&rpar is parsed.
         **/
        signals[DISJUNCTION] =
                g_signal_new ("disjunction",
                              GUPNP_TYPE_SEARCH_CRITERIA_PARSER,
                              G_SIGNAL_RUN_LAST,
                              G_STRUCT_OFFSET (GUPnPSearchCriteriaParserClass,
                                               disjunction),
                              NULL,
                              NULL,
                              g_cclosure_marshal_VOID__VOID,
                              G_TYPE_NONE,
                              0);

        /**
         * GUPnPSearchCriteriaParser::expression:
         * @parser: The #GUPnPSearchCriteriaParser that received the signal
         * @property: The property
         * @op: The operator as #GUPnPSearchCriteriaOp
         * @value: The value as string
         * @error: Place-holder for any possible errors from handler
         *
         * The ::expression signal is emitted whenever an expression is parsed.
         * Set @error and return %FALSE if an error occurred.
         **/
        signals[EXPRESSION] =
                g_signal_new ("expression",
                              GUPNP_TYPE_SEARCH_CRITERIA_PARSER,
                              G_SIGNAL_RUN_LAST,
                              G_STRUCT_OFFSET (GUPnPSearchCriteriaParserClass,
                                               expression),
                              NULL,
                              NULL,
                              gupnp_av_marshal_BOOLEAN__STRING_UINT_STRING_POINTER,
                              G_TYPE_BOOLEAN,
                              4,
                              G_TYPE_STRING,
                              GUPNP_TYPE_SEARCH_CRITERIA_OP,
                              G_TYPE_STRING,
                              G_TYPE_POINTER);
}

/**
 * gupnp_search_criteria_parser_new:
 *
 * Return value: A new #GUPnPSearchCriteriaParser object.
 **/
GUPnPSearchCriteriaParser *
gupnp_search_criteria_parser_new (void)
{
        return g_object_new (GUPNP_TYPE_SEARCH_CRITERIA_PARSER, NULL);
}

/* Scan a relExp portion of a search criteria string */
static gboolean
scan_rel_exp (GUPnPSearchCriteriaParser *parser,
              GError                   **error)
{
        GTokenValue value;
        gboolean ret;
        guint token;
        GUPnPSearchCriteriaOp op;
        char *arg1;
        GUPnPSearchCriteriaParserPrivate *priv =
                gupnp_search_criteria_parser_get_instance_private (parser);

        token = g_scanner_get_next_token (priv->scanner);
        g_assert (token == G_TOKEN_IDENTIFIER); /* Already checked */

        value = g_scanner_cur_value (priv->scanner);
        arg1 = g_strdup (value.v_string);

        token = g_scanner_get_next_token (priv->scanner);
        switch (token) {
        case GUPNP_SEARCH_CRITERIA_OP_EQ:
        case GUPNP_SEARCH_CRITERIA_OP_NEQ:
        case GUPNP_SEARCH_CRITERIA_OP_LESS:
        case GUPNP_SEARCH_CRITERIA_OP_LEQ:
        case GUPNP_SEARCH_CRITERIA_OP_GREATER:
        case GUPNP_SEARCH_CRITERIA_OP_GEQ:
        case GUPNP_SEARCH_CRITERIA_OP_CONTAINS:
        case GUPNP_SEARCH_CRITERIA_OP_DOES_NOT_CONTAIN:
        case GUPNP_SEARCH_CRITERIA_OP_DERIVED_FROM:
                op = token;

                token = g_scanner_get_next_token (priv->scanner);
                if (token != G_TOKEN_STRING) {
                        g_set_error (error,
                                     GUPNP_SEARCH_CRITERIA_PARSER_ERROR,
                                     GUPNP_SEARCH_CRITERIA_PARSER_ERROR_FAILED,
                                     "Expected quoted string at position %u",
                                     g_scanner_cur_position (priv->scanner));

                        ret = FALSE;

                        break;
                }

                value = g_scanner_cur_value (priv->scanner);

                g_signal_emit (parser,
                               signals[EXPRESSION],
                               0,
                               arg1,
                               op,
                               value.v_string,
                               error,
                               &ret);

                break;

        case GUPNP_SEARCH_CRITERIA_OP_EXISTS:
                op = token;

                token = g_scanner_get_next_token (priv->scanner);
                switch (token) {
                case SYMBOL_TRUE:
                        g_signal_emit (parser, signals[EXPRESSION], 0,
                                       arg1, op, "true", error, &ret);

                        break;
                case SYMBOL_FALSE:
                        g_signal_emit (parser, signals[EXPRESSION], 0,
                                       arg1, op, "false", error, &ret);

                        break;
                default:
                        g_set_error (error,
                                     GUPNP_SEARCH_CRITERIA_PARSER_ERROR,
                                     GUPNP_SEARCH_CRITERIA_PARSER_ERROR_FAILED,
                                     "Expected boolean value at position %u",
                                     g_scanner_cur_position (priv->scanner));

                        ret = FALSE;

                        break;
                }

                break;

        default:
                g_set_error (error,
                             GUPNP_SEARCH_CRITERIA_PARSER_ERROR,
                             GUPNP_SEARCH_CRITERIA_PARSER_ERROR_FAILED,
                             "Expected operator at position %u",
                             g_scanner_cur_position (priv->scanner));

                ret = FALSE;
        }

        g_free (arg1);

        return ret;
}

static gboolean
scan_search_exp (GUPnPSearchCriteriaParser *parser,
                 GError                   **error);

/* Scan a Logical operator and the part after that */
static gboolean
scan_logical_op (GUPnPSearchCriteriaParser *parser,
                 GError                   **error)
{
        gboolean ret;
        guint token;
        GUPnPSearchCriteriaParserPrivate *priv =
                gupnp_search_criteria_parser_get_instance_private (parser);

        token = g_scanner_peek_next_token (priv->scanner);

        switch (token) {
                case SYMBOL_AND:
                        g_scanner_get_next_token (priv->scanner);

                        g_signal_emit (parser, signals[CONJUNCTION], 0);

                        ret = scan_search_exp (parser, error);

                        break;

                case SYMBOL_OR:
                        g_scanner_get_next_token (priv->scanner);

                        g_signal_emit (parser, signals[DISJUNCTION], 0);

                        ret = scan_search_exp (parser, error);

                        break;

                default:

                        ret = TRUE;

                        break;

        }

        return ret;
}

/* Scan a searchExp portion of a search criteria string */
static gboolean
scan_search_exp (GUPnPSearchCriteriaParser *parser,
                 GError                   **error)
{
        gboolean ret;
        guint token;
        GUPnPSearchCriteriaParserPrivate *priv =
                gupnp_search_criteria_parser_get_instance_private (parser);

        token = g_scanner_peek_next_token (priv->scanner);
        switch (token) {
        case G_TOKEN_LEFT_PAREN:
                g_scanner_get_next_token (priv->scanner);

                g_signal_emit (parser, signals[BEGIN_PARENS], 0);

                ret = scan_search_exp (parser, error);
                if (ret == FALSE)
                        break;

                token = g_scanner_get_next_token (priv->scanner);
                if (token != G_TOKEN_RIGHT_PAREN) {
                        g_set_error (
                                error,
                                GUPNP_SEARCH_CRITERIA_PARSER_ERROR,
                                GUPNP_SEARCH_CRITERIA_PARSER_ERROR_FAILED,
                                "Expected right parenthesis at position %u",
                                g_scanner_cur_position (priv->scanner));

                        ret = FALSE;

                        break;
                }

                g_signal_emit (parser, signals[END_PARENS], 0);

                ret = scan_logical_op (parser, error);

                break;

        case G_TOKEN_IDENTIFIER:
                ret = scan_rel_exp (parser, error);
                if (ret == FALSE)
                        break;

                ret = scan_logical_op (parser, error);

                break;

        default:
                g_scanner_get_next_token (priv->scanner);

                g_set_error (error,
                             GUPNP_SEARCH_CRITERIA_PARSER_ERROR,
                             GUPNP_SEARCH_CRITERIA_PARSER_ERROR_FAILED,
                             "Expected property name or left parenthesis at "
                             "position %u",
                             g_scanner_cur_position (priv->scanner));

                ret = FALSE;
        }

        return ret;
}

/**
 * gupnp_search_criteria_parser_parse_text:
 * @parser: A #GUPnPSearchCriteriaParser
 * @text: The search criteria string to be parsed
 * @error: The location where to store the error information if any, or NULL
 *
 * Parses @text, emitting the various defined signals on the way. If an
 * error occured @error will be set.
 *
 * Return value: TRUE on success.
 **/
gboolean
gupnp_search_criteria_parser_parse_text (GUPnPSearchCriteriaParser *parser,
                                         const char                *text,
                                         GError                   **error)
{
        gboolean ret;
        guint token;

        g_return_val_if_fail (GUPNP_IS_SEARCH_CRITERIA_PARSER (parser),
                              FALSE);
        g_return_val_if_fail (text != NULL, FALSE);
        GUPnPSearchCriteriaParserPrivate *priv =
                gupnp_search_criteria_parser_get_instance_private (parser);

        /* Feed into scanner */
        g_scanner_input_text (priv->scanner, text, strlen (text));

        token = g_scanner_peek_next_token (priv->scanner);
        if (token == SYMBOL_ASTERISK) {
                g_scanner_get_next_token (priv->scanner);

                /* Do nothing. */

                ret = TRUE;
        } else
                ret = scan_search_exp (parser, error);

        if (ret == TRUE) {
                /* Confirm that we have EOF now */
                token = g_scanner_get_next_token (priv->scanner);
                if (token != G_TOKEN_EOF) {
                        g_set_error (error,
                                     GUPNP_SEARCH_CRITERIA_PARSER_ERROR,
                                     GUPNP_SEARCH_CRITERIA_PARSER_ERROR_FAILED,
                                     "Expected EOF at position %u",
                                     g_scanner_cur_position (priv->scanner));
                }
        }

        return ret;
}
