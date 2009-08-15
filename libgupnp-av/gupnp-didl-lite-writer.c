/*
 * Copyright (C) 2007, 2008 OpenedHand Ltd.
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

/**
 * SECTION:gupnp-didl-lite-writer
 * @short_description: DIDL-Lite fragment writer
 *
 * #GUPnPDIDLLiteWriter is a helper class for writing DIDL-Lite fragments.
 */

#include <string.h>

#include "gupnp-didl-lite-writer.h"
#include "gupnp-didl-lite-object.h"
#include "gupnp-dlna-private.h"

G_DEFINE_TYPE (GUPnPDIDLLiteWriter,
               gupnp_didl_lite_writer,
               G_TYPE_OBJECT);

#define INITIAL_STRING_SIZE 200

struct _GUPnPDIDLLiteWriterPrivate {
        GString *str;

        char *language;
};

enum {
        PROP_0,
        PROP_LANGUAGE,
};

static void
gupnp_didl_lite_writer_init (GUPnPDIDLLiteWriter *writer)
{
        writer->priv = G_TYPE_INSTANCE_GET_PRIVATE (writer,
                                                    GUPNP_TYPE_DIDL_LITE_WRITER,
                                                    GUPnPDIDLLiteWriterPrivate);
}

static void
gupnp_didl_lite_writer_set_property (GObject      *object,
                                     guint         property_id,
                                     const GValue *value,
                                     GParamSpec   *pspec)

{
        GUPnPDIDLLiteWriter *writer;

        writer = GUPNP_DIDL_LITE_WRITER (object);

        switch (property_id) {
        case PROP_LANGUAGE:
                writer->priv->language = g_value_dup_string (value);
                break;
        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
                break;
        }
}

static void
gupnp_didl_lite_writer_get_property (GObject    *object,
                                     guint       property_id,
                                     GValue     *value,
                                     GParamSpec *pspec)
{
        GUPnPDIDLLiteWriter *writer;

        writer = GUPNP_DIDL_LITE_WRITER (object);

        switch (property_id) {
        case PROP_LANGUAGE:
                g_value_take_string
                        (value, gupnp_didl_lite_writer_get_language (writer));
                break;
        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
                break;
        }
}

static void
gupnp_didl_lite_writer_finalize (GObject *object)
{
        GObjectClass        *object_class;
        GUPnPDIDLLiteWriter *writer;

        writer = GUPNP_DIDL_LITE_WRITER (object);

        if (writer->priv->str)
                g_string_free (writer->priv->str, TRUE);

        if (writer->priv->language)
                g_free (writer->priv->language);

        object_class = G_OBJECT_CLASS (gupnp_didl_lite_writer_parent_class);
        object_class->finalize (object);
}

static void
gupnp_didl_lite_writer_class_init (GUPnPDIDLLiteWriterClass *klass)
{
        GObjectClass *object_class;

        object_class = G_OBJECT_CLASS (klass);

        object_class->set_property = gupnp_didl_lite_writer_set_property;
        object_class->get_property = gupnp_didl_lite_writer_get_property;
        object_class->finalize = gupnp_didl_lite_writer_finalize;

        g_type_class_add_private (klass, sizeof (GUPnPDIDLLiteWriterPrivate));

        /**
         * GUPnPDIDLLiteWriter:language
         *
         * The language the DIDL-Lite fragment is in.
         *
         **/
        g_object_class_install_property
                (object_class,
                 PROP_LANGUAGE,
                 g_param_spec_string ("language",
                                      "Language",
                                      "The language the DIDL-Lite fragment"
                                      " is in.",
                                      NULL,
                                      G_PARAM_CONSTRUCT_ONLY |
                                      G_PARAM_READWRITE |
                                      G_PARAM_STATIC_NAME |
                                      G_PARAM_STATIC_NICK |
                                      G_PARAM_STATIC_BLURB));
}

/**
 * gupnp_didl_lite_writer_new
 * @language: The language the DIDL-Lite fragment is in, or NULL
 *
 * Return value: A new #GUPnPDIDLLiteWriter object.
 **/
GUPnPDIDLLiteWriter *
gupnp_didl_lite_writer_new (const char *language)
{
        return g_object_new (GUPNP_TYPE_DIDL_LITE_WRITER,
                             "language", language,
                             NULL);
}

#define DIDL_LITE_HEADER \
        "<DIDL-Lite xmlns:" GUPNP_DIDL_LITE_WRITER_NAMESPACE_DC \
        "=\"http://purl.org/dc/elements/1.1/\" " \
        "xmlns:" GUPNP_DIDL_LITE_WRITER_NAMESPACE_UPNP \
        "=\"urn:schemas-upnp-org:metadata-1-0/upnp/\" " \
        "xmlns=\"urn:schemas-upnp-org:metadata-1-0/DIDL-Lite/\""

/**
 * gupnp_didl_lite_writer_start_didl_lite
 * @writer: A #GUPnPDIDLLiteWriter
 *
 * Starts the DIDL-Lite element.
 **/
void
gupnp_didl_lite_writer_start_didl_lite (GUPnPDIDLLiteWriter *writer)
{
        g_return_if_fail (GUPNP_IS_DIDL_LITE_WRITER (writer));
        g_return_if_fail (writer->priv->str == NULL);

        writer->priv->str = g_string_sized_new (INITIAL_STRING_SIZE);

        g_string_append (writer->priv->str, DIDL_LITE_HEADER);

        if (writer->priv->language) {
                g_string_append (writer->priv->str, " lang=\"");
                g_string_append (writer->priv->str, writer->priv->language);
                g_string_append_c (writer->priv->str, '"');
        }

        g_string_append_c (writer->priv->str, '>');
}

/**
 * gupnp_didl_lite_writer_end_didl_lite
 * @writer: A #GUPnPDIDLLiteWriter
 *
 * Closes the DIDL-Lite element.
 **/
void
gupnp_didl_lite_writer_end_didl_lite (GUPnPDIDLLiteWriter *writer)
{
        g_return_if_fail (GUPNP_IS_DIDL_LITE_WRITER (writer));
        g_return_if_fail (writer->priv->str);

        g_string_append (writer->priv->str, "</DIDL-Lite>");
}

/**
 * gupnp_didl_lite_writer_add_object
 * @writer: A #GUPnPDIDLLiteWriter
 * @object: A #GUPnPDIDLLiteObject
 *
 * Adds @object to element.
 **/
void
gupnp_didl_lite_writer_add_object (GUPnPDIDLLiteWriter *writer,
                                   GUPnPDIDLLiteObject *object)
{
        char *str;

        g_return_if_fail (GUPNP_IS_DIDL_LITE_WRITER (writer));
        g_return_if_fail (GUPNP_IS_DIDL_LITE_OBJECT (object));

        str = gupnp_didl_lite_object_to_string (object);
        g_string_append (writer->priv->str, str);
        g_free (str);
}

/**
 * gupnp_didl_lite_writer_add_descriptor
 * @writer: A #GUPnPDIDLLiteWriter
 * @descriptor: The #GUPnPDIDLLiteDescriptor to add
 *
 * Adds the @descriptor to element.
 **/
void
gupnp_didl_lite_writer_add_desc (GUPnPDIDLLiteWriter     *writer,
                                 GUPnPDIDLLiteDescriptor *descriptor)
{
        char *desc;

        g_return_if_fail (GUPNP_IS_DIDL_LITE_WRITER (writer));
        g_return_if_fail (GUPNP_IS_DIDL_LITE_DESCRIPTOR (descriptor));
        g_return_if_fail (writer->priv->str);

        desc = gupnp_didl_lite_descriptor_to_string (descriptor);
        g_return_if_fail (desc != NULL);

        g_string_append (writer->priv->str, desc);

        g_free (desc);
}

/**
 * gupnp_didl_lite_writer_get_string
 * @writer: A #GUPnPDIDLLiteWriter
 *
 * Returns a pointer the generated DIDL-Lite string.
 *
 * Return value: The generated DIDL-Lite string.
 **/
const char *
gupnp_didl_lite_writer_get_string (GUPnPDIDLLiteWriter *writer)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_WRITER (writer), NULL);
        g_return_val_if_fail (writer->priv->str, NULL);

        return writer->priv->str->str;
}

/**
 * gupnp_didl_lite_writer_get_language
 * @writer: #GUPnPDIDLLiteWriter
 *
 * Get the language the DIDL-Lite fragment is in.
 *
 * Return value: The language of the @writer, or %NULL. #g_free after
 * usage.
 **/
char *
gupnp_didl_lite_writer_get_language (GUPnPDIDLLiteWriter *writer)
{
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_WRITER (writer), NULL);

        return g_strdup (writer->priv->language);
}
