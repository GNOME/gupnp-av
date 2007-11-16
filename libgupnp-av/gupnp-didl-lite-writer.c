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
 *
 * TODO
 *  - docs
 */

#include <string.h>

#include "gupnp-didl-lite-writer.h"

G_DEFINE_TYPE (GUPnPDIDLLiteWriter,
               gupnp_didl_lite_writer,
               G_TYPE_OBJECT);

#define INITIAL_STRING_SIZE 200

struct _GUPnPDIDLLiteWriterPrivate {
        GString *str;

        SoupUri *base_url;

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
        "=\"http://purl.org/dc/elements/1.1/\"" \
        "xmlns:" GUPNP_DIDL_LITE_WRITER_NAMESPACE_UPNP \
        "=\"urn:schemas-upnp-org:metadata-1-0/upnp/\"" \
        "xmlns=\"n:schemas-upnp-org:metadata-1-0/DIDL-Lite/\""

void
gupnp_didl_lite_writer_start_didl_lite (GUPnPDIDLLiteWriter *writer,
                                        const char          *lang,
                                        SoupUri             *url_base,
                                        gboolean             need_escape)
{
        g_return_if_fail (GUPNP_IS_DIDL_LITE_WRITER (writer));

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

void
gupnp_didl_lite_writer_end_didl_lite (GUPnPDIDLLiteWriter *writer)
{
        g_return_if_fail (GUPNP_IS_DIDL_LITE_WRITER (writer));
        g_return_if_fail (writer->priv->str);

        g_string_append (writer->priv->str, "</DIDL-Lite>");
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

        g_string_append (writer->priv->str, "<container id=\"");

        if (writer->priv->need_escape)
                append_escaped_text (writer, id);
        else
                g_string_append (writer->priv->str, id);

        g_string_append (writer->priv->str, "\" parentID=\"");

        if (writer->priv->need_escape)
                append_escaped_text (writer, parent_id);
        else
                g_string_append (writer->priv->str, parent_id);

        g_string_append (writer->priv->str, "\" restricted=\"");
        g_string_append (writer->priv->str, restricted ? "true" : "false");
        g_string_append (writer->priv->str, "\" searchable=\"");
        g_string_append (writer->priv->str, searchable ? "true" : "false");
        g_string_append (writer->priv->str, "\">");
}

void
gupnp_didl_lite_writer_end_container (GUPnPDIDLLiteWriter *writer)
{
        g_return_if_fail (GUPNP_IS_DIDL_LITE_WRITER (writer));
        g_return_if_fail (writer->priv->str);

        g_string_append (writer->priv->str, "</container>");
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

        g_string_append (writer->priv->str, "<item id=\"");

        if (writer->priv->need_escape)
                append_escaped_text (writer, id);
        else
                g_string_append (writer->priv->str, id);

        g_string_append (writer->priv->str, "\" parentID=\"");

        if (writer->priv->need_escape)
                append_escaped_text (writer, parent_id);
        else
                g_string_append (writer->priv->str, parent_id);

        g_string_append (writer->priv->str, "\" restricted=\"");
        g_string_append (writer->priv->str, restricted ? "true" : "false");
        g_string_append (writer->priv->str, "\">");
}

void
gupnp_didl_lite_writer_end_item (GUPnPDIDLLiteWriter *writer)
{
        g_return_if_fail (GUPNP_IS_DIDL_LITE_WRITER (writer));
        g_return_if_fail (writer->priv->str);

        g_string_append (writer->priv->str, "</item>");
}

void
gupnp_didl_lite_resource_empty (GUPnPDIDLLiteResource *res)
{
        memset (res, sizeof (GUPnPDIDLLiteResource), 0);

        res->size             = -1;
        res->seconds          = -1;
        res->bitrate          = -1;
        res->sample_freq      = -1;
        res->bits_per_sample  = -1;
        res->n_audio_channels = -1;
        res->width            = -1;
        res->height           = -1;
        res->color_depth      = -1;
}

void
gupnp_didl_lite_writer_add_res (GUPnPDIDLLiteWriter   *writer,
                                GUPnPDIDLLiteResource *res)
{
        SoupUri *uri;
        char *uri_str;

        g_return_if_fail (GUPNP_IS_DIDL_LITE_WRITER (writer));
        g_return_if_fail (writer->priv->str);
        g_return_if_fail (res != NULL);
        g_return_if_fail (res->uri != NULL);
        g_return_if_fail (res->protocol_info != NULL);

        g_string_append (writer->priv->str, "<res protocolInfo=\"");

        if (writer->priv->need_escape)
                append_escaped_text (writer, res->protocol_info);
        else
                g_string_append (writer->priv->str, res->protocol_info);

        g_string_append_c (writer->priv->str, '"');

        if (res->import_uri) {
                g_string_append (writer->priv->str, " importUri=\"");

                if (writer->priv->base_url || writer->priv->need_escape) {
                        uri = soup_uri_new_with_base (writer->priv->base_url,
                                                      res->import_uri);
                        uri_str = soup_uri_to_string (uri, FALSE);
                        soup_uri_free (uri);

                        g_string_append (writer->priv->str, uri_str);

                        g_free (uri_str);
                } else
                        g_string_append (writer->priv->str, res->import_uri);

                g_string_append_c (writer->priv->str, '"');
        }

        if (res->size >= 0)
                g_string_append_printf (writer->priv->str,
                                        " size=\"%ld\"", res->size);

        g_string_append_printf (writer->priv->str,
                                " duration=\"%ld:%.2ld:%.2ld.\"",
                                res->seconds / (60 * 60),
                                res->seconds % (60 * 60),
                                res->seconds % 60);

        if (res->bitrate >= 0)
                g_string_append_printf (writer->priv->str,
                                        " bitrate=\"%d\"", res->bitrate);

        if (res->sample_freq >= 0)
                g_string_append_printf (writer->priv->str,
                                        " sampleFrequency=\"%d\"",
                                        res->sample_freq);

        if (res->bits_per_sample >= 0)
                g_string_append_printf (writer->priv->str,
                                        " bitsPerSample=\"%d\"",
                                        res->bits_per_sample);

        if (res->protection) {
                g_string_append (writer->priv->str, " protection=\"");

                if (writer->priv->need_escape)
                        append_escaped_text (writer, res->protection);
                else
                        g_string_append (writer->priv->str, res->protection);

                g_string_append_c (writer->priv->str, '"');
        }

        if (res->n_audio_channels >= 0)
                g_string_append_printf (writer->priv->str,
                                        " nrAudioChannels=\"%d\"",
                                        res->n_audio_channels);

        if (res->width >= 0 && res->height >= 0) {
                g_string_append_printf (writer->priv->str,
                                        " resolution=\"%dx%d\"",
                                        res->width,
                                        res->height);
        }

        if (res->color_depth >= 0)
                g_string_append_printf (writer->priv->str,
                                        " colorDepth=\"%d\"",
                                        res->color_depth);

        g_string_append_c (writer->priv->str, '>');

        if (writer->priv->base_url || writer->priv->need_escape) {
                uri = soup_uri_new_with_base (writer->priv->base_url, res->uri);
                uri_str = soup_uri_to_string (uri, FALSE);
                soup_uri_free (uri);

                g_string_append (writer->priv->str, uri_str);

                g_free (uri_str);
        } else
                g_string_append (writer->priv->str, res->uri);

        g_string_append (writer->priv->str, "</res>");
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

        g_string_append (writer->priv->str, "<desc");

        if (id) {
                g_string_append (writer->priv->str, " id=\"");

                if (writer->priv->need_escape)
                        append_escaped_text (writer, id);
                else
                        g_string_append (writer->priv->str, id);

                g_string_append_c (writer->priv->str, '"');
        }

        if (name) {
                g_string_append (writer->priv->str, " name=\"");

                if (writer->priv->need_escape)
                        append_escaped_text (writer, name);
                else
                        g_string_append (writer->priv->str, name);

                g_string_append_c (writer->priv->str, '"');
        }

        if (ns_uri) {
                g_string_append (writer->priv->str, " nameSpace=\"");

                if (writer->priv->base_url || writer->priv->need_escape) {
                        SoupUri *uri;
                        char *uri_str;

                        uri = soup_uri_new_with_base (writer->priv->base_url,
                                                      ns_uri);
                        uri_str = soup_uri_to_string (uri, FALSE);
                        soup_uri_free (uri);

                        g_string_append (writer->priv->str, uri_str);

                        g_free (uri_str);
                } else
                        g_string_append (writer->priv->str, ns_uri);

                g_string_append_c (writer->priv->str, '"');
        }

        g_string_append_c (writer->priv->str, '>');

        if (desc) {
                if (writer->priv->need_escape)
                        append_escaped_text (writer, desc);
                else
                        g_string_append (writer->priv->str, desc);
        }

        g_string_append (writer->priv->str, "</desc>");
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

                if (writer->priv->base_url || writer->priv->need_escape) {
                        SoupUri *uri;
                        char *uri_str;

                        uri = soup_uri_new_with_base (writer->priv->base_url,
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
        const char *attr_name, *attr_value;

        g_return_if_fail (GUPNP_IS_DIDL_LITE_WRITER (writer));
        g_return_if_fail (writer->priv->str);
        g_return_if_fail (property != NULL);

        begin_property (writer, property, prefix, ns_uri);

        attr_name = va_arg (var_args, const char *);
        while (attr_name) {
                attr_value = va_arg (var_args, const char *);

                g_string_append_c (writer->priv->str, ' ');
                g_string_append (writer->priv->str, attr_name);
                g_string_append (writer->priv->str, "=\"");

                if (writer->priv->need_escape)
                        append_escaped_text (writer, attr_value);
                else
                        g_string_append (writer->priv->str, attr_value);

                g_string_append_c (writer->priv->str, '"');
        }

        g_string_append_c (writer->priv->str, '>');

        if (value)
                g_string_append (writer->priv->str, value);

        end_property (writer, property, prefix);
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

        begin_property_simple (writer, property, prefix, ns_uri);

        g_string_append (writer->priv->str, value ? "true" : "false");

        end_property (writer, property, prefix);
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

        begin_property_simple (writer, property, prefix, ns_uri);

        g_string_append_printf (writer->priv->str, "%d", value);

        end_property (writer, property, prefix);
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

        begin_property_simple (writer, property, prefix, ns_uri);

        g_string_append_printf (writer->priv->str, "%u", value);

        end_property (writer, property, prefix);
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

        begin_property_simple (writer, property, prefix, ns_uri);

        g_string_append_printf (writer->priv->str, "%ld", value);

        end_property (writer, property, prefix);
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

        begin_property_simple (writer, property, prefix, ns_uri);

        g_string_append_printf (writer->priv->str, "%lu", value);

        end_property (writer, property, prefix);
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
