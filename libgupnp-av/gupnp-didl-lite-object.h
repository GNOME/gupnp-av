/*
 * Copyright (C) 2009 Nokia Corporation.
 * Copyright (C) 2007, 2008 OpenedHand Ltd.
 * Copyright (C) 2012 Intel Corporation
 *
 * Authors: Zeeshan Ali (Khattak) <zeeshan.ali@nokia.com>
 *                                <zeeshanak@gnome.org>
 *          Jorn Baayen <jorn@openedhand.com>
 *          Krzesimir Nowak <krnowak@openismus.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */

#ifndef GUPNP_DIDL_LITE_OBJECT_H
#define GUPNP_DIDL_LITE_OBJECT_H

#include <stdarg.h>
#include <glib-object.h>
#include <libxml/tree.h>

#include "gupnp-didl-lite-resource.h"
#include "gupnp-didl-lite-descriptor.h"
#include "gupnp-didl-lite-contributor.h"
#include "gupnp-av-enums.h"

G_BEGIN_DECLS

G_DECLARE_DERIVABLE_TYPE(GUPnPDIDLLiteObject,
                         gupnp_didl_lite_object,
                         GUPNP,
                         DIDL_LITE_OBJECT,
                         GObject)

#define GUPNP_TYPE_DIDL_LITE_OBJECT \
                (gupnp_didl_lite_object_get_type ())

typedef struct _GUPnPDIDLLiteObjectPrivate GUPnPDIDLLiteObjectPrivate;

struct _GUPnPDIDLLiteObjectClass {
        GObjectClass parent_class;

        /* future padding */
        void (* _gupnp_reserved1) (void);
        void (* _gupnp_reserved2) (void);
        void (* _gupnp_reserved3) (void);
        void (* _gupnp_reserved4) (void);
};

xmlNode *
gupnp_didl_lite_object_get_xml_node     (GUPnPDIDLLiteObject *object);

xmlNsPtr
gupnp_didl_lite_object_get_upnp_namespace
                                        (GUPnPDIDLLiteObject *object);

xmlNsPtr
gupnp_didl_lite_object_get_dc_namespace (GUPnPDIDLLiteObject *object);

xmlNsPtr
gupnp_didl_lite_object_get_dlna_namespace
                                        (GUPnPDIDLLiteObject *object);

xmlNsPtr
gupnp_didl_lite_object_get_pv_namespace
                                        (GUPnPDIDLLiteObject *object);
const char *
gupnp_didl_lite_object_get_upnp_class   (GUPnPDIDLLiteObject *object);

const char *
gupnp_didl_lite_object_get_id           (GUPnPDIDLLiteObject *object);

const char *
gupnp_didl_lite_object_get_parent_id    (GUPnPDIDLLiteObject *object);

GList *
gupnp_didl_lite_object_get_properties   (GUPnPDIDLLiteObject *object,
                                         const char          *name);

gboolean
gupnp_didl_lite_object_is_restricted_set (GUPnPDIDLLiteObject *object);

gboolean
gupnp_didl_lite_object_get_restricted   (GUPnPDIDLLiteObject *object);

const char *
gupnp_didl_lite_object_get_title        (GUPnPDIDLLiteObject *object);

const char *
gupnp_didl_lite_object_get_creator      (GUPnPDIDLLiteObject *object);

#ifndef GUPNP_DISABLE_DEPRECATED
const char *
gupnp_didl_lite_object_get_artist       (GUPnPDIDLLiteObject *object);

const char *
gupnp_didl_lite_object_get_author       (GUPnPDIDLLiteObject *object);

#endif /* GUPNP_DISABLE_DEPRECATED */

GList *
gupnp_didl_lite_object_get_creators     (GUPnPDIDLLiteObject *object);

GList *
gupnp_didl_lite_object_get_artists      (GUPnPDIDLLiteObject *object);

GList *
gupnp_didl_lite_object_get_authors      (GUPnPDIDLLiteObject *object);

GList *
gupnp_didl_lite_object_get_descriptors  (GUPnPDIDLLiteObject *object);

const char *
gupnp_didl_lite_object_get_genre        (GUPnPDIDLLiteObject *object);

const char *
gupnp_didl_lite_object_get_write_status (GUPnPDIDLLiteObject *object);

const char *
gupnp_didl_lite_object_get_album        (GUPnPDIDLLiteObject *object);

const char *
gupnp_didl_lite_object_get_album_art    (GUPnPDIDLLiteObject *object);

const char *
gupnp_didl_lite_object_get_description  (GUPnPDIDLLiteObject *object);

const char *
gupnp_didl_lite_object_get_date         (GUPnPDIDLLiteObject *object);

int
gupnp_didl_lite_object_get_track_number (GUPnPDIDLLiteObject *object);

GUPnPOCMFlags
gupnp_didl_lite_object_get_dlna_managed (GUPnPDIDLLiteObject *object);

guint
gupnp_didl_lite_object_get_update_id    (GUPnPDIDLLiteObject *object);

gboolean
gupnp_didl_lite_object_update_id_is_set (GUPnPDIDLLiteObject *object);

GList *
gupnp_didl_lite_object_get_resources    (GUPnPDIDLLiteObject *object);

GUPnPDIDLLiteResource *
gupnp_didl_lite_object_get_compat_resource
                                        (GUPnPDIDLLiteObject *object,
                                         const char
                                         *sink_protocol_info,
                                         gboolean             lenient);

GUPnPDIDLLiteResource *
gupnp_didl_lite_object_add_resource     (GUPnPDIDLLiteObject *object);

GUPnPDIDLLiteDescriptor *
gupnp_didl_lite_object_add_descriptor   (GUPnPDIDLLiteObject *object);

void
gupnp_didl_lite_object_set_upnp_class   (GUPnPDIDLLiteObject *object,
                                         const char          *upnp_class);

void
gupnp_didl_lite_object_set_id           (GUPnPDIDLLiteObject *object,
                                         const char          *id);

void
gupnp_didl_lite_object_set_parent_id    (GUPnPDIDLLiteObject *object,
                                         const char          *parent_id);

void
gupnp_didl_lite_object_set_restricted   (GUPnPDIDLLiteObject *object,
                                         gboolean             restricted);

void
gupnp_didl_lite_object_set_title        (GUPnPDIDLLiteObject *object,
                                         const char          *title);

void
gupnp_didl_lite_object_set_creator      (GUPnPDIDLLiteObject *object,
                                         const char          *creator);

#ifndef GUPNP_DISABLE_DEPRECATED
void
gupnp_didl_lite_object_set_artist       (GUPnPDIDLLiteObject *object,
                                         const char          *artist);

void
gupnp_didl_lite_object_set_author       (GUPnPDIDLLiteObject *object,
                                         const char          *author);

#endif /* GUPNP_DISABLE_DEPRECATED */

GUPnPDIDLLiteContributor *
gupnp_didl_lite_object_add_creator      (GUPnPDIDLLiteObject *object);

GUPnPDIDLLiteContributor *
gupnp_didl_lite_object_add_artist       (GUPnPDIDLLiteObject *object);

GUPnPDIDLLiteContributor *
gupnp_didl_lite_object_add_author       (GUPnPDIDLLiteObject *object);

void
gupnp_didl_lite_object_set_genre        (GUPnPDIDLLiteObject *object,
                                         const char          *genre);

void
gupnp_didl_lite_object_set_write_status (GUPnPDIDLLiteObject *object,
                                         const char          *write_status);

void
gupnp_didl_lite_object_set_album        (GUPnPDIDLLiteObject *object,
                                         const char          *album);

void
gupnp_didl_lite_object_set_album_art    (GUPnPDIDLLiteObject *object,
                                         const char          *album_art);

void
gupnp_didl_lite_object_set_description  (GUPnPDIDLLiteObject *object,
                                         const char          *description);

void
gupnp_didl_lite_object_set_date         (GUPnPDIDLLiteObject *object,
                                         const char          *date);

void
gupnp_didl_lite_object_set_track_number (GUPnPDIDLLiteObject *object,
                                         int                  track_number);

void
gupnp_didl_lite_object_set_dlna_managed (GUPnPDIDLLiteObject *object,
                                         GUPnPOCMFlags        dlna_managed);

void
gupnp_didl_lite_object_set_update_id    (GUPnPDIDLLiteObject *object,
                                         guint                update_id);

void
gupnp_didl_lite_object_unset_update_id  (GUPnPDIDLLiteObject *object);

char *
gupnp_didl_lite_object_get_title_xml_string
                                        (GUPnPDIDLLiteObject *object);

char *
gupnp_didl_lite_object_get_date_xml_string
                                        (GUPnPDIDLLiteObject *object);

char *
gupnp_didl_lite_object_get_upnp_class_xml_string
                                        (GUPnPDIDLLiteObject *object);

char *
gupnp_didl_lite_object_get_album_xml_string
                                        (GUPnPDIDLLiteObject *object);

char *
gupnp_didl_lite_object_get_track_number_xml_string
                                        (GUPnPDIDLLiteObject *object);

char *
gupnp_didl_lite_object_get_artists_xml_string
                                        (GUPnPDIDLLiteObject *object);

void
gupnp_didl_lite_object_unset_artists    (GUPnPDIDLLiteObject *object);

GUPnPDIDLLiteFragmentResult
gupnp_didl_lite_object_apply_fragments
                                      (GUPnPDIDLLiteObject  *object,
                                       gchar               **current_fragments,
                                       gint                  current_size,
                                       gchar               **new_fragments,
                                       gint                  new_size);

char *
gupnp_didl_lite_object_get_xml_string   (GUPnPDIDLLiteObject *object);

char *
gupnp_format_date_time_for_didl_lite (GDateTime *date_time, gboolean date_only);

G_END_DECLS

#endif /* __GUPNP_DIDL_LITE_OBJECT_H__ */
