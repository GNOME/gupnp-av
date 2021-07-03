/*
 * Copyright (C) 2007, 2008 OpenedHand Ltd.
 *
 * Authors: Jorn Baayen <jorn@openedhand.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */

#ifndef GUPNP_DIDL_LITE_WRITER_H
#define GUPNP_DIDL_LITE_WRITER_H

#include <stdarg.h>
#include <glib-object.h>

#include "gupnp-dlna.h"
#include "gupnp-didl-lite-item.h"
#include "gupnp-didl-lite-container.h"
#include "gupnp-didl-lite-resource.h"
#include "gupnp-didl-lite-descriptor.h"

G_BEGIN_DECLS


G_DECLARE_DERIVABLE_TYPE(GUPnPDIDLLiteWriter,
                          gupnp_didl_lite_writer,
                          GUPNP,
                          DIDL_LITE_WRITER,
                          GObject)

#define GUPNP_TYPE_DIDL_LITE_WRITER \
                (gupnp_didl_lite_writer_get_type ())

struct _GUPnPDIDLLiteWriterClass {
        GObjectClass parent_class;

        /* future padding */
        void (* _gupnp_reserved1) (void);
        void (* _gupnp_reserved2) (void);
        void (* _gupnp_reserved3) (void);
        void (* _gupnp_reserved4) (void);
};

#define GUPNP_DIDL_LITE_WRITER_NAMESPACE_DC   "dc"
#define GUPNP_DIDL_LITE_WRITER_NAMESPACE_UPNP "upnp"
#define GUPNP_DIDL_LITE_WRITER_NAMESPACE_DLNA "dlna"
#define GUPNP_DIDL_LITE_WRITER_NAMESPACE_PV "pv"

GUPnPDIDLLiteWriter *
gupnp_didl_lite_writer_new              (const char *language);

GUPnPDIDLLiteItem *
gupnp_didl_lite_writer_add_item         (GUPnPDIDLLiteWriter *writer);

GUPnPDIDLLiteContainer *
gupnp_didl_lite_writer_add_container    (GUPnPDIDLLiteWriter *writer);

GUPnPDIDLLiteDescriptor *
gupnp_didl_lite_writer_add_descriptor   (GUPnPDIDLLiteWriter *writer);

xmlNode *
gupnp_didl_lite_writer_get_xml_node     (GUPnPDIDLLiteWriter   *writer);

char *
gupnp_didl_lite_writer_get_string       (GUPnPDIDLLiteWriter   *writer);

const char *
gupnp_didl_lite_writer_get_language     (GUPnPDIDLLiteWriter   *writer);

void
gupnp_didl_lite_writer_filter           (GUPnPDIDLLiteWriter   *writer,
                                         const char            *filter);

G_END_DECLS

#endif /* GUPNP_DIDL_LITE_WRITER_H */
