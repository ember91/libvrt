#include <vrt/vrt_read.h>

#include <stddef.h>

#include <vrt/vrt_common.h>

#include "vrt_fixed_point.h"
#include "vrt_util.h"

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

int32_t vrt_read_header(const void* buf, uint32_t buf_words, vrt_header* header) {
    /* Size is always 1 */
    const uint32_t words = 1;

    /* Check if buf size is sufficient */
    if (buf_words < words) {
        return VRT_ERR;
    }

    /* Word in header section */
    uint32_t b = *((const uint32_t*)buf);

    /* Decode in order from msb to lsb */
    header->packet_type  = (vrt_packet_type)msk(b, 28, 4);
    header->has.class_id = vrt_u2b(msk(b, 27, 1));
    header->has.trailer  = vrt_u2b(msk(b, 26, 1));
    header->tsm          = vrt_u2b(msk(b, 24, 1));
    header->tsi          = (vrt_tsi)msk(b, 22, 2);
    header->tsf          = (vrt_tsf)msk(b, 20, 2);
    header->packet_count = (uint8_t)msk(b, 16, 4);
    header->packet_size  = (uint16_t)msk(b, 0, 16);

    return words;
}

int32_t vrt_read_fields(const vrt_header* header, const void* buf, uint32_t buf_words, vrt_fields* fields) {
    const uint32_t words = vrt_words_fields(header);

    /* Check if buf size is sufficient */
    if (buf_words < words) {
        return VRT_ERR;
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
        b += 2;
    } else {
        /* Zero Class ID here, just to be sure */
        fields->class_id.oui                    = 0;
        fields->class_id.information_class_code = 0;
        fields->class_id.packet_class_code      = 0;
    }

    if (vrt_has_integer_timestamp(header->tsi)) {
        fields->integer_seconds_timestamp = b[0];
        b += 1;
    } else {
        /* Zero integer timestamp here, just to be sure */
        fields->integer_seconds_timestamp = 0;
    }

    if (vrt_has_fractional_timestamp(header->tsf)) {
        fields->fractional_seconds_timestamp = read_uint64(b);
        /* No point in increasing b pointer here, since we're finished */
    } else {
        /* Zero fractional timestamp here, just to be sure */
        fields->fractional_seconds_timestamp = 0;
    }

    return (int32_t)words;
}

int32_t vrt_read_trailer(const void* buf, uint32_t buf_words, vrt_trailer* trailer) {
    /* Number of words are always 1 */
    const uint32_t words = 1;

    /* Check if buf size is sufficient */
    if (buf_words < words) {
        return VRT_ERR;
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
        trailer->agc_or_mgc = msk(b, 16, 1);
    } else {
        trailer->agc_or_mgc = false;
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

    return words;
}

/**
 * Read IF context indicator field into its struct.
 *
 * \param b Word to read from.
 * \param c IF context to read into.
 *
 * \return Number of read words.
 */
static uint32_t if_context_read_indicator_field(uint32_t b, vrt_if_context* c) {
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

    return 1;
}

/**
 * Read IF context state and event indicator field into its struct.
 *
 * \param has True if it is included.
 * \param b   Word to read from.
 * \param s   State and event field struct to read into.
 *
 * \return Number of read words.
 */
static uint32_t if_context_read_state_and_event_indicators(bool has, uint32_t b, vrt_state_and_event* s) {
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
            s->agc_or_mgc = vrt_u2b(msk(b, 16, 1));
        } else {
            s->agc_or_mgc = 0;
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
    s->agc_or_mgc             = false;
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
 * \param has True if it is included.
 * \param b   Buffer to read from.
 * \param f   Data packet payload format struct to read into.
 *
 * \return Number of read words.
 */
static uint32_t if_context_read_data_packet_payload_format(bool                            has,
                                                           const uint32_t*                 b,
                                                           vrt_data_packet_payload_format* f) {
    if (has) {
        f->packing_method          = vrt_u2b(msk(b[0], 31, 1));
        f->real_or_complex         = (vrt_real_complex)msk(b[0], 29, 2);
        f->data_item_format        = (vrt_data_item_format)msk(b[0], 24, 5);
        f->sample_component_repeat = vrt_u2b(msk(b[0], 23, 1));
        f->event_tag_size          = (uint8_t)msk(b[0], 20, 3);
        f->channel_tag_size        = (uint8_t)msk(b[0], 16, 4);
        f->item_packing_field_size = (uint8_t)msk(b[0], 6, 6);
        f->data_item_size          = (uint8_t)msk(b[0], 0, 6);

        f->repeat_count = (uint16_t)msk(b[1], 16, 16);
        f->vector_size  = (uint16_t)msk(b[1], 0, 16);

        return 2;
    }

    f->packing_method          = false;
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
 * \param has True if it is included.
 * \param b   Buffer to read from.
 * \param l   Formatted GPS/INS geolocation struct to read into.
 *
 * \return Number of read words.
 */
static uint32_t if_context_read_formatted_geolocation(bool has, const uint32_t* b, vrt_formatted_geolocation* l) {
    if (has) {
        l->tsi = (vrt_tsi)msk(b[0], 26, 2);
        l->tsf = (vrt_tsf)msk(b[0], 24, 2);
        l->oui = msk(b[0], 0, 24);
        if (l->tsi == VRT_TSI_UNDEFINED) {
            l->integer_second_timestamp = 0xFFFFFFFFU;
        } else {
            l->integer_second_timestamp = b[1];
        }
        if (l->tsf == VRT_TSF_UNDEFINED) {
            l->fractional_second_timestamp = 0xFFFFFFFFFFFFFFFFU;
        } else {
            l->fractional_second_timestamp = read_uint64(b + 2);
        }
        l->latitude          = vrt_fixed_point_i32_to_double((int32_t)b[4], 22);
        l->longitude         = vrt_fixed_point_i32_to_double((int32_t)b[5], 22);
        l->altitude          = vrt_fixed_point_i32_to_double((int32_t)b[6], 5);
        l->speed_over_ground = vrt_fixed_point_u32_to_double(b[7], 16);
        l->heading_angle     = vrt_fixed_point_i32_to_double((int32_t)b[8], 22);
        l->track_angle       = vrt_fixed_point_i32_to_double((int32_t)b[9], 22);
        /* There seems to be an error in Rule 7.1.5.19-13. A correction seems to be 6.2.5.15-2 -> 7.1.5.19-2.*/
        l->magnetic_variation = vrt_fixed_point_i32_to_double((int32_t)b[10], 22);

        return 11;
    }

    l->tsi = VRT_TSI_UNDEFINED;
    l->tsf = VRT_TSF_UNDEFINED;
    l->oui = 0;
    /* See Rule 7.1.5.19-1: When the TSI or TSF fields are zero the corresponding Timestamp of Position Fix subfield
     * words shall take the value 0xFFFFFFFF. */
    l->integer_second_timestamp    = 0xFFFFFFFF;
    l->fractional_second_timestamp = 0xFFFFFFFFFFFFFFFF;
    l->latitude                    = vrt_fixed_point_i32_to_double(0x7FFFFFFF, 22);
    l->longitude                   = vrt_fixed_point_i32_to_double(0x7FFFFFFF, 22);
    l->altitude                    = vrt_fixed_point_i32_to_double(0x7FFFFFFF, 5);
    l->speed_over_ground           = vrt_fixed_point_u32_to_double(0x7FFFFFFF, 16);
    l->heading_angle               = vrt_fixed_point_i32_to_double(0x7FFFFFFF, 22);
    l->track_angle                 = vrt_fixed_point_i32_to_double(0x7FFFFFFF, 22);
    l->magnetic_variation          = vrt_fixed_point_i32_to_double(0x7FFFFFFF, 22);

    return 0;
}

/**
 * Read IF context ephemeris field into its struct.
 *
 * \param has True if it is included.
 * \param b   Buffer to read from [0] or [13].
 * \param e   Ephemeris struct to read into.
 *
 * \return Number of read words.
 */
static uint32_t if_context_read_ephemeris(bool has, const uint32_t* b, vrt_ephemeris* e) {
    if (has) {
        e->tsi = (vrt_tsi)msk(b[0], 26, 2);
        e->tsf = (vrt_tsf)msk(b[0], 24, 2);
        e->oui = msk(b[0], 0, 24);
        if (e->tsi == VRT_TSI_UNDEFINED) {
            e->integer_second_timestamp = 0xFFFFFFFF;
        } else {
            e->integer_second_timestamp = b[1];
        }
        if (e->tsf == VRT_TSF_UNDEFINED) {
            e->fractional_second_timestamp = 0xFFFFFFFFFFFFFFFF;
        } else {
            e->fractional_second_timestamp = read_uint64(b + 2);
        }
        e->position_x     = vrt_fixed_point_i32_to_double((int32_t)b[4], 5);
        e->position_y     = vrt_fixed_point_i32_to_double((int32_t)b[5], 5);
        e->position_z     = vrt_fixed_point_i32_to_double((int32_t)b[6], 5);
        e->altitude_alpha = vrt_fixed_point_i32_to_double((int32_t)b[7], 22);
        e->altitude_beta  = vrt_fixed_point_i32_to_double((int32_t)b[8], 22);
        e->altitude_phi   = vrt_fixed_point_i32_to_double((int32_t)b[9], 22);
        e->velocity_dx    = vrt_fixed_point_i32_to_double((int32_t)b[10], 16);
        e->velocity_dy    = vrt_fixed_point_i32_to_double((int32_t)b[11], 16);
        e->velocity_dz    = vrt_fixed_point_i32_to_double((int32_t)b[12], 16);

        return 13;
    }

    /* Note that Rule 7.1.5.21-8: "Each word of the ECEF Ephemeris field shall take the value 0x7FFF FFFF when
     * unknown" clashes with Rule 7.1.5.21-4: "The TSI, TSF, OUI, and Timestamp of Position Fix fields shall follow
     * the rules of the corresponding Formatted GPS Geolocation fields given in Section 7.1.5.19". For consistency
     * with formatted GPS/INS geolocation, 0xFFFFFFFF is chosen for timestamp fields. */
    e->tsi                         = VRT_TSI_UNDEFINED;
    e->tsf                         = VRT_TSF_UNDEFINED;
    e->oui                         = 0;
    e->integer_second_timestamp    = 0xFFFFFFFF;
    e->fractional_second_timestamp = 0xFFFFFFFFFFFFFFFF;
    e->position_x                  = vrt_fixed_point_i32_to_double(0x7FFFFFFF, 5);
    e->position_y                  = vrt_fixed_point_i32_to_double(0x7FFFFFFF, 5);
    e->position_z                  = vrt_fixed_point_i32_to_double(0x7FFFFFFF, 5);
    e->altitude_alpha              = vrt_fixed_point_i32_to_double(0x7FFFFFFF, 22);
    e->altitude_beta               = vrt_fixed_point_i32_to_double(0x7FFFFFFF, 22);
    e->altitude_phi                = vrt_fixed_point_i32_to_double(0x7FFFFFFF, 22);
    e->velocity_dx                 = vrt_fixed_point_i32_to_double(0x7FFFFFFF, 16);
    e->velocity_dy                 = vrt_fixed_point_i32_to_double(0x7FFFFFFF, 16);
    e->velocity_dz                 = vrt_fixed_point_i32_to_double(0x7FFFFFFF, 16);

    return 0;
}

/**
 * Read IF context GPS ASCII field into its struct.
 *
 * \param has True if it is included.
 * \param b   Buffer to read from.
 * \param g   GPS ASCII struct to read into.
 *
 * \return Number of read words.
 */
static uint32_t if_context_read_gps_ascii(bool has, const uint32_t* b, vrt_gps_ascii* g) {
    if (has) {
        g->oui             = msk(b[0], 0, 24);
        g->number_of_words = b[1];
        if (g->number_of_words == 0) {
            g->ascii = NULL;
        } else {
            g->ascii = (const char*)(b + 2);
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
static uint32_t if_context_read_association_lists(bool has, const uint32_t* b, vrt_context_association_lists* l) {
    if (has) {
        l->source_list_size                  = (uint16_t)msk(b[0], 16, 9);
        l->system_list_size                  = (uint16_t)msk(b[0], 0, 9);
        l->vector_component_list_size        = (uint16_t)msk(b[1], 16, 16);
        l->has.asynchronous_channel_tag_list = vrt_u2b(msk(b[1], 15, 1));
        l->asynchronous_channel_list_size    = (uint16_t)msk(b[1], 0, 15);

        uint32_t words = 2;
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

int32_t vrt_read_if_context(const void* buf, uint32_t buf_words, vrt_if_context* if_context) {
    /* Cannot count words here since the IF context section hasn't been read yet */

    uint32_t words = 1;

    if (buf_words < words) {
        return VRT_ERR;
    }

    const uint32_t* b = (const uint32_t*)buf;

    /* Go from msb to lsb. Make sure to zero fields if not present, just to be sure. */
    b += if_context_read_indicator_field(b[0], if_context);

    /* Replace context_words here instead of increasing it */
    words = vrt_words_if_context_indicator(&if_context->has);
    if (buf_words < words) {
        return VRT_ERR;
    }

    if (if_context->has.reference_point_identifier) {
        if_context->reference_point_identifier = b[0];
        b += 1;
    } else {
        if_context->reference_point_identifier = 0;
    }
    if (if_context->has.bandwidth) {
        if_context->bandwidth = vrt_fixed_point_i64_to_double((int64_t)read_uint64(b), 20);
        b += 2;
    } else {
        if_context->bandwidth = 0.0;
    }
    if (if_context->has.if_reference_frequency) {
        if_context->if_reference_frequency = vrt_fixed_point_i64_to_double((int64_t)read_uint64(b), 20);
        b += 2;
    } else {
        if_context->if_reference_frequency = 0.0;
    }
    if (if_context->has.rf_reference_frequency) {
        if_context->rf_reference_frequency = vrt_fixed_point_i64_to_double((int64_t)read_uint64(b), 20);
        b += 2;
    } else {
        if_context->rf_reference_frequency = 0.0;
    }
    if (if_context->has.rf_reference_frequency_offset) {
        if_context->rf_reference_frequency_offset = vrt_fixed_point_i64_to_double((int64_t)read_uint64(b), 20);
        b += 2;
    } else {
        if_context->rf_reference_frequency_offset = 0.0;
    }
    if (if_context->has.if_band_offset) {
        if_context->if_band_offset = vrt_fixed_point_i64_to_double((int64_t)read_uint64(b), 20);
        b += 2;
    } else {
        if_context->if_band_offset = 0.0;
    }
    if (if_context->has.reference_level) {
        if_context->reference_level = vrt_fixed_point_i16_to_float((int16_t)(b[0] & 0x0000FFFFU), 7);
        b += 1;
    } else {
        if_context->reference_level = 0.0F;
    }
    if (if_context->has.gain) {
        int16_t fp1             = b[0] & 0x0000FFFFU;
        int16_t fp2             = (b[0] >> 16U) & 0x0000FFFFU;
        if_context->gain.stage1 = vrt_fixed_point_i16_to_float(fp1, 7);
        if_context->gain.stage2 = vrt_fixed_point_i16_to_float(fp2, 7);
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
        if_context->sample_rate = vrt_fixed_point_i64_to_double((int64_t)read_uint64(b), 20);
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
        if_context->temperature = vrt_fixed_point_i16_to_float(b[0] & 0x0000FFFFU, 6);
        b += 1;
    } else {
        if_context->temperature = 0.0F;
    }
    if (if_context->has.device_identifier) {
        if_context->device_identifier.oui         = msk(b[0], 0, 24);
        if_context->device_identifier.device_code = (uint16_t)msk(b[1], 0, 16);
        b += 2;
    } else {
        if_context->device_identifier.oui         = 0;
        if_context->device_identifier.device_code = 0;
    }
    b += if_context_read_state_and_event_indicators(if_context->has.state_and_event_indicators, b[0],
                                                    &if_context->state_and_event_indicators);
    b += if_context_read_data_packet_payload_format(if_context->has.data_packet_payload_format, b,
                                                    &if_context->data_packet_payload_format);
    b += if_context_read_formatted_geolocation(if_context->has.formatted_gps_geolocation, b,
                                               &if_context->formatted_gps_geolocation);
    b += if_context_read_formatted_geolocation(if_context->has.formatted_ins_geolocation, b,
                                               &if_context->formatted_ins_geolocation);
    b += if_context_read_ephemeris(if_context->has.ecef_ephemeris, b, &if_context->ecef_ephemeris);
    b += if_context_read_ephemeris(if_context->has.relative_ephemeris, b, &if_context->relative_ephemeris);
    if (if_context->has.ephemeris_reference_identifier) {
        if_context->ephemeris_reference_identifier = b[0];
        b += 1;
    } else {
        if_context->ephemeris_reference_identifier = 0;
    }

    uint32_t gps_ascii_words = if_context_read_gps_ascii(if_context->has.gps_ascii, b, &if_context->gps_ascii);
    b += gps_ascii_words;
    words += gps_ascii_words;

    /* No need to increase b here since it is last */
    words += if_context_read_association_lists(if_context->has.context_association_lists, b,
                                               &if_context->context_association_lists);

    return (int32_t)words;
}
