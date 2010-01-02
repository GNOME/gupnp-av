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

GType
gupnp_dlna_conversion_get_type (void)
{
        static GType type = 0;

        if (type == 0) {
                static const GFlagsValue values[] = {
                        { GUPNP_DLNA_CONVERSION_NONE,
                          "GUPNP_DLNA_CONVERSION_NONE",
                          "none" },
                        { GUPNP_DLNA_CONVERSION_TRANSCODED,
                          "GUPNP_DLNA_CONVERSION_TRANSCODED",
                          "transcoded" },
                        { 0, NULL, NULL }
                };

                type = g_flags_register_static
                                (g_intern_static_string ("GUPnPDLNAConversion"),
                                 values);
        }

        return type;
}

GType
gupnp_dlna_operation_get_type (void)
{
        static GType type = 0;

        if (type == 0) {
                static const GFlagsValue values[] = {
                        { GUPNP_DLNA_OPERATION_NONE,
                          "GUPNP_DLNA_OPERATION_NONE",
                          "none" },
                        { GUPNP_DLNA_OPERATION_RANGE,
                          "GUPNP_DLNA_OPERATION_RANGE",
                          "range" },
                        { GUPNP_DLNA_OPERATION_TIMESEEK,
                          "GUPNP_DLNA_OPERATION_TIMESEEK",
                          "timeseek" },
                        { 0, NULL, NULL }
                };

                type = g_flags_register_static
                                (g_intern_static_string ("GUPnPDLNAOperation"),
                                 values);
        }

        return type;
}

        GType
gupnp_dlna_flags_get_type (void)
{
        static GType type = 0;

        if (type == 0) {
                static const GFlagsValue values[] = {
                        { GUPNP_DLNA_FLAGS_NONE,
                          "GUPNP_DLNA_FLAGS_NONE",
                          "none" },
                        { GUPNP_DLNA_FLAGS_SENDER_PACED,
                          "GUPNP_DLNA_FLAGS_SENDER_PACED",
                          "sender-paced" },
                        { GUPNP_DLNA_FLAGS_TIME_BASED_SEEK,
                          "GUPNP_DLNA_FLAGS_TIME_BASED_SEEK",
                          "time-based-seek" },
                        { GUPNP_DLNA_FLAGS_BYTE_BASED_SEEK,
                          "GUPNP_DLNA_FLAGS_BYTE_BASED_SEEK",
                          "byte-based-seek" },
                        { GUPNP_DLNA_FLAGS_PLAY_CONTAINER,
                          "GUPNP_DLNA_FLAGS_PLAY_CONTAINER",
                          "play-container" },
                        { GUPNP_DLNA_FLAGS_S0_INCREASE,
                          "GUPNP_DLNA_FLAGS_S0_INCREASE",
                          "s0-increase" },
                        { GUPNP_DLNA_FLAGS_SN_INCREASE,
                          "GUPNP_DLNA_FLAGS_SN_INCREASE",
                          "sn-increase" },
                        { GUPNP_DLNA_FLAGS_RTSP_PAUSE,
                          "GUPNP_DLNA_FLAGS_RTSP_PAUSE",
                          "rtsp-pause" },
                        { GUPNP_DLNA_FLAGS_STREAMING_TRANSFER_MODE,
                          "GUPNP_DLNA_FLAGS_STREAMING_TRANSFER_MODE",
                          "streaming-transfer-mode" },
                        { GUPNP_DLNA_FLAGS_INTERACTIVE_TRANSFER_MODE,
                          "GUPNP_DLNA_FLAGS_INTERACTIVE_TRANSFER_MODE",
                          "interactive-transfer-mode" },
                        { GUPNP_DLNA_FLAGS_BACKGROUND_TRANSFER_MODE,
                          "GUPNP_DLNA_FLAGS_BACKGROUND_TRANSFER_MODE",
                          "background-transfer-mode" },
                        { GUPNP_DLNA_FLAGS_CONNECTION_STALL,
                          "GUPNP_DLNA_FLAGS_CONNECTION_STALL",
                          "connection-stall" },
                        { GUPNP_DLNA_FLAGS_DLNA_V15,
                          "GUPNP_DLNA_FLAGS_DLNA_V15",
                          "dlna-v15" },
                        { 0, NULL, NULL }
                };

                type = g_flags_register_static
                                (g_intern_static_string ("GUPnPDLNAFlags"),
                                 values);
        }

        return type;
}

static const char *
guess_jpeg_profile (GUPnPDIDLLiteResource *resource)
{
        int width;
        int height;

        width = gupnp_didl_lite_resource_get_width (resource);
        height = gupnp_didl_lite_resource_get_height (resource);

        if (width < 0 || height < 0) {
                return "JPEG_MED";
        } else if (width <= 48 && height <= 48) {
                return "JPEG_SM_ICO";
        } else if (width <= 120 && height <= 120) {
                return "JPEG_LRG_ICO";
        } else if (width <= 160 && height <= 160) {
                return "JPEG_TN"; /* Thumbnail */
        } else if (width <= 640 && height <= 480) {
                return "JPEG_SM";
        } else if (width <= 1024 && height <= 768) {
                return "JPEG_MED";
        } else if (width <= 4096 && height <= 4096) {
                return "JPEG_LRG";
        } else {
                return NULL;
        }
}

static const char *
guess_png_profile (GUPnPDIDLLiteResource *resource)
{
        int width;
        int height;

        width = gupnp_didl_lite_resource_get_width (resource);
        height = gupnp_didl_lite_resource_get_height (resource);

        if (width < 0 || height < 0) {
                return "PNG_MED";
        } else if (width <= 48 && height <= 48) {
                return "PNG_SM_ICO";
        } else if (width <= 120 && height <= 120) {
                return "PNG_LRG_ICO";
        } else if (width <= 160 && height <= 160) {
                return "PNG_TN"; /* Thumbnail */
        } else if (width <= 640 && height <= 480) {
                return "PNG_SM";
        } else if (width <= 1024 && height <= 768) {
                return "PNG_MED";
        } else if (width <= 4096 && height <= 4096) {
                return "PNG_LRG";
        } else {
                return NULL;
        }
}

static void
check_int_allowed (int         value,
                   const char *value_name,
                   const int  *allowed_values,
                   const char *profile)
{
        if (value > 0) {
                const int *allowed_value;
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
                 const int             *allowed_freq,
                 const char            *profile)
{
        int sample_freq;

        sample_freq = gupnp_didl_lite_resource_get_sample_freq (resource);

        check_int_allowed (sample_freq,
                           "frequency",
                           allowed_freq,
                           profile);
}

static void
check_bitrate (GUPnPDIDLLiteResource *resource,
               const int             *allowed_bitrates,
               const char            *profile)
{
        int bitrate;

        bitrate = gupnp_didl_lite_resource_get_bitrate (resource);

        check_int_allowed (bitrate,
                           "bitrate",
                           allowed_bitrates,
                           profile);
}

static void
check_sample_size (GUPnPDIDLLiteResource *resource,
                   const int             *allowed_sample_size,
                   const char            *profile)
{
        int sample_size;

        sample_size = gupnp_didl_lite_resource_get_bits_per_sample (resource);

        check_int_allowed (sample_size,
                           "bitsPerSample",
                           allowed_sample_size,
                           profile);
}

static void
check_num_channels (GUPnPDIDLLiteResource *resource,
                    const int             *allowed_num_channels,
                    const char            *profile)
{
        int channels;

        channels = gupnp_didl_lite_resource_get_audio_channels (resource);

        check_int_allowed (channels,
                           "nrAudioChannels",
                           allowed_num_channels,
                           profile);
}

static const char *
guess_ac3_profile (GUPnPDIDLLiteResource *resource)
{
        const int allowed_freq[] = { 32000, 44100, 48000, -1 };

        check_frequency (resource, allowed_freq, "AC3");

        return "AC3";
}

static const char *
guess_lpcm_profile (GUPnPDIDLLiteResource *resource)
{
        const int allowed_freq[] = { 44100, 48000, -1 };
        const int allowed_num_channels[] = { 1, 2, -1 };
        const int allowed_sample_size [] = { 16, -1 };

        check_frequency (resource, allowed_freq, "LPCM");
        check_num_channels (resource, allowed_num_channels, "LPCM");
        check_sample_size (resource, allowed_sample_size, "LPCM");

        return "LPCM";
}

static const char *
guess_mp3_profile (GUPnPDIDLLiteResource *resource)
{
        const int allowed_freq[] = { 32000, 44100, 48000, -1 };
        const int allowed_num_channels[] = { 1, 2, -1 };
        const int allowed_bitrates[] = { 4000,
                                         5000,
                                         6000,
                                         7000,
                                         8000,
                                         10000,
                                         12000,
                                         14000,
                                         16000,
                                         20000,
                                         24000,
                                         28000,
                                         32000,
                                         40000,
                                         -1 };

        check_frequency (resource, allowed_freq, "MP3");
        check_num_channels (resource, allowed_num_channels, "MP3");
        check_bitrate (resource, allowed_bitrates, "MP3");

        return "MP3";
}

static const char *
guess_aac_profile (GUPnPDIDLLiteResource *resource)
{
        const int allowed_freq[] = { 8000,
                                     11025,
                                     12000,
                                     16000,
                                     22050,
                                     24000,
                                     32000,
                                     44100,
                                     48000,
                                     -1 };
        const int allowed_num_channels[] = { 1, 2, -1 };
        int bitrate;

        check_frequency (resource, allowed_freq, "AAC_ISO");
        check_num_channels (resource, allowed_num_channels, "AAC_ISO");

        bitrate = gupnp_didl_lite_resource_get_bitrate (resource);
        if (bitrate > 0 && bitrate <= 4000) {
                return "AAC_ISO_320";
        } else if (bitrate <= 7200) {
                return "AAC_ISO";
        }
        return NULL;
}

static const char *
guess_wma_profile (GUPnPDIDLLiteResource *resource)
{
        int sample_freq;
        int channels;
        int bitrate;

        sample_freq = gupnp_didl_lite_resource_get_sample_freq (resource);
        channels = gupnp_didl_lite_resource_get_audio_channels (resource);
        bitrate = gupnp_didl_lite_resource_get_bitrate (resource);

        if (sample_freq > 0 && channels > 0 &&
            sample_freq <= 48000 && channels <= 2) {
                if (bitrate > 0 && bitrate <=  24125) {
                        return "WMABASE";
                } else {
                        return "WMAFULL";
                }
        } else {
                return "WMAPRO";
        }
}

static const char *
guess_mpeg_ts_profile (GUPnPDIDLLiteResource *resource)
{
        int width;
        int height;

        width = gupnp_didl_lite_resource_get_width (resource);
        height = gupnp_didl_lite_resource_get_height (resource);

        if (width > 0 && height < 0 && width / height == 16 / 9) {
                return "MPEG_TS_HD_NA";
        } else {
                return "MPEG_TS_SD_NA";
        }
}

const char *
dlna_guess_profile (GUPnPDIDLLiteResource *resource,
                    const char            *mime_type)
{
        if (g_str_has_prefix (mime_type, "image/jpeg")) {
                return guess_jpeg_profile (resource);
        } else if (g_str_has_prefix (mime_type, "image/png")) {
                return guess_png_profile (resource);
        } else if (g_str_has_prefix (mime_type,
                                     "audio/vnd.dolby.dd-raw")) {
                return guess_ac3_profile (resource);
        } else if (g_str_has_prefix (mime_type, "audio/mp4") ||
                   g_str_has_prefix (mime_type, "audio/3gpp")) {
                return guess_aac_profile (resource);
        } else if (g_str_has_prefix (mime_type, "audio/L16")) {
                return guess_lpcm_profile (resource);
        } else if (g_str_has_prefix (mime_type, "audio/mpeg")) {
                return guess_mp3_profile (resource);
        } else if (g_str_has_prefix (mime_type, "audio/x-ms-wma")) {
                return guess_wma_profile (resource);
        } else if (g_str_has_prefix (mime_type, "video/mpeg")) {
                return guess_mpeg_ts_profile (resource);
        } else if (g_str_has_prefix (mime_type, "video/mp4") ||
                   g_str_has_prefix (mime_type, "video/3gpp")) {
                return "MPEG4_P2_MP4_SP_VGA_AAC";
        } else {
                return NULL;
        }
}
