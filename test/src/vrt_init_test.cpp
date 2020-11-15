#include <gtest/gtest.h>

#include <vrt/vrt_common.h>

/* This is not nice, but whatever... */
#include <../src/vrt_fixed_point.h>

#include "hex.h"
#include "init_garbage.h"

TEST(InitTest, InitHeader) {
    vrt_header h;
    init_header_garbage(&h);

    vrt_init_header(&h);
    ASSERT_EQ(h.packet_type, 0);
    ASSERT_FALSE(h.has.class_id);
    ASSERT_FALSE(h.has.trailer);
    ASSERT_EQ(h.tsm, VRT_TSM_FINE);
    ASSERT_EQ(h.tsi, VRT_TSI_NONE);
    ASSERT_EQ(h.tsf, VRT_TSF_NONE);
    ASSERT_EQ(Hex(h.packet_count), Hex(0));
    ASSERT_EQ(Hex(h.packet_size), Hex(0));
}

TEST(InitTest, InitFields) {
    vrt_fields f;
    init_fields_garbage(&f);

    vrt_init_fields(&f);
    ASSERT_EQ(Hex(f.class_id.oui), Hex(0));
    ASSERT_EQ(Hex(f.class_id.information_class_code), Hex(0));
    ASSERT_EQ(Hex(f.class_id.packet_class_code), Hex(0));
    ASSERT_EQ(Hex(f.integer_seconds_timestamp), Hex(0));
    ASSERT_EQ(Hex(f.fractional_seconds_timestamp), Hex(0));
}

TEST(InitTest, InitTrailer) {
    vrt_trailer t;
    init_trailer_garbage(&t);

    vrt_init_trailer(&t);
    ASSERT_FALSE(t.has.calibrated_time);
    ASSERT_FALSE(t.has.valid_data);
    ASSERT_FALSE(t.has.reference_lock);
    ASSERT_FALSE(t.has.agc_or_mgc);
    ASSERT_FALSE(t.has.detected_signal);
    ASSERT_FALSE(t.has.spectral_inversion);
    ASSERT_FALSE(t.has.over_range);
    ASSERT_FALSE(t.has.sample_loss);
    ASSERT_FALSE(t.has.user_defined11);
    ASSERT_FALSE(t.has.user_defined10);
    ASSERT_FALSE(t.has.user_defined9);
    ASSERT_FALSE(t.has.user_defined8);
    ASSERT_FALSE(t.calibrated_time);
    ASSERT_FALSE(t.valid_data);
    ASSERT_FALSE(t.reference_lock);
    ASSERT_EQ(t.agc_or_mgc, VRT_AOM_MGC);
    ASSERT_FALSE(t.detected_signal);
    ASSERT_FALSE(t.spectral_inversion);
    ASSERT_FALSE(t.over_range);
    ASSERT_FALSE(t.sample_loss);
    ASSERT_FALSE(t.user_defined11);
    ASSERT_FALSE(t.user_defined10);
    ASSERT_FALSE(t.user_defined9);
    ASSERT_FALSE(t.user_defined8);
    ASSERT_FALSE(t.has.associated_context_packet_count);
    ASSERT_EQ(Hex(t.associated_context_packet_count), Hex(0));
}

TEST(InitTest, InitContext) {
    vrt_if_context c;
    init_if_context_garbage(&c);

    vrt_init_if_context(&c);
    ASSERT_FALSE(c.context_field_change_indicator);
    ASSERT_FALSE(c.has.reference_point_identifier);
    ASSERT_FALSE(c.has.bandwidth);
    ASSERT_FALSE(c.has.if_reference_frequency);
    ASSERT_FALSE(c.has.rf_reference_frequency);
    ASSERT_FALSE(c.has.rf_reference_frequency_offset);
    ASSERT_FALSE(c.has.if_band_offset);
    ASSERT_FALSE(c.has.reference_level);
    ASSERT_FALSE(c.has.gain);
    ASSERT_FALSE(c.has.over_range_count);
    ASSERT_FALSE(c.has.sample_rate);
    ASSERT_FALSE(c.has.timestamp_adjustment);
    ASSERT_FALSE(c.has.timestamp_calibration_time);
    ASSERT_FALSE(c.has.temperature);
    ASSERT_FALSE(c.has.device_identifier);
    ASSERT_FALSE(c.has.state_and_event_indicators);
    ASSERT_FALSE(c.has.data_packet_payload_format);
    ASSERT_FALSE(c.has.formatted_gps_geolocation);
    ASSERT_FALSE(c.has.formatted_ins_geolocation);
    ASSERT_FALSE(c.has.ecef_ephemeris);
    ASSERT_FALSE(c.has.relative_ephemeris);
    ASSERT_FALSE(c.has.ephemeris_reference_identifier);
    ASSERT_FALSE(c.has.gps_ascii);
    ASSERT_FALSE(c.has.context_association_lists);

    ASSERT_EQ(Hex(c.reference_point_identifier), Hex(0));
    ASSERT_DOUBLE_EQ(c.bandwidth, 0.0);
    ASSERT_DOUBLE_EQ(c.if_reference_frequency, 0.0);
    ASSERT_DOUBLE_EQ(c.rf_reference_frequency, 0.0);
    ASSERT_DOUBLE_EQ(c.rf_reference_frequency_offset, 0.0);
    ASSERT_DOUBLE_EQ(c.if_band_offset, 0.0);
    ASSERT_FLOAT_EQ(c.reference_level, 0.0F);
    ASSERT_FLOAT_EQ(c.gain.stage1, 0.0F);
    ASSERT_FLOAT_EQ(c.gain.stage2, 0.0F);
    ASSERT_EQ(Hex(c.over_range_count), Hex(0));
    ASSERT_EQ(c.sample_rate, 0.0);
    ASSERT_EQ(Hex(c.timestamp_adjustment), Hex(0));
    ASSERT_EQ(Hex(c.timestamp_calibration_time), Hex(0));
    ASSERT_FLOAT_EQ(c.temperature, 0.0F);
    ASSERT_EQ(Hex(c.device_identifier.oui), Hex(0));
    ASSERT_EQ(Hex(c.device_identifier.device_code), Hex(0));

    ASSERT_FALSE(c.state_and_event_indicators.has.calibrated_time);
    ASSERT_FALSE(c.state_and_event_indicators.has.valid_data);
    ASSERT_FALSE(c.state_and_event_indicators.has.reference_lock);
    ASSERT_FALSE(c.state_and_event_indicators.has.agc_or_mgc);
    ASSERT_FALSE(c.state_and_event_indicators.has.detected_signal);
    ASSERT_FALSE(c.state_and_event_indicators.has.spectral_inversion);
    ASSERT_FALSE(c.state_and_event_indicators.has.over_range);
    ASSERT_FALSE(c.state_and_event_indicators.has.sample_loss);
    ASSERT_FALSE(c.state_and_event_indicators.calibrated_time);
    ASSERT_FALSE(c.state_and_event_indicators.valid_data);
    ASSERT_FALSE(c.state_and_event_indicators.reference_lock);
    ASSERT_EQ(c.state_and_event_indicators.agc_or_mgc, VRT_AOM_MGC);
    ASSERT_FALSE(c.state_and_event_indicators.detected_signal);
    ASSERT_FALSE(c.state_and_event_indicators.spectral_inversion);
    ASSERT_FALSE(c.state_and_event_indicators.over_range);
    ASSERT_FALSE(c.state_and_event_indicators.sample_loss);
    ASSERT_FALSE(c.state_and_event_indicators.user_defined7);
    ASSERT_FALSE(c.state_and_event_indicators.user_defined6);
    ASSERT_FALSE(c.state_and_event_indicators.user_defined5);
    ASSERT_FALSE(c.state_and_event_indicators.user_defined4);
    ASSERT_FALSE(c.state_and_event_indicators.user_defined3);
    ASSERT_FALSE(c.state_and_event_indicators.user_defined2);
    ASSERT_FALSE(c.state_and_event_indicators.user_defined1);
    ASSERT_FALSE(c.state_and_event_indicators.user_defined0);

    ASSERT_EQ(c.data_packet_payload_format.packing_method, VRT_PM_PROCESSING_EFFICIENT);
    ASSERT_EQ(Hex(c.data_packet_payload_format.real_or_complex), Hex(VRT_ROC_REAL));
    ASSERT_EQ(Hex(c.data_packet_payload_format.data_item_format), Hex(VRT_DIF_SIGNED_FIXED_POINT));
    ASSERT_FALSE(c.data_packet_payload_format.sample_component_repeat);
    ASSERT_EQ(Hex(c.data_packet_payload_format.event_tag_size), Hex(0));
    ASSERT_EQ(Hex(c.data_packet_payload_format.channel_tag_size), Hex(0));
    ASSERT_EQ(Hex(c.data_packet_payload_format.item_packing_field_size), Hex(0));
    ASSERT_EQ(Hex(c.data_packet_payload_format.data_item_size), Hex(0));
    ASSERT_EQ(Hex(c.data_packet_payload_format.repeat_count), Hex(0));
    ASSERT_EQ(Hex(c.data_packet_payload_format.vector_size), Hex(0));

    ASSERT_EQ(Hex(c.formatted_gps_geolocation.tsi), Hex(VRT_TSI_UNDEFINED));
    ASSERT_EQ(Hex(c.formatted_gps_geolocation.tsf), Hex(VRT_TSF_UNDEFINED));
    ASSERT_EQ(Hex(c.formatted_gps_geolocation.oui), Hex(0));
    ASSERT_EQ(Hex(c.formatted_gps_geolocation.integer_second_timestamp), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(c.formatted_gps_geolocation.fractional_second_timestamp), Hex(0xFFFFFFFFFFFFFFFF));
    ASSERT_DOUBLE_EQ(c.formatted_gps_geolocation.latitude, vrt_fixed_point_i32_to_double(0x7FFFFFFF, 22));
    ASSERT_DOUBLE_EQ(c.formatted_gps_geolocation.longitude, vrt_fixed_point_i32_to_double(0x7FFFFFFF, 22));
    ASSERT_DOUBLE_EQ(c.formatted_gps_geolocation.altitude, vrt_fixed_point_i32_to_double(0x7FFFFFFF, 5));
    ASSERT_DOUBLE_EQ(c.formatted_gps_geolocation.speed_over_ground, vrt_fixed_point_u32_to_double(0x7FFFFFFF, 16));
    ASSERT_DOUBLE_EQ(c.formatted_gps_geolocation.heading_angle, vrt_fixed_point_i32_to_double(0x7FFFFFFF, 22));
    ASSERT_DOUBLE_EQ(c.formatted_gps_geolocation.track_angle, vrt_fixed_point_i32_to_double(0x7FFFFFFF, 22));
    ASSERT_DOUBLE_EQ(c.formatted_gps_geolocation.magnetic_variation, vrt_fixed_point_i32_to_double(0x7FFFFFFF, 22));

    ASSERT_EQ(Hex(c.formatted_ins_geolocation.tsi), Hex(VRT_TSI_UNDEFINED));
    ASSERT_EQ(Hex(c.formatted_ins_geolocation.tsf), Hex(VRT_TSF_UNDEFINED));
    ASSERT_EQ(Hex(c.formatted_ins_geolocation.oui), Hex(0));
    ASSERT_EQ(Hex(c.formatted_ins_geolocation.integer_second_timestamp), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(c.formatted_ins_geolocation.fractional_second_timestamp), Hex(0xFFFFFFFFFFFFFFFF));
    ASSERT_DOUBLE_EQ(c.formatted_ins_geolocation.latitude, vrt_fixed_point_i32_to_double(0x7FFFFFFF, 22));
    ASSERT_DOUBLE_EQ(c.formatted_ins_geolocation.longitude, vrt_fixed_point_i32_to_double(0x7FFFFFFF, 22));
    ASSERT_DOUBLE_EQ(c.formatted_ins_geolocation.altitude, vrt_fixed_point_i32_to_double(0x7FFFFFFF, 5));
    ASSERT_DOUBLE_EQ(c.formatted_ins_geolocation.speed_over_ground, vrt_fixed_point_u32_to_double(0x7FFFFFFF, 16));
    ASSERT_DOUBLE_EQ(c.formatted_ins_geolocation.heading_angle, vrt_fixed_point_i32_to_double(0x7FFFFFFF, 22));
    ASSERT_DOUBLE_EQ(c.formatted_ins_geolocation.track_angle, vrt_fixed_point_i32_to_double(0x7FFFFFFF, 22));
    ASSERT_DOUBLE_EQ(c.formatted_ins_geolocation.magnetic_variation, vrt_fixed_point_i32_to_double(0x7FFFFFFF, 22));

    ASSERT_EQ(Hex(c.ecef_ephemeris.tsi), Hex(VRT_TSI_UNDEFINED));
    ASSERT_EQ(Hex(c.ecef_ephemeris.tsf), Hex(VRT_TSF_UNDEFINED));
    ASSERT_EQ(Hex(c.ecef_ephemeris.oui), Hex(0));
    ASSERT_EQ(Hex(c.ecef_ephemeris.integer_second_timestamp), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(c.ecef_ephemeris.fractional_second_timestamp), Hex(0xFFFFFFFFFFFFFFFF));
    ASSERT_DOUBLE_EQ(c.ecef_ephemeris.position_x, vrt_fixed_point_i32_to_double(0x7FFFFFFF, 5));
    ASSERT_DOUBLE_EQ(c.ecef_ephemeris.position_y, vrt_fixed_point_i32_to_double(0x7FFFFFFF, 5));
    ASSERT_DOUBLE_EQ(c.ecef_ephemeris.position_z, vrt_fixed_point_i32_to_double(0x7FFFFFFF, 5));
    ASSERT_DOUBLE_EQ(c.ecef_ephemeris.attitude_alpha, vrt_fixed_point_i32_to_double(0x7FFFFFFF, 22));
    ASSERT_DOUBLE_EQ(c.ecef_ephemeris.attitude_beta, vrt_fixed_point_i32_to_double(0x7FFFFFFF, 22));
    ASSERT_DOUBLE_EQ(c.ecef_ephemeris.attitude_phi, vrt_fixed_point_i32_to_double(0x7FFFFFFF, 22));
    ASSERT_DOUBLE_EQ(c.ecef_ephemeris.velocity_dx, vrt_fixed_point_i32_to_double(0x7FFFFFFF, 16));
    ASSERT_DOUBLE_EQ(c.ecef_ephemeris.velocity_dy, vrt_fixed_point_i32_to_double(0x7FFFFFFF, 16));
    ASSERT_DOUBLE_EQ(c.ecef_ephemeris.velocity_dz, vrt_fixed_point_i32_to_double(0x7FFFFFFF, 16));

    ASSERT_EQ(Hex(c.relative_ephemeris.tsi), Hex(VRT_TSI_UNDEFINED));
    ASSERT_EQ(Hex(c.relative_ephemeris.tsf), Hex(VRT_TSF_UNDEFINED));
    ASSERT_EQ(Hex(c.relative_ephemeris.oui), Hex(0));
    ASSERT_EQ(Hex(c.relative_ephemeris.integer_second_timestamp), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(c.relative_ephemeris.fractional_second_timestamp), Hex(0xFFFFFFFFFFFFFFFF));
    ASSERT_DOUBLE_EQ(c.relative_ephemeris.position_x, vrt_fixed_point_i32_to_double(0x7FFFFFFF, 5));
    ASSERT_DOUBLE_EQ(c.relative_ephemeris.position_y, vrt_fixed_point_i32_to_double(0x7FFFFFFF, 5));
    ASSERT_DOUBLE_EQ(c.relative_ephemeris.position_z, vrt_fixed_point_i32_to_double(0x7FFFFFFF, 5));
    ASSERT_DOUBLE_EQ(c.relative_ephemeris.attitude_alpha, vrt_fixed_point_i32_to_double(0x7FFFFFFF, 22));
    ASSERT_DOUBLE_EQ(c.relative_ephemeris.attitude_beta, vrt_fixed_point_i32_to_double(0x7FFFFFFF, 22));
    ASSERT_DOUBLE_EQ(c.relative_ephemeris.attitude_phi, vrt_fixed_point_i32_to_double(0x7FFFFFFF, 22));
    ASSERT_DOUBLE_EQ(c.relative_ephemeris.velocity_dx, vrt_fixed_point_i32_to_double(0x7FFFFFFF, 16));
    ASSERT_DOUBLE_EQ(c.relative_ephemeris.velocity_dy, vrt_fixed_point_i32_to_double(0x7FFFFFFF, 16));
    ASSERT_DOUBLE_EQ(c.relative_ephemeris.velocity_dz, vrt_fixed_point_i32_to_double(0x7FFFFFFF, 16));

    ASSERT_EQ(Hex(c.ephemeris_reference_identifier), Hex(0));

    ASSERT_EQ(Hex(c.gps_ascii.oui), Hex(0));
    ASSERT_EQ(Hex(c.gps_ascii.number_of_words), Hex(0));
    ASSERT_EQ(c.gps_ascii.ascii, nullptr);

    ASSERT_EQ(Hex(c.context_association_lists.source_list_size), Hex(0));
    ASSERT_EQ(Hex(c.context_association_lists.system_list_size), Hex(0));
    ASSERT_EQ(Hex(c.context_association_lists.vector_component_list_size), Hex(0));
    ASSERT_FALSE(c.context_association_lists.has.asynchronous_channel_tag_list);
    ASSERT_EQ(Hex(c.context_association_lists.asynchronous_channel_list_size), Hex(0));
    ASSERT_EQ(c.context_association_lists.source_context_association_list, nullptr);
    ASSERT_EQ(c.context_association_lists.system_context_association_list, nullptr);
    ASSERT_EQ(c.context_association_lists.vector_component_context_association_list, nullptr);
    ASSERT_EQ(c.context_association_lists.asynchronous_channel_context_association_list, nullptr);
    ASSERT_EQ(c.context_association_lists.asynchronous_channel_tag_list, nullptr);
}
