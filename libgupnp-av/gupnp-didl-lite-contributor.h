/*
 * Copyright (C) 2009 Nokia Corporation.
 *
 * Authors: Zeeshan Ali (Khattak) <zeeshan.ali@nokia.com>
 *                                <zeeshanak@gnome.org>
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef __GUPNP_DIDL_LITE_CONTRIBUTOR_H__
#define __GUPNP_DIDL_LITE_CONTRIBUTOR_H__

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
