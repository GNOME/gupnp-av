#include "gupnp-feature-container-shortcuts.h"
#include "gupnp-feature-private.h"
#include "xml-util.h"

#include <libxml/tree.h>

struct _GUPnPContainerShortcut {
        GUPnPAVXMLDoc *doc;
        xmlNodePtr node;
};


struct _GUPnPFeatureContainerShortcuts {
        GUPnPFeature parent;
};

G_DEFINE_TYPE (GUPnPFeatureContainerShortcuts,
               gupnp_feature_container_shortcuts,
               GUPNP_TYPE_FEATURE)

static void
gupnp_feature_container_shortcuts_init (GUPnPFeatureContainerShortcuts *self)
{
}

static void
gupnp_feature_container_shortcuts_class_init (
        GUPnPFeatureContainerShortcutsClass *klass)
{
}

/**
 * gupnp_feature_container_shortcuts_get_list:
 *
 * Get the supported shortcuts on that feature.
 *
 * Returns: (element-type: GUPnPContainerShortcut): List
 * of shortcuts included in this feature.
 */
GSList *
gupnp_feature_container_shortcuts_get_list (
        GUPnPFeatureContainerShortcuts *self)
{
        xmlNode *node = gupnp_feature_get_node (GUPNP_FEATURE (self));
        xmlNode *shortcuts_node =
                av_xml_util_get_element (node, "shortcutlist", NULL);

        if (shortcuts_node == NULL) {
                return NULL;
        }

        GList *shortcuts =
                av_xml_util_get_child_elements_by_name (shortcuts_node,
                                                        "shortcut");
        GList *it = shortcuts;
        GSList *retval = NULL;

        while (it != NULL) {
                GUPnPContainerShortcut *shortcut =
                        g_rc_box_new0 (GUPnPContainerShortcut);

                shortcut->doc = gupnp_feature_get_doc (GUPNP_FEATURE (self));
                shortcut->node = it->data;

                retval = g_slist_prepend (retval, shortcut);
                it = it->next;
        }

        return g_slist_reverse(retval);
}

/**
 * gupnp_container_shortcut_type:
 *
 * Get the gtype for GUPnPServiceActon
 *
 * Return value: The gtype of GUPnPServiceAction
 **/
GType
gupnp_container_shortcut_get_type (void)
{
        static GType our_type = 0;

        if (our_type == 0)
                our_type = g_boxed_type_register_static (
                        "GUPnPServiceAction",
                        (GBoxedCopyFunc) gupnp_container_shortcut_ref,
                        (GBoxedFreeFunc) gupnp_container_shortcut_unref);

        return our_type;
}

GUPnPContainerShortcut *
gupnp_container_shortcut_ref (GUPnPContainerShortcut *shortcut)
{
        return g_rc_box_acquire (shortcut);
}

static void
gupnp_container_shortcut_free (GUPnPContainerShortcut *shortcut)
{
        g_clear_pointer (&shortcut->doc, av_xml_doc_unref);
}

void
gupnp_container_shortcut_unref (GUPnPContainerShortcut *shortcut)
{
        g_rc_box_release_full (shortcut,
                               (GDestroyNotify) gupnp_container_shortcut_free);
}


const char *
gupnp_container_shortcut_get_name (GUPnPContainerShortcut *shortcut)
{
        return av_xml_util_get_child_element_content (shortcut->node, "name");
}

const char *
gupnp_container_shortcut_get_object_id (GUPnPContainerShortcut *shortcut)
{
        return av_xml_util_get_child_element_content (shortcut->node,
                                                      "objectID");
}
