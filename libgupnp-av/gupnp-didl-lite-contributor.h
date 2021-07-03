/*
 * Copyright (C) 2009 Nokia Corporation.
 *
 * Authors: Zeeshan Ali (Khattak) <zeeshan.ali@nokia.com>
 *                                <zeeshanak@gnome.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */

#ifndef GUPNP_DIDL_LITE_CONTRIBUTOR_H
#define GUPNP_DIDL_LITE_CONTRIBUTOR_H

#include <glib-object.h>
#include <libxml/tree.h>

G_BEGIN_DECLS

G_DECLARE_DERIVABLE_TYPE(GUPnPDIDLLiteContributor,
                         gupnp_didl_lite_contributor,
                         GUPNP,
                         DIDL_LITE_CONTRIBUTOR,
                         GObject)

#define GUPNP_TYPE_DIDL_LITE_CONTRIBUTOR \
                (gupnp_didl_lite_contributor_get_type ())

struct _GUPnPDIDLLiteContributorClass{
        GObjectClass parent_class;

        /* future padding */
        void (* _gupnp_reserved1) (void);
        void (* _gupnp_reserved2) (void);
        void (* _gupnp_reserved3) (void);
        void (* _gupnp_reserved4) (void);
};

const char *
gupnp_didl_lite_contributor_get_role    (GUPnPDIDLLiteContributor *contributor);

const char *
gupnp_didl_lite_contributor_get_name    (GUPnPDIDLLiteContributor *contributor);

void
gupnp_didl_lite_contributor_set_role    (GUPnPDIDLLiteContributor *contributor,
                                         const char               *role);

void
gupnp_didl_lite_contributor_set_name    (GUPnPDIDLLiteContributor *contributor,
                                         const char               *name);

xmlNode *
gupnp_didl_lite_contributor_get_xml_node
                                        (GUPnPDIDLLiteContributor *contributor);

G_END_DECLS

#endif /* __GUPNP_DIDL_LITE_CONTRIBUTOR_H__ */
