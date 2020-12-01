#include "init_garbage.h"

#include <cstdint>

#include <vrt/vrt_types.h>

void init_garbage_header(struct vrt_header* h) {
    h->packet_type  = static_cast<vrt_packet_type>(0xFFFFFFFF);
    h->has.class_id = true;
    h->has.trailer  = true;
    h->tsm          = VRT_TSM_COARSE;
    h->tsi          = static_cast<vrt_tsi>(0xF);
    h->tsf          = static_cast<vrt_tsf>(0xF);
    h->packet_count = 0xF;
    h->packet_size  = 0xFFFF;
}

void init_garbage_fields(struct vrt_fields* f) {
    f->stream_id                       = 0xFFFFFFFF;
    f->class_id.oui                    = 0x00FFFFFF;
    f->class_id.information_class_code = 0xFFFF;
    f->class_id.packet_class_code      = 0xFFFF;
    f->integer_seconds_timestamp       = 0xFFFFFFFF;
    f->fractional_seconds_timestamp    = 0xFFFFFFFFFFFFFFFF;
}

void init_garbage_trailer(struct vrt_trailer* t) {
    t->has.calibrated_time                 = true;
    t->has.valid_data                      = true;
    t->has.reference_lock                  = true;
    t->has.agc_or_mgc                      = true;
    t->has.detected_signal                 = true;
    t->has.spectral_inversion              = true;
    t->has.over_range                      = true;
    t->has.sample_loss                     = true;
    t->has.user_defined11                  = true;
    t->has.user_defined10                  = true;
    t->has.user_defined9                   = true;
    t->has.user_defined8                   = true;
    t->calibrated_time                     = true;
    t->valid_data                          = true;
    t->reference_lock                      = true;
    t->agc_or_mgc                          = VRT_AOM_AGC;
    t->detected_signal                     = true;
    t->spectral_inversion                  = true;
    t->over_range                          = true;
    t->sample_loss                         = true;
    t->user_defined11                      = true;
    t->user_defined10                      = true;
    t->user_defined9                       = true;
    t->user_defined8                       = true;
    t->has.associated_context_packet_count = true;
    t->associated_context_packet_count     = 0x7F;
}

/**
 * Initialize Formatted GPS/INS geolocation to an easy to spot, non-default sequence.
 *
 * \param g Formatted GPS/INS geolocation.
 */
static void init_garbage_formatted_geolocation(vrt_formatted_geolocation* g) {
    g->tsi = VRT_TSI_OTHER;
    g->tsf = VRT_TSF_FREE_RUNNING_COUNT;
    g->oui = 0x00FFFFFF;
    /* Avoid FFFF... here, since that's the actual 'no value' bit sequence */
    g->integer_second_timestamp    = 0xABABABAB;
    g->fractional_second_timestamp = 0xABABABABABABABAB;
    g->has.latitude                = true;
    g->has.longitude               = true;
    g->has.altitude                = true;
    g->has.speed_over_ground       = true;
    g->has.heading_angle           = true;
    g->has.track_angle             = true;
    g->has.magnetic_variation      = true;
    g->latitude                    = 987.654;
    g->longitude                   = 987.654;
    g->altitude                    = 987.654;
    g->speed_over_ground           = 987.654;
    g->heading_angle               = 987.654;
    g->track_angle                 = 987.654;
    g->magnetic_variation          = 987.654;
}

/**
 * Initialize ECEF/Relative ephemeris to an easy to spot, non-default sequence.
 *
 * \param e ECEF/Relative ephemeris.
 */
static void init_garbage_ephemeris(vrt_ephemeris* e) {
    e->tsi                         = VRT_TSI_OTHER;
    e->tsf                         = VRT_TSF_FREE_RUNNING_COUNT;
    e->oui                         = 0x00FFFFFF;
    e->integer_second_timestamp    = 0xABABABAB;
    e->fractional_second_timestamp = 0xABABABABABABABAB;
    /* Avoid FFFF... here, since that's the actual 'no value' bit sequence */
    e->has.position_x     = true;
    e->has.position_y     = true;
    e->has.position_z     = true;
    e->has.attitude_alpha = true;
    e->has.attitude_beta  = true;
    e->has.attitude_phi   = true;
    e->has.velocity_dx    = true;
    e->has.velocity_dy    = true;
    e->has.velocity_dz    = true;
    e->position_x         = 987.654;
    e->position_y         = 987.654;
    e->position_z         = 987.654;
    e->attitude_alpha     = 987.654;
    e->attitude_beta      = 987.654;
    e->attitude_phi       = 987.654;
    e->velocity_dx        = 987.654;
    e->velocity_dy        = 987.654;
    e->velocity_dz        = 987.654;
}

void init_garbage_if_context(struct vrt_if_context* c) {
    c->context_field_change_indicator     = true;
    c->has.reference_point_identifier     = true;
    c->has.bandwidth                      = true;
    c->has.if_reference_frequency         = true;
    c->has.rf_reference_frequency         = true;
    c->has.rf_reference_frequency_offset  = true;
    c->has.if_band_offset                 = true;
    c->has.reference_level                = true;
    c->has.gain                           = true;
    c->has.over_range_count               = true;
    c->has.sample_rate                    = true;
    c->has.timestamp_adjustment           = true;
    c->has.timestamp_calibration_time     = true;
    c->has.temperature                    = true;
    c->has.device_identifier              = true;
    c->has.state_and_event_indicators     = true;
    c->has.data_packet_payload_format     = true;
    c->has.formatted_gps_geolocation      = true;
    c->has.formatted_ins_geolocation      = true;
    c->has.ecef_ephemeris                 = true;
    c->has.relative_ephemeris             = true;
    c->has.ephemeris_reference_identifier = true;
    c->has.gps_ascii                      = true;
    c->has.context_association_lists      = true;

    c->reference_point_identifier    = 0xFFFFFFFF;
    c->bandwidth                     = 987.654;
    c->if_reference_frequency        = 987.654;
    c->rf_reference_frequency        = 987.654;
    c->rf_reference_frequency_offset = 987.654;
    c->if_band_offset                = 987.654;
    c->reference_level               = 987.654F;
    c->gain.stage1                   = 987.654F;
    c->gain.stage2                   = 987.654F;
    c->over_range_count              = 0xFFFFFFFF;
    c->sample_rate                   = 987.654;
    c->timestamp_adjustment          = 0xFFFFFFFFFFFFFFFF;
    c->timestamp_calibration_time    = 0xFFFFFFFF;
    c->temperature                   = 987.654F;

    c->device_identifier.oui         = 0x00FFFFFF;
    c->device_identifier.device_code = 0xFFFF;

    c->state_and_event_indicators.has.calibrated_time    = true;
    c->state_and_event_indicators.has.valid_data         = true;
    c->state_and_event_indicators.has.reference_lock     = true;
    c->state_and_event_indicators.has.agc_or_mgc         = true;
    c->state_and_event_indicators.has.detected_signal    = true;
    c->state_and_event_indicators.has.spectral_inversion = true;
    c->state_and_event_indicators.has.over_range         = true;
    c->state_and_event_indicators.has.sample_loss        = true;
    c->state_and_event_indicators.calibrated_time        = true;
    c->state_and_event_indicators.valid_data             = true;
    c->state_and_event_indicators.reference_lock         = true;
    c->state_and_event_indicators.agc_or_mgc             = VRT_AOM_AGC;
    c->state_and_event_indicators.detected_signal        = true;
    c->state_and_event_indicators.spectral_inversion     = true;
    c->state_and_event_indicators.over_range             = true;
    c->state_and_event_indicators.sample_loss            = true;
    c->state_and_event_indicators.user_defined           = 0xFF;

    c->data_packet_payload_format.packing_method          = VRT_PM_LINK_EFFICIENT;
    c->data_packet_payload_format.real_or_complex         = VRT_ROC_COMPLEX_POLAR;
    c->data_packet_payload_format.data_item_format        = VRT_DIF_UNSIGNED_VRT_6_BIT_EXPONENT;
    c->data_packet_payload_format.sample_component_repeat = true;
    c->data_packet_payload_format.event_tag_size          = 0x07;
    c->data_packet_payload_format.channel_tag_size        = 0x0F;
    c->data_packet_payload_format.item_packing_field_size = 0x3F;
    c->data_packet_payload_format.data_item_size          = 0x3F;
    c->data_packet_payload_format.repeat_count            = 0xFFFF;
    c->data_packet_payload_format.vector_size             = 0xFFFF;

    init_garbage_formatted_geolocation(&c->formatted_gps_geolocation);
    init_garbage_formatted_geolocation(&c->formatted_ins_geolocation);
    init_garbage_ephemeris(&c->ecef_ephemeris);
    init_garbage_ephemeris(&c->relative_ephemeris);

    c->ephemeris_reference_identifier = 0xFFFFFFFF;

    c->gps_ascii.oui             = 0x00FFFFFF;
    c->gps_ascii.number_of_words = 0x0000000F;
    c->gps_ascii.ascii           = "ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ";

    c->context_association_lists.has.asynchronous_channel_tag_list             = true;
    c->context_association_lists.source_list_size                              = 0x01FF;
    c->context_association_lists.system_list_size                              = 0x01FF;
    c->context_association_lists.vector_component_list_size                    = 0xFFFF;
    c->context_association_lists.asynchronous_channel_list_size                = 0xFFFF;
    c->context_association_lists.source_context_association_list               = reinterpret_cast<uint32_t*>(c);
    c->context_association_lists.system_context_association_list               = reinterpret_cast<uint32_t*>(c);
    c->context_association_lists.vector_component_context_association_list     = reinterpret_cast<uint32_t*>(c);
    c->context_association_lists.asynchronous_channel_context_association_list = reinterpret_cast<uint32_t*>(c);
    c->context_association_lists.asynchronous_channel_tag_list                 = reinterpret_cast<uint32_t*>(c);
}

void init_garbage_packet(struct vrt_packet* p) {
    init_garbage_header(&p->header);
    init_garbage_fields(&p->fields);
    p->words_body = 0x0000FFFF;
    p->body       = reinterpret_cast<uint32_t*>(p);
    init_garbage_trailer(&p->trailer);
    init_garbage_if_context(&p->if_context);
}
