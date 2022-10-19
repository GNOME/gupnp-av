// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: 2022 Jens Georg <mail@jensge.org>

#pragma once

#include <config.h>

#include "gupnp-feature.h"

#include <glib-object.h>

G_BEGIN_DECLS

#define GUPNP_TYPE_FEATURE_CONTAINER_SHORTCUTS                                 \
        (gupnp_feature_container_shortcuts_get_type ())

G_DECLARE_FINAL_TYPE (GUPnPFeatureContainerShortcuts,
                      gupnp_feature_container_shortcuts,
                      GUPNP,
                      FEATURE_CONTAINER_SHORTCUTS,
                      GUPnPFeature)

typedef struct _GUPnPContainerShortcut GUPnPContainerShortcut;

#define GUPNP_TYPE_CONTAINER_SHORTCUT (gupnp_container_shortcut_get_type ())

struct _GUPnPFeatureContainerShortcutsClass {
        GUPnPFeature parent_class;
};

GSList *gupnp_feature_container_shortcuts_get_list(
        GUPnPFeatureContainerShortcuts *self);

GType
gupnp_container_shortcut_get_type (void);

GUPnPContainerShortcut *
gupnp_container_shortcut_ref (GUPnPContainerShortcut *);

void
gupnp_container_shortcut_unref (GUPnPContainerShortcut *);

G_DEFINE_AUTOPTR_CLEANUP_FUNC (GUPnPContainerShortcut,
                               gupnp_container_shortcut_unref)

const char *
gupnp_container_shortcut_get_name (GUPnPContainerShortcut *);

const char *
gupnp_container_shortcut_get_object_id (GUPnPContainerShortcut *);

G_END_DECLS
