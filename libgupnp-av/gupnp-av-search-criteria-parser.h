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

#ifndef __GUPNP_AV_SEARCH_CRITERIA_PARSER_H__
#define __GUPNP_AV_SEARCH_CRITERIA_PARSER_H__

#include <glib-object.h>

G_BEGIN_DECLS

GType
gupnp_av_search_criteria_parser_get_type (void) G_GNUC_CONST;

#define GUPNP_AV_TYPE_SEARCH_CRITERIA_PARSER \
                (gupnp_av_search_criteria_parser_get_type ())
#define GUPNP_AV_SEARCH_CRITERIA_PARSER(obj) \
                (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
                 GUPNP_AV_TYPE_SEARCH_CRITERIA_PARSER, \
                 GUPnPAVSearchCriteriaParser))
#define GUPNP_AV_SEARCH_CRITERIA_PARSER_CLASS(obj) \
                (G_TYPE_CHECK_CLASS_CAST ((obj), \
                 GUPNP_AV_TYPE_SEARCH_CRITERIA_PARSER, \
                 GUPnPAVSearchCriteriaParserClass))
#define GUPNP_AV_IS_SEARCH_CRITERIA_PARSER(obj) \
                (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                 GUPNP_AV_TYPE_SEARCH_CRITERIA_PARSER))
#define GUPNP_AV_IS_SEARCH_CRITERIA_PARSER_CLASS(obj) \
                (G_TYPE_CHECK_CLASS_TYPE ((obj), \
                 GUPNP_AV_TYPE_SEARCH_CRITERIA_PARSER))
#define GUPNP_AV_SEARCH_CRITERIA_PARSER_GET_CLASS(obj) \
                (G_TYPE_INSTANCE_GET_CLASS ((obj), \
                 GUPNP_AV_TYPE_SEARCH_CRITERIA_PARSER, \
                 GUPnPAVSearchCriteriaParserClass))

typedef enum {
        /* G_TYPE_STRING */
        GUPNP_AV_SEARCH_CRITERIA_OP_EQ               = G_TOKEN_LAST + 1,
        GUPNP_AV_SEARCH_CRITERIA_OP_NEQ              = G_TOKEN_LAST + 2,
        GUPNP_AV_SEARCH_CRITERIA_OP_LESS             = G_TOKEN_LAST + 3,
        GUPNP_AV_SEARCH_CRITERIA_OP_LEQ              = G_TOKEN_LAST + 4,
        GUPNP_AV_SEARCH_CRITERIA_OP_GREATER          = G_TOKEN_LAST + 5,
        GUPNP_AV_SEARCH_CRITERIA_OP_GEQ              = G_TOKEN_LAST + 6,
        GUPNP_AV_SEARCH_CRITERIA_OP_CONTAINS         = G_TOKEN_LAST + 7,
        GUPNP_AV_SEARCH_CRITERIA_OP_DOES_NOT_CONTAIN = G_TOKEN_LAST + 8,
        GUPNP_AV_SEARCH_CRITERIA_OP_DERIVED_FROM     = G_TOKEN_LAST + 9,

        /* G_TYPE_BOOLEAN */
        GUPNP_AV_SEARCH_CRITERIA_OP_EXISTS           = G_TOKEN_LAST + 10
} GUPnPAVSearchCriteriaOp;

#define GUPNP_AV_SEARCH_CRITERIA_PARSER_ERROR \
                (gupnp_av_search_criteria_parser_error_quark ())

GQuark
gupnp_av_search_criteria_parser_error_quark (void);

typedef enum {
        GUPNP_AV_SEARCH_CRITERIA_PARSER_ERROR_FAILED
} GUPnPAVSearchCriteriaParserError;

typedef struct _GUPnPAVSearchCriteriaParserPrivate
                GUPnPAVSearchCriteriaParserPrivate;

typedef struct {
        GObject parent;

        GUPnPAVSearchCriteriaParserPrivate *priv;
} GUPnPAVSearchCriteriaParser;

typedef struct {
        GObjectClass parent_class;

        /* signals */
        void (* begin_parens) (GUPnPAVSearchCriteriaParser  *parser);
        void (* end_parens)   (GUPnPAVSearchCriteriaParser  *parser);
        void (* conjunction)  (GUPnPAVSearchCriteriaParser  *parser);
        void (* disjunction)  (GUPnPAVSearchCriteriaParser  *parser);
        void (* expression)   (GUPnPAVSearchCriteriaParser  *parser,
                               const char                   *property,
                               GUPnPAVSearchCriteriaOp       op,
                               const char                   *value);

        /* future padding */
        void (* _gupnp_reserved1) (void);
        void (* _gupnp_reserved2) (void);
        void (* _gupnp_reserved3) (void);
        void (* _gupnp_reserved4) (void);
} GUPnPAVSearchCriteriaParserClass;

GUPnPAVSearchCriteriaParser *
gupnp_av_search_criteria_parser_new        (void);

gboolean
gupnp_av_search_criteria_parser_parse_text (GUPnPAVSearchCriteriaParser *parser,
                                            const char                  *text,
                                            GError                     **error);

G_END_DECLS

#endif /* __GUPNP_AV_SEARCH_CRITERIA_PARSER_H__ */
