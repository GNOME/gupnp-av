/*
 * Copyright (C) 2012 Intel Corporation.
 *
 * Authors: Jens Georg <jensg@openismus.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */

#ifndef __GUPNP_CDS_LAST_CHANGE_PARSER_H__
#define __GUPNP_CDS_LAST_CHANGE_PARSER_H__

#include <glib-object.h>

G_BEGIN_DECLS


GType
gupnp_cds_last_change_entry_get_type (void) G_GNUC_CONST;

G_DECLARE_DERIVABLE_TYPE(GUPnPCDSLastChangeParser,
                         gupnp_cds_last_change_parser,
                         GUPNP,
                         CDS_LAST_CHANGE_PARSER,
                         GObject)
#define GUPNP_TYPE_CDS_LAST_CHANGE_PARSER gupnp_cds_last_change_parser_get_type()

struct _GUPnPCDSLastChangeParserClass
{
    GObjectClass parent_class;
};

typedef struct _GUPnPCDSLastChangeEntry GUPnPCDSLastChangeEntry;

/**
 * GUPnPCDSLastChangeEvent:
 * @GUPNP_CDS_LAST_CHANGE_EVENT_INVALID: Invalid #GUPnPCDSLastChangeEntry.
 * @GUPNP_CDS_LAST_CHANGE_EVENT_OBJECT_ADDED: The #GUPnPCDSLastChangeEntry is
 * an object added event.
 * @GUPNP_CDS_LAST_CHANGE_EVENT_OBJECT_REMOVED: The #GUPnPCDSLastChangeEntry
 * is an object removal event.
 * @GUPNP_CDS_LAST_CHANGE_EVENT_OBJECT_MODIFIED: The #GUPnPCDSLastChangeEntry
 * is an object modification event.
 * @GUPNP_CDS_LAST_CHANGE_EVENT_ST_DONE: The #GUPnPCDSLastChangeEntry is a
 * subtree update done event.
 */
typedef enum GUPnPCDSLastChangeEvent {
        GUPNP_CDS_LAST_CHANGE_EVENT_INVALID,
        GUPNP_CDS_LAST_CHANGE_EVENT_OBJECT_ADDED,
        GUPNP_CDS_LAST_CHANGE_EVENT_OBJECT_REMOVED,
        GUPNP_CDS_LAST_CHANGE_EVENT_OBJECT_MODIFIED,
        GUPNP_CDS_LAST_CHANGE_EVENT_ST_DONE
} GUPnPCDSLastChangeEvent;

GUPnPCDSLastChangeParser *
gupnp_cds_last_change_parser_new (void);

GList *
gupnp_cds_last_change_parser_parse (GUPnPCDSLastChangeParser *parser,
                                    const char               *last_change,
                                    GError                  **error);

GUPnPCDSLastChangeEntry *
gupnp_cds_last_change_entry_ref    (GUPnPCDSLastChangeEntry *entry);
void
gupnp_cds_last_change_entry_unref  (GUPnPCDSLastChangeEntry *entry);

GUPnPCDSLastChangeEvent
gupnp_cds_last_change_entry_get_event         (GUPnPCDSLastChangeEntry *entry);

const char *
gupnp_cds_last_change_entry_get_object_id     (GUPnPCDSLastChangeEntry *entry);

const char *
gupnp_cds_last_change_entry_get_parent_id     (GUPnPCDSLastChangeEntry *entry);

const char *
gupnp_cds_last_change_entry_get_class         (GUPnPCDSLastChangeEntry *entry);

gboolean
gupnp_cds_last_change_entry_is_subtree_update (GUPnPCDSLastChangeEntry *entry);

guint32
gupnp_cds_last_change_entry_get_update_id    (GUPnPCDSLastChangeEntry *entry);

G_END_DECLS

#endif /* __GUPNP_CDS_LAST_CHANGE_PARSER_H__ */
