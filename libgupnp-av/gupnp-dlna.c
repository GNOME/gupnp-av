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

static void
check_int_allowed (int         value,
                   const char *value_name,
                   int        *allowed_values,
                   const char *profile)
{
        if (value > 0) {
                int     *allowed_value;
                gboolean not_allowed = TRUE;

            for (allowed_value = allowed_values;
                 *allowed_value != -1;
                 allowed_value++) {
                if (value == *allowed_value) {
                        not_allowed = FALSE;
                        break;
                }
            }

            if (not_allowed) {
                g_warning ("DLNA specs do not allow %d as %s for %s profile",
                           value,
                           value_name,
                           profile);
            }
        }
}

static void
check_frequency (GUPnPDIDLLiteResource *resource,
                 int                   *allowed_freq,
                 const char            *profile)
{
        check_int_allowed (resource->sample_freq,
                           "frequency",
                           allowed_freq,
                           profile);
}

static void
check_bitrate (GUPnPDIDLLiteResource *resource,
               int                   *allowed_bitrates,
               const char            *profile)
{
        check_int_allowed (resource->bitrate,
                           "bitrate",
                           allowed_bitrates,
                           profile);
}

static void
check_sample_size (GUPnPDIDLLiteResource *resource,
                   int                   *allowed_sample_size,
                   const char            *profile)
{
        check_int_allowed (resource->bits_per_sample,
                           "bitsPerSample",
                           allowed_sample_size,
                           profile);
}

static void
check_num_channels (GUPnPDIDLLiteResource *resource,
                    int                   *allowed_num_channels,
                    const char            *profile)
{
        check_int_allowed (resource->n_audio_channels,
                           "nrAudioChannels",
                           allowed_num_channels,
                           profile);
}

static const char *
guess_ac3_profile (GUPnPDIDLLiteResource *resource)
{
        int allowed_freq[] = { 32000, 41000, 4800, -1 };

        check_frequency (resource, allowed_freq, "AC3");

        return "AC3";
}

static const char *
guess_lpcm_profile (GUPnPDIDLLiteResource *resource)
{
        int allowed_freq[] = { 41000, 4800, -1 };
        int allowed_num_channels[] = { 1, 2, -1 };
        int allowed_sample_size [] = { 16, -1 };

        check_frequency (resource, allowed_freq, "LPCM");
        check_num_channels (resource, allowed_num_channels, "LPCM");
        check_sample_size (resource, allowed_sample_size, "LPCM");

        return "LPCM";
}

static const char *
guess_mp3_profile (GUPnPDIDLLiteResource *resource)
{
        int allowed_freq[] = { 32000, 41000, 48000, -1 };
        int allowed_num_channels[] = { 1, 2, -1 };
        int allowed_bitrates[] = { 32000,
                                   40000,
                                   48000,
                                   56000,
                                   64000,
                                   80000,
                                   96000,
                                   112000,
                                   128000,
                                   160000,
                                   192000,
                                   224000,
                                   256000,
                                   320000,
                                   -1 };

        check_frequency (resource, allowed_freq, "MP3");
        check_num_channels (resource, allowed_num_channels, "MP3");
        check_bitrate (resource, allowed_bitrates, "MP3");

        return "MP3";
}

static const char *
guess_aac_profile (GUPnPDIDLLiteResource *resource)
{
        int allowed_freq[] = { 8000,
                               11025,
                               12000,
                               16000,
                               22050,
                               24000,
                               32000,
                               41000,
                               48000,
                               -1 };
        int allowed_num_channels[] = { 1, 2, -1 };

        check_frequency (resource, allowed_freq, "AAC_ISO");
        check_num_channels (resource, allowed_num_channels, "AAC_ISO");

        if (resource->bitrate > 0 && resource->bitrate <=  32000) {
                return "AAC_ISO_320";
        } else if (resource->bitrate <= 57600) {
                return "AAC_ISO";
        }
}

static const char *
guess_wma_profile (GUPnPDIDLLiteResource *resource)
{
        if (resource->sample_freq > 0 && resource->n_audio_channels > 0 &&
            resource->sample_freq <= 48000 && resource->n_audio_channels <= 2) {
                if (resource->bitrate > 0 && resource->bitrate <=  192999) {
                        return "WMABASE";
                } else {
                        return "WMAFULL";
                }
        } else {
                return "WMAPRO";
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
        } else if (g_str_has_prefix (resource->mime_type, "audio/x-ms-wma")) {
                return guess_wma_profile (resource);
        } else {
                return NULL;
        }
}
