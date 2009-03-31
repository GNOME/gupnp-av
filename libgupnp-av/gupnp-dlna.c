/*
 * Copyright (C) 2009 Nokia Corporation.
 *
 * Authors: Zeeshan Ali <zeeshanak@gnome.org>
 *                      <zeeshan.ali@nokia.com>
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

#include "gupnp-dlna-private.h"

static const char *
guess_jpeg_profile (GUPnPDIDLLiteResource *resource)
{
        if (resource->width < 0 || resource->height < 0) {
                return "JPEG_MED";
        } else if (resource->width <= 48 && resource->height <= 48) {
                return "JPEG_SM_ICO";
        } else if (resource->width <= 120 && resource->height <= 120) {
                return "JPEG_LRG_ICO";
        } else if (resource->width <= 160 && resource->height <= 160) {
                return "JPEG_TN"; /* Thumbnail */
        } else if (resource->width <= 640 && resource->height <= 480) {
                return "JPEG_SM";
        } else if (resource->width <= 1024 && resource->height <= 768) {
                return "JPEG_MED";
        } else if (resource->width <= 4096 && resource->height <= 4096) {
                return "JPEG_LRG";
        } else {
                return NULL;
        }
}

static const char *
guess_ac3_profile (GUPnPDIDLLiteResource *resource)
{
        if (resource->sample_freq < 0 ||       /* Unknown */
            resource->sample_freq == 32000 ||
            resource->sample_freq == 41000 ||
            resource->sample_freq == 48000) {
                return "AC3";
        } else {
                return NULL;
        }
}

static const char *
guess_lpcm_profile (GUPnPDIDLLiteResource *resource)
{
        if ((resource->sample_freq < 0 ||       /* Unknown */
             resource->sample_freq == 41000 ||
             resource->sample_freq == 48000) &&
            resource->n_audio_channels <= 2 &&
            (resource->bits_per_sample == 16)) {
                return "LPCM";
        } else {
                return NULL;
        }
}

static const char *
guess_mp3_profile (GUPnPDIDLLiteResource *resource)
{
        if ((resource->sample_freq < 0 ||       /* Unknown */
             resource->sample_freq == 32000 ||
             resource->sample_freq == 41000 ||
             resource->sample_freq == 48000) &&
            resource->n_audio_channels <= 2 &&
            (resource->bitrate < 0 ||           /* VBR or Unknown */
             resource->bitrate ==  32000 ||
             resource->bitrate ==  40000 ||
             resource->bitrate ==  48000 ||
             resource->bitrate ==  56000 ||
             resource->bitrate ==  64000 ||
             resource->bitrate ==  80000 ||
             resource->bitrate ==  96000 ||
             resource->bitrate ==  112000 ||
             resource->bitrate ==  128000 ||
             resource->bitrate ==  160000 ||
             resource->bitrate ==  192000 ||
             resource->bitrate ==  224000 ||
             resource->bitrate ==  256000 ||
             resource->bitrate ==  320000)) {
                return "MP3";
        } else {
                return NULL;
        }
}

static const char *
guess_aac_profile (GUPnPDIDLLiteResource *resource)
{
        if ((resource->sample_freq < 0 ||       /* Unknown */
             resource->sample_freq == 8000 ||
             resource->sample_freq == 11025 ||
             resource->sample_freq == 12000 ||
             resource->sample_freq == 16000 ||
             resource->sample_freq == 22050 ||
             resource->sample_freq == 24000 ||
             resource->sample_freq == 32000 ||
             resource->sample_freq == 41000 ||
             resource->sample_freq == 48000) &&
            resource->n_audio_channels <= 2) {
                if (resource->bitrate > 0) {
                        if (resource->bitrate <=  32000) {
                                return "AAC_ISO_320";
                        } else if (resource->bitrate <=  57600) {
                                return "AAC_ISO";
                        } else {
                                return NULL;
                        }
                } else {
                        /* VBR or Unknown bitrate */
                        return "AAC_ISO";
                }
        } else {
                return NULL;
        }
}

const char *
dlna_guess_profile (GUPnPDIDLLiteResource *resource)
{
        if (g_str_has_prefix (resource->mime_type, "image/jpeg")) {
                return guess_jpeg_profile (resource);
        } else if (g_str_has_prefix (resource->mime_type,
                                     "audio/vnd.dolby.dd-raw")) {
                return guess_ac3_profile (resource);
        } else if (g_str_has_prefix (resource->mime_type, "audio/mp4") ||
                   g_str_has_prefix (resource->mime_type, "audio/3gpp")) {
                return guess_aac_profile (resource);
        } else if (g_str_has_prefix (resource->mime_type, "audio/L16")) {
                return guess_lpcm_profile (resource);
        } else if (g_str_has_prefix (resource->mime_type, "audio/mpeg")) {
                return guess_mp3_profile (resource);
        } else {
                return NULL;
        }
}
