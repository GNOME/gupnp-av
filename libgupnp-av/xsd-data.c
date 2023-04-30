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
#include <libxml/xmlschemas.h>

#include "xsd-data.h"

struct _XSDData {
        xmlSchemaPtr schema;
        xmlSchemaValidCtxtPtr valid_context;
};

XSDData *
xsd_data_new (const gchar *xsd_file)
{
        XSDData *xsd_data = g_slice_new0 (XSDData);
        gboolean failed = TRUE;
        xmlSchemaParserCtxtPtr context = xmlSchemaNewParserCtxt (xsd_file);

        if (context == NULL)
                /* unable to create a parser context for the schema */
                goto out;
        xsd_data->schema = xmlSchemaParse (context);
        if (xsd_data->schema == NULL)
                /* the schema itself is not valid */
                goto out;
        xsd_data->valid_context = xmlSchemaNewValidCtxt (xsd_data->schema);
        if (xsd_data->valid_context == NULL)
                /* unable to create a validation context for the schema */
                goto out;
        failed = FALSE;
 out:
        if (context != NULL)
                xmlSchemaFreeParserCtxt (context);
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
        g_clear_pointer (&xsd_data->valid_context, xmlSchemaFreeValidCtxt);
        g_clear_pointer (&xsd_data->schema, xmlSchemaFree);

        g_slice_free (XSDData, xsd_data);
}

gboolean
xsd_data_validate_doc (XSDData *xsd_data,
                       xmlDoc  *doc)
{

        gboolean result = FALSE;

        if (xsd_data == NULL)
                return TRUE;

        if (!xmlSchemaValidateDoc (xsd_data->valid_context, doc))
                result = TRUE;

        return result;
}
