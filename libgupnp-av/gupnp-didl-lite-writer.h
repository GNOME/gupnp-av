/*
 * Copyright (C) 2007 OpenedHand Ltd.
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

#ifndef __GUPNP_DIDL_LITE_WRITER_H__
#define __GUPNP_DIDL_LITE_WRITER_H__

#include <stdarg.h>
#include <glib-object.h>
#include <libsoup/soup-uri.h>

G_BEGIN_DECLS

GType
gupnp_didl_lite_writer_get_type (void) G_GNUC_CONST;

#define GUPNP_TYPE_DIDL_LITE_WRITER \
                (gupnp_didl_lite_writer_get_type ())
#define GUPNP_DIDL_LITE_WRITER(obj) \
                (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
                 GUPNP_TYPE_DIDL_LITE_WRITER, \
                 GUPnPDIDLLiteWriter))
#define GUPNP_DIDL_LITE_WRITER_CLASS(obj) \
                (G_TYPE_CHECK_CLASS_CAST ((obj), \
                 GUPNP_TYPE_DIDL_LITE_WRITER, \
                 GUPnPDIDLLiteWriterClass))
#define GUPNP_IS_DIDL_LITE_WRITER(obj) \
                (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                 GUPNP_TYPE_DIDL_LITE_WRITER))
#define GUPNP_IS_DIDL_LITE_WRITER_CLASS(obj) \
                (G_TYPE_CHECK_CLASS_TYPE ((obj), \
                 GUPNP_TYPE_DIDL_LITE_WRITER))
#define GUPNP_DIDL_LITE_WRITER_GET_CLASS(obj) \
                (G_TYPE_INSTANCE_GET_CLASS ((obj), \
                 GUPNP_TYPE_DIDL_LITE_WRITER, \
                 GUPnPDIDLLiteWriterClass))

typedef struct _GUPnPDIDLLiteWriterPrivate GUPnPDIDLLiteWriterPrivate;

typedef struct {
        GObject parent;

        GUPnPDIDLLiteWriterPrivate *priv;
} GUPnPDIDLLiteWriter;

typedef struct {
        GObjectClass parent_class;

        /* future padding */
        void (* _gupnp_reserved1) (void);
        void (* _gupnp_reserved2) (void);
        void (* _gupnp_reserved3) (void);
        void (* _gupnp_reserved4) (void);
} GUPnPDIDLLiteWriterClass;

#define GUPNP_DIDL_LITE_WRITER_NAMESPACE_DC   "dc"
#define GUPNP_DIDL_LITE_WRITER_NAMESPACE_UPNP "upnp"

GUPnPDIDLLiteWriter *
gupnp_didl_lite_writer_new                 (void);

void
gupnp_didl_lite_writer_start_didl_lite     (GUPnPDIDLLiteWriter *writer,
                                            const char          *lang,
                                            SoupURI             *url_base,
                                            gboolean             need_escape);

void
gupnp_didl_lite_writer_end_didl_lite       (GUPnPDIDLLiteWriter *writer);

void
gupnp_didl_lite_writer_start_container     (GUPnPDIDLLiteWriter *writer,
                                            const char          *id,
                                            const char          *parent_id,
                                            int                  child_count,
                                            gboolean             restricted,
                                            gboolean             searchable);

void
gupnp_didl_lite_writer_end_container       (GUPnPDIDLLiteWriter *writer);

void
gupnp_didl_lite_writer_start_item          (GUPnPDIDLLiteWriter *writer,
                                            const char          *id,
                                            const char          *parent_id,
                                            const char          *ref_id,
                                            gboolean             restricted);

void
gupnp_didl_lite_writer_end_item            (GUPnPDIDLLiteWriter *writer);

typedef struct {
        char   *uri;           /* Required */
        char   *import_uri;
        char   *protocol_info; /* Required */

        /* Stream data */
        long    size;
        long    duration;
        int     bitrate;
        int     sample_freq;
        int     bits_per_sample;
        char   *protection;

        /* Audio */
        int     n_audio_channels;

        /* Video */
        int     width;
        int     height;
        int     color_depth;
} GUPnPDIDLLiteResource;

void
gupnp_didl_lite_resource_reset (GUPnPDIDLLiteResource *res);

void
gupnp_didl_lite_writer_add_res               (GUPnPDIDLLiteWriter   *writer,
                                              GUPnPDIDLLiteResource *res);

void
gupnp_didl_lite_writer_add_desc              (GUPnPDIDLLiteWriter   *writer,
                                              const char            *desc,
                                              const char            *id,
                                              const char            *name,
                                              const char            *ns_uri);

void
gupnp_didl_lite_writer_add_string            (GUPnPDIDLLiteWriter   *writer,
                                              const char            *property,
                                              const char            *prefix,
                                              const char            *ns_uri,
                                              const char            *value);

void
gupnp_didl_lite_writer_add_string_with_attrs (GUPnPDIDLLiteWriter   *writer,
                                              const char            *property,
                                              const char            *prefix,
                                              const char            *ns_uri,
                                              const char            *value,
                                              ...) G_GNUC_NULL_TERMINATED;

void
gupnp_didl_lite_writer_add_string_with_attrs_valist
                                             (GUPnPDIDLLiteWriter   *writer,
                                              const char            *property,
                                              const char            *prefix,
                                              const char            *ns_uri,
                                              const char            *value,
                                              va_list                var_args);

void
gupnp_didl_lite_writer_add_boolean           (GUPnPDIDLLiteWriter   *writer,
                                              const char            *property,
                                              const char            *prefix,
                                              const char            *ns_uri,
                                              gboolean               value);

void
gupnp_didl_lite_writer_add_int               (GUPnPDIDLLiteWriter   *writer,
                                              const char            *property,
                                              const char            *prefix,
                                              const char            *ns_uri,
                                              int                    value);

void
gupnp_didl_lite_writer_add_uint              (GUPnPDIDLLiteWriter   *writer,
                                              const char            *property,
                                              const char            *prefix,
                                              const char            *ns_uri,
                                              guint                  value);

void
gupnp_didl_lite_writer_add_long              (GUPnPDIDLLiteWriter   *writer,
                                              const char            *property,
                                              const char            *prefix,
                                              const char            *ns_uri,
                                              long                   value);

void
gupnp_didl_lite_writer_add_ulong             (GUPnPDIDLLiteWriter   *writer,
                                              const char            *property,
                                              const char            *prefix,
                                              const char            *ns_uri,
                                              gulong                 value);

const char *
gupnp_didl_lite_writer_get_string            (GUPnPDIDLLiteWriter   *writer);

void
gupnp_didl_lite_writer_reset                 (GUPnPDIDLLiteWriter   *writer);

G_END_DECLS

#endif /* __GUPNP_DIDL_LITE_WRITER_H__ */
