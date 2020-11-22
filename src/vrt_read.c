#include "vrt_read.h"

#include <stddef.h>

#include "vrt_error_code.h"
#include "vrt_types.h"
#include "vrt_util.h"
#include "vrt_words.h"

#include "vrt_fixed_point.h"
#include "vrt_util_internal.h"

/**
 * Mask and shift a specified number of consecutive bits from a specified position in a word.
 *
 * \param val Word to read from.
 * \param pos Position of bits in word, starting from lsb.
 * \param n   Number of bits to mask.
 *
 * \return Value of shifted and masked bits.
 */
static inline uint32_t msk(uint32_t val, uint32_t pos, uint32_t n) {
    uint32_t mask = (1U << n) - 1;
    return (val >> pos) & mask;
}

/**
 * Read uint64 from uint32 buffer by interpreting the first word as most significant.
 *
 * \param b Buffer to read from.
 *
 * \return uint64.
 */
static inline uint64_t read_uint64(const uint32_t* b) {
    return (uint64_t)b[0] << 32U | (uint64_t)b[1];
}

int32_t vrt_read_header(const void* buf, uint32_t buf_words, vrt_header* header, bool validate) {
    /* Size is always 1 */
    const int32_t words = 1;

    /* Check if buf size is sufficient */
    if (buf_words < (uint32_t)words) {
        return VRT_ERR_BUF_SIZE;
    }

    /* Word in header section */
    uint32_t b = *((const uint32_t*)buf);

    /* Decode in order from msb to lsb */
    header->packet_type  = (vrt_packet_type)msk(b, 28, 4);
    header->has.class_id = vrt_u2b(msk(b, 27, 1));
    header->has.trailer  = vrt_u2b(msk(b, 26, 1));
    header->tsm          = (vrt_tsm)msk(b, 24, 1);
    header->tsi          = (vrt_tsi)msk(b, 22, 2);
    header->tsf          = (vrt_tsf)msk(b, 20, 2);
    header->packet_count = (uint8_t)msk(b, 16, 4);
    header->packet_size  = (uint16_t)msk(b, 0, 16);

    if (validate) {
        if (header->packet_type > VRT_PT_EXT_CONTEXT) {
            return VRT_ERR_INVALID_PACKET_TYPE;
        }
        if (vrt_is_context(header->packet_type)) {
            if (header->has.trailer) {
                return VRT_ERR_TRAILER_IN_CONTEXT;
            }
        } else {
            if (header->tsm != VRT_TSM_FINE) {
                return VRT_ERR_TSM_IN_DATA;
            }
        }
        if ((b & 0x02000000U) != 0) {
            return VRT_ERR_RESERVED;
        }
    }

    return words;
}

int32_t vrt_read_fields(const vrt_header* header,
                        const void*       buf,
                        uint32_t          buf_words,
                        vrt_fields*       fields,
                        bool              validate) {
    const int32_t words = (int32_t)vrt_words_fields(header);

    /* Check if buf size is sufficient */
    if (buf_words < (uint32_t)words) {
        return VRT_ERR_BUF_SIZE;
    }

    const uint32_t* b = ((const uint32_t*)buf);

    if (vrt_has_stream_id(header->packet_type)) {
        fields->stream_id = b[0];
        b += 1;
    } else {
        /* Zero Stream ID here, just to be sure */
        fields->stream_id = 0;
    }

    if (header->has.class_id) {
        fields->class_id.oui                    = msk(b[0], 0, 24);
        fields->class_id.information_class_code = (uint16_t)msk(b[1], 16, 16);
        fields->class_id.packet_class_code      = (uint16_t)b[1];

        if (validate) {
            if ((b[0] & 0xFF000000U) != 0) {
                return VRT_ERR_RESERVED;
            }
        }

        b += 2;
    } else {
        /* Zero Class ID here, just to be sure */
        fields->class_id.oui                    = 0;
        fields->class_id.information_class_code = 0;
        fields->class_id.packet_class_code      = 0;
    }

    if (header->tsi == VRT_TSI_NONE) {
        /* Zero integer timestamp here, just to be sure */
        fields->integer_seconds_timestamp = 0;
    } else {
        fields->integer_seconds_timestamp = b[0];
        b += 1;
    }

    if (vrt_has_fractional_timestamp(header->tsf)) {
        fields->fractional_seconds_timestamp = read_uint64(b);

        if (validate) {
            if (header->tsf == VRT_TSF_REAL_TIME && fields->fractional_seconds_timestamp >= (uint64_t)1000000000000) {
                return VRT_ERR_BOUNDS_REAL_TIME;
            }
        }

        /* No point in increasing b pointer here, since we're finished */
    } else {
        /* Zero fractional timestamp here, just to be sure */
        fields->fractional_seconds_timestamp = 0;
    }

    return words;
}

int32_t vrt_read_trailer(const void* buf, uint32_t buf_words, vrt_trailer* trailer) {
    /* Number of words are always 1 */
    const int32_t words = 1;

    /* Check if buf size is sufficient */
    if (buf_words < (uint32_t)words) {
        return VRT_ERR_BUF_SIZE;
    }

    uint32_t b = *(const uint32_t*)buf;

    /* Go from msb to lsb. Make sure to zero fields if not present, just to be sure. */
    trailer->has.calibrated_time    = msk(b, 31, 1);
    trailer->has.valid_data         = msk(b, 30, 1);
    trailer->has.reference_lock     = msk(b, 29, 1);
    trailer->has.agc_or_mgc         = msk(b, 28, 1);
    trailer->has.detected_signal    = msk(b, 27, 1);
    trailer->has.spectral_inversion = msk(b, 26, 1);
    trailer->has.over_range         = msk(b, 25, 1);
    trailer->has.sample_loss        = msk(b, 24, 1);
    trailer->has.user_defined11     = msk(b, 23, 1);
    trailer->has.user_defined10     = msk(b, 22, 1);
    trailer->has.user_defined9      = msk(b, 21, 1);
    trailer->has.user_defined8      = msk(b, 20, 1);

    if (trailer->has.calibrated_time) {
        trailer->calibrated_time = msk(b, 19, 1);
    } else {
        trailer->calibrated_time = false;
    }
    if (trailer->has.valid_data) {
        trailer->valid_data = msk(b, 18, 1);
    } else {
        trailer->valid_data = false;
    }
    if (trailer->has.reference_lock) {
        trailer->reference_lock = msk(b, 17, 1);
    } else {
        trailer->reference_lock = false;
    }
    if (trailer->has.agc_or_mgc) {
        trailer->agc_or_mgc = (vrt_agc_or_mgc)msk(b, 16, 1);
    } else {
        trailer->agc_or_mgc = VRT_AOM_MGC;
    }
    if (trailer->has.detected_signal) {
        trailer->detected_signal = msk(b, 15, 1);
    } else {
        trailer->detected_signal = false;
    }
    if (trailer->has.spectral_inversion) {
        trailer->spectral_inversion = msk(b, 14, 1);
    } else {
        trailer->spectral_inversion = false;
    }
    if (trailer->has.over_range) {
        trailer->over_range = msk(b, 13, 1);
    } else {
        trailer->over_range = false;
    }
    if (trailer->has.sample_loss) {
        trailer->sample_loss = msk(b, 12, 1);
    } else {
        trailer->sample_loss = false;
    }
    if (trailer->has.user_defined11) {
        trailer->user_defined11 = msk(b, 11, 1);
    } else {
        trailer->user_defined11 = false;
    }
    if (trailer->has.user_defined10) {
        trailer->user_defined10 = msk(b, 10, 1);
    } else {
        trailer->user_defined10 = false;
    }
    if (trailer->has.user_defined9) {
        trailer->user_defined9 = msk(b, 9, 1);
    } else {
        trailer->user_defined9 = false;
    }
    if (trailer->has.user_defined8) {
        trailer->user_defined8 = msk(b, 8, 1);
    } else {
        trailer->user_defined8 = false;
    }

    trailer->has.associated_context_packet_count = vrt_u2b(msk(b, 7, 1));
    if (trailer->has.associated_context_packet_count) {
        trailer->associated_context_packet_count = (uint8_t)msk(b, 0, 7);
    } else {
        trailer->associated_context_packet_count = 0;
    }

    /*
     * Due to Recommendation 6.1.7-1: All unused trailer bits should be set to zero, there is not much to validate here.
     */

    return words;
}

/**
 * Read IF context indicator field into its struct.
 *
 * \param b        Word to read from.
 * \param c        IF context to read into.
 * \param validate True if data should be validated.
 *
 * \return Number of read words, or a negative number if error.
 */
static int32_t if_context_read_indicator_field(uint32_t b, vrt_if_context* c, bool validate) {
    c->context_field_change_indicator     = vrt_u2b(msk(b, 31, 1));
    c->has.reference_point_identifier     = vrt_u2b(msk(b, 30, 1));
    c->has.bandwidth                      = vrt_u2b(msk(b, 29, 1));
    c->has.if_reference_frequency         = vrt_u2b(msk(b, 28, 1));
    c->has.rf_reference_frequency         = vrt_u2b(msk(b, 27, 1));
    c->has.rf_reference_frequency_offset  = vrt_u2b(msk(b, 26, 1));
    c->has.if_band_offset                 = vrt_u2b(msk(b, 25, 1));
    c->has.reference_level                = vrt_u2b(msk(b, 24, 1));
    c->has.gain                           = vrt_u2b(msk(b, 23, 1));
    c->has.over_range_count               = vrt_u2b(msk(b, 22, 1));
    c->has.sample_rate                    = vrt_u2b(msk(b, 21, 1));
    c->has.timestamp_adjustment           = vrt_u2b(msk(b, 20, 1));
    c->has.timestamp_calibration_time     = vrt_u2b(msk(b, 19, 1));
    c->has.temperature                    = vrt_u2b(msk(b, 18, 1));
    c->has.device_identifier              = vrt_u2b(msk(b, 17, 1));
    c->has.state_and_event_indicators     = vrt_u2b(msk(b, 16, 1));
    c->has.data_packet_payload_format     = vrt_u2b(msk(b, 15, 1));
    c->has.formatted_gps_geolocation      = vrt_u2b(msk(b, 14, 1));
    c->has.formatted_ins_geolocation      = vrt_u2b(msk(b, 13, 1));
    c->has.ecef_ephemeris                 = vrt_u2b(msk(b, 12, 1));
    c->has.relative_ephemeris             = vrt_u2b(msk(b, 11, 1));
    c->has.ephemeris_reference_identifier = vrt_u2b(msk(b, 10, 1));
    c->has.gps_ascii                      = vrt_u2b(msk(b, 9, 1));
    c->has.context_association_lists      = vrt_u2b(msk(b, 8, 1));

    if (validate) {
        if ((b & 0x000000FFU) != 0) {
            return VRT_ERR_RESERVED;
        }
    }

    return 1;
}

/**
 * Read IF context state and event indicator field into its struct.
 *
 * \param has      True if it is included.
 * \param b        Word to read from.
 * \param s        State and event field struct to read into.
 * \param validate True if data should be validated.
 *
 * \return Number of read words, or a negative number if error.
 */
static int32_t if_context_read_state_and_event_indicators(bool has, uint32_t b, vrt_state_and_event* s, bool validate) {
    if (has) {
        s->has.calibrated_time    = vrt_u2b(msk(b, 31, 1));
        s->has.valid_data         = vrt_u2b(msk(b, 30, 1));
        s->has.reference_lock     = vrt_u2b(msk(b, 29, 1));
        s->has.agc_or_mgc         = vrt_u2b(msk(b, 28, 1));
        s->has.detected_signal    = vrt_u2b(msk(b, 27, 1));
        s->has.spectral_inversion = vrt_u2b(msk(b, 26, 1));
        s->has.over_range         = vrt_u2b(msk(b, 25, 1));
        s->has.sample_loss        = vrt_u2b(msk(b, 24, 1));

        if (s->has.calibrated_time) {
            s->calibrated_time = vrt_u2b(msk(b, 19, 1));
        } else {
            s->calibrated_time = 0;
        }
        if (s->has.valid_data) {
            s->valid_data = vrt_u2b(msk(b, 18, 1));
        } else {
            s->valid_data = 0;
        }
        if (s->has.reference_lock) {
            s->reference_lock = vrt_u2b(msk(b, 17, 1));
        } else {
            s->reference_lock = 0;
        }
        if (s->has.agc_or_mgc) {
            s->agc_or_mgc = (vrt_agc_or_mgc)msk(b, 16, 1);
        } else {
            s->agc_or_mgc = VRT_AOM_MGC;
        }
        if (s->has.detected_signal) {
            s->detected_signal = vrt_u2b(msk(b, 15, 1));
        } else {
            s->detected_signal = 0;
        }
        if (s->has.spectral_inversion) {
            s->spectral_inversion = vrt_u2b(msk(b, 14, 1));
        } else {
            s->spectral_inversion = 0;
        }
        if (s->has.over_range) {
            s->over_range = vrt_u2b(msk(b, 13, 1));
        } else {
            s->over_range = 0;
        }
        if (s->has.sample_loss) {
            s->sample_loss = vrt_u2b(msk(b, 12, 1));
        } else {
            s->sample_loss = 0;
        }

        s->user_defined7 = vrt_u2b(msk(b, 7, 1));
        s->user_defined6 = vrt_u2b(msk(b, 6, 1));
        s->user_defined5 = vrt_u2b(msk(b, 5, 1));
        s->user_defined4 = vrt_u2b(msk(b, 4, 1));
        s->user_defined3 = vrt_u2b(msk(b, 3, 1));
        s->user_defined2 = vrt_u2b(msk(b, 2, 1));
        s->user_defined1 = vrt_u2b(msk(b, 1, 1));
        s->user_defined0 = vrt_u2b(msk(b, 0, 1));

        if (validate) {
            if ((b & 0x00F00F00U) != 0) {
                return VRT_ERR_RESERVED;
            }
        }

        return 1;
    }

    s->has.calibrated_time    = false;
    s->has.valid_data         = false;
    s->has.reference_lock     = false;
    s->has.agc_or_mgc         = false;
    s->has.detected_signal    = false;
    s->has.spectral_inversion = false;
    s->has.over_range         = false;
    s->has.sample_loss        = false;
    s->calibrated_time        = false;
    s->valid_data             = false;
    s->reference_lock         = false;
    s->agc_or_mgc             = VRT_AOM_MGC;
    s->detected_signal        = false;
    s->spectral_inversion     = false;
    s->over_range             = false;
    s->sample_loss            = false;
    s->user_defined7          = false;
    s->user_defined6          = false;
    s->user_defined5          = false;
    s->user_defined4          = false;
    s->user_defined3          = false;
    s->user_defined2          = false;
    s->user_defined1          = false;
    s->user_defined0          = false;

    return 0;
}

/**
 * Read IF context data packet payload format field into its struct.
 *
 * \param has      True if it is included.
 * \param b        Buffer to read from.
 * \param f        Data packet payload format struct to read into.
 * \param validate True if data should be validated.
 *
 * \return Number of read words, or a negative number if error.
 */
static int32_t if_context_read_data_packet_payload_format(bool                            has,
                                                          const uint32_t*                 b,
                                                          vrt_data_packet_payload_format* f,
                                                          bool                            validate) {
    if (has) {
        f->packing_method          = (vrt_packing_method)msk(b[0], 31, 1);
        f->real_or_complex         = (vrt_real_complex)msk(b[0], 29, 2);
        f->data_item_format        = (vrt_data_item_format)msk(b[0], 24, 5);
        f->sample_component_repeat = vrt_u2b(msk(b[0], 23, 1));
        f->event_tag_size          = (uint8_t)msk(b[0], 20, 3);
        f->channel_tag_size        = (uint8_t)msk(b[0], 16, 4);
        f->item_packing_field_size = (uint8_t)msk(b[0], 6, 6);
        f->data_item_size          = (uint8_t)msk(b[0], 0, 6);

        f->repeat_count = (uint16_t)msk(b[1], 16, 16);
        f->vector_size  = (uint16_t)msk(b[1], 0, 16);

        if (validate) {
            if (f->real_or_complex > VRT_ROC_COMPLEX_POLAR) {
                return VRT_ERR_INVALID_REAL_OR_COMPLEX;
            }
            if ((f->data_item_format > VRT_DIF_SIGNED_VRT_6_BIT_EXPONENT &&
                 f->data_item_format < VRT_DIF_IEEE_754_SINGLE_PRECISION_FLOATING_POINT) ||
                f->data_item_format > VRT_DIF_UNSIGNED_VRT_6_BIT_EXPONENT) {
                return VRT_ERR_INVALID_DATA_ITEM_FORMAT;
            }

            if ((b[0] & 0x0000F000U) != 0) {
                return VRT_ERR_RESERVED;
            }
        }

        return 2;
    }

    f->packing_method          = VRT_PM_PROCESSING_EFFICIENT;
    f->real_or_complex         = VRT_ROC_REAL;
    f->data_item_format        = VRT_DIF_SIGNED_FIXED_POINT;
    f->sample_component_repeat = false;
    f->event_tag_size          = 0;
    f->channel_tag_size        = 0;
    f->item_packing_field_size = 0;
    f->data_item_size          = 0;
    f->repeat_count            = 0;
    f->vector_size             = 0;

    return 0;
}

/**
 * Read IF context formatted GPS/INS geolocation field into its struct.
 *
 * \param has      True if it is included.
 * \param b        Buffer to read from.
 * \param g        Formatted GPS/INS geolocation struct to read into.
 * \param validate True if data should be validated.
 *
 * \return Number of read words, or a negative number if error.
 */
static int32_t if_context_read_formatted_geolocation(bool                       has,
                                                     const uint32_t*            b,
                                                     vrt_formatted_geolocation* g,
                                                     bool                       validate) {
    if (has) {
        g->tsi                         = (vrt_tsi)msk(b[0], 26, 2);
        g->tsf                         = (vrt_tsf)msk(b[0], 24, 2);
        g->oui                         = msk(b[0], 0, 24);
        g->integer_second_timestamp    = b[1];
        g->fractional_second_timestamp = read_uint64(b + 2);
        g->has.latitude                = (b[4] != VRT_UNSPECIFIED_FIXED_POINT);
        g->latitude                    = vrt_fixed_point_i32_to_double((int32_t)b[4], VRT_RADIX_ANGLE);
        g->has.longitude               = (b[5] != VRT_UNSPECIFIED_FIXED_POINT);
        g->longitude                   = vrt_fixed_point_i32_to_double((int32_t)b[5], VRT_RADIX_ANGLE);
        g->has.altitude                = (b[6] != VRT_UNSPECIFIED_FIXED_POINT);
        g->altitude                    = vrt_fixed_point_i32_to_double((int32_t)b[6], VRT_RADIX_ALTITUDE);
        g->has.speed_over_ground       = (b[7] != VRT_UNSPECIFIED_FIXED_POINT);
        g->speed_over_ground           = vrt_fixed_point_u32_to_double(b[7], VRT_RADIX_SPEED_VELOCITY);
        g->has.heading_angle           = (b[8] != VRT_UNSPECIFIED_FIXED_POINT);
        g->heading_angle               = vrt_fixed_point_i32_to_double((int32_t)b[8], VRT_RADIX_ANGLE);
        g->has.track_angle             = (b[9] != VRT_UNSPECIFIED_FIXED_POINT);
        g->track_angle                 = vrt_fixed_point_i32_to_double((int32_t)b[9], VRT_RADIX_ANGLE);
        g->has.magnetic_variation      = (b[10] != VRT_UNSPECIFIED_FIXED_POINT);
        /* There seems to be an error in Rule 7.1.5.19-13. A correction seems to be 6.2.5.15-2 -> 7.1.5.19-2.*/
        g->magnetic_variation = vrt_fixed_point_i32_to_double((int32_t)b[10], VRT_RADIX_ANGLE);

        if (validate) {
            if ((b[0] & 0xF0000000U) != 0) {
                return VRT_ERR_RESERVED;
            }
            if (g->tsi == VRT_TSI_UNDEFINED && g->integer_second_timestamp != VRT_UNSPECIFIED_TSI) {
                return VRT_ERR_SET_INTEGER_SECOND_TIMESTAMP;
            }
            if (g->tsf == VRT_TSF_UNDEFINED && g->fractional_second_timestamp != VRT_UNSPECIFIED_TSF) {
                return VRT_ERR_SET_FRACTIONAL_SECOND_TIMESTAMP;
            }
            if (g->tsf == VRT_TSF_REAL_TIME && g->fractional_second_timestamp >= (uint64_t)1000000000000) {
                return VRT_ERR_BOUNDS_REAL_TIME;
            }
            if (g->has.latitude && (g->latitude < -90.0 || g->latitude > 90.0)) {
                return VRT_ERR_BOUNDS_LATITUDE;
            }
            if (g->has.longitude && (g->longitude < -180.0 || g->longitude > 180.0)) {
                return VRT_ERR_BOUNDS_LONGITUDE;
            }
            if (g->has.heading_angle && (g->heading_angle < 0.0 || g->heading_angle > 359.999999761582)) {
                return VRT_ERR_BOUNDS_HEADING_ANGLE;
            }
            if (g->has.track_angle && (g->track_angle < 0.0 || g->track_angle > 359.999999761582)) {
                return VRT_ERR_BOUNDS_TRACK_ANGLE;
            }
            if (g->has.magnetic_variation && (g->magnetic_variation < -180.0 || g->magnetic_variation > 180.0)) {
                return VRT_ERR_BOUNDS_MAGNETIC_VARIATION;
            }
        }

        return 11;
    }

    g->tsi = VRT_TSI_UNDEFINED;
    g->tsf = VRT_TSF_UNDEFINED;
    g->oui = 0;

    g->has.latitude           = false;
    g->has.longitude          = false;
    g->has.altitude           = false;
    g->has.speed_over_ground  = false;
    g->has.heading_angle      = false;
    g->has.track_angle        = false;
    g->has.magnetic_variation = false;

    /* See Rule 7.1.5.19-1: When the TSI or TSF fields are zero the corresponding Timestamp of Position Fix subfield
     * words shall take the value 0xFFFFFFFF. */
    g->integer_second_timestamp    = VRT_UNSPECIFIED_TSI;
    g->fractional_second_timestamp = VRT_UNSPECIFIED_TSF;
    g->latitude                    = vrt_fixed_point_i32_to_double(VRT_UNSPECIFIED_FIXED_POINT, VRT_RADIX_ANGLE);
    g->longitude                   = vrt_fixed_point_i32_to_double(VRT_UNSPECIFIED_FIXED_POINT, VRT_RADIX_ANGLE);
    g->altitude                    = vrt_fixed_point_i32_to_double(VRT_UNSPECIFIED_FIXED_POINT, VRT_RADIX_ALTITUDE);
    g->speed_over_ground  = vrt_fixed_point_u32_to_double(VRT_UNSPECIFIED_FIXED_POINT, VRT_RADIX_SPEED_VELOCITY);
    g->heading_angle      = vrt_fixed_point_i32_to_double(VRT_UNSPECIFIED_FIXED_POINT, VRT_RADIX_ANGLE);
    g->track_angle        = vrt_fixed_point_i32_to_double(VRT_UNSPECIFIED_FIXED_POINT, VRT_RADIX_ANGLE);
    g->magnetic_variation = vrt_fixed_point_i32_to_double(VRT_UNSPECIFIED_FIXED_POINT, VRT_RADIX_ANGLE);

    return 0;
}

/**
 * Read IF context ephemeris field into its struct.
 *
 * \param has      True if it is included.
 * \param b        Buffer to read from [0] or [13].
 * \param e        Ephemeris struct to read into.
 * \param validate True if data should be validated.
 *
 * \return Number of read words, or a negative number if error.
 */
static int32_t if_context_read_ephemeris(bool has, const uint32_t* b, vrt_ephemeris* e, bool validate) {
    if (has) {
        e->tsi                         = (vrt_tsi)msk(b[0], 26, 2);
        e->tsf                         = (vrt_tsf)msk(b[0], 24, 2);
        e->oui                         = msk(b[0], 0, 24);
        e->integer_second_timestamp    = b[1];
        e->fractional_second_timestamp = read_uint64(b + 2);

        e->has.position_x     = (b[4] != VRT_UNSPECIFIED_FIXED_POINT);
        e->position_x         = vrt_fixed_point_i32_to_double((int32_t)b[4], VRT_RADIX_POSITION);
        e->has.position_y     = (b[5] != VRT_UNSPECIFIED_FIXED_POINT);
        e->position_y         = vrt_fixed_point_i32_to_double((int32_t)b[5], VRT_RADIX_POSITION);
        e->has.position_z     = (b[6] != VRT_UNSPECIFIED_FIXED_POINT);
        e->position_z         = vrt_fixed_point_i32_to_double((int32_t)b[6], VRT_RADIX_POSITION);
        e->has.attitude_alpha = (b[7] != VRT_UNSPECIFIED_FIXED_POINT);
        e->attitude_alpha     = vrt_fixed_point_i32_to_double((int32_t)b[7], VRT_RADIX_ANGLE);
        e->has.attitude_beta  = (b[8] != VRT_UNSPECIFIED_FIXED_POINT);
        e->attitude_beta      = vrt_fixed_point_i32_to_double((int32_t)b[8], VRT_RADIX_ANGLE);
        e->has.attitude_phi   = (b[9] != VRT_UNSPECIFIED_FIXED_POINT);
        e->attitude_phi       = vrt_fixed_point_i32_to_double((int32_t)b[9], VRT_RADIX_ANGLE);
        e->has.velocity_dx    = (b[10] != VRT_UNSPECIFIED_FIXED_POINT);
        e->velocity_dx        = vrt_fixed_point_i32_to_double((int32_t)b[10], VRT_RADIX_SPEED_VELOCITY);
        e->has.velocity_dy    = (b[11] != VRT_UNSPECIFIED_FIXED_POINT);
        e->velocity_dy        = vrt_fixed_point_i32_to_double((int32_t)b[11], VRT_RADIX_SPEED_VELOCITY);
        e->has.velocity_dz    = (b[12] != VRT_UNSPECIFIED_FIXED_POINT);
        e->velocity_dz        = vrt_fixed_point_i32_to_double((int32_t)b[12], VRT_RADIX_SPEED_VELOCITY);

        if (validate) {
            if ((b[0] & 0xF0000000U) != 0) {
                return VRT_ERR_RESERVED;
            }
            if (e->tsi == VRT_TSI_UNDEFINED && e->integer_second_timestamp != VRT_UNSPECIFIED_TSI) {
                return VRT_ERR_SET_INTEGER_SECOND_TIMESTAMP;
            }
            if (e->tsf == VRT_TSF_UNDEFINED && e->fractional_second_timestamp != VRT_UNSPECIFIED_TSF) {
                return VRT_ERR_SET_FRACTIONAL_SECOND_TIMESTAMP;
            }
            if (e->tsf == VRT_TSF_REAL_TIME && e->fractional_second_timestamp >= (uint64_t)1000000000000) {
                return VRT_ERR_BOUNDS_REAL_TIME;
            }
        }

        return 13;
    }

    e->tsi = VRT_TSI_UNDEFINED;
    e->tsf = VRT_TSF_UNDEFINED;
    e->oui = 0;

    e->has.position_x     = false;
    e->has.position_y     = false;
    e->has.position_z     = false;
    e->has.attitude_alpha = false;
    e->has.attitude_beta  = false;
    e->has.attitude_phi   = false;
    e->has.velocity_dx    = false;
    e->has.velocity_dy    = false;
    e->has.velocity_dz    = false;

    /* Note that Rule 7.1.5.21-8: "Each word of the ECEF Ephemeris field shall take the value 0x7FFF FFFF when
     * unknown" clashes with Rule 7.1.5.21-4: "The TSI, TSF, OUI, and Timestamp of Position Fix fields shall follow
     * the rules of the corresponding Formatted GPS Geolocation fields given in Section 7.1.5.19". For consistency
     * with formatted GPS/INS geolocation, 0xFFFFFFFF is chosen for timestamp fields. */
    e->integer_second_timestamp    = VRT_UNSPECIFIED_TSI;
    e->fractional_second_timestamp = VRT_UNSPECIFIED_TSF;
    e->position_x                  = vrt_fixed_point_i32_to_double(VRT_UNSPECIFIED_FIXED_POINT, VRT_RADIX_POSITION);
    e->position_y                  = vrt_fixed_point_i32_to_double(VRT_UNSPECIFIED_FIXED_POINT, VRT_RADIX_POSITION);
    e->position_z                  = vrt_fixed_point_i32_to_double(VRT_UNSPECIFIED_FIXED_POINT, VRT_RADIX_POSITION);
    e->attitude_alpha              = vrt_fixed_point_i32_to_double(VRT_UNSPECIFIED_FIXED_POINT, VRT_RADIX_ANGLE);
    e->attitude_beta               = vrt_fixed_point_i32_to_double(VRT_UNSPECIFIED_FIXED_POINT, VRT_RADIX_ANGLE);
    e->attitude_phi                = vrt_fixed_point_i32_to_double(VRT_UNSPECIFIED_FIXED_POINT, VRT_RADIX_ANGLE);
    e->velocity_dx = vrt_fixed_point_i32_to_double(VRT_UNSPECIFIED_FIXED_POINT, VRT_RADIX_SPEED_VELOCITY);
    e->velocity_dy = vrt_fixed_point_i32_to_double(VRT_UNSPECIFIED_FIXED_POINT, VRT_RADIX_SPEED_VELOCITY);
    e->velocity_dz = vrt_fixed_point_i32_to_double(VRT_UNSPECIFIED_FIXED_POINT, VRT_RADIX_SPEED_VELOCITY);

    return 0;
}

/**
 * Read IF context GPS ASCII field into its struct.
 *
 * \param has      True if it is included.
 * \param b        Buffer to read from.
 * \param g        GPS ASCII struct to read into.
 * \param validate True if data should be validated.
 *
 * \return Number of read words, or a negative number if error.
 */
static int32_t if_context_read_gps_ascii(bool has, const uint32_t* b, vrt_gps_ascii* g, bool validate) {
    if (has) {
        g->oui             = msk(b[0], 0, 24);
        g->number_of_words = b[1];
        if (g->number_of_words == 0) {
            g->ascii = NULL;
        } else {
            g->ascii = (const char*)(b + 2);
        }

        if (validate) {
            if ((b[0] & 0xFF000000U) != 0) {
                return VRT_ERR_RESERVED;
            }
        }

        return 2 + g->number_of_words;
    }

    g->oui             = 0;
    g->number_of_words = 0;
    g->ascii           = NULL;

    return 0;
}

/**
 * Read IF context association lists field into its struct.
 *
 * \param has True if it is included.
 * \param b   Buffer to read from.
 * \param l   Association lists struct to read into.
 *
 * \return Number of read words.
 */
static int32_t if_context_read_association_lists(bool has, const uint32_t* b, vrt_context_association_lists* l) {
    if (has) {
        l->source_list_size                  = (uint16_t)msk(b[0], 16, 9);
        l->system_list_size                  = (uint16_t)msk(b[0], 0, 9);
        l->vector_component_list_size        = (uint16_t)msk(b[1], 16, 16);
        l->has.asynchronous_channel_tag_list = vrt_u2b(msk(b[1], 15, 1));
        l->asynchronous_channel_list_size    = (uint16_t)msk(b[1], 0, 15);

        int32_t words = 2;
        if (l->source_list_size == 0) {
            l->source_context_association_list = NULL;
        } else {
            l->source_context_association_list = b + words;
            words += l->source_list_size;
        }
        if (l->system_list_size == 0) {
            l->system_context_association_list = NULL;
        } else {
            l->system_context_association_list = b + words;
            words += l->system_list_size;
        }
        if (l->vector_component_list_size == 0) {
            l->vector_component_context_association_list = NULL;
        } else {
            l->vector_component_context_association_list = b + words;
            words += l->vector_component_list_size;
        }
        if (l->asynchronous_channel_list_size == 0) {
            l->asynchronous_channel_context_association_list = NULL;
            l->asynchronous_channel_tag_list                 = NULL;
        } else {
            l->asynchronous_channel_context_association_list = b + words;
            words += l->asynchronous_channel_list_size;
            if (l->has.asynchronous_channel_tag_list) {
                l->asynchronous_channel_tag_list = b + words;
                words += l->asynchronous_channel_list_size;
            } else {
                l->asynchronous_channel_tag_list = NULL;
            }
        }

        return words;
    }

    l->source_list_size                  = 0;
    l->system_list_size                  = 0;
    l->vector_component_list_size        = 0;
    l->has.asynchronous_channel_tag_list = 0;
    l->asynchronous_channel_list_size    = 0;

    l->source_context_association_list               = NULL;
    l->system_context_association_list               = NULL;
    l->vector_component_context_association_list     = NULL;
    l->asynchronous_channel_context_association_list = NULL;
    l->asynchronous_channel_tag_list                 = NULL;

    return 0;
}

int32_t vrt_read_if_context(const void* buf, uint32_t buf_words, vrt_if_context* if_context, bool validate) {
    /* Cannot count words here since the IF context section hasn't been read yet */

    int32_t words = 1;

    if (buf_words < (uint32_t)words) {
        return VRT_ERR_BUF_SIZE;
    }

    const uint32_t* b = (const uint32_t*)buf;

    /* Go from msb to lsb. Make sure to zero fields if not present, just to be sure. */
    int32_t rv = if_context_read_indicator_field(b[0], if_context, validate);
    if (rv < 0) {
        return rv;
    }
    b += rv;

    /* Replace context_words here instead of increasing it */
    words = vrt_words_if_context_indicator(&if_context->has);
    if (buf_words < (uint32_t)words) {
        return VRT_ERR_BUF_SIZE;
    }

    if (if_context->has.reference_point_identifier) {
        if_context->reference_point_identifier = b[0];
        b += 1;
    } else {
        if_context->reference_point_identifier = 0;
    }
    if (if_context->has.bandwidth) {
        if_context->bandwidth = vrt_fixed_point_i64_to_double((int64_t)read_uint64(b), VRT_RADIX_FREQUENCY);

        if (validate) {
            if (if_context->bandwidth < 0.0) {
                return VRT_ERR_BOUNDS_BANDWIDTH;
            }
        }

        b += 2;
    } else {
        if_context->bandwidth = 0.0;
    }
    if (if_context->has.if_reference_frequency) {
        if_context->if_reference_frequency =
            vrt_fixed_point_i64_to_double((int64_t)read_uint64(b), VRT_RADIX_FREQUENCY);
        b += 2;
    } else {
        if_context->if_reference_frequency = 0.0;
    }
    if (if_context->has.rf_reference_frequency) {
        if_context->rf_reference_frequency =
            vrt_fixed_point_i64_to_double((int64_t)read_uint64(b), VRT_RADIX_FREQUENCY);
        b += 2;
    } else {
        if_context->rf_reference_frequency = 0.0;
    }
    if (if_context->has.rf_reference_frequency_offset) {
        if_context->rf_reference_frequency_offset =
            vrt_fixed_point_i64_to_double((int64_t)read_uint64(b), VRT_RADIX_FREQUENCY);
        b += 2;
    } else {
        if_context->rf_reference_frequency_offset = 0.0;
    }
    if (if_context->has.if_band_offset) {
        if_context->if_band_offset = vrt_fixed_point_i64_to_double((int64_t)read_uint64(b), VRT_RADIX_FREQUENCY);
        b += 2;
    } else {
        if_context->if_band_offset = 0.0;
    }
    if (if_context->has.reference_level) {
        if_context->reference_level =
            vrt_fixed_point_i16_to_float((int16_t)(b[0] & 0x0000FFFFU), VRT_RADIX_REFERENCE_LEVEL);

        if (validate) {
            if ((b[0] & 0xFFFF0000U) != 0) {
                return VRT_ERR_RESERVED;
            }
        }

        b += 1;
    } else {
        if_context->reference_level = 0.0F;
    }
    if (if_context->has.gain) {
        int16_t fp1             = b[0] & 0x0000FFFFU;
        int16_t fp2             = (b[0] >> 16U) & 0x0000FFFFU;
        if_context->gain.stage1 = vrt_fixed_point_i16_to_float(fp1, VRT_RADIX_GAIN);
        if_context->gain.stage2 = vrt_fixed_point_i16_to_float(fp2, VRT_RADIX_GAIN);
        b += 1;
    } else {
        if_context->gain.stage1 = 0.0F;
        if_context->gain.stage2 = 0.0F;
    }
    if (if_context->has.over_range_count) {
        if_context->over_range_count = b[0];
        b += 1;
    } else {
        if_context->over_range_count = 0;
    }
    if (if_context->has.sample_rate) {
        if_context->sample_rate = vrt_fixed_point_i64_to_double((int64_t)read_uint64(b), VRT_RADIX_FREQUENCY);

        if (validate) {
            if (if_context->sample_rate < 0.0) {
                return VRT_ERR_BOUNDS_SAMPLE_RATE;
            }
        }

        b += 2;
    } else {
        if_context->sample_rate = 0.0;
    }
    if (if_context->has.timestamp_adjustment) {
        if_context->timestamp_adjustment = read_uint64(b);
        b += 2;
    } else {
        if_context->timestamp_adjustment = 0;
    }
    if (if_context->has.timestamp_calibration_time) {
        if_context->timestamp_calibration_time = b[0];
        b += 1;
    } else {
        if_context->timestamp_calibration_time = 0;
    }
    if (if_context->has.temperature) {
        if_context->temperature = vrt_fixed_point_i16_to_float(b[0] & 0x0000FFFFU, VRT_RADIX_TEMPERATURE);

        if (validate) {
            if (if_context->temperature < -273.15F) {
                return VRT_ERR_BOUNDS_TEMPERATURE;
            }
            if ((b[0] & 0xFFFF0000U) != 0) {
                return VRT_ERR_RESERVED;
            }
        }

        b += 1;
    } else {
        if_context->temperature = 0.0F;
    }
    if (if_context->has.device_identifier) {
        if_context->device_identifier.oui         = msk(b[0], 0, 24);
        if_context->device_identifier.device_code = (uint16_t)msk(b[1], 0, 16);

        if (validate) {
            if ((b[0] & 0xFF000000U) != 0 || (b[1] & 0xFFFF0000U) != 0) {
                return VRT_ERR_RESERVED;
            }
        }

        b += 2;
    } else {
        if_context->device_identifier.oui         = 0;
        if_context->device_identifier.device_code = 0;
    }
    rv = if_context_read_state_and_event_indicators(if_context->has.state_and_event_indicators, b[0],
                                                    &if_context->state_and_event_indicators, validate);
    if (rv < 0) {
        return rv;
    }
    b += rv;
    rv = if_context_read_data_packet_payload_format(if_context->has.data_packet_payload_format, b,
                                                    &if_context->data_packet_payload_format, validate);
    if (rv < 0) {
        return rv;
    }
    b += rv;
    rv = if_context_read_formatted_geolocation(if_context->has.formatted_gps_geolocation, b,
                                               &if_context->formatted_gps_geolocation, validate);
    if (rv < 0) {
        return rv;
    }
    b += rv;
    rv = if_context_read_formatted_geolocation(if_context->has.formatted_ins_geolocation, b,
                                               &if_context->formatted_ins_geolocation, validate);
    if (rv < 0) {
        return rv;
    }
    b += rv;
    rv = if_context_read_ephemeris(if_context->has.ecef_ephemeris, b, &if_context->ecef_ephemeris, validate);
    if (rv < 0) {
        return rv;
    }
    b += rv;
    rv = if_context_read_ephemeris(if_context->has.relative_ephemeris, b, &if_context->relative_ephemeris, validate);
    if (rv < 0) {
        return rv;
    }
    b += rv;
    if (if_context->has.ephemeris_reference_identifier) {
        if_context->ephemeris_reference_identifier = b[0];
        b += 1;
    } else {
        if_context->ephemeris_reference_identifier = 0;
    }

    rv = if_context_read_gps_ascii(if_context->has.gps_ascii, b, &if_context->gps_ascii, validate);
    if (rv < 0) {
        return rv;
    }
    b += rv;
    words += rv;

    /* No need to increase b here since it is last */
    rv = if_context_read_association_lists(if_context->has.context_association_lists, b,
                                           &if_context->context_association_lists);
    if (rv < 0) {
        return rv;
    }
    words += rv;

    return words;
}
