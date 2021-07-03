/*
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */
#include <config.h>

#include <libgupnp-av/gupnp-didl-lite-object.h>
#include <libgupnp-av/gupnp-didl-lite-writer.h>

static void
namespace_getters (void)
{
  GUPnPDIDLLiteWriter *writer = gupnp_didl_lite_writer_new (NULL);
  GUPnPDIDLLiteObject *object = GUPNP_DIDL_LITE_OBJECT (gupnp_didl_lite_writer_add_item (writer));
  xmlNs *namespace;

  namespace = gupnp_didl_lite_object_get_upnp_namespace (object);
  g_assert (namespace != NULL);
  g_assert_cmpstr ((char *) namespace->prefix, ==, "upnp");

  namespace = gupnp_didl_lite_object_get_dlna_namespace (object);
  g_assert (namespace != NULL);
  g_assert_cmpstr ((char *) namespace->prefix, ==, "dlna");


  namespace = gupnp_didl_lite_object_get_dc_namespace (object);
  g_assert (namespace != NULL);
  g_assert_cmpstr ((char *) namespace->prefix, ==, "dc");

  namespace = gupnp_didl_lite_object_get_pv_namespace (object);
  g_assert (namespace != NULL);
  g_assert_cmpstr ((char *) namespace->prefix, ==, "pv");
}

int
main (int argc, char **argv)
{
  g_test_init (&argc, &argv, NULL);

  g_test_add_func ("/didl-lite-object/namespace-getters", namespace_getters);

  g_test_run ();

  return 0;
}
