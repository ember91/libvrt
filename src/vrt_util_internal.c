#include "vrt_util_internal.h"

#include "vrt_types.h"

/* Defined inline in header */
extern uint32_t vrt_b2u(bool b);
extern uint32_t vrt_u2b(uint32_t u);
extern bool     vrt_has_fractional_timestamp(enum vrt_tsf t);

int32_t vrt_words_if_context_indicator(const struct vrt_context_indicators* ind) {
    /* For context indicator field */
    int32_t words = 1;

    /* Note that Context Field Change Identifier contributes with 0 words, since it's only in
     * the context indicator field. */
    if (ind->reference_point_identifier) {
        words += 1;
    }
    if (ind->bandwidth) {
        words += 2;
    }
    if (ind->if_reference_frequency) {
        words += 2;
    }
    if (ind->rf_reference_frequency) {
        words += 2;
    }
    if (ind->rf_reference_frequency_offset) {
        words += 2;
    }
    if (ind->if_band_offset) {
        words += 2;
    }
    if (ind->reference_level) {
        words += 1;
    }
    if (ind->gain) {
        words += 1;
    }
    if (ind->over_range_count) {
        words += 1;
    }
    if (ind->sample_rate) {
        words += 2;
    }
    if (ind->timestamp_adjustment) {
        words += 2;
    }
    if (ind->timestamp_calibration_time) {
        words += 1;
    }
    if (ind->temperature) {
        words += 1;
    }
    if (ind->device_identifier) {
        words += 2;
    }
    if (ind->state_and_event_indicators) {
        words += 1;
    }
    if (ind->data_packet_payload_format) {
        words += 2;
    }
    if (ind->formatted_gps_geolocation) {
        words += 11;
    }
    if (ind->formatted_ins_geolocation) {
        words += 11;
    }
    if (ind->ecef_ephemeris) {
        words += 13;
    }
    if (ind->relative_ephemeris) {
        words += 13;
    }
    if (ind->ephemeris_reference_identifier) {
        words += 1;
    }

    return words;
}
