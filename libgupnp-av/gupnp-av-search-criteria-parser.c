/*
 * Copyright (C) 2008 OpenedHand Ltd.
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

/**
 * SECTION:gupnp-av-search-criteria-parser
 * @short_description: AV search criteria parser
 *
 * #GUPnPAVSearchCriteriaParser parses ContentDirectory search criteria
 * strings.
 *
 * Note that no signals will be emitted if a wildcard is specified,
 * and that the user is responsible for ensuring precedence of conjunction
 * over disjunction.
 */

#include <string.h>

#include "gupnp-av-search-criteria-parser.h"
#include "gupnp-av-marshal.h"

/* GUPnPAVSearchCriteriaParserError */
GQuark
gupnp_av_search_criteria_parser_error_quark (void)
{
        return g_quark_from_static_string
                ("gupnp-av-search-criteria-parser-error-quark");
}

/* GUPnPAVSearchCriteriaParser */
G_DEFINE_TYPE (GUPnPAVSearchCriteriaParser,
               gupnp_av_search_criteria_parser,
               G_TYPE_OBJECT);

struct _GUPnPAVSearchCriteriaParserPrivate {
        GScanner *scanner;
};

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
          GUPNP_AV_SEARCH_CRITERIA_OP_EQ },
        { "!=",
          GUPNP_AV_SEARCH_CRITERIA_OP_NEQ },
        { "<",
          GUPNP_AV_SEARCH_CRITERIA_OP_LESS },
        { "<=",
          GUPNP_AV_SEARCH_CRITERIA_OP_LEQ },
        { ">",
          GUPNP_AV_SEARCH_CRITERIA_OP_GREATER },
        { ">=",
          GUPNP_AV_SEARCH_CRITERIA_OP_GEQ },

        { "contains",
          GUPNP_AV_SEARCH_CRITERIA_OP_CONTAINS },
        { "doesNotContain",
          GUPNP_AV_SEARCH_CRITERIA_OP_DOES_NOT_CONTAIN },
        { "derivedfrom",
          GUPNP_AV_SEARCH_CRITERIA_OP_DERIVED_FROM },

        { "exists",
          GUPNP_AV_SEARCH_CRITERIA_OP_EXISTS },

        { "true",
          SYMBOL_TRUE },
        { "false",
          SYMBOL_FALSE }
};

static void
gupnp_av_search_criteria_parser_init (GUPnPAVSearchCriteriaParser *parser)
{
        int i;

        parser->priv = G_TYPE_INSTANCE_GET_PRIVATE
                         (parser,
                          GUPNP_AV_TYPE_SEARCH_CRITERIA_PARSER,
                          GUPnPAVSearchCriteriaParserPrivate);

        /* Set up GScanner */
        parser->priv->scanner = g_scanner_new (NULL);

        parser->priv->scanner->config->cset_skip_characters  = " \t\n\r\012"
                                                               "\013\014\015";
        parser->priv->scanner->config->scan_identifier_1char = TRUE;
        parser->priv->scanner->config->cset_identifier_first = G_CSET_a_2_z
                                                               "_*<>=!"
                                                               G_CSET_A_2_Z;
        parser->priv->scanner->config->cset_identifier_nth =   G_CSET_a_2_z
                                                               "_0123456789="
                                                               G_CSET_A_2_Z
                                                               G_CSET_LATINS
                                                               G_CSET_LATINC;
        parser->priv->scanner->config->symbol_2_token        = TRUE;

        /* Add symbols */
        for (i = 0; i < NUM_SYMBOLS; i++) {
                g_scanner_scope_add_symbol (parser->priv->scanner,
                                            0,
                                            symbols[i].name,
                                            GINT_TO_POINTER (symbols[i].token));
        }
}

static void
gupnp_av_search_criteria_parser_finalize (GObject *object)
{
        GObjectClass *gobject_class;
        GUPnPAVSearchCriteriaParser *parser;

        parser = GUPNP_AV_SEARCH_CRITERIA_PARSER (object);

        /* Destroy GScanner */
        g_scanner_destroy (parser->priv->scanner);

        gobject_class =
                G_OBJECT_CLASS (gupnp_av_search_criteria_parser_parent_class);
        gobject_class->dispose (object);
}

static void
gupnp_av_search_criteria_parser_class_init
                                    (GUPnPAVSearchCriteriaParserClass *klass)
{
        GObjectClass *object_class;

        object_class = G_OBJECT_CLASS (klass);

        object_class->finalize = gupnp_av_search_criteria_parser_finalize;

        /**
         * GUPnPAVSearchCriteriaParser::begin_parens
         * @parser: The #GUPnPGUPnPAVSearchCriteriaParser that received the
         * signal
         *
         * The ::begin_parens signal is emitted to mark the beginning
         * of a parenthetical expression.
         **/
        signals[BEGIN_PARENS] =
                g_signal_new ("begin-parens",
                              GUPNP_AV_TYPE_SEARCH_CRITERIA_PARSER,
                              G_SIGNAL_RUN_LAST,
                              G_STRUCT_OFFSET (GUPnPAVSearchCriteriaParserClass,
                                               begin_parens),
                              NULL,
                              NULL,
                              g_cclosure_marshal_VOID__VOID,
                              G_TYPE_NONE,
                              0);

        /**
         * GUPnPAVSearchCriteriaParser::end_parens
         * @parser: The #GUPnPGUPnPAVSearchCriteriaParser that received the
         * signal
         *
         * The ::end_parens signal is emitted to mark the end
         * of a parenthetical expression.
         **/
        signals[END_PARENS] =
                g_signal_new ("end-parens",
                              GUPNP_AV_TYPE_SEARCH_CRITERIA_PARSER,
                              G_SIGNAL_RUN_LAST,
                              G_STRUCT_OFFSET (GUPnPAVSearchCriteriaParserClass,
                                               end_parens),
                              NULL,
                              NULL,
                              g_cclosure_marshal_VOID__VOID,
                              G_TYPE_NONE,
                              0);

        /**
         * GUPnPAVSearchCriteriaParser::conjunction
         * @parser: The #GUPnPGUPnPAVSearchCriteriaParser that received the
         * signal
         *
         * The ::conjuction signal is emitted whenever a conjuction marker
         * (and) is parsed.
         **/
        signals[CONJUNCTION] =
                g_signal_new ("conjunction",
                              GUPNP_AV_TYPE_SEARCH_CRITERIA_PARSER,
                              G_SIGNAL_RUN_LAST,
                              G_STRUCT_OFFSET (GUPnPAVSearchCriteriaParserClass,
                                               conjunction),
                              NULL,
                              NULL,
                              g_cclosure_marshal_VOID__VOID,
                              G_TYPE_NONE,
                              0);

        /**
         * GUPnPAVSearchCriteriaParser::disjunction
         * @parser: The #GUPnPGUPnPAVSearchCriteriaParser that received the
         * signal
         *
         * The ::disjuction signal is emitted whenever a disjuction marker
         * (or) is parsed.
         **/
        signals[DISJUNCTION] =
                g_signal_new ("disjunction",
                              GUPNP_AV_TYPE_SEARCH_CRITERIA_PARSER,
                              G_SIGNAL_RUN_LAST,
                              G_STRUCT_OFFSET (GUPnPAVSearchCriteriaParserClass,
                                               disjunction),
                              NULL,
                              NULL,
                              g_cclosure_marshal_VOID__VOID,
                              G_TYPE_NONE,
                              0);

        /**
         * GUPnPAVSearchCriteriaParser::expression
         * @parser: The #GUPnPGUPnPAVSearchCriteriaParser that received the
         * signal
         *
         * The ::expression signal is emitted whenever an expression
         * (or) is parsed.
         **/
        signals[EXPRESSION] =
                g_signal_new ("expression",
                              GUPNP_AV_TYPE_SEARCH_CRITERIA_PARSER,
                              G_SIGNAL_RUN_LAST,
                              G_STRUCT_OFFSET (GUPnPAVSearchCriteriaParserClass,
                                               expression),
                              NULL,
                              NULL,
                              gupnp_av_marshal_VOID__STRING_UINT_STRING,
                              G_TYPE_NONE,
                              3,
                              G_TYPE_STRING,
                              G_TYPE_UINT,
                              G_TYPE_STRING);

        g_type_class_add_private (klass,
                                  sizeof (GUPnPAVSearchCriteriaParserPrivate));
}

/**
 * gupnp_av_search_criteria_parser_new
 *
 * Return value: A new #GUPnPAVSearchCriteriaParser object.
 **/
GUPnPAVSearchCriteriaParser *
gupnp_av_search_criteria_parser_new (void)
{
        return g_object_new (GUPNP_AV_TYPE_SEARCH_CRITERIA_PARSER, NULL);
}

/* Scan a relExp portion of a search criteria string */
static gboolean
scan_rel_exp (GUPnPAVSearchCriteriaParser *parser,
              GError                     **error)
{
        GTokenValue value;
        gboolean ret;
        guint token;
        GUPnPAVSearchCriteriaOp op;
        char *arg1;

        token = g_scanner_get_next_token (parser->priv->scanner);
        g_assert (token == G_TOKEN_IDENTIFIER); /* Already checked */

        value = g_scanner_cur_value (parser->priv->scanner);
        arg1 = g_strdup (value.v_string);

        token = g_scanner_get_next_token (parser->priv->scanner);
        switch (token) {
        case GUPNP_AV_SEARCH_CRITERIA_OP_EQ:
        case GUPNP_AV_SEARCH_CRITERIA_OP_NEQ:
        case GUPNP_AV_SEARCH_CRITERIA_OP_LESS:
        case GUPNP_AV_SEARCH_CRITERIA_OP_LEQ:
        case GUPNP_AV_SEARCH_CRITERIA_OP_GREATER:
        case GUPNP_AV_SEARCH_CRITERIA_OP_GEQ:
        case GUPNP_AV_SEARCH_CRITERIA_OP_CONTAINS:
        case GUPNP_AV_SEARCH_CRITERIA_OP_DOES_NOT_CONTAIN:
        case GUPNP_AV_SEARCH_CRITERIA_OP_DERIVED_FROM:
                op = token;

                token = g_scanner_get_next_token (parser->priv->scanner);
                if (token != G_TOKEN_STRING) {
                        g_set_error
                                (error,
                                 GUPNP_AV_SEARCH_CRITERIA_PARSER_ERROR,
                                 GUPNP_AV_SEARCH_CRITERIA_PARSER_ERROR_FAILED,
                                 "Expected quoted string at position %u",
                                 g_scanner_cur_position
                                       (parser->priv->scanner));

                        ret = FALSE;
                }

                value = g_scanner_cur_value (parser->priv->scanner);

                g_signal_emit (parser, signals[EXPRESSION], 0,
                               arg1, op, value.v_string);

                ret = TRUE;

                break;

        case GUPNP_AV_SEARCH_CRITERIA_OP_EXISTS:
                op = token;

                token = g_scanner_get_next_token (parser->priv->scanner);
                switch (token) {
                case SYMBOL_TRUE:
                        g_signal_emit (parser, signals[EXPRESSION], 0,
                                       arg1, op, NULL);

                        ret = TRUE;

                        break;
                case SYMBOL_FALSE:
                        ret = TRUE;

                        break;
                default:
                        g_set_error
                                (error,
                                 GUPNP_AV_SEARCH_CRITERIA_PARSER_ERROR,
                                 GUPNP_AV_SEARCH_CRITERIA_PARSER_ERROR_FAILED,
                                 "Expected boolean value at position %u",
                                 g_scanner_cur_position
                                       (parser->priv->scanner));

                        ret = FALSE;

                        break;
                }

                break;

        default:
                g_set_error (error,
                             GUPNP_AV_SEARCH_CRITERIA_PARSER_ERROR,
                             GUPNP_AV_SEARCH_CRITERIA_PARSER_ERROR_FAILED,
                             "Expected operator at position %u",
                             g_scanner_cur_position
                               (parser->priv->scanner));

                ret = FALSE;
        }

        g_free (arg1);

        return ret;
}

/* Scan a searchExp portion of a search criteria string */
static gboolean
scan_search_exp (GUPnPAVSearchCriteriaParser *parser,
                 GError                     **error)
{
        gboolean ret;
        guint token;

        token = g_scanner_peek_next_token (parser->priv->scanner);
        switch (token) {
        case G_TOKEN_LEFT_PAREN:
                g_scanner_get_next_token (parser->priv->scanner);

                g_signal_emit (parser, signals[BEGIN_PARENS], 0);

                ret = scan_search_exp (parser, error);
                if (ret == FALSE)
                        break;

                token = g_scanner_get_next_token (parser->priv->scanner);
                if (token != G_TOKEN_RIGHT_PAREN) {
                        g_set_error
                                (error,
                                 GUPNP_AV_SEARCH_CRITERIA_PARSER_ERROR,
                                 GUPNP_AV_SEARCH_CRITERIA_PARSER_ERROR_FAILED,
                                 "Expected right parenthesis at position %u",
                                 g_scanner_cur_position
                                        (parser->priv->scanner));

                        ret = FALSE;
                }

                g_signal_emit (parser, signals[END_PARENS], 0);

                break;

        case G_TOKEN_IDENTIFIER:
                ret = scan_rel_exp (parser, error);
                if (ret == FALSE)
                        break;

                token = g_scanner_peek_next_token (parser->priv->scanner);

                switch (token) {
                case SYMBOL_AND:
                        g_scanner_get_next_token (parser->priv->scanner);

                        g_signal_emit (parser, signals[CONJUNCTION], 0);

                        ret = scan_search_exp (parser, error);

                        break;

                case SYMBOL_OR:
                        g_scanner_get_next_token (parser->priv->scanner);

                        g_signal_emit (parser, signals[DISJUNCTION], 0);

                        ret = scan_search_exp (parser, error);

                        break;

                default:
                        break;
                }

                break;

        default:
                g_scanner_get_next_token (parser->priv->scanner);

                g_set_error (error,
                             GUPNP_AV_SEARCH_CRITERIA_PARSER_ERROR,
                             GUPNP_AV_SEARCH_CRITERIA_PARSER_ERROR_FAILED,
                             "Expected property name or left parenthesis at "
                             "position %u",
                             g_scanner_cur_position (parser->priv->scanner));

                ret = FALSE;
        }

        return ret;
}

/**
 * gupnp_av_search_criteria_parser_parse_text
 * @parser: A #GUPnPAVSearchCriteriaParser
 * @text: The search criteria string to be parsed
 * @error: The location where to store the error information if any, or NULL
 *
 * Parses @text, emitting the various defined signals on the way. If an
 * error occured @error will be set.
 *
 * Return value: TRUE on success.
 **/
gboolean
gupnp_av_search_criteria_parser_parse_text (GUPnPAVSearchCriteriaParser *parser,
                                            const char                  *text,
                                            GError                     **error)
{
        gboolean ret;
        guint token;

        g_return_val_if_fail (GUPNP_AV_IS_SEARCH_CRITERIA_PARSER (parser),
                              FALSE);
        g_return_val_if_fail (text != NULL, FALSE);

        /* Feed into scanner */
        g_scanner_input_text (parser->priv->scanner, text, strlen (text));

        token = g_scanner_peek_next_token (parser->priv->scanner);
        if (token == SYMBOL_ASTERISK) {
                g_scanner_get_next_token (parser->priv->scanner);

                /* Do nothing. */

                ret = TRUE;
        } else
                ret = scan_search_exp (parser, error);

        if (ret == TRUE) {
                /* Confirm that we have EOF now */
                token = g_scanner_get_next_token (parser->priv->scanner);
                if (token != G_TOKEN_EOF) {
                        g_set_error
                                (error,
                                 GUPNP_AV_SEARCH_CRITERIA_PARSER_ERROR,
                                 GUPNP_AV_SEARCH_CRITERIA_PARSER_ERROR_FAILED,
                                 "Expected EOF at position %u",
                                 g_scanner_cur_position
                                       (parser->priv->scanner));
                }
        }

        return ret;
}
