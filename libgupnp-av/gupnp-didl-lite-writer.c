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
#include "gupnp-dlna-private.h"

G_DEFINE_TYPE (GUPnPDIDLLiteWriter,
               gupnp_didl_lite_writer,
               G_TYPE_OBJECT);

#define INITIAL_STRING_SIZE 200

struct _GUPnPDIDLLiteWriterPrivate {
        GString *str;

        SoupURI *url_base;

        gboolean need_escape;
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
        object_class->finalize (object);
}

static void
gupnp_didl_lite_writer_class_init (GUPnPDIDLLiteWriterClass *klass)
{
        GObjectClass *object_class;

        object_class = G_OBJECT_CLASS (klass);

        object_class->finalize = gupnp_didl_lite_writer_finalize;

        g_type_class_add_private (klass, sizeof (GUPnPDIDLLiteWriterPrivate));
}

/**
 * gupnp_didl_lite_writer_new
 *
 * Return value: A new #GUPnPDIDLLiteWriter object.
 **/
GUPnPDIDLLiteWriter *
gupnp_didl_lite_writer_new (void)
{
        return g_object_new (GUPNP_TYPE_DIDL_LITE_WRITER, NULL);
}

/* Modified from GLib gmarkup.c */
static void
append_escaped_text (GUPnPDIDLLiteWriter *writer,
                     const gchar         *text)    
{
  const gchar *p;

  p = text;

  while (*p)
    {
      const gchar *next;
      next = g_utf8_next_char (p);

      switch (*p)
        {
        case '&':
          g_string_append (writer->priv->str, "&amp;");
          break;

        case '<':
          g_string_append (writer->priv->str, "&lt;");
          break;

        case '>':
          g_string_append (writer->priv->str, "&gt;");
          break;

        case '"':
          g_string_append (writer->priv->str, "&quot;");
          break;

        default:
          g_string_append_len (writer->priv->str, p, next - p);
          break;
        }

      p = next;
    }
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
 * @lang: The language the DIDL-Lite fragment is in, or NULL
 * @url_base: A #SoupURI specifying the URI relative to which incoming URI
 * are resolved, or NULL
 * @need_escape: TRUE to force @writer to escape incoming string data
 *
 * Starts the DIDL-Lite element.
 **/
void
gupnp_didl_lite_writer_start_didl_lite (GUPnPDIDLLiteWriter *writer,
                                        const char          *lang,
                                        SoupURI             *url_base,
                                        gboolean             need_escape)
{
        g_return_if_fail (GUPNP_IS_DIDL_LITE_WRITER (writer));
        g_return_if_fail (writer->priv->str == NULL);

        writer->priv->url_base    = url_base;
        writer->priv->need_escape = need_escape;

        writer->priv->str = g_string_sized_new (INITIAL_STRING_SIZE);

        g_string_append (writer->priv->str, DIDL_LITE_HEADER);

        if (lang) {
                g_string_append (writer->priv->str, " lang=\"");
                g_string_append (writer->priv->str, lang);
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

static void
begin_property (GUPnPDIDLLiteWriter *writer,
                const char          *property,
                const char          *prefix,
                const char          *ns_uri)
{
        g_string_append_c (writer->priv->str, '<');

        if (prefix) {
                g_string_append (writer->priv->str, prefix);
                g_string_append_c (writer->priv->str, ':');
        }

        g_string_append (writer->priv->str, property);
        
        if (ns_uri) {
                g_string_append (writer->priv->str, " xmlns:");
                g_string_append (writer->priv->str, prefix);
                g_string_append (writer->priv->str, "=\"");

                if (writer->priv->url_base || writer->priv->need_escape) {
                        SoupURI *uri;
                        char *uri_str;

                        uri = soup_uri_new_with_base (writer->priv->url_base,
                                                      ns_uri);
                        uri_str = soup_uri_to_string (uri, FALSE);
                        soup_uri_free (uri);

                        g_string_append (writer->priv->str, uri_str);

                        g_free (uri_str);
                } else
                        g_string_append (writer->priv->str, ns_uri);

                g_string_append_c (writer->priv->str, '"');
        }
}

static void
begin_property_simple (GUPnPDIDLLiteWriter *writer,
                       const char          *property,
                       const char          *prefix,
                       const char          *ns_uri)
{
        begin_property (writer, property, prefix, ns_uri);

        g_string_append_c (writer->priv->str, '>');
}

static void
end_property (GUPnPDIDLLiteWriter *writer,
              const char          *property,
              const char          *prefix)
{
        g_string_append (writer->priv->str, "</");

        if (prefix) {
                g_string_append (writer->priv->str, prefix);
                g_string_append_c (writer->priv->str, ':');
        }

        g_string_append (writer->priv->str, property);
        g_string_append_c (writer->priv->str, '>');
}

/**
 * gupnp_didl_lite_writer_add_string
 * @writer: A #GUPnPDIDLLiteWriter
 * @property: The property name
 * @prefix: The property namespace prefix, or NULL
 * @ns_uri: The namespace URI to be used, or NULL
 * @value: The property value
 *
 * Adds a new string property.
 **/
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

        begin_property_simple (writer, property, prefix, ns_uri);

        if (value) {
                if (writer->priv->need_escape)
                        append_escaped_text (writer, value);
                else
                        g_string_append (writer->priv->str, value);
        }

        end_property (writer, property, prefix);
}

/**
 * gupnp_didl_lite_writer_add_string_with_attrs
 * @writer: A #GUPnPDIDLLiteWriter
 * @property: The property name
 * @prefix: The property namespace prefix, or NULL
 * @ns_uri: The namespace URI to be used, or NULL
 * @value: The property value
 * @Varargs: A NULL terminated list of attribute name-attribute value string
 * pairs.
 *
 * Adds a new string property with attributes.
 **/
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

/**
 * gupnp_didl_lite_writer_add_string_with_attrs_valist
 * @writer: A #GUPnPDIDLLiteWriter
 * @property: The property name
 * @prefix: The property namespace prefix, or NULL
 * @ns_uri: The namespace URI to be used, or NULL
 * @value: The property value
 * @var_args: A #va_list with attribute name-attribute value string pairs
 *
 * Adds a new string property with attributes.
 **/
void
gupnp_didl_lite_writer_add_string_with_attrs_valist
                                             (GUPnPDIDLLiteWriter *writer,
                                              const char          *property,
                                              const char          *prefix,
                                              const char          *ns_uri,
                                              const char          *value,
                                              va_list              var_args)
{
        const char *attr_name, *attr_value;

        g_return_if_fail (GUPNP_IS_DIDL_LITE_WRITER (writer));
        g_return_if_fail (writer->priv->str);
        g_return_if_fail (property != NULL);

        begin_property (writer, property, prefix, ns_uri);

        attr_name = va_arg (var_args, const char *);
        while (attr_name) {
                if ((attr_value = va_arg (var_args, const char *)) == NULL)
                        break;

                g_string_append_c (writer->priv->str, ' ');
                g_string_append (writer->priv->str, attr_name);
                g_string_append (writer->priv->str, "=\"");

                if (writer->priv->need_escape)
                        append_escaped_text (writer, attr_value);
                else
                        g_string_append (writer->priv->str, attr_value);

                g_string_append_c (writer->priv->str, '"');

                attr_name = va_arg (var_args, const char *);
        }

        g_string_append_c (writer->priv->str, '>');

        if (value)
                g_string_append (writer->priv->str, value);

        end_property (writer, property, prefix);
}

/**
 * gupnp_didl_lite_writer_add_boolean
 * @writer: A #GUPnPDIDLLiteWriter
 * @property: The property name
 * @prefix: The property namespace prefix, or NULL
 * @ns_uri: The namespace URI to be used, or NULL
 * @value: The property value
 *
 * Adds a new boolean property.
 **/
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

        begin_property_simple (writer, property, prefix, ns_uri);

        g_string_append_c (writer->priv->str, value ? '1' : '0');

        end_property (writer, property, prefix);
}

/**
 * gupnp_didl_lite_writer_add_int
 * @writer: A #GUPnPDIDLLiteWriter
 * @property: The property name
 * @prefix: The property namespace prefix, or NULL
 * @ns_uri: The namespace URI to be used, or NULL
 * @value: The property value
 *
 * Adds a new integer property.
 **/
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

        begin_property_simple (writer, property, prefix, ns_uri);

        g_string_append_printf (writer->priv->str, "%d", value);

        end_property (writer, property, prefix);
}

/**
 * gupnp_didl_lite_writer_add_uint
 * @writer: A #GUPnPDIDLLiteWriter
 * @property: The property name
 * @prefix: The property namespace prefix, or NULL
 * @ns_uri: The namespace URI to be used, or NULL
 * @value: The property value
 *
 * Adds a new unsigned integer property.
 **/
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

        begin_property_simple (writer, property, prefix, ns_uri);

        g_string_append_printf (writer->priv->str, "%u", value);

        end_property (writer, property, prefix);
}

/**
 * gupnp_didl_lite_writer_add_long
 * @writer: A #GUPnPDIDLLiteWriter
 * @property: The property name
 * @prefix: The property namespace prefix, or NULL
 * @ns_uri: The namespace URI to be used, or NULL
 * @value: The property value
 *
 * Adds a new long integer property.
 **/
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

        begin_property_simple (writer, property, prefix, ns_uri);

        g_string_append_printf (writer->priv->str, "%ld", value);

        end_property (writer, property, prefix);
}

/**
 * gupnp_didl_lite_writer_add_ulong
 * @writer: A #GUPnPDIDLLiteWriter
 * @property: The property name
 * @prefix: The property namespace prefix, or NULL
 * @ns_uri: The namespace URI to be used, or NULL
 * @value: The property value
 *
 * Adds a new unsigned long integer property.
 **/
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

        begin_property_simple (writer, property, prefix, ns_uri);

        g_string_append_printf (writer->priv->str, "%lu", value);

        end_property (writer, property, prefix);
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
 * gupnp_didl_lite_writer_reset
 * @writer: A #GUPnPDIDLLiteWriter
 *
 * Resets @writer.
 **/
void
gupnp_didl_lite_writer_reset (GUPnPDIDLLiteWriter *writer)
{
        g_return_if_fail (GUPNP_IS_DIDL_LITE_WRITER (writer));
        g_return_if_fail (writer->priv->str);

        g_string_free (writer->priv->str, FALSE);
        writer->priv->str = NULL;
}
