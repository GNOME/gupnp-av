/*
 * Copyright (C) 2007 OpenedHand Ltd
 *
 * Authors: Jorn Baayen <jorn@openedhand.com>
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

#include "gupnp-didl-lite-writer.h"

G_DEFINE_TYPE (GUPnPDIDLLiteWriter,
               gupnp_didl_lite_writer,
               G_TYPE_OBJECT);

#define INITIAL_STRING_SIZE 200

struct _GUPnPDIDLLiteWriterPrivate {
        GString *str;

        SoupUri *base_url;
};

static void
gupnp_didl_lite_writer_init (GUPnPDIDLLiteWriter *writer)
{
        writer->priv = G_TYPE_INSTANCE_GET_PRIVATE (writer,
                                                    GUPNP_TYPE_DIDL_LITE_WRITER,
                                                    GUPnPDIDLLiteWriterPrivate);
}

static void
gupnp_didl_lite_writer_finalize (GObject *object)
{
        GObjectClass        *object_class;
        GUPnPDIDLLiteWriter *writer;

        writer = GUPNP_DIDL_LITE_WRITER (object);

        if (writer->priv->str)
                g_string_free (writer->priv->str, TRUE);

        object_class = G_OBJECT_CLASS (gupnp_didl_lite_writer_parent_class);
        object_class->dispose (object);
}

static void
gupnp_didl_lite_writer_class_init (GUPnPDIDLLiteWriterClass *klass)
{
        GObjectClass *object_class;

        object_class = G_OBJECT_CLASS (klass);

        object_class->finalize = gupnp_didl_lite_writer_finalize;

        g_type_class_add_private (klass, sizeof (GUPnPDIDLLiteWriterPrivate));
}

GUPnPDIDLLiteWriter *
gupnp_didl_lite_writer_new (void)
{
        return g_object_new (GUPNP_TYPE_DIDL_LITE_WRITER, NULL);
}

void
gupnp_didl_lite_writer_start_didl_lite (GUPnPDIDLLiteWriter *writer,
                                        const char          *lang,
                                        SoupUri             *url_base)
{
        g_return_if_fail (GUPNP_IS_DIDL_LITE_WRITER (writer));

        writer->priv->str = g_string_sized_new (INITIAL_STRING_SIZE);
}

void
gupnp_didl_lite_writer_end_didl_lite (GUPnPDIDLLiteWriter *writer)
{
        g_return_if_fail (GUPNP_IS_DIDL_LITE_WRITER (writer));
        g_return_if_fail (writer->priv->str);
}

void
gupnp_didl_lite_writer_start_container (GUPnPDIDLLiteWriter *writer,
                                        const char          *id,
                                        const char          *parent_id,
                                        gboolean             restricted,
                                        gboolean             searchable)
{
        g_return_if_fail (GUPNP_IS_DIDL_LITE_WRITER (writer));
        g_return_if_fail (writer->priv->str);
        g_return_if_fail (id != NULL);
        g_return_if_fail (parent_id != NULL);
}

void
gupnp_didl_lite_writer_end_container (GUPnPDIDLLiteWriter *writer)
{
        g_return_if_fail (GUPNP_IS_DIDL_LITE_WRITER (writer));
        g_return_if_fail (writer->priv->str);
}

void
gupnp_didl_lite_writer_start_item (GUPnPDIDLLiteWriter *writer,
                                   const char          *id,
                                   const char          *parent_id,
                                   gboolean             restricted)
{
        g_return_if_fail (GUPNP_IS_DIDL_LITE_WRITER (writer));
        g_return_if_fail (writer->priv->str);
        g_return_if_fail (id != NULL);
        g_return_if_fail (parent_id != NULL);
}

void
gupnp_didl_lite_writer_end_item (GUPnPDIDLLiteWriter *writer)
{
        g_return_if_fail (GUPNP_IS_DIDL_LITE_WRITER (writer));
        g_return_if_fail (writer->priv->str);
}

void
gupnp_didl_lite_writer_add_res (GUPnPDIDLLiteWriter   *writer,
                                GUPnPDIDLLiteResource *res)
{
        g_return_if_fail (GUPNP_IS_DIDL_LITE_WRITER (writer));
        g_return_if_fail (writer->priv->str);
        g_return_if_fail (res != NULL);
        g_return_if_fail (res->protocol_info != NULL);
}

void
gupnp_didl_lite_writer_add_desc (GUPnPDIDLLiteWriter *writer,
                                 const char          *desc,
                                 const char          *id,
                                 const char          *name,
                                 const char          *ns_uri)
{
        g_return_if_fail (GUPNP_IS_DIDL_LITE_WRITER (writer));
        g_return_if_fail (writer->priv->str);
}

void
gupnp_didl_lite_writer_add_string (GUPnPDIDLLiteWriter *writer,
                                   const char          *property,
                                   const char          *prefix,
                                   const char          *ns_uri,
                                   const char          *value)
{
        g_return_if_fail (GUPNP_IS_DIDL_LITE_WRITER (writer));
        g_return_if_fail (writer->priv->str);
        g_return_if_fail (property != NULL);
}

void
gupnp_didl_lite_writer_add_string_with_attrs (GUPnPDIDLLiteWriter *writer,
                                              const char          *property,
                                              const char          *prefix,
                                              const char          *ns_uri,
                                              const char          *value,
                                              ...)
{
        va_list var_args;

        g_return_if_fail (GUPNP_IS_DIDL_LITE_WRITER (writer));
        g_return_if_fail (writer->priv->str);
        g_return_if_fail (property != NULL);

        va_start (var_args, value);
        gupnp_didl_lite_writer_add_string_with_attrs_valist (writer,
                                                             property,
                                                             prefix,
                                                             ns_uri,
                                                             value,
                                                             var_args);
        va_end (var_args);
}

void
gupnp_didl_lite_writer_add_string_with_attrs_valist
                                             (GUPnPDIDLLiteWriter *writer,
                                              const char          *property,
                                              const char          *prefix,
                                              const char          *ns_uri,
                                              const char          *value,
                                              va_list              var_args)
{
        g_return_if_fail (GUPNP_IS_DIDL_LITE_WRITER (writer));
        g_return_if_fail (writer->priv->str);
        g_return_if_fail (property != NULL);
}

void
gupnp_didl_lite_writer_add_boolean (GUPnPDIDLLiteWriter *writer,
                                    const char          *property,
                                    const char          *prefix,
                                    const char          *ns_uri,
                                    gboolean             value)
{
        g_return_if_fail (GUPNP_IS_DIDL_LITE_WRITER (writer));
        g_return_if_fail (writer->priv->str);
        g_return_if_fail (property != NULL);
}

void
gupnp_didl_lite_writer_add_int (GUPnPDIDLLiteWriter *writer,
                                const char          *property,
                                const char          *prefix,
                                const char          *ns_uri,
                                int                  value)
{
        g_return_if_fail (GUPNP_IS_DIDL_LITE_WRITER (writer));
        g_return_if_fail (writer->priv->str);
        g_return_if_fail (property != NULL);
}

void
gupnp_didl_lite_writer_add_uint (GUPnPDIDLLiteWriter *writer,
                                 const char          *property,
                                 const char          *prefix,
                                 const char          *ns_uri,
                                 guint                value)
{
        g_return_if_fail (GUPNP_IS_DIDL_LITE_WRITER (writer));
        g_return_if_fail (writer->priv->str);
        g_return_if_fail (property != NULL);
}

void
gupnp_didl_lite_writer_add_long (GUPnPDIDLLiteWriter *writer,
                                 const char          *property,
                                 const char          *prefix,
                                 const char          *ns_uri,
                                 long                 value)
{
        g_return_if_fail (GUPNP_IS_DIDL_LITE_WRITER (writer));
        g_return_if_fail (writer->priv->str);
        g_return_if_fail (property != NULL);
}

void
gupnp_didl_lite_writer_add_ulong (GUPnPDIDLLiteWriter *writer,
                                  const char          *property,
                                  const char          *prefix,
                                  const char          *ns_uri,
                                  gulong               value)
{
        g_return_if_fail (GUPNP_IS_DIDL_LITE_WRITER (writer));
        g_return_if_fail (writer->priv->str);
        g_return_if_fail (property != NULL);
}

void
gupnp_didl_lite_writer_set_value_and_reset (GUPnPDIDLLiteWriter *writer,
                                            GValue              *value)
{
        g_return_if_fail (GUPNP_IS_DIDL_LITE_WRITER (writer));
        g_return_if_fail (writer->priv->str);
        g_return_if_fail (value != NULL);

        g_value_set_string_take_ownership (value, writer->priv->str->str);

        g_string_free (writer->priv->str, FALSE);
        writer->priv->str = NULL;
}

char *
gupnp_didl_lite_writer_get_string_and_reset (GUPnPDIDLLiteWriter *writer)
{
        char *ret;

        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_WRITER (writer), NULL);
        g_return_val_if_fail (writer->priv->str, NULL);

        ret = writer->priv->str->str;

        g_string_free (writer->priv->str, FALSE);
        writer->priv->str = NULL;

        return ret;
}
