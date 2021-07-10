// SPDX-License-Identifier: LGPL-2.1-or-later

#include <config.h>

#include "gupnp-didl-lite-extended-resource-private.h"
#include "gupnp-didl-lite-clocksync-resource.h"
#include "xml-util.h"

#include <libxml/tree.h>

struct _GUPnPDIDLLiteClocksyncResource {
        GUPnPDIDLLiteExtendedResource parent;
};

G_DEFINE_TYPE (GUPnPDIDLLiteClocksyncResource,
               gupnp_didl_lite_clocksync_resource,
               GUPNP_TYPE_DIDL_LITE_EXTENDED_RESOURCE)

enum
{
        PROP_0,
        PROP_DEVICE_CLOCK_ID,
        PROP_SUPPORTED_TIMESTAMPS_ID,
        PROP_COUNT
};

static GParamSpec *pspecs[PROP_COUNT];

static void
gupnp_didl_lite_clocksync_resource_init (GUPnPDIDLLiteClocksyncResource *self)
{
        // Nothing
}

static void
set_properties (GObject *object,
                guint property_id,
                const GValue *value,
                GParamSpec *pspec)
{
        GUPnPDIDLLiteClocksyncResource *self =
                GUPNP_DIDL_LITE_CLOCKSYNC_RESOURCE (object);

        switch (property_id) {
        case PROP_DEVICE_CLOCK_ID:
                gupnp_didl_lite_clocksync_resource_set_device_clock_id (
                        self,
                        g_value_get_string (value));
                break;
        case PROP_SUPPORTED_TIMESTAMPS_ID:
                gupnp_didl_lite_clocksync_resource_set_timestamps_id (
                        self,
                        g_value_get_string (value));
                break;
        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
                break;
        }
}

static void
get_properties (GObject *object,
                guint property_id,
                GValue *value,
                GParamSpec *pspec)
{
        GUPnPDIDLLiteClocksyncResource *self =
                GUPNP_DIDL_LITE_CLOCKSYNC_RESOURCE (object);

        switch (property_id) {
        case PROP_DEVICE_CLOCK_ID:
                g_value_set_string (
                        value,
                        gupnp_didl_lite_clocksync_resource_get_device_clock_id (
                                self));
                break;
        case PROP_SUPPORTED_TIMESTAMPS_ID:
                g_value_set_string (
                        value,
                        gupnp_didl_lite_clocksync_resource_get_timestamps_id (
                                self));
                break;
        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
                break;
        }
}

static void
gupnp_didl_lite_clocksync_resource_class_init (
        GUPnPDIDLLiteClocksyncResourceClass *klass)
{
        GObjectClass *object_class = G_OBJECT_CLASS (klass);

        object_class->set_property = set_properties;
        object_class->get_property = get_properties;

        pspecs[PROP_DEVICE_CLOCK_ID] = g_param_spec_string (
                "device-clock-id",
                "device clock id",
                "The clock Id to use for this resource",
                NULL,
                G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);

        pspecs[PROP_SUPPORTED_TIMESTAMPS_ID] = g_param_spec_string (
                "supported-timestamps-id",
                "supported timestamps id",
                "The timestamp id from feature list",
                NULL,
                G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);

        g_object_class_install_properties (object_class, PROP_COUNT, pspecs);
}

const char *
gupnp_didl_lite_clocksync_resource_get_device_clock_id (
        GUPnPDIDLLiteClocksyncResource *self)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_CLOCKSYNC_RESOURCE (self),
                              NULL);

        GUPnPDIDLLiteExtendedResource *res =
                GUPNP_DIDL_LITE_EXTENDED_RESOURCE (self);
        xmlNodePtr node = gupnp_didl_lite_extended_resource_get_xml_node (res);
        return av_xml_util_get_attribute_content (node, "deviceClockInfoID");
}

void
gupnp_didl_lite_clocksync_resource_set_device_clock_id (
        GUPnPDIDLLiteClocksyncResource *self,
        const char *id)
{
        g_return_if_fail (GUPNP_IS_DIDL_LITE_CLOCKSYNC_RESOURCE (self));
        GUPnPDIDLLiteExtendedResource *res =
                GUPNP_DIDL_LITE_EXTENDED_RESOURCE (self);
        xmlNodePtr node = gupnp_didl_lite_extended_resource_get_xml_node (res);

        if (id == NULL) {
                xmlUnsetProp (node, BAD_CAST "deviceClockInfoID");
        } else {
                xmlSetProp (node, BAD_CAST "deviceClockInfoID", BAD_CAST id);
        }
}

const char *
gupnp_didl_lite_clocksync_resource_get_timestamps_id (GUPnPDIDLLiteClocksyncResource *self)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_CLOCKSYNC_RESOURCE (self),
                              NULL);
        GUPnPDIDLLiteExtendedResource *res =
                GUPNP_DIDL_LITE_EXTENDED_RESOURCE (self);
        xmlNodePtr node = gupnp_didl_lite_extended_resource_get_xml_node (res);
        return av_xml_util_get_attribute_content (node, "supportedTimestampsID");
}

void
gupnp_didl_lite_clocksync_resource_set_timestamps_id (GUPnPDIDLLiteClocksyncResource *self, const char *id)
{
        g_return_if_fail (GUPNP_IS_DIDL_LITE_CLOCKSYNC_RESOURCE (self));
        GUPnPDIDLLiteExtendedResource *res =
                GUPNP_DIDL_LITE_EXTENDED_RESOURCE (self);
        xmlNodePtr node = gupnp_didl_lite_extended_resource_get_xml_node (res);

        if (id == NULL) {
                xmlUnsetProp (node, BAD_CAST "supportedTimestampsID");
        } else {
                xmlSetProp (node, BAD_CAST "supportedTimestampsID", BAD_CAST id);
        }
}
