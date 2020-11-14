#include <vrt/vrt_write.h>

#include <string.h>

#include <vrt/vrt_common.h>

#include "vrt_fixed_point.h"
#include "vrt_util.h"

/**
 * Mask a specified number of consecutive bits and shift them to a position in a word.
 *
 * \param val Word to mask.
 * \param pos Position of bits in word, starting from lsb.
 * \param n   Number of bits to mask.
 *
 * \return Bits after masking and shifting.
 */
static inline uint32_t msk(uint32_t val, uint32_t pos, uint32_t n) {
    uint32_t mask = (1U << n) - 1;
    return (val & mask) << pos;
}

/**
 * Write uint64 to uint32 buffer by interpreting the first word as most significant.
 *
 * \param val Value to write.
 * \param b   Buffer to write to.
 */
static inline void write_uint64(uint64_t val, uint32_t* b) {
    b[0] = (uint32_t)(val >> 32U);
    b[1] = (uint32_t)val;
}

int32_t vrt_write_header(const vrt_header* header, void* buf, uint32_t buf_words) {
    /* Number of words are always 1 */
    const uint32_t words = 1;

    /* Check if buf size is sufficient */
    if (buf_words < words) {
        return VRT_ERR;
    }

    /* Word in header section */
    uint32_t* b = (uint32_t*)buf;

    /* Rule 6.1.1-6: The reserved bits shall be set to 0. */
    b[0] = 0;

    b[0] |= msk(header->packet_type, 28, 4);
    b[0] |= msk(vrt_b2u(header->has.class_id), 27, 1);
    b[0] |= msk(vrt_b2u(header->has.trailer), 26, 1);
    b[0] |= msk(vrt_b2u(header->tsm), 24, 1);
    b[0] |= msk(header->tsi, 22, 2);
    b[0] |= msk(header->tsf, 20, 2);
    b[0] |= msk(header->packet_count, 16, 4);
    b[0] |= header->packet_size;

    return words;
}

int32_t vrt_write_fields(const vrt_header* header, const vrt_fields* fields, void* buf, uint32_t buf_words) {
    const uint32_t words = vrt_words_fields(header);

    /* Check if buf size is sufficient */
    if (buf_words < words) {
        return VRT_ERR;
    }

    uint32_t* b = ((uint32_t*)buf);

    if (vrt_has_stream_id(header->packet_type)) {
        b[0] = fields->stream_id;
        b += 1;
    }
    if (header->has.class_id) {
        b[0] = msk(fields->class_id.oui, 0, 24);
        b[1] = msk(fields->class_id.information_class_code, 16, 16) | fields->class_id.packet_class_code;
        b += 2;
    }
    if (vrt_has_integer_timestamp(header->tsi)) {
        b[0] = fields->integer_seconds_timestamp;
        b += 1;
    }
    if (vrt_has_fractional_timestamp(header->tsf)) {
        write_uint64(fields->fractional_seconds_timestamp, b);
        /* No point in increasing b pointer here */
    }

    return (int32_t)words;
}

int32_t vrt_write_trailer(const vrt_trailer* trailer, void* buf, uint32_t buf_words) {
    /* Number of words are always 1 */
    const uint32_t words = 1;

    /* Check if buf size is sufficient */
    if (buf_words < words) {
        return VRT_ERR;
    }

    uint32_t* b = (uint32_t*)buf;

    /* Recommendation 6.1.7-1: All unused trailer bits should be set to zero. */
    b[0] = 0;

    /* Go from msb to lsb */
    if (trailer->has.calibrated_time) {
        b[0] |= msk(vrt_b2u(trailer->calibrated_time), 19, 1);
        b[0] |= msk(1, 31, 1);
    }
    if (trailer->has.valid_data) {
        b[0] |= msk(vrt_b2u(trailer->valid_data), 18, 1);
        b[0] |= msk(1, 30, 1);
    }
    if (trailer->has.reference_lock) {
        b[0] |= msk(vrt_b2u(trailer->reference_lock), 17, 1);
        b[0] |= msk(1, 29, 1);
    }
    if (trailer->has.agc_or_mgc) {
        b[0] |= msk(vrt_b2u(trailer->agc_or_mgc), 16, 1);
        b[0] |= msk(1, 28, 1);
    }
    if (trailer->has.detected_signal) {
        b[0] |= msk(vrt_b2u(trailer->detected_signal), 15, 1);
        b[0] |= msk(1, 27, 1);
    }
    if (trailer->has.spectral_inversion) {
        b[0] |= msk(vrt_b2u(trailer->spectral_inversion), 14, 1);
        b[0] |= msk(1, 26, 1);
    }
    if (trailer->has.over_range) {
        b[0] |= msk(vrt_b2u(trailer->over_range), 13, 1);
        b[0] |= msk(1, 25, 1);
    }
    if (trailer->has.sample_loss) {
        b[0] |= msk(vrt_b2u(trailer->sample_loss), 12, 1);
        b[0] |= msk(1, 24, 1);
    }
    if (trailer->has.user_defined11) {
        b[0] |= msk(vrt_b2u(trailer->user_defined11), 11, 1);
        b[0] |= msk(1, 23, 1);
    }
    if (trailer->has.user_defined10) {
        b[0] |= msk(vrt_b2u(trailer->user_defined10), 10, 1);
        b[0] |= msk(1, 22, 1);
    }
    if (trailer->has.user_defined9) {
        b[0] |= msk(vrt_b2u(trailer->user_defined9), 9, 1);
        b[0] |= msk(1, 21, 1);
    }
    if (trailer->has.user_defined8) {
        b[0] |= msk(vrt_b2u(trailer->user_defined8), 8, 1);
        b[0] |= msk(1, 20, 1);
    }
    if (trailer->has.associated_context_packet_count) {
        b[0] |= msk(trailer->associated_context_packet_count, 0, 7);
        b[0] |= msk(1, 7, 1);
    }

    return words;
}

/**
 * Write IF context indicator field to buffer.
 *
 * \param c IF context to write.
 * \param b Buffer to write to.
 *
 * \return Number of written words.
 */
static uint32_t if_context_write_context_indicator_field(const vrt_if_context* c, uint32_t* b) {
    /* Zero reserved bits */
    b[0] = 0;

    b[0] |= msk(vrt_b2u(c->context_field_change_indicator), 31, 1);
    b[0] |= msk(vrt_b2u(c->has.reference_point_identifier), 30, 1);
    b[0] |= msk(vrt_b2u(c->has.bandwidth), 29, 1);
    b[0] |= msk(vrt_b2u(c->has.if_reference_frequency), 28, 1);
    b[0] |= msk(vrt_b2u(c->has.rf_reference_frequency), 27, 1);
    b[0] |= msk(vrt_b2u(c->has.rf_reference_frequency_offset), 26, 1);
    b[0] |= msk(vrt_b2u(c->has.if_band_offset), 25, 1);
    b[0] |= msk(vrt_b2u(c->has.reference_level), 24, 1);
    b[0] |= msk(vrt_b2u(c->has.gain), 23, 1);
    b[0] |= msk(vrt_b2u(c->has.over_range_count), 22, 1);
    b[0] |= msk(vrt_b2u(c->has.sample_rate), 21, 1);
    b[0] |= msk(vrt_b2u(c->has.timestamp_adjustment), 20, 1);
    b[0] |= msk(vrt_b2u(c->has.timestamp_calibration_time), 19, 1);
    b[0] |= msk(vrt_b2u(c->has.temperature), 18, 1);
    b[0] |= msk(vrt_b2u(c->has.device_identifier), 17, 1);
    b[0] |= msk(vrt_b2u(c->has.state_and_event_indicators), 16, 1);
    b[0] |= msk(vrt_b2u(c->has.data_packet_payload_format), 15, 1);
    b[0] |= msk(vrt_b2u(c->has.formatted_gps_geolocation), 14, 1);
    b[0] |= msk(vrt_b2u(c->has.formatted_ins_geolocation), 13, 1);
    b[0] |= msk(vrt_b2u(c->has.ecef_ephemeris), 12, 1);
    b[0] |= msk(vrt_b2u(c->has.relative_ephemeris), 11, 1);
    b[0] |= msk(vrt_b2u(c->has.ephemeris_reference_identifier), 10, 1);
    b[0] |= msk(vrt_b2u(c->has.gps_ascii), 9, 1);
    b[0] |= msk(vrt_b2u(c->has.context_association_lists), 8, 1);

    return 1;
}

/**
 * Write IF context state and event indicator field to buffer.
 *
 * \param has True if it is included.
 * \param s   State and event fields struct to write.
 * \param b   Buffer to write to.
 *
 * \return Number of written words.
 */
static uint32_t if_context_write_state_and_event_indicator_field(bool has, const vrt_state_and_event* s, uint32_t* b) {
    if (has) {
        /* Zero reserved bits */
        b[0] = 0;

        if (s->has.calibrated_time) {
            b[0] |= msk(vrt_b2u(s->has.calibrated_time), 31, 1);
            b[0] |= msk(vrt_b2u(s->calibrated_time), 19, 1);
        }
        if (s->has.valid_data) {
            b[0] |= msk(vrt_b2u(s->has.valid_data), 30, 1);
            b[0] |= msk(vrt_b2u(s->valid_data), 18, 1);
        }
        if (s->has.reference_lock) {
            b[0] |= msk(vrt_b2u(s->has.reference_lock), 29, 1);
            b[0] |= msk(vrt_b2u(s->reference_lock), 17, 1);
        }
        if (s->has.agc_or_mgc) {
            b[0] |= msk(vrt_b2u(s->has.agc_or_mgc), 28, 1);
            b[0] |= msk(vrt_b2u(s->agc_or_mgc), 16, 1);
        }
        if (s->has.detected_signal) {
            b[0] |= msk(vrt_b2u(s->has.detected_signal), 27, 1);
            b[0] |= msk(vrt_b2u(s->detected_signal), 15, 1);
        }
        if (s->has.spectral_inversion) {
            b[0] |= msk(vrt_b2u(s->has.spectral_inversion), 26, 1);
            b[0] |= msk(vrt_b2u(s->spectral_inversion), 14, 1);
        }
        if (s->has.over_range) {
            b[0] |= msk(vrt_b2u(s->has.over_range), 25, 1);
            b[0] |= msk(vrt_b2u(s->over_range), 13, 1);
        }
        if (s->has.sample_loss) {
            b[0] |= msk(vrt_b2u(s->has.sample_loss), 24, 1);
            b[0] |= msk(vrt_b2u(s->sample_loss), 12, 1);
        }

        b[0] |= msk(vrt_b2u(s->user_defined7), 7, 1);
        b[0] |= msk(vrt_b2u(s->user_defined6), 6, 1);
        b[0] |= msk(vrt_b2u(s->user_defined5), 5, 1);
        b[0] |= msk(vrt_b2u(s->user_defined4), 4, 1);
        b[0] |= msk(vrt_b2u(s->user_defined3), 3, 1);
        b[0] |= msk(vrt_b2u(s->user_defined2), 2, 1);
        b[0] |= msk(vrt_b2u(s->user_defined1), 1, 1);
        b[0] |= msk(vrt_b2u(s->user_defined0), 0, 1);

        return 1;
    }

    return 0;
}

/**
 * Write IF context data packet payload format field to buffer.
 *
 * \param has True if it is included.
 * \param f   Data packet payload format struct to write.
 * \param b   Buffer to write to.
 *
 * \return Number of written words.
 */
static uint32_t if_context_write_data_packet_payload_format(bool                                  has,
                                                            const vrt_data_packet_payload_format* f,
                                                            uint32_t*                             b) {
    if (has) {
        /* Zero reserved bits */
        b[0] = 0;

        b[0] |= msk(vrt_b2u(f->packing_method), 31, 1);
        b[0] |= msk(f->real_or_complex, 29, 2);
        b[0] |= msk(f->data_item_format, 24, 5);
        b[0] |= msk(vrt_b2u(f->sample_component_repeat), 23, 1);
        b[0] |= msk(f->event_tag_size, 20, 3);
        b[0] |= msk(f->channel_tag_size, 16, 4);
        b[0] |= msk(f->item_packing_field_size, 6, 6);
        b[0] |= msk(f->data_item_size, 0, 6);

        b[1] = msk(f->repeat_count, 16, 16) | msk(f->vector_size, 0, 16);

        return 2;
    }

    return 0;
}

/**
 * Write IF context formatted GPS/INS geolocation field to buffer.
 *
 * \param has True if it is included.
 * \param g   GPS/INS geolocation field to write.
 * \param b   Buffer to write to.
 *
 * \return Number of written words.
 */
static uint32_t if_context_write_formatted_geolocation(bool has, const vrt_formatted_geolocation* g, uint32_t* b) {
    if (has) {
        /* Zero reserved bits */
        b[0] = 0;

        b[0] |= msk(g->tsi, 26, 2);
        b[0] |= msk(g->tsf, 24, 2);
        b[0] |= msk(g->oui, 0, 24);
        b[1] = vrt_has_integer_timestamp(g->tsi) ? g->integer_second_timestamp : 0xFFFFFFFF;
        write_uint64(vrt_has_fractional_timestamp(g->tsf) ? g->fractional_second_timestamp : 0xFFFFFFFFFFFFFFFF, b + 2);
        b[4] = (uint32_t)vrt_double_to_fixed_point_i32(g->latitude, 22);
        b[5] = (uint32_t)vrt_double_to_fixed_point_i32(g->longitude, 22);
        b[6] = (uint32_t)vrt_double_to_fixed_point_i32(g->altitude, 5);
        b[7] = vrt_double_to_fixed_point_u32(g->speed_over_ground, 16);
        b[8] = (uint32_t)vrt_double_to_fixed_point_i32(g->heading_angle, 22);
        b[9] = (uint32_t)vrt_double_to_fixed_point_i32(g->track_angle, 22);
        /* There seems to be an error in Rule 7.1.5.19-13. A correction seems to be 6.2.5.15-2 -> 7.1.5.19-2.*/
        b[10] = (uint32_t)vrt_double_to_fixed_point_i32(g->magnetic_variation, 22);

        return 11;
    }

    return 0;
}

/**
 * Write IF context ephemeris field to buffer.
 *
 * \param has True if it is included.
 * \param e   Ephemeris struct to write.
 * \param b   Buffer to write to.
 *
 * \return Number of written words.
 */
static uint32_t if_context_write_ephemeris(bool has, const vrt_ephemeris* e, uint32_t* b) {
    if (has) {
        /* Zero reserved bits */
        b[0] = 0;

        b[0] |= msk(e->tsi, 26, 2);
        b[0] |= msk(e->tsf, 24, 2);
        b[0] |= msk(e->oui, 0, 24);
        b[1] = vrt_has_integer_timestamp(e->tsi) ? e->integer_second_timestamp : 0xFFFFFFFF;
        write_uint64(vrt_has_fractional_timestamp(e->tsf) ? e->fractional_second_timestamp : 0xFFFFFFFFFFFFFFFF, b + 2);
        b[4]  = (uint32_t)vrt_double_to_fixed_point_i32(e->position_x, 5);
        b[5]  = (uint32_t)vrt_double_to_fixed_point_i32(e->position_y, 5);
        b[6]  = (uint32_t)vrt_double_to_fixed_point_i32(e->position_z, 5);
        b[7]  = (uint32_t)vrt_double_to_fixed_point_i32(e->attitude_alpha, 22);
        b[8]  = (uint32_t)vrt_double_to_fixed_point_i32(e->attitude_beta, 22);
        b[9]  = (uint32_t)vrt_double_to_fixed_point_i32(e->attitude_phi, 22);
        b[10] = (uint32_t)vrt_double_to_fixed_point_i32(e->velocity_dx, 16);
        b[11] = (uint32_t)vrt_double_to_fixed_point_i32(e->velocity_dy, 16);
        b[12] = (uint32_t)vrt_double_to_fixed_point_i32(e->velocity_dz, 16);

        return 13;
    }

    return 0;
}

/**
 * Write IF context GPS ASCII field to buffer.
 *
 * \param has True if it is included.
 * \param g   GPS ASCII struct to write.
 * \param b   Buffer to write to.
 *
 * \return Number of written words.
 */
static uint32_t if_context_write_gps_ascii(bool has, const vrt_gps_ascii* g, uint32_t* b) {
    if (has) {
        b[0] = msk(g->oui, 0, 24);
        b[1] = g->number_of_words;

        /* Protect against g->ascii == NULL when number_of_words == 0 */
        if (g->number_of_words != 0) {
            memcpy(b + 2, g->ascii, sizeof(uint32_t) * g->number_of_words);
        }

        return 2 + g->number_of_words;
    }

    return 0;
}

/**
 * Write IF context association lists field to buffer.
 *
 * \param has True if it is included.
 * \param l   Context association lists struct to write.
 * \param b   Buffer to write to.
 */
static void if_context_write_context_association_lists(bool has, const vrt_context_association_lists* l, uint32_t* b) {
    if (has) {
        uint16_t sz1 = (uint16_t)msk(l->source_list_size, 0, 9);
        uint16_t sz2 = (uint16_t)msk(l->system_list_size, 0, 9);
        uint16_t sz3 = l->vector_component_list_size;
        uint16_t sz4 = (uint16_t)msk(l->asynchronous_channel_list_size, 0, 15);

        b[0] = sz1 << 16U | sz2;
        b[1] = sz3 << 16U | vrt_b2u(l->has.asynchronous_channel_tag_list) << 15U | sz4;
        b += 2;
        memcpy(b, l->source_context_association_list, sizeof(uint32_t) * sz1);
        b += sz1;
        memcpy(b, l->system_context_association_list, sizeof(uint32_t) * sz2);
        b += sz2;
        memcpy(b, l->vector_component_context_association_list, sizeof(uint32_t) * sz3);
        b += sz3;
        memcpy(b, l->asynchronous_channel_context_association_list, sizeof(uint32_t) * sz4);
        if (l->has.asynchronous_channel_tag_list) {
            b += sz4;
            memcpy(b, l->asynchronous_channel_tag_list, sizeof(uint32_t) * sz4);
        }
    }
}

int32_t vrt_write_if_context(const vrt_if_context* if_context, void* buf, uint32_t buf_words) {
    const uint32_t words = vrt_words_if_context(if_context);

    /* Check if buf size is sufficient */
    if (buf_words < words) {
        return VRT_ERR;
    }

    uint32_t* b = (uint32_t*)buf;

    b += if_context_write_context_indicator_field(if_context, b);
    if (if_context->has.reference_point_identifier) {
        b[0] = if_context->reference_point_identifier;
        b += 1;
    }
    if (if_context->has.bandwidth) {
        write_uint64((uint64_t)vrt_double_to_fixed_point_i64(if_context->bandwidth, 20), b);
        b += 2;
    }
    if (if_context->has.if_reference_frequency) {
        write_uint64((uint64_t)vrt_double_to_fixed_point_i64(if_context->if_reference_frequency, 20), b);
        b += 2;
    }
    if (if_context->has.rf_reference_frequency) {
        write_uint64((uint64_t)vrt_double_to_fixed_point_i64(if_context->rf_reference_frequency, 20), b);
        b += 2;
    }
    if (if_context->has.rf_reference_frequency_offset) {
        write_uint64((uint64_t)vrt_double_to_fixed_point_i64(if_context->rf_reference_frequency_offset, 20), b);
        b += 2;
    }
    if (if_context->has.if_band_offset) {
        write_uint64((uint64_t)vrt_double_to_fixed_point_i64(if_context->if_band_offset, 20), b);
        b += 2;
    }
    if (if_context->has.reference_level) {
        b[0] = (uint32_t)vrt_float_to_fixed_point_i16(if_context->reference_level, 7) & 0x0000FFFFU;
        b += 1;
    }
    if (if_context->has.gain) {
        b[0] = ((int32_t)msk(vrt_float_to_fixed_point_i16(if_context->gain.stage2, 7), 16, 16) & 0xFFFF0000) |
               ((int32_t)vrt_float_to_fixed_point_i16(if_context->gain.stage1, 7) & 0x0000FFFF);
        b += 1;
    }
    if (if_context->has.over_range_count) {
        b[0] = if_context->over_range_count;
        b += 1;
    }
    if (if_context->has.sample_rate) {
        write_uint64((uint64_t)vrt_double_to_fixed_point_i64(if_context->sample_rate, 20), b);
        b += 2;
    }
    if (if_context->has.timestamp_adjustment) {
        write_uint64(if_context->timestamp_adjustment, b);
        b += 2;
    }
    if (if_context->has.timestamp_calibration_time) {
        b[0] = if_context->timestamp_calibration_time;
        b += 1;
    }
    if (if_context->has.temperature) {
        b[0] = (int32_t)vrt_float_to_fixed_point_i16(if_context->temperature, 6) & 0x0000FFFF;
        b += 1;
    }
    if (if_context->has.device_identifier) {
        b[0] = msk(if_context->device_identifier.oui, 0, 24);
        b[1] = if_context->device_identifier.device_code;
        b += 2;
    }
    b += if_context_write_state_and_event_indicator_field(if_context->has.state_and_event_indicators,
                                                          &if_context->state_and_event_indicators, b);
    b += if_context_write_data_packet_payload_format(if_context->has.data_packet_payload_format,
                                                     &if_context->data_packet_payload_format, b);
    b += if_context_write_formatted_geolocation(if_context->has.formatted_gps_geolocation,
                                                &if_context->formatted_gps_geolocation, b);
    b += if_context_write_formatted_geolocation(if_context->has.formatted_ins_geolocation,
                                                &if_context->formatted_ins_geolocation, b);
    b += if_context_write_ephemeris(if_context->has.ecef_ephemeris, &if_context->ecef_ephemeris, b);
    b += if_context_write_ephemeris(if_context->has.relative_ephemeris, &if_context->relative_ephemeris, b);
    if (if_context->has.ephemeris_reference_identifier) {
        b[0] = if_context->ephemeris_reference_identifier;
        b += 1;
    }
    b += if_context_write_gps_ascii(if_context->has.gps_ascii, &if_context->gps_ascii, b);
    /* No need to increase b here since it is last */
    if_context_write_context_association_lists(if_context->has.context_association_lists,
                                               &if_context->context_association_lists, b);

    return (int32_t)words;
}
