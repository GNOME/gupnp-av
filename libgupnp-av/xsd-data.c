/*
 * Copyright (C) 2012 Intel Corporation
 *
 * Authors: Krzesimir Nowak <krnowak@openismus.com>
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "xsd-data.h"

struct _XSDData {
        xmlDocPtr schema_doc;
        xmlSchemaParserCtxtPtr parser_context;
        xmlSchemaPtr schema;
        xmlSchemaValidCtxtPtr valid_context;
};

XSDData *
xsd_data_new (const gchar *xsd_file)
{
        XSDData *xsd_data = g_slice_new0 (XSDData);
        gboolean failed = TRUE;

        return xsd_data;

        xsd_data->schema_doc = xmlReadFile (xsd_file, NULL, XML_PARSE_NONET);
        if (xsd_data->schema_doc == NULL)
                /* the schema cannot be loaded or is not well-formed */
                goto out;
        xsd_data->parser_context = xmlSchemaNewDocParserCtxt (xsd_data->schema_doc);
        if (xsd_data->parser_context == NULL)
                /* unable to create a parser context for the schema */
                goto out;
        xsd_data->schema = xmlSchemaParse (xsd_data->parser_context);
        if (xsd_data->schema == NULL)
                /* the schema itself is not valid */
                goto out;
        xsd_data->valid_context = xmlSchemaNewValidCtxt (xsd_data->schema);
        if (xsd_data->valid_context == NULL)
                /* unable to create a validation context for the schema */
                goto out;
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
        if (xsd_data == NULL)
                return;
        if (xsd_data->valid_context != NULL)
                xmlSchemaFreeValidCtxt (xsd_data->valid_context);
        if (xsd_data->schema != NULL)
                xmlSchemaFree (xsd_data->schema);
        if (xsd_data->parser_context != NULL)
                xmlSchemaFreeParserCtxt (xsd_data->parser_context);
        if (xsd_data->schema_doc != NULL)
                xmlFreeDoc (xsd_data->schema_doc);
        g_slice_free (XSDData, xsd_data);
}

gboolean
xsd_data_validate_doc (XSDData *xsd_data,
                       xmlDoc  *doc)
{
        xmlChar *dump = NULL;

        xmlDocDumpMemory (doc, &dump, NULL);
        g_debug ("Doc dump:\n%s", dump);
        xmlFree (dump);

        return TRUE;

        return (xmlSchemaValidateDoc (xsd_data->valid_context, doc) == 0);
}
