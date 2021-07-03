/*
 * Copyright (C) 2012 Intel Corporation.
 * Copyright (C) 2009 Nokia Corporation.
 *
 * Author: Christophe Guiraud <christophe.guiraud@intel.com>
 * Author: Zeeshan Ali (Khattak) <zeeshanak@gnome.org>
 *                               <zeeshan.ali@nokia.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */

#ifndef GUPNP_DIDL_LITE_CREATE_CLASS_H
#define GUPNP_DIDL_LITE_CREATE_CLASS_H

#include <glib-object.h>
#include <libxml/tree.h>
#include "gupnp-didl-lite-container.h"

G_BEGIN_DECLS

G_DECLARE_DERIVABLE_TYPE(GUPnPDIDLLiteCreateClass,
                          gupnp_didl_lite_create_class,
                          GUPNP,
                          DIDL_LITE_CREATE_CLASS,
                          GObject)

#define GUPNP_TYPE_DIDL_LITE_CREATE_CLASS \
                (gupnp_didl_lite_create_class_get_type ())

struct _GUPnPDIDLLiteCreateClassClass {
        GObjectClass parent_class;

        /* future padding */
        void (* _gupnp_reserved1) (void);
        void (* _gupnp_reserved2) (void);
        void (* _gupnp_reserved3) (void);
        void (* _gupnp_reserved4) (void);
};

const char *
gupnp_didl_lite_create_class_get_content
                                    (GUPnPDIDLLiteCreateClass *create_class);

void
gupnp_didl_lite_create_class_set_content
                                    (GUPnPDIDLLiteCreateClass *create_class,
                                     const char               *content);

gboolean
gupnp_didl_lite_create_class_get_include_derived
                                    (GUPnPDIDLLiteCreateClass *create_class);

void
gupnp_didl_lite_create_class_set_include_derived
                                    (GUPnPDIDLLiteCreateClass *create_class,
                                     gboolean                 include_derived);

const char *
gupnp_didl_lite_create_class_get_friendly_name
                                    (GUPnPDIDLLiteCreateClass *create_class);

void
gupnp_didl_lite_create_class_set_friendly_name
                                    (GUPnPDIDLLiteCreateClass *create_class,
                                     const char               *friendly_name);

xmlNode *
gupnp_didl_lite_create_class_get_xml_node
                                    (GUPnPDIDLLiteCreateClass *create_class);

G_END_DECLS

#endif /* __GUPNP_DIDL_LITE_CREATE_CLASS_H__ */
