/*
 * Copyright (C) 2009 Nokia Corporation.
 * Copyright (C) 2007, 2008 OpenedHand Ltd.
 *
 * Authors: Zeeshan Ali (Khattak) <zeeshan.ali@nokia.com>
 *                                <zeeshanak@gnome.org>
 *          Jorn Baayen <jorn@openedhand.com>
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

#ifndef __GUPNP_DIDL_LITE_OBJECT_H__
#define __GUPNP_DIDL_LITE_OBJECT_H__

#include <stdarg.h>
#include <glib-object.h>
#include <libxml/tree.h>

#include "gupnp-didl-lite-resource.h"

G_BEGIN_DECLS

GType
gupnp_didl_lite_object_get_type (void) G_GNUC_CONST;

#define GUPNP_TYPE_DIDL_LITE_OBJECT \
                (gupnp_didl_lite_object_get_type ())
#define GUPNP_DIDL_LITE_OBJECT(obj) \
                (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
                 GUPNP_TYPE_DIDL_LITE_OBJECT, \
                 GUPnPDIDLLiteObject))
#define GUPNP_DIDL_LITE_OBJECT_CLASS(obj) \
                (G_TYPE_CHECK_CLASS_CAST ((obj), \
                 GUPNP_TYPE_DIDL_LITE_OBJECT, \
                 GUPnPDIDLLiteObjectClass))
#define GUPNP_IS_DIDL_LITE_OBJECT(obj) \
                (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                 GUPNP_TYPE_DIDL_LITE_OBJECT))
#define GUPNP_IS_DIDL_LITE_OBJECT_CLASS(obj) \
                (G_TYPE_CHECK_CLASS_TYPE ((obj), \
                 GUPNP_TYPE_DIDL_LITE_OBJECT))
#define GUPNP_DIDL_LITE_OBJECT_GET_CLASS(obj) \
                (G_TYPE_INSTANCE_GET_CLASS ((obj), \
                 GUPNP_TYPE_DIDL_LITE_OBJECT, \
                 GUPnPDIDLLiteObjectClass))

typedef struct _GUPnPDIDLLiteObjectPrivate GUPnPDIDLLiteObjectPrivate;

typedef struct {
        GObject parent;

        GUPnPDIDLLiteObjectPrivate *priv;
} GUPnPDIDLLiteObject;

typedef struct {
        GObjectClass parent_class;

        /* future padding */
        void (* _gupnp_reserved1) (void);
        void (* _gupnp_reserved2) (void);
        void (* _gupnp_reserved3) (void);
        void (* _gupnp_reserved4) (void);
} GUPnPDIDLLiteObjectClass;

xmlNode *
gupnp_didl_lite_object_get_xml_node     (GUPnPDIDLLiteObject *object);

char *
gupnp_didl_lite_object_get_upnp_class   (GUPnPDIDLLiteObject *object);

char *
gupnp_didl_lite_object_get_upnp_class_name
                                        (GUPnPDIDLLiteObject *object);

char *
gupnp_didl_lite_object_get_id           (GUPnPDIDLLiteObject *object);

char *
gupnp_didl_lite_object_get_parent_id    (GUPnPDIDLLiteObject *object);

GList *
gupnp_didl_lite_object_get_properties   (GUPnPDIDLLiteObject *object,
                                         const char          *name);

gboolean
gupnp_didl_lite_object_get_restricted   (GUPnPDIDLLiteObject *object);

char *
gupnp_didl_lite_object_get_title        (GUPnPDIDLLiteObject *object);

char *
gupnp_didl_lite_object_get_creator      (GUPnPDIDLLiteObject *object);

char *
gupnp_didl_lite_object_get_write_status (GUPnPDIDLLiteObject *object);

GList *
gupnp_didl_lite_object_get_resources    (GUPnPDIDLLiteObject *object);

GUPnPDIDLLiteResource *
gupnp_didl_lite_object_get_compat_resource
                                        (GUPnPDIDLLiteObject *object,
                                         const char
                                         *sink_protocol_info,
                                         gboolean             lenient);

char *
gupnp_didl_lite_object_to_string      (GUPnPDIDLLiteObject *object);

G_END_DECLS

#endif /* __GUPNP_DIDL_LITE_OBJECT_H__ */
