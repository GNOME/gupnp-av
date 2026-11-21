/*
 * Copyright (C) 2012 Intel Corporation
 *
 * Authors: Krzesimir Nowak <krnowak@openismus.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */
#include <config.h>

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlsave.h>
#include <libxml/xmlschemas.h>

#include "xsd-data.h"

struct _XSDData {
        xmlSchemaPtr schema;
        xmlSchemaValidCtxtPtr valid_context;
};

G_DEFINE_AUTOPTR_CLEANUP_FUNC (xmlSchemaParserCtxt, xmlSchemaFreeParserCtxt)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (xmlBuffer, xmlBufferFree)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (xmlDoc, xmlFreeDoc)

/* Resolve schema imports by file name in the main schema's directory. */
static xmlParserErrors
xsd_resource_loader (void                *loader_data,
                     const char          *url,
                     G_GNUC_UNUSED const char      *public_id,
                     G_GNUC_UNUSED xmlResourceType  type,
                     xmlParserInputFlags           flags,
                     xmlParserInput              **out)
{
        const gchar *dir = loader_data;
        g_autofree gchar *basename = g_path_get_basename (url);
        g_autofree gchar *path = g_build_filename (dir, basename, NULL);

        return xmlNewInputFromUrl (path, flags, out);
}

XSDData *
xsd_data_new (const gchar *xsd_file)
{
        XSDData *xsd_data = g_slice_new0 (XSDData);
        gboolean failed = TRUE;
        g_autofree gchar *dirname = g_path_get_dirname (xsd_file);
        g_autoptr (xmlSchemaParserCtxt) context =
                xmlSchemaNewParserCtxt (xsd_file);

        if (context == NULL) {
                /* unable to create a parser context for the schema */
                goto out;
        }
        xmlSchemaSetResourceLoader (context, xsd_resource_loader, dirname);
        xsd_data->schema = xmlSchemaParse (context);
        if (xsd_data->schema == NULL) {
                /* the schema itself is not valid */
                goto out;
        }
        xsd_data->valid_context = xmlSchemaNewValidCtxt (xsd_data->schema);
        if (xsd_data->valid_context == NULL) {
                /* unable to create a validation context for the schema */
                goto out;
        }
        failed = FALSE;
 out:
        if (failed) {
                xsd_data_free (xsd_data);
                xsd_data = NULL;
        }

        return xsd_data;
}

void
xsd_data_free (XSDData *xsd_data)
{
        if (xsd_data == NULL) {
                return;
        }
        g_clear_pointer (&xsd_data->valid_context, xmlSchemaFreeValidCtxt);
        g_clear_pointer (&xsd_data->schema, xmlSchemaFree);

        g_slice_free (XSDData, xsd_data);
}

gboolean
xsd_data_validate_doc (XSDData *xsd_data,
                       xmlDoc  *doc)
{
        g_autoptr (xmlBuffer) buffer = NULL;
        g_autoptr (xmlDoc) dump_doc = NULL;
        xmlSaveCtxtPtr save_context = NULL;
        const xmlChar *content;
        gboolean result = FALSE;

        if (xsd_data == NULL) {
                return TRUE;
        }

        buffer = xmlBufferCreate ();
        if (buffer == NULL) {
                return FALSE;
        }
        save_context = xmlSaveToBuffer (buffer, NULL, 0);
        if (save_context == NULL) {
                goto out;
        }
        if (xmlSaveDoc (save_context, doc) < 0) {
                goto out;
        }
        /* Flushes the buffer and frees the save context. */
        if (xmlSaveClose (save_context) < 0) {
                save_context = NULL;
                goto out;
        }
        save_context = NULL;
        content = xmlBufferContent (buffer);
        if (content == NULL) {
                goto out;
        }
        g_debug ("Doc dump:\n%.*s",
                 (int) xmlBufferLength (buffer),
                 (const char *) content);
        /* Nodes may lack namespace info; validate a re-parsed copy. */
        dump_doc = xmlReadMemory ((const char *) content,
                                  (int) xmlBufferLength (buffer),
                                  NULL,
                                  NULL,
                                  XML_PARSE_NONET);
        if (dump_doc == NULL) {
                goto out;
        }
        if (xmlSchemaValidateDoc (xsd_data->valid_context, dump_doc) == 0) {
                result = TRUE;
        }
 out:
        if (save_context != NULL) {
                xmlSaveClose (save_context);
        }
        return result;
}
