#include <gtest/gtest.h>

#include <any>
#include <array>
#include <cstdint>
#include <cstring>
#include <map>
#include <string>

#include <vrt/vrt_common.h>
#include <vrt/vrt_read.h>

/* This is not nice, but whatever... */
#include <../src/vrt_fixed_point.h>

#include "hex.h"
#include "init_garbage.h"
#include "read_util.h"

class ReadIfContextTest : public ::testing::Test {
   protected:
    ReadIfContextTest() : c_(), buf_() {}

    void SetUp() override {
        init_if_context_garbage(&c_);
        buf_.fill(0xBAADF00D);
    }

    vrt_if_context            c_;
    std::array<uint32_t, 128> buf_;
};

/**
 * Assert IF context values.
 *
 * \param c      IF context.
 * \param values Lists non-default values, i.e. values that differ from the vrt_init_if_context() state, as field name
 *               -> field value.
 */
static void assert_if_context(const vrt_if_context& c, const std::map<std::string, std::any>& values) {
    std::map<std::string, std::any> val_cp(values);
    ASSERT_EQ(c.context_field_change_indicator, get_val<bool>(&val_cp, "context_field_change_indicator", false));
    ASSERT_EQ(c.has.reference_point_identifier, get_val<bool>(&val_cp, "has.reference_point_identifier", false));
    ASSERT_EQ(c.has.bandwidth, get_val<bool>(&val_cp, "has.bandwidth", false));
    ASSERT_EQ(c.has.if_reference_frequency, get_val<bool>(&val_cp, "has.if_reference_frequency", false));
    ASSERT_EQ(c.has.rf_reference_frequency, get_val<bool>(&val_cp, "has.rf_reference_frequency", false));
    ASSERT_EQ(c.has.rf_reference_frequency_offset, get_val<bool>(&val_cp, "has.rf_reference_frequency_offset", false));
    ASSERT_EQ(c.has.if_band_offset, get_val<bool>(&val_cp, "has.if_band_offset", false));
    ASSERT_EQ(c.has.reference_level, get_val<bool>(&val_cp, "has.reference_level", false));
    ASSERT_EQ(c.has.gain, get_val<bool>(&val_cp, "has.gain", false));
    ASSERT_EQ(c.has.over_range_count, get_val<bool>(&val_cp, "has.over_range_count", false));
    ASSERT_EQ(c.has.sample_rate, get_val<bool>(&val_cp, "has.sample_rate", false));
    ASSERT_EQ(c.has.timestamp_adjustment, get_val<bool>(&val_cp, "has.timestamp_adjustment", false));
    ASSERT_EQ(c.has.timestamp_calibration_time, get_val<bool>(&val_cp, "has.timestamp_calibration_time", false));
    ASSERT_EQ(c.has.temperature, get_val<bool>(&val_cp, "has.temperature", false));
    ASSERT_EQ(c.has.device_identifier, get_val<bool>(&val_cp, "has.device_identifier", false));
    ASSERT_EQ(c.has.state_and_event_indicators, get_val<bool>(&val_cp, "has.state_and_event_indicators", false));
    ASSERT_EQ(c.has.data_packet_payload_format, get_val<bool>(&val_cp, "has.data_packet_payload_format", false));
    ASSERT_EQ(c.has.formatted_gps_geolocation, get_val<bool>(&val_cp, "has.formatted_gps_geolocation", false));
    ASSERT_EQ(c.has.formatted_ins_geolocation, get_val<bool>(&val_cp, "has.formatted_ins_geolocation", false));
    ASSERT_EQ(c.has.ecef_ephemeris, get_val<bool>(&val_cp, "has.ecef_ephemeris", false));
    ASSERT_EQ(c.has.relative_ephemeris, get_val<bool>(&val_cp, "has.relative_ephemeris", false));
    ASSERT_EQ(c.has.ephemeris_reference_identifier,
              get_val<bool>(&val_cp, "has.ephemeris_reference_identifier", false));
    ASSERT_EQ(c.has.gps_ascii, get_val<bool>(&val_cp, "has.gps_ascii", false));
    ASSERT_EQ(c.has.context_association_lists, get_val<bool>(&val_cp, "has.context_association_lists", false));

    ASSERT_EQ(Hex(c.reference_point_identifier), Hex(get_val<uint32_t>(&val_cp, "reference_point_identifier", 0)));
    ASSERT_DOUBLE_EQ(c.bandwidth, get_val<double>(&val_cp, "bandwidth", 0.0));
    ASSERT_DOUBLE_EQ(c.if_reference_frequency, get_val<double>(&val_cp, "if_reference_frequency", 0.0));
    ASSERT_DOUBLE_EQ(c.rf_reference_frequency, get_val<double>(&val_cp, "rf_reference_frequency", 0.0));
    ASSERT_DOUBLE_EQ(c.rf_reference_frequency_offset, get_val<double>(&val_cp, "rf_reference_frequency_offset", 0.0));
    ASSERT_DOUBLE_EQ(c.if_band_offset, get_val<double>(&val_cp, "if_band_offset", 0.0));
    ASSERT_FLOAT_EQ(c.reference_level, get_val<float>(&val_cp, "reference_level", 0.0F));
    ASSERT_FLOAT_EQ(c.gain.stage1, get_val<float>(&val_cp, "gain.stage1", 0.0F));
    ASSERT_FLOAT_EQ(c.gain.stage2, get_val<float>(&val_cp, "gain.stage2", 0.0F));
    ASSERT_EQ(Hex(c.over_range_count), Hex(get_val<uint32_t>(&val_cp, "over_range_count", 0)));
    ASSERT_DOUBLE_EQ(c.sample_rate, get_val<double>(&val_cp, "sample_rate", 0.0));
    ASSERT_EQ(Hex(c.timestamp_adjustment), Hex(get_val<uint64_t>(&val_cp, "timestamp_adjustment", 0)));
    ASSERT_EQ(Hex(c.timestamp_calibration_time), Hex(get_val<uint32_t>(&val_cp, "timestamp_calibration_time", 0)));
    ASSERT_FLOAT_EQ(c.temperature, get_val<float>(&val_cp, "temperature", 0.0F));
    ASSERT_EQ(Hex(c.device_identifier.oui), Hex(get_val<uint32_t>(&val_cp, "device_identifier.oui", 0)));
    ASSERT_EQ(Hex(c.device_identifier.device_code),
              Hex(get_val<uint16_t>(&val_cp, "device_identifier.device_code", 0)));

    ASSERT_EQ(c.state_and_event_indicators.has.calibrated_time,
              get_val<bool>(&val_cp, "state_and_event_indicators.has.calibrated_time", false));
    ASSERT_EQ(c.state_and_event_indicators.has.valid_data,
              get_val<bool>(&val_cp, "state_and_event_indicators.has.valid_data", false));
    ASSERT_EQ(c.state_and_event_indicators.has.reference_lock,
              get_val<bool>(&val_cp, "state_and_event_indicators.has.reference_lock", false));
    ASSERT_EQ(c.state_and_event_indicators.has.agc_or_mgc,
              get_val<bool>(&val_cp, "state_and_event_indicators.has.agc_or_mgc", false));
    ASSERT_EQ(c.state_and_event_indicators.has.detected_signal,
              get_val<bool>(&val_cp, "state_and_event_indicators.has.detected_signal", false));
    ASSERT_EQ(c.state_and_event_indicators.has.spectral_inversion,
              get_val<bool>(&val_cp, "state_and_event_indicators.has.spectral_inversion", false));
    ASSERT_EQ(c.state_and_event_indicators.has.over_range,
              get_val<bool>(&val_cp, "state_and_event_indicators.has.over_range", false));
    ASSERT_EQ(c.state_and_event_indicators.has.sample_loss,
              get_val<bool>(&val_cp, "state_and_event_indicators.has.sample_loss", false));
    ASSERT_EQ(c.state_and_event_indicators.calibrated_time,
              get_val<bool>(&val_cp, "state_and_event_indicators.calibrated_time", false));
    ASSERT_EQ(c.state_and_event_indicators.valid_data,
              get_val<bool>(&val_cp, "state_and_event_indicators.valid_data", false));
    ASSERT_EQ(c.state_and_event_indicators.reference_lock,
              get_val<bool>(&val_cp, "state_and_event_indicators.reference_lock", false));
    ASSERT_EQ(c.state_and_event_indicators.agc_or_mgc,
              get_val<bool>(&val_cp, "state_and_event_indicators.agc_or_mgc", false));
    ASSERT_EQ(c.state_and_event_indicators.detected_signal,
              get_val<bool>(&val_cp, "state_and_event_indicators.detected_signal", false));
    ASSERT_EQ(c.state_and_event_indicators.spectral_inversion,
              get_val<bool>(&val_cp, "state_and_event_indicators.spectral_inversion", false));
    ASSERT_EQ(c.state_and_event_indicators.over_range,
              get_val<bool>(&val_cp, "state_and_event_indicators.over_range", false));
    ASSERT_EQ(c.state_and_event_indicators.sample_loss,
              get_val<bool>(&val_cp, "state_and_event_indicators.sample_loss", false));
    ASSERT_EQ(c.state_and_event_indicators.user_defined7,
              get_val<bool>(&val_cp, "state_and_event_indicators.user_defined7", false));
    ASSERT_EQ(c.state_and_event_indicators.user_defined6,
              get_val<bool>(&val_cp, "state_and_event_indicators.user_defined6", false));
    ASSERT_EQ(c.state_and_event_indicators.user_defined5,
              get_val<bool>(&val_cp, "state_and_event_indicators.user_defined5", false));
    ASSERT_EQ(c.state_and_event_indicators.user_defined4,
              get_val<bool>(&val_cp, "state_and_event_indicators.user_defined4", false));
    ASSERT_EQ(c.state_and_event_indicators.user_defined3,
              get_val<bool>(&val_cp, "state_and_event_indicators.user_defined3", false));
    ASSERT_EQ(c.state_and_event_indicators.user_defined2,
              get_val<bool>(&val_cp, "state_and_event_indicators.user_defined2", false));
    ASSERT_EQ(c.state_and_event_indicators.user_defined1,
              get_val<bool>(&val_cp, "state_and_event_indicators.user_defined1", false));
    ASSERT_EQ(c.state_and_event_indicators.user_defined0,
              get_val<bool>(&val_cp, "state_and_event_indicators.user_defined0", false));

    ASSERT_EQ(c.data_packet_payload_format.packing_method,
              get_val<bool>(&val_cp, "data_packet_payload_format.packing_method", false));
    ASSERT_EQ(Hex(c.data_packet_payload_format.real_or_complex),
              Hex(get_val<vrt_real_complex>(&val_cp, "data_packet_payload_format.real_or_complex", VRT_ROC_REAL)));
    ASSERT_EQ(Hex(c.data_packet_payload_format.data_item_format),
              Hex(get_val<vrt_data_item_format>(&val_cp, "data_packet_payload_format.data_item_format",
                                                VRT_DIF_SIGNED_FIXED_POINT)));
    ASSERT_EQ(c.data_packet_payload_format.sample_component_repeat,
              get_val<bool>(&val_cp, "data_packet_payload_format.sample_component_repeat", false));
    ASSERT_EQ(Hex(c.data_packet_payload_format.event_tag_size),
              Hex(get_val<uint8_t>(&val_cp, "data_packet_payload_format.event_tag_size", 0)));
    ASSERT_EQ(Hex(c.data_packet_payload_format.channel_tag_size),
              Hex(get_val<uint8_t>(&val_cp, "data_packet_payload_format.channel_tag_size", 0)));
    ASSERT_EQ(Hex(c.data_packet_payload_format.item_packing_field_size),
              Hex(get_val<uint8_t>(&val_cp, "data_packet_payload_format.item_packing_field_size", 0)));
    ASSERT_EQ(Hex(c.data_packet_payload_format.data_item_size),
              Hex(get_val<uint8_t>(&val_cp, "data_packet_payload_format.data_item_size", 0)));
    ASSERT_EQ(Hex(c.data_packet_payload_format.repeat_count),
              Hex(get_val<uint16_t>(&val_cp, "data_packet_payload_format.repeat_count", 0)));
    ASSERT_EQ(Hex(c.data_packet_payload_format.vector_size),
              Hex(get_val<uint16_t>(&val_cp, "data_packet_payload_format.vector_size", 0)));

    ASSERT_EQ(Hex(c.formatted_gps_geolocation.tsi),
              Hex(get_val<vrt_tsi>(&val_cp, "formatted_gps_geolocation.tsi", VRT_TSI_UNDEFINED)));
    ASSERT_EQ(Hex(c.formatted_gps_geolocation.tsf),
              Hex(get_val<vrt_tsf>(&val_cp, "formatted_gps_geolocation.tsf", VRT_TSF_UNDEFINED)));
    ASSERT_EQ(Hex(c.formatted_gps_geolocation.oui),
              Hex(get_val<uint32_t>(&val_cp, "formatted_gps_geolocation.oui", 0)));
    ASSERT_EQ(Hex(c.formatted_gps_geolocation.integer_second_timestamp),
              Hex(get_val<uint32_t>(&val_cp, "formatted_gps_geolocation.integer_second_timestamp", 0xFFFFFFFF)));
    ASSERT_EQ(
        Hex(c.formatted_gps_geolocation.fractional_second_timestamp),
        Hex(get_val<uint64_t>(&val_cp, "formatted_gps_geolocation.fractional_second_timestamp", 0xFFFFFFFFFFFFFFFF)));
    ASSERT_DOUBLE_EQ(
        c.formatted_gps_geolocation.latitude,
        get_val<double>(&val_cp, "formatted_gps_geolocation.latitude", vrt_fixed_point_i32_to_double(0x7FFFFFFF, 22)));
    ASSERT_EQ(c.formatted_gps_geolocation.longitude, get_val<double>(&val_cp, "formatted_gps_geolocation.longitude",
                                                                     vrt_fixed_point_i32_to_double(0x7FFFFFFF, 22)));
    ASSERT_DOUBLE_EQ(
        c.formatted_gps_geolocation.altitude,
        get_val<double>(&val_cp, "formatted_gps_geolocation.altitude", vrt_fixed_point_i32_to_double(0x7FFFFFFF, 5)));
    ASSERT_DOUBLE_EQ(c.formatted_gps_geolocation.speed_over_ground,
                     get_val<double>(&val_cp, "formatted_gps_geolocation.speed_over_ground",
                                     vrt_fixed_point_u32_to_double(0x7FFFFFFF, 16)));
    ASSERT_DOUBLE_EQ(c.formatted_gps_geolocation.heading_angle,
                     get_val<double>(&val_cp, "formatted_gps_geolocation.heading_angle",
                                     vrt_fixed_point_i32_to_double(0x7FFFFFFF, 22)));
    ASSERT_DOUBLE_EQ(c.formatted_gps_geolocation.track_angle,
                     get_val<double>(&val_cp, "formatted_gps_geolocation.track_angle",
                                     vrt_fixed_point_i32_to_double(0x7FFFFFFF, 22)));
    ASSERT_DOUBLE_EQ(c.formatted_gps_geolocation.magnetic_variation,
                     get_val<double>(&val_cp, "formatted_gps_geolocation.magnetic_variation",
                                     vrt_fixed_point_i32_to_double(0x7FFFFFFF, 22)));

    ASSERT_EQ(Hex(c.formatted_ins_geolocation.tsi),
              Hex(get_val<vrt_tsi>(&val_cp, "formatted_ins_geolocation.tsi", VRT_TSI_UNDEFINED)));
    ASSERT_EQ(Hex(c.formatted_ins_geolocation.tsf),
              Hex(get_val<vrt_tsf>(&val_cp, "formatted_ins_geolocation.tsf", VRT_TSF_UNDEFINED)));
    ASSERT_EQ(Hex(c.formatted_ins_geolocation.oui),
              Hex(get_val<uint32_t>(&val_cp, "formatted_ins_geolocation.oui", 0)));
    ASSERT_EQ(Hex(c.formatted_ins_geolocation.integer_second_timestamp),
              Hex(get_val<uint32_t>(&val_cp, "formatted_ins_geolocation.integer_second_timestamp", 0xFFFFFFFF)));
    ASSERT_EQ(
        Hex(c.formatted_ins_geolocation.fractional_second_timestamp),
        Hex(get_val<uint64_t>(&val_cp, "formatted_ins_geolocation.fractional_second_timestamp", 0xFFFFFFFFFFFFFFFF)));
    ASSERT_DOUBLE_EQ(
        c.formatted_ins_geolocation.latitude,
        get_val<double>(&val_cp, "formatted_ins_geolocation.latitude", vrt_fixed_point_i32_to_double(0x7FFFFFFF, 22)));
    ASSERT_DOUBLE_EQ(
        c.formatted_ins_geolocation.longitude,
        get_val<double>(&val_cp, "formatted_ins_geolocation.longitude", vrt_fixed_point_i32_to_double(0x7FFFFFFF, 22)));
    ASSERT_DOUBLE_EQ(
        c.formatted_ins_geolocation.altitude,
        get_val<double>(&val_cp, "formatted_ins_geolocation.altitude", vrt_fixed_point_i32_to_double(0x7FFFFFFF, 5)));
    ASSERT_DOUBLE_EQ(c.formatted_ins_geolocation.speed_over_ground,
                     get_val<double>(&val_cp, "formatted_ins_geolocation.speed_over_ground",
                                     vrt_fixed_point_u32_to_double(0x7FFFFFFF, 16)));
    ASSERT_DOUBLE_EQ(c.formatted_ins_geolocation.heading_angle,
                     get_val<double>(&val_cp, "formatted_ins_geolocation.heading_angle",
                                     vrt_fixed_point_i32_to_double(0x7FFFFFFF, 22)));
    ASSERT_DOUBLE_EQ(c.formatted_ins_geolocation.track_angle,
                     get_val<double>(&val_cp, "formatted_ins_geolocation.track_angle",
                                     vrt_fixed_point_i32_to_double(0x7FFFFFFF, 22)));
    ASSERT_DOUBLE_EQ(c.formatted_ins_geolocation.magnetic_variation,
                     get_val<double>(&val_cp, "formatted_ins_geolocation.magnetic_variation",
                                     vrt_fixed_point_i32_to_double(0x7FFFFFFF, 22)));

    ASSERT_EQ(Hex(c.ecef_ephemeris.tsi), Hex(get_val<vrt_tsi>(&val_cp, "ecef_ephemeris.tsi", VRT_TSI_UNDEFINED)));
    ASSERT_EQ(Hex(c.ecef_ephemeris.tsf), Hex(get_val<vrt_tsf>(&val_cp, "ecef_ephemeris.tsf", VRT_TSF_UNDEFINED)));
    ASSERT_EQ(Hex(c.ecef_ephemeris.oui), Hex(get_val<uint32_t>(&val_cp, "ecef_ephemeris.oui", 0)));
    ASSERT_EQ(Hex(c.ecef_ephemeris.integer_second_timestamp),
              Hex(get_val<uint32_t>(&val_cp, "ecef_ephemeris.integer_second_timestamp", 0xFFFFFFFF)));
    ASSERT_EQ(Hex(c.ecef_ephemeris.fractional_second_timestamp),
              Hex(get_val<uint64_t>(&val_cp, "ecef_ephemeris.fractional_second_timestamp", 0xFFFFFFFFFFFFFFFF)));
    ASSERT_DOUBLE_EQ(c.ecef_ephemeris.position_x, get_val<double>(&val_cp, "ecef_ephemeris.position_x",
                                                                  vrt_fixed_point_i32_to_double(0x7FFFFFFF, 5)));
    ASSERT_DOUBLE_EQ(c.ecef_ephemeris.position_y, get_val<double>(&val_cp, "ecef_ephemeris.position_y",
                                                                  vrt_fixed_point_i32_to_double(0x7FFFFFFF, 5)));
    ASSERT_DOUBLE_EQ(c.ecef_ephemeris.position_z, get_val<double>(&val_cp, "ecef_ephemeris.position_z",
                                                                  vrt_fixed_point_i32_to_double(0x7FFFFFFF, 5)));
    ASSERT_DOUBLE_EQ(c.ecef_ephemeris.attitude_alpha, get_val<double>(&val_cp, "ecef_ephemeris.attitude_alpha",
                                                                      vrt_fixed_point_i32_to_double(0x7FFFFFFF, 22)));
    ASSERT_DOUBLE_EQ(c.ecef_ephemeris.attitude_beta, get_val<double>(&val_cp, "ecef_ephemeris.attitude_beta",
                                                                     vrt_fixed_point_i32_to_double(0x7FFFFFFF, 22)));
    ASSERT_DOUBLE_EQ(c.ecef_ephemeris.attitude_phi, get_val<double>(&val_cp, "ecef_ephemeris.attitude_phi",
                                                                    vrt_fixed_point_i32_to_double(0x7FFFFFFF, 22)));
    ASSERT_DOUBLE_EQ(c.ecef_ephemeris.velocity_dx, get_val<double>(&val_cp, "ecef_ephemeris.velocity_dx",
                                                                   vrt_fixed_point_i32_to_double(0x7FFFFFFF, 16)));
    ASSERT_DOUBLE_EQ(c.ecef_ephemeris.velocity_dy, get_val<double>(&val_cp, "ecef_ephemeris.velocity_dy",
                                                                   vrt_fixed_point_i32_to_double(0x7FFFFFFF, 16)));
    ASSERT_DOUBLE_EQ(c.ecef_ephemeris.velocity_dz, get_val<double>(&val_cp, "ecef_ephemeris.velocity_dz",
                                                                   vrt_fixed_point_i32_to_double(0x7FFFFFFF, 16)));

    ASSERT_EQ(Hex(c.relative_ephemeris.tsi),
              Hex(get_val<vrt_tsi>(&val_cp, "relative_ephemeris.tsi", VRT_TSI_UNDEFINED)));
    ASSERT_EQ(Hex(c.relative_ephemeris.tsf),
              Hex(get_val<vrt_tsf>(&val_cp, "relative_ephemeris.tsf", VRT_TSF_UNDEFINED)));
    ASSERT_EQ(Hex(c.relative_ephemeris.oui), Hex(get_val<uint32_t>(&val_cp, "relative_ephemeris.oui", 0)));
    ASSERT_EQ(Hex(c.relative_ephemeris.integer_second_timestamp),
              Hex(get_val<uint32_t>(&val_cp, "relative_ephemeris.integer_second_timestamp", 0xFFFFFFFF)));
    ASSERT_EQ(Hex(c.relative_ephemeris.fractional_second_timestamp),
              Hex(get_val<uint64_t>(&val_cp, "relative_ephemeris.fractional_second_timestamp", 0xFFFFFFFFFFFFFFFF)));
    ASSERT_DOUBLE_EQ(c.relative_ephemeris.position_x, get_val<double>(&val_cp, "relative_ephemeris.position_x",
                                                                      vrt_fixed_point_i32_to_double(0x7FFFFFFF, 5)));
    ASSERT_DOUBLE_EQ(c.relative_ephemeris.position_y, get_val<double>(&val_cp, "relative_ephemeris.position_y",
                                                                      vrt_fixed_point_i32_to_double(0x7FFFFFFF, 5)));
    ASSERT_DOUBLE_EQ(c.relative_ephemeris.position_z, get_val<double>(&val_cp, "relative_ephemeris.position_z",
                                                                      vrt_fixed_point_i32_to_double(0x7FFFFFFF, 5)));
    ASSERT_DOUBLE_EQ(
        c.relative_ephemeris.attitude_alpha,
        get_val<double>(&val_cp, "relative_ephemeris.attitude_alpha", vrt_fixed_point_i32_to_double(0x7FFFFFFF, 22)));
    ASSERT_DOUBLE_EQ(
        c.relative_ephemeris.attitude_beta,
        get_val<double>(&val_cp, "relative_ephemeris.attitude_beta", vrt_fixed_point_i32_to_double(0x7FFFFFFF, 22)));
    ASSERT_DOUBLE_EQ(c.relative_ephemeris.attitude_phi, get_val<double>(&val_cp, "relative_ephemeris.attitude_phi",
                                                                        vrt_fixed_point_i32_to_double(0x7FFFFFFF, 22)));
    ASSERT_DOUBLE_EQ(c.relative_ephemeris.velocity_dx, get_val<double>(&val_cp, "relative_ephemeris.velocity_dx",
                                                                       vrt_fixed_point_i32_to_double(0x7FFFFFFF, 16)));
    ASSERT_DOUBLE_EQ(c.relative_ephemeris.velocity_dy, get_val<double>(&val_cp, "relative_ephemeris.velocity_dy",
                                                                       vrt_fixed_point_i32_to_double(0x7FFFFFFF, 16)));
    ASSERT_DOUBLE_EQ(c.relative_ephemeris.velocity_dz, get_val<double>(&val_cp, "relative_ephemeris.velocity_dz",
                                                                       vrt_fixed_point_i32_to_double(0x7FFFFFFF, 16)));

    ASSERT_EQ(Hex(c.ephemeris_reference_identifier),
              Hex(get_val<uint32_t>(&val_cp, "ephemeris_reference_identifier", 0)));

    ASSERT_EQ(Hex(c.gps_ascii.oui), Hex(get_val<uint32_t>(&val_cp, "gps_ascii.oui", 0)));
    ASSERT_EQ(Hex(c.gps_ascii.number_of_words), Hex(get_val<uint32_t>(&val_cp, "gps_ascii.number_of_words", 0)));
    ASSERT_EQ(c.gps_ascii.ascii, get_val<const char*>(&val_cp, "gps_ascii.ascii", nullptr));

    ASSERT_EQ(Hex(c.context_association_lists.source_list_size),
              Hex(get_val<uint16_t>(&val_cp, "context_association_lists.source_list_size", 0)));
    ASSERT_EQ(Hex(c.context_association_lists.system_list_size),
              Hex(get_val<uint16_t>(&val_cp, "context_association_lists.system_list_size", 0)));
    ASSERT_EQ(Hex(c.context_association_lists.vector_component_list_size),
              Hex(get_val<uint16_t>(&val_cp, "context_association_lists.vector_component_list_size", 0)));
    ASSERT_EQ(c.context_association_lists.has.asynchronous_channel_tag_list,
              get_val<bool>(&val_cp, "context_association_lists.has.asynchronous_channel_tag_list", false));
    ASSERT_EQ(Hex(c.context_association_lists.asynchronous_channel_list_size),
              Hex(get_val<uint16_t>(&val_cp, "context_association_lists.asynchronous_channel_list_size", 0)));
    ASSERT_EQ(c.context_association_lists.source_context_association_list,
              get_val<const uint32_t*>(&val_cp, "context_association_lists.source_context_association_list", nullptr));
    ASSERT_EQ(c.context_association_lists.system_context_association_list,
              get_val<const uint32_t*>(&val_cp, "context_association_lists.system_context_association_list", nullptr));
    ASSERT_EQ(c.context_association_lists.vector_component_context_association_list,
              get_val<const uint32_t*>(&val_cp, "context_association_lists.vector_component_context_association_list",
                                       nullptr));
    ASSERT_EQ(c.context_association_lists.asynchronous_channel_context_association_list,
              get_val<const uint32_t*>(
                  &val_cp, "context_association_lists.asynchronous_channel_context_association_list", nullptr));
    ASSERT_EQ(c.context_association_lists.asynchronous_channel_tag_list,
              get_val<const uint32_t*>(&val_cp, "context_association_lists.asynchronous_channel_tag_list", nullptr));

    check_remaining(val_cp);
}

TEST_F(ReadIfContextTest, ZeroSizeBuffer) {
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 0, &c_), VRT_ERR_BUF_SIZE);
}

TEST_F(ReadIfContextTest, None) {
    buf_[0] = 0x00000000;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 1, &c_), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {});
}

TEST_F(ReadIfContextTest, ContextFieldChangeIndicator) {
    buf_[0] = 0x80000000;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 1, &c_), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"context_field_change_indicator", true}});
}

TEST_F(ReadIfContextTest, ContextIndicatorsReserved) {
    buf_[0] = 0x000000FF;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 1, &c_), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {});
}

TEST_F(ReadIfContextTest, ReferencePointIdentifier) {
    buf_[0] = 0x40000000;
    buf_[1] = 0xFEDCBA98;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 2, &c_), 2);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.reference_point_identifier", true}, {"reference_point_identifier", 0xFEDCBA98}});
}

TEST_F(ReadIfContextTest, Bandwidth) {
    buf_[0] = 0x20000000;
    buf_[1] = 0x00000001;
    buf_[2] = 0x00100000;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 3, &c_), 3);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.bandwidth", true}, {"bandwidth", 4097.0}});
}

TEST_F(ReadIfContextTest, IfReferenceFrequency) {
    buf_[0] = 0x10000000;
    buf_[1] = 0x00000001;
    buf_[2] = 0x00100000;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 3, &c_), 3);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.if_reference_frequency", true}, {"if_reference_frequency", 4097.0}});
}

TEST_F(ReadIfContextTest, RfReferenceFrequency) {
    buf_[0] = 0x08000000;
    buf_[1] = 0x00000001;
    buf_[2] = 0x00100000;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 3, &c_), 3);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.rf_reference_frequency", true}, {"rf_reference_frequency", 4097.0}});
}

TEST_F(ReadIfContextTest, RfReferenceFrequencyOffset) {
    buf_[0] = 0x04000000;
    buf_[1] = 0x00000001;
    buf_[2] = 0x00100000;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 3, &c_), 3);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.rf_reference_frequency_offset", true}, {"rf_reference_frequency_offset", 4097.0}});
}

TEST_F(ReadIfContextTest, IfBandOffset) {
    buf_[0] = 0x02000000;
    buf_[1] = 0x00000001;
    buf_[2] = 0x00100000;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 3, &c_), 3);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.if_band_offset", true}, {"if_band_offset", 4097.0}});
}

TEST_F(ReadIfContextTest, ReferenceLevel) {
    buf_[0] = 0x01000000;
    buf_[1] = 0x0000FF80;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 2, &c_), 2);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.reference_level", true}, {"reference_level", -1.0F}});
}

TEST_F(ReadIfContextTest, Gain) {
    buf_[0] = 0x00800000;
    buf_[1] = 0xFF80FF80;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 2, &c_), 2);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.gain", true}, {"gain.stage2", -1.0F}, {"gain.stage1", -1.0F}});
}

TEST_F(ReadIfContextTest, OverRangeCount) {
    buf_[0] = 0x00400000;
    buf_[1] = 0xFFEEDDCC;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 2, &c_), 2);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.over_range_count", true}, {"over_range_count", static_cast<uint32_t>(0xFFEEDDCC)}});
}

TEST_F(ReadIfContextTest, SampleRate) {
    buf_[0] = 0x00200000;
    buf_[1] = 0x00000001;
    buf_[2] = 0x00100000;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 3, &c_), 3);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.sample_rate", true}, {"sample_rate", 4097.0}});
}

TEST_F(ReadIfContextTest, TimestampAdjustment) {
    buf_[0] = 0x00100000;
    buf_[1] = 0xABCDEFAB;
    buf_[2] = 0xBABBABBA;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 3, &c_), 3);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(
        c_, {{"has.timestamp_adjustment", true}, {"timestamp_adjustment", static_cast<uint64_t>(0xABCDEFABBABBABBA)}});
}

TEST_F(ReadIfContextTest, TimestampCalibrationTime) {
    buf_[0] = 0x00080000;
    buf_[1] = 0xABCDEFAB;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 2, &c_), 2);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.timestamp_calibration_time", true},
                           {"timestamp_calibration_time", static_cast<uint32_t>(0xABCDEFAB)}});
}

TEST_F(ReadIfContextTest, Temperature) {
    buf_[0] = 0x00040000;
    buf_[1] = 0x0000FFC0;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 2, &c_), 2);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.temperature", true}, {"temperature", -1.0F}});
}

TEST_F(ReadIfContextTest, DeviceIdentifier) {
    buf_[0] = 0x00020000;
    buf_[1] = 0xFFABCDEF;
    buf_[2] = 0xFFFFF045;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 3, &c_), 3);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.device_identifier", true},
                           {"device_identifier.oui", static_cast<uint32_t>(0x00ABCDEF)},
                           {"device_identifier.device_code", static_cast<uint16_t>(0xF045)}});
}

TEST_F(ReadIfContextTest, StateAndEventIndicatorsHasCalibratedTime) {
    buf_[0] = 0x00010000;
    buf_[1] = 0x80000000;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 2, &c_), 2);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(
        c_, {{"has.state_and_event_indicators", true}, {"state_and_event_indicators.has.calibrated_time", true}});
}

TEST_F(ReadIfContextTest, StateAndEventIndicatorsHasValidData) {
    buf_[0] = 0x00010000;
    buf_[1] = 0x40000000;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 2, &c_), 2);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_,
                      {{"has.state_and_event_indicators", true}, {"state_and_event_indicators.has.valid_data", true}});
}

TEST_F(ReadIfContextTest, StateAndEventIndicatorsHasReferenceLock) {
    buf_[0] = 0x00010000;
    buf_[1] = 0x20000000;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 2, &c_), 2);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(
        c_, {{"has.state_and_event_indicators", true}, {"state_and_event_indicators.has.reference_lock", true}});
}

TEST_F(ReadIfContextTest, StateAndEventIndicatorsHasAgcOrMgc) {
    buf_[0] = 0x00010000;
    buf_[1] = 0x10000000;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 2, &c_), 2);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_,
                      {{"has.state_and_event_indicators", true}, {"state_and_event_indicators.has.agc_or_mgc", true}});
}

TEST_F(ReadIfContextTest, StateAndEventIndicatorsHasDetectedSignal) {
    buf_[0] = 0x00010000;
    buf_[1] = 0x08000000;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 2, &c_), 2);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(
        c_, {{"has.state_and_event_indicators", true}, {"state_and_event_indicators.has.detected_signal", true}});
}

TEST_F(ReadIfContextTest, StateAndEventIndicatorsHasSpectralInversion) {
    buf_[0] = 0x00010000;
    buf_[1] = 0x04000000;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 2, &c_), 2);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(
        c_, {{"has.state_and_event_indicators", true}, {"state_and_event_indicators.has.spectral_inversion", true}});
}

TEST_F(ReadIfContextTest, StateAndEventIndicatorsHasOverRange) {
    buf_[0] = 0x00010000;
    buf_[1] = 0x02000000;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 2, &c_), 2);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_,
                      {{"has.state_and_event_indicators", true}, {"state_and_event_indicators.has.over_range", true}});
}

TEST_F(ReadIfContextTest, StateAndEventIndicatorsHasSampleLoss) {
    buf_[0] = 0x00010000;
    buf_[1] = 0x01000000;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 2, &c_), 2);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_,
                      {{"has.state_and_event_indicators", true}, {"state_and_event_indicators.has.sample_loss", true}});
}

TEST_F(ReadIfContextTest, StateAndEventIndicatorsCalibratedTime) {
    buf_[0] = 0x00010000;
    buf_[1] = 0x00080000;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 2, &c_), 2);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.state_and_event_indicators", true}});
}

TEST_F(ReadIfContextTest, StateAndEventIndicatorsValidData) {
    buf_[0] = 0x00010000;
    buf_[1] = 0x00040000;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 2, &c_), 2);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.state_and_event_indicators", true}});
}

TEST_F(ReadIfContextTest, StateAndEventIndicatorsReferenceLock) {
    buf_[0] = 0x00010000;
    buf_[1] = 0x00020000;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 2, &c_), 2);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.state_and_event_indicators", true}});
}

TEST_F(ReadIfContextTest, StateAndEventIndicatorsAgcOrMgc) {
    buf_[0] = 0x00010000;
    buf_[1] = 0x00010000;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 2, &c_), 2);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.state_and_event_indicators", true}});
}

TEST_F(ReadIfContextTest, StateAndEventIndicatorsDetectedSignal) {
    buf_[0] = 0x00010000;
    buf_[1] = 0x00008000;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 2, &c_), 2);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.state_and_event_indicators", true}});
}

TEST_F(ReadIfContextTest, StateAndEventIndicatorsSpectralInversion) {
    buf_[0] = 0x00010000;
    buf_[1] = 0x00004000;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 2, &c_), 2);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.state_and_event_indicators", true}});
}

TEST_F(ReadIfContextTest, StateAndEventIndicatorsOverRange) {
    buf_[0] = 0x00010000;
    buf_[1] = 0x00002000;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 2, &c_), 2);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.state_and_event_indicators", true}});
}

TEST_F(ReadIfContextTest, StateAndEventIndicatorsSampleLoss) {
    buf_[0] = 0x00010000;
    buf_[1] = 0x00001000;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 2, &c_), 2);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.state_and_event_indicators", true}});
}

TEST_F(ReadIfContextTest, StateAndEventIndicatorsBothCalibratedTime) {
    buf_[0] = 0x00010000;
    buf_[1] = 0x80080000;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 2, &c_), 2);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.state_and_event_indicators", true},
                           {"state_and_event_indicators.has.calibrated_time", true},
                           {"state_and_event_indicators.calibrated_time", true}});
}

TEST_F(ReadIfContextTest, StateAndEventIndicatorsBothValidData) {
    buf_[0] = 0x00010000;
    buf_[1] = 0x40040000;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 2, &c_), 2);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.state_and_event_indicators", true},
                           {"state_and_event_indicators.has.valid_data", true},
                           {"state_and_event_indicators.valid_data", true}});
}

TEST_F(ReadIfContextTest, StateAndEventIndicatorsBothReferenceLock) {
    buf_[0] = 0x00010000;
    buf_[1] = 0x20020000;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 2, &c_), 2);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.state_and_event_indicators", true},
                           {"state_and_event_indicators.has.reference_lock", true},
                           {"state_and_event_indicators.reference_lock", true}});
}

TEST_F(ReadIfContextTest, StateAndEventIndicatorsBothAgcOrMgc) {
    buf_[0] = 0x00010000;
    buf_[1] = 0x10010000;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 2, &c_), 2);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.state_and_event_indicators", true},
                           {"state_and_event_indicators.has.agc_or_mgc", true},
                           {"state_and_event_indicators.agc_or_mgc", true}});
}

TEST_F(ReadIfContextTest, StateAndEventIndicatorsBothDetectedSignal) {
    buf_[0] = 0x00010000;
    buf_[1] = 0x08008000;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 2, &c_), 2);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.state_and_event_indicators", true},
                           {"state_and_event_indicators.has.detected_signal", true},
                           {"state_and_event_indicators.detected_signal", true}});
}

TEST_F(ReadIfContextTest, StateAndEventIndicatorsBothSpectralInversion) {
    buf_[0] = 0x00010000;
    buf_[1] = 0x04004000;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 2, &c_), 2);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.state_and_event_indicators", true},
                           {"state_and_event_indicators.has.spectral_inversion", true},
                           {"state_and_event_indicators.spectral_inversion", true}});
}

TEST_F(ReadIfContextTest, StateAndEventIndicatorsBothOverRange) {
    buf_[0] = 0x00010000;
    buf_[1] = 0x02002000;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 2, &c_), 2);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.state_and_event_indicators", true},
                           {"state_and_event_indicators.has.over_range", true},
                           {"state_and_event_indicators.over_range", true}});
}

TEST_F(ReadIfContextTest, StateAndEventIndicatorsBothSampleLoss) {
    buf_[0] = 0x00010000;
    buf_[1] = 0x01001000;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 2, &c_), 2);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.state_and_event_indicators", true},
                           {"state_and_event_indicators.has.sample_loss", true},
                           {"state_and_event_indicators.sample_loss", true}});
}

TEST_F(ReadIfContextTest, StateAndEventIndicatorsUserDefined7) {
    buf_[0] = 0x00010000;
    buf_[1] = 0x00000080;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 2, &c_), 2);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_,
                      {{"has.state_and_event_indicators", true}, {"state_and_event_indicators.user_defined7", true}});
}

TEST_F(ReadIfContextTest, StateAndEventIndicatorsUserDefined6) {
    buf_[0] = 0x00010000;
    buf_[1] = 0x00000040;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 2, &c_), 2);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_,
                      {{"has.state_and_event_indicators", true}, {"state_and_event_indicators.user_defined6", true}});
}

TEST_F(ReadIfContextTest, StateAndEventIndicatorsUserDefined5) {
    buf_[0] = 0x00010000;
    buf_[1] = 0x00000020;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 2, &c_), 2);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_,
                      {{"has.state_and_event_indicators", true}, {"state_and_event_indicators.user_defined5", true}});
}

TEST_F(ReadIfContextTest, StateAndEventIndicatorsUserDefine4) {
    buf_[0] = 0x00010000;
    buf_[1] = 0x00000010;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 2, &c_), 2);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_,
                      {{"has.state_and_event_indicators", true}, {"state_and_event_indicators.user_defined4", true}});
}

TEST_F(ReadIfContextTest, StateAndEventIndicatorsUserDefined3) {
    buf_[0] = 0x00010000;
    buf_[1] = 0x00000008;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 2, &c_), 2);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_,
                      {{"has.state_and_event_indicators", true}, {"state_and_event_indicators.user_defined3", true}});
}

TEST_F(ReadIfContextTest, StateAndEventIndicatorsUserDefined2) {
    buf_[0] = 0x00010000;
    buf_[1] = 0x00000004;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 2, &c_), 2);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_,
                      {{"has.state_and_event_indicators", true}, {"state_and_event_indicators.user_defined2", true}});
}

TEST_F(ReadIfContextTest, StateAndEventIndicatorsUserDefined1) {
    buf_[0] = 0x00010000;
    buf_[1] = 0x00000002;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 2, &c_), 2);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_,
                      {{"has.state_and_event_indicators", true}, {"state_and_event_indicators.user_defined1", true}});
}

TEST_F(ReadIfContextTest, StateAndEventIndicatorsUserDefined0) {
    buf_[0] = 0x00010000;
    buf_[1] = 0x00000001;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 2, &c_), 2);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_,
                      {{"has.state_and_event_indicators", true}, {"state_and_event_indicators.user_defined0", true}});
}

TEST_F(ReadIfContextTest, StateAndEventIndicatorsReserved) {
    buf_[0] = 0x00010000;
    buf_[1] = 0x00F00F00;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 2, &c_), 2);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.state_and_event_indicators", true}});
}

TEST_F(ReadIfContextTest, DataPacketPayloadFormatPackingMethod) {
    buf_[0] = 0x00008000;
    buf_[1] = 0x80000000;
    buf_[2] = 0x00000000;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 3, &c_), 3);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_,
                      {{"has.data_packet_payload_format", true}, {"data_packet_payload_format.packing_method", true}});
}

TEST_F(ReadIfContextTest, DataPacketPayloadFormatPackingMethodRealOrComplex) {
    buf_[0] = 0x00008000;
    buf_[1] = 0x40000000;
    buf_[2] = 0x00000000;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 3, &c_), 3);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.data_packet_payload_format", true},
                           {"data_packet_payload_format.real_or_complex", VRT_ROC_COMPLEX_POLAR}});
}

TEST_F(ReadIfContextTest, DataPacketPayloadFormatDataItemFormat) {
    buf_[0] = 0x00008000;
    buf_[1] = 0x16000000;
    buf_[2] = 0x00000000;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 3, &c_), 3);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.data_packet_payload_format", true},
                           {"data_packet_payload_format.data_item_format", VRT_DIF_UNSIGNED_VRT_6_BIT_EXPONENT}});
}

TEST_F(ReadIfContextTest, DataPacketPayloadFormatSampleComponentRepeat) {
    buf_[0] = 0x00008000;
    buf_[1] = 0x00800000;
    buf_[2] = 0x00000000;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 3, &c_), 3);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(
        c_, {{"has.data_packet_payload_format", true}, {"data_packet_payload_format.sample_component_repeat", true}});
}

TEST_F(ReadIfContextTest, DataPacketPayloadFormatEventTagSize) {
    buf_[0] = 0x00008000;
    buf_[1] = 0x00700000;
    buf_[2] = 0x00000000;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 3, &c_), 3);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.data_packet_payload_format", true},
                           {"data_packet_payload_format.event_tag_size", static_cast<uint8_t>(0x7)}});
}

TEST_F(ReadIfContextTest, DataPacketPayloadFormatChannelTagSize) {
    buf_[0] = 0x00008000;
    buf_[1] = 0x000F0000;
    buf_[2] = 0x00000000;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 3, &c_), 3);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.data_packet_payload_format", true},
                           {"data_packet_payload_format.channel_tag_size", static_cast<uint8_t>(0xF)}});
}

TEST_F(ReadIfContextTest, DataPacketPayloadFormatItemPackingFieldSize) {
    buf_[0] = 0x00008000;
    buf_[1] = 0x00000FC0;
    buf_[2] = 0x00000000;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 3, &c_), 3);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.data_packet_payload_format", true},
                           {"data_packet_payload_format.item_packing_field_size", static_cast<uint8_t>(0x3F)}});
}

TEST_F(ReadIfContextTest, DataPacketPayloadFormatDataItemSize) {
    buf_[0] = 0x00008000;
    buf_[1] = 0x0000003F;
    buf_[2] = 0x00000000;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 3, &c_), 3);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.data_packet_payload_format", true},
                           {"data_packet_payload_format.data_item_size", static_cast<uint8_t>(0x3F)}});
}

TEST_F(ReadIfContextTest, DataPacketPayloadFormatRepeatCount) {
    buf_[0] = 0x00008000;
    buf_[1] = 0x00000000;
    buf_[2] = 0xFFFF0000;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 3, &c_), 3);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.data_packet_payload_format", true},
                           {"data_packet_payload_format.repeat_count", static_cast<uint16_t>(0xFFFF)}});
}

TEST_F(ReadIfContextTest, DataPacketPayloadFormatVectorSize) {
    buf_[0] = 0x00008000;
    buf_[1] = 0x00000000;
    buf_[2] = 0x0000FFFF;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 3, &c_), 3);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.data_packet_payload_format", true},
                           {"data_packet_payload_format.vector_size", static_cast<uint16_t>(0xFFFF)}});
}

TEST_F(ReadIfContextTest, Reserved) {
    buf_[0] = 0x00008000;
    buf_[1] = 0x0000F000;
    buf_[2] = 0x00000000;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 3, &c_), 3);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.data_packet_payload_format", true}});
}

TEST_F(ReadIfContextTest, FormattedGpsGeolocationTsi) {
    buf_[0]  = 0x00004000;
    buf_[1]  = 0x0C000000;
    buf_[2]  = 0xFFFFFFFF;
    buf_[3]  = 0xFFFFFFFF;
    buf_[4]  = 0xFFFFFFFF;
    buf_[5]  = 0x7FFFFFFF;
    buf_[6]  = 0x7FFFFFFF;
    buf_[7]  = 0x7FFFFFFF;
    buf_[8]  = 0x7FFFFFFF;
    buf_[9]  = 0x7FFFFFFF;
    buf_[10] = 0x7FFFFFFF;
    buf_[11] = 0x7FFFFFFF;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 12, &c_), 12);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.formatted_gps_geolocation", true}, {"formatted_gps_geolocation.tsi", VRT_TSI_OTHER}});
}

TEST_F(ReadIfContextTest, FormattedGpsGeolocationTsf) {
    buf_[0]  = 0x00004000;
    buf_[1]  = 0x03000000;
    buf_[2]  = 0xFFFFFFFF;
    buf_[3]  = 0xFFFFFFFF;
    buf_[4]  = 0xFFFFFFFF;
    buf_[5]  = 0x7FFFFFFF;
    buf_[6]  = 0x7FFFFFFF;
    buf_[7]  = 0x7FFFFFFF;
    buf_[8]  = 0x7FFFFFFF;
    buf_[9]  = 0x7FFFFFFF;
    buf_[10] = 0x7FFFFFFF;
    buf_[11] = 0x7FFFFFFF;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 12, &c_), 12);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(
        c_, {{"has.formatted_gps_geolocation", true}, {"formatted_gps_geolocation.tsf", VRT_TSF_FREE_RUNNING_COUNT}});
}

TEST_F(ReadIfContextTest, FormattedGpsGeolocationOui) {
    buf_[0]  = 0x00004000;
    buf_[1]  = 0x00FFFFFF;
    buf_[2]  = 0xFFFFFFFF;
    buf_[3]  = 0xFFFFFFFF;
    buf_[4]  = 0xFFFFFFFF;
    buf_[5]  = 0x7FFFFFFF;
    buf_[6]  = 0x7FFFFFFF;
    buf_[7]  = 0x7FFFFFFF;
    buf_[8]  = 0x7FFFFFFF;
    buf_[9]  = 0x7FFFFFFF;
    buf_[10] = 0x7FFFFFFF;
    buf_[11] = 0x7FFFFFFF;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 12, &c_), 12);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.formatted_gps_geolocation", true},
                           {"formatted_gps_geolocation.oui", static_cast<uint32_t>(0x00FFFFFF)}});
}

TEST_F(ReadIfContextTest, FormattedGpsGeolocationIntegerSecondTimestamp) {
    buf_[0]  = 0x00004000;
    buf_[1]  = 0x04000000;
    buf_[2]  = 0xABABABAB;
    buf_[3]  = 0xFFFFFFFF;
    buf_[4]  = 0xFFFFFFFF;
    buf_[5]  = 0x7FFFFFFF;
    buf_[6]  = 0x7FFFFFFF;
    buf_[7]  = 0x7FFFFFFF;
    buf_[8]  = 0x7FFFFFFF;
    buf_[9]  = 0x7FFFFFFF;
    buf_[10] = 0x7FFFFFFF;
    buf_[11] = 0x7FFFFFFF;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 12, &c_), 12);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.formatted_gps_geolocation", true},
                           {"formatted_gps_geolocation.tsi", VRT_TSI_UTC},
                           {"formatted_gps_geolocation.integer_second_timestamp", static_cast<uint32_t>(0xABABABAB)}});
}

TEST_F(ReadIfContextTest, FormattedGpsGeolocationFractionalSecondTimestamp) {
    buf_[0]  = 0x00004000;
    buf_[1]  = 0x01000000;
    buf_[2]  = 0xFFFFFFFF;
    buf_[3]  = 0xABABABAB;
    buf_[4]  = 0xABBABBAB;
    buf_[5]  = 0x7FFFFFFF;
    buf_[6]  = 0x7FFFFFFF;
    buf_[7]  = 0x7FFFFFFF;
    buf_[8]  = 0x7FFFFFFF;
    buf_[9]  = 0x7FFFFFFF;
    buf_[10] = 0x7FFFFFFF;
    buf_[11] = 0x7FFFFFFF;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 12, &c_), 12);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(
        c_, {{"has.formatted_gps_geolocation", true},
             {"formatted_gps_geolocation.tsf", VRT_TSF_SAMPLE_COUNT},
             {"formatted_gps_geolocation.fractional_second_timestamp", static_cast<uint64_t>(0xABABABABABBABBAB)}});
}

TEST_F(ReadIfContextTest, FormattedGpsGeolocationIntegerLatitude) {
    buf_[0]  = 0x00004000;
    buf_[1]  = 0x00000000;
    buf_[2]  = 0xFFFFFFFF;
    buf_[3]  = 0xFFFFFFFF;
    buf_[4]  = 0xFFFFFFFF;
    buf_[5]  = 0x00400000;
    buf_[6]  = 0x7FFFFFFF;
    buf_[7]  = 0x7FFFFFFF;
    buf_[8]  = 0x7FFFFFFF;
    buf_[9]  = 0x7FFFFFFF;
    buf_[10] = 0x7FFFFFFF;
    buf_[11] = 0x7FFFFFFF;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 12, &c_), 12);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.formatted_gps_geolocation", true}, {"formatted_gps_geolocation.latitude", 1.0}});
}

TEST_F(ReadIfContextTest, FormattedGpsGeolocationIntegerLongitude) {
    buf_[0]  = 0x00004000;
    buf_[1]  = 0x00000000;
    buf_[2]  = 0xFFFFFFFF;
    buf_[3]  = 0xFFFFFFFF;
    buf_[4]  = 0xFFFFFFFF;
    buf_[5]  = 0x7FFFFFFF;
    buf_[6]  = 0x00400000;
    buf_[7]  = 0x7FFFFFFF;
    buf_[8]  = 0x7FFFFFFF;
    buf_[9]  = 0x7FFFFFFF;
    buf_[10] = 0x7FFFFFFF;
    buf_[11] = 0x7FFFFFFF;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 12, &c_), 12);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.formatted_gps_geolocation", true}, {"formatted_gps_geolocation.longitude", 1.0}});
}

TEST_F(ReadIfContextTest, FormattedGpsGeolocationIntegerAltitude) {
    buf_[0]  = 0x00004000;
    buf_[1]  = 0x00000000;
    buf_[2]  = 0xFFFFFFFF;
    buf_[3]  = 0xFFFFFFFF;
    buf_[4]  = 0xFFFFFFFF;
    buf_[5]  = 0x7FFFFFFF;
    buf_[6]  = 0x7FFFFFFF;
    buf_[7]  = 0x00000020;
    buf_[8]  = 0x7FFFFFFF;
    buf_[9]  = 0x7FFFFFFF;
    buf_[10] = 0x7FFFFFFF;
    buf_[11] = 0x7FFFFFFF;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 12, &c_), 12);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.formatted_gps_geolocation", true}, {"formatted_gps_geolocation.altitude", 1.0}});
}

TEST_F(ReadIfContextTest, FormattedGpsGeolocationSpeedOverGround) {
    buf_[0]  = 0x00004000;
    buf_[1]  = 0x00000000;
    buf_[2]  = 0xFFFFFFFF;
    buf_[3]  = 0xFFFFFFFF;
    buf_[4]  = 0xFFFFFFFF;
    buf_[5]  = 0x7FFFFFFF;
    buf_[6]  = 0x7FFFFFFF;
    buf_[7]  = 0x7FFFFFFF;
    buf_[8]  = 0x00010000;
    buf_[9]  = 0x7FFFFFFF;
    buf_[10] = 0x7FFFFFFF;
    buf_[11] = 0x7FFFFFFF;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 12, &c_), 12);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_,
                      {{"has.formatted_gps_geolocation", true}, {"formatted_gps_geolocation.speed_over_ground", 1.0}});
}

TEST_F(ReadIfContextTest, FormattedGpsGeolocationHeadingAngle) {
    buf_[0]  = 0x00004000;
    buf_[1]  = 0x00000000;
    buf_[2]  = 0xFFFFFFFF;
    buf_[3]  = 0xFFFFFFFF;
    buf_[4]  = 0xFFFFFFFF;
    buf_[5]  = 0x7FFFFFFF;
    buf_[6]  = 0x7FFFFFFF;
    buf_[7]  = 0x7FFFFFFF;
    buf_[8]  = 0x7FFFFFFF;
    buf_[9]  = 0x00400000;
    buf_[10] = 0x7FFFFFFF;
    buf_[11] = 0x7FFFFFFF;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 12, &c_), 12);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.formatted_gps_geolocation", true}, {"formatted_gps_geolocation.heading_angle", 1.0}});
}

TEST_F(ReadIfContextTest, FormattedGpsGeolocationTrackAngle) {
    buf_[0]  = 0x00004000;
    buf_[1]  = 0x00000000;
    buf_[2]  = 0xFFFFFFFF;
    buf_[3]  = 0xFFFFFFFF;
    buf_[4]  = 0xFFFFFFFF;
    buf_[5]  = 0x7FFFFFFF;
    buf_[6]  = 0x7FFFFFFF;
    buf_[7]  = 0x7FFFFFFF;
    buf_[8]  = 0x7FFFFFFF;
    buf_[9]  = 0x7FFFFFFF;
    buf_[10] = 0x00400000;
    buf_[11] = 0x7FFFFFFF;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 12, &c_), 12);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.formatted_gps_geolocation", true}, {"formatted_gps_geolocation.track_angle", 1.0}});
}

TEST_F(ReadIfContextTest, FormattedGpsGeolocationMagneticVariation) {
    buf_[0]  = 0x00004000;
    buf_[1]  = 0x00000000;
    buf_[2]  = 0xFFFFFFFF;
    buf_[3]  = 0xFFFFFFFF;
    buf_[4]  = 0xFFFFFFFF;
    buf_[5]  = 0x7FFFFFFF;
    buf_[6]  = 0x7FFFFFFF;
    buf_[7]  = 0x7FFFFFFF;
    buf_[8]  = 0x7FFFFFFF;
    buf_[9]  = 0x7FFFFFFF;
    buf_[10] = 0x7FFFFFFF;
    buf_[11] = 0x00400000;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 12, &c_), 12);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_,
                      {{"has.formatted_gps_geolocation", true}, {"formatted_gps_geolocation.magnetic_variation", 1.0}});
}

TEST_F(ReadIfContextTest, FormattedGpsGeolocationReserved) {
    buf_[0]  = 0x00004000;
    buf_[1]  = 0xF0000000;
    buf_[2]  = 0xFFFFFFFF;
    buf_[3]  = 0xFFFFFFFF;
    buf_[4]  = 0xFFFFFFFF;
    buf_[5]  = 0x7FFFFFFF;
    buf_[6]  = 0x7FFFFFFF;
    buf_[7]  = 0x7FFFFFFF;
    buf_[8]  = 0x7FFFFFFF;
    buf_[9]  = 0x7FFFFFFF;
    buf_[10] = 0x7FFFFFFF;
    buf_[11] = 0x7FFFFFFF;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 12, &c_), 12);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.formatted_gps_geolocation", true}});
}

TEST_F(ReadIfContextTest, FormattedInsGeolocationTsi) {
    buf_[0]  = 0x00002000;
    buf_[1]  = 0x0C000000;
    buf_[2]  = 0xFFFFFFFF;
    buf_[3]  = 0xFFFFFFFF;
    buf_[4]  = 0xFFFFFFFF;
    buf_[5]  = 0x7FFFFFFF;
    buf_[6]  = 0x7FFFFFFF;
    buf_[7]  = 0x7FFFFFFF;
    buf_[8]  = 0x7FFFFFFF;
    buf_[9]  = 0x7FFFFFFF;
    buf_[10] = 0x7FFFFFFF;
    buf_[11] = 0x7FFFFFFF;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 12, &c_), 12);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.formatted_ins_geolocation", true}, {"formatted_ins_geolocation.tsi", VRT_TSI_OTHER}});
}

TEST_F(ReadIfContextTest, FormattedInsGeolocationTsf) {
    buf_[0]  = 0x00002000;
    buf_[1]  = 0x03000000;
    buf_[2]  = 0xFFFFFFFF;
    buf_[3]  = 0xFFFFFFFF;
    buf_[4]  = 0xFFFFFFFF;
    buf_[5]  = 0x7FFFFFFF;
    buf_[6]  = 0x7FFFFFFF;
    buf_[7]  = 0x7FFFFFFF;
    buf_[8]  = 0x7FFFFFFF;
    buf_[9]  = 0x7FFFFFFF;
    buf_[10] = 0x7FFFFFFF;
    buf_[11] = 0x7FFFFFFF;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 12, &c_), 12);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(
        c_, {{"has.formatted_ins_geolocation", true}, {"formatted_ins_geolocation.tsf", VRT_TSF_FREE_RUNNING_COUNT}});
}

TEST_F(ReadIfContextTest, FormattedInsGeolocationOui) {
    buf_[0]  = 0x00002000;
    buf_[1]  = 0x00FFFFFF;
    buf_[2]  = 0xFFFFFFFF;
    buf_[3]  = 0xFFFFFFFF;
    buf_[4]  = 0xFFFFFFFF;
    buf_[5]  = 0x7FFFFFFF;
    buf_[6]  = 0x7FFFFFFF;
    buf_[7]  = 0x7FFFFFFF;
    buf_[8]  = 0x7FFFFFFF;
    buf_[9]  = 0x7FFFFFFF;
    buf_[10] = 0x7FFFFFFF;
    buf_[11] = 0x7FFFFFFF;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 12, &c_), 12);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.formatted_ins_geolocation", true},
                           {"formatted_ins_geolocation.oui", static_cast<uint32_t>(0x00FFFFFF)}});
}

TEST_F(ReadIfContextTest, FormattedInsGeolocationIntegerSecondTimestamp) {
    buf_[0]  = 0x00002000;
    buf_[1]  = 0x04000000;
    buf_[2]  = 0xABABABAB;
    buf_[3]  = 0xFFFFFFFF;
    buf_[4]  = 0xFFFFFFFF;
    buf_[5]  = 0x7FFFFFFF;
    buf_[6]  = 0x7FFFFFFF;
    buf_[7]  = 0x7FFFFFFF;
    buf_[8]  = 0x7FFFFFFF;
    buf_[9]  = 0x7FFFFFFF;
    buf_[10] = 0x7FFFFFFF;
    buf_[11] = 0x7FFFFFFF;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 12, &c_), 12);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.formatted_ins_geolocation", true},
                           {"formatted_ins_geolocation.tsi", VRT_TSI_UTC},
                           {"formatted_ins_geolocation.integer_second_timestamp", static_cast<uint32_t>(0xABABABAB)}});
}

TEST_F(ReadIfContextTest, FormattedInsGeolocationFractionalSecondTimestamp) {
    buf_[0]  = 0x00002000;
    buf_[1]  = 0x01000000;
    buf_[2]  = 0xFFFFFFFF;
    buf_[3]  = 0xABABABAB;
    buf_[4]  = 0xABBABBAB;
    buf_[5]  = 0x7FFFFFFF;
    buf_[6]  = 0x7FFFFFFF;
    buf_[7]  = 0x7FFFFFFF;
    buf_[8]  = 0x7FFFFFFF;
    buf_[9]  = 0x7FFFFFFF;
    buf_[10] = 0x7FFFFFFF;
    buf_[11] = 0x7FFFFFFF;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 12, &c_), 12);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(
        c_, {{"has.formatted_ins_geolocation", true},
             {"formatted_ins_geolocation.tsf", VRT_TSF_SAMPLE_COUNT},
             {"formatted_ins_geolocation.fractional_second_timestamp", static_cast<uint64_t>(0xABABABABABBABBAB)}});
}

TEST_F(ReadIfContextTest, FormattedInsGeolocationIntegerLatitude) {
    buf_[0]  = 0x00002000;
    buf_[1]  = 0x00000000;
    buf_[2]  = 0xFFFFFFFF;
    buf_[3]  = 0xFFFFFFFF;
    buf_[4]  = 0xFFFFFFFF;
    buf_[5]  = 0x00400000;
    buf_[6]  = 0x7FFFFFFF;
    buf_[7]  = 0x7FFFFFFF;
    buf_[8]  = 0x7FFFFFFF;
    buf_[9]  = 0x7FFFFFFF;
    buf_[10] = 0x7FFFFFFF;
    buf_[11] = 0x7FFFFFFF;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 12, &c_), 12);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.formatted_ins_geolocation", true}, {"formatted_ins_geolocation.latitude", 1.0}});
}

TEST_F(ReadIfContextTest, FormattedInsGeolocationIntegerLongitude) {
    buf_[0]  = 0x00002000;
    buf_[1]  = 0x00000000;
    buf_[2]  = 0xFFFFFFFF;
    buf_[3]  = 0xFFFFFFFF;
    buf_[4]  = 0xFFFFFFFF;
    buf_[5]  = 0x7FFFFFFF;
    buf_[6]  = 0x00400000;
    buf_[7]  = 0x7FFFFFFF;
    buf_[8]  = 0x7FFFFFFF;
    buf_[9]  = 0x7FFFFFFF;
    buf_[10] = 0x7FFFFFFF;
    buf_[11] = 0x7FFFFFFF;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 12, &c_), 12);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.formatted_ins_geolocation", true}, {"formatted_ins_geolocation.longitude", 1.0}});
}

TEST_F(ReadIfContextTest, FormattedInsGeolocationIntegerAltitude) {
    buf_[0]  = 0x00002000;
    buf_[1]  = 0x00000000;
    buf_[2]  = 0xFFFFFFFF;
    buf_[3]  = 0xFFFFFFFF;
    buf_[4]  = 0xFFFFFFFF;
    buf_[5]  = 0x7FFFFFFF;
    buf_[6]  = 0x7FFFFFFF;
    buf_[7]  = 0x00000020;
    buf_[8]  = 0x7FFFFFFF;
    buf_[9]  = 0x7FFFFFFF;
    buf_[10] = 0x7FFFFFFF;
    buf_[11] = 0x7FFFFFFF;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 12, &c_), 12);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.formatted_ins_geolocation", true}, {"formatted_ins_geolocation.altitude", 1.0}});
}

TEST_F(ReadIfContextTest, FormattedInsGeolocationSpeedOverGround) {
    buf_[0]  = 0x00002000;
    buf_[1]  = 0x00000000;
    buf_[2]  = 0xFFFFFFFF;
    buf_[3]  = 0xFFFFFFFF;
    buf_[4]  = 0xFFFFFFFF;
    buf_[5]  = 0x7FFFFFFF;
    buf_[6]  = 0x7FFFFFFF;
    buf_[7]  = 0x7FFFFFFF;
    buf_[8]  = 0x00010000;
    buf_[9]  = 0x7FFFFFFF;
    buf_[10] = 0x7FFFFFFF;
    buf_[11] = 0x7FFFFFFF;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 12, &c_), 12);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_,
                      {{"has.formatted_ins_geolocation", true}, {"formatted_ins_geolocation.speed_over_ground", 1.0}});
}

TEST_F(ReadIfContextTest, FormattedInsGeolocationHeadingAngle) {
    buf_[0]  = 0x00002000;
    buf_[1]  = 0x00000000;
    buf_[2]  = 0xFFFFFFFF;
    buf_[3]  = 0xFFFFFFFF;
    buf_[4]  = 0xFFFFFFFF;
    buf_[5]  = 0x7FFFFFFF;
    buf_[6]  = 0x7FFFFFFF;
    buf_[7]  = 0x7FFFFFFF;
    buf_[8]  = 0x7FFFFFFF;
    buf_[9]  = 0x00400000;
    buf_[10] = 0x7FFFFFFF;
    buf_[11] = 0x7FFFFFFF;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 12, &c_), 12);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.formatted_ins_geolocation", true}, {"formatted_ins_geolocation.heading_angle", 1.0}});
}

TEST_F(ReadIfContextTest, FormattedInsGeolocationTrackAngle) {
    buf_[0]  = 0x00002000;
    buf_[1]  = 0x00000000;
    buf_[2]  = 0xFFFFFFFF;
    buf_[3]  = 0xFFFFFFFF;
    buf_[4]  = 0xFFFFFFFF;
    buf_[5]  = 0x7FFFFFFF;
    buf_[6]  = 0x7FFFFFFF;
    buf_[7]  = 0x7FFFFFFF;
    buf_[8]  = 0x7FFFFFFF;
    buf_[9]  = 0x7FFFFFFF;
    buf_[10] = 0x00400000;
    buf_[11] = 0x7FFFFFFF;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 12, &c_), 12);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.formatted_ins_geolocation", true}, {"formatted_ins_geolocation.track_angle", 1.0}});
}

TEST_F(ReadIfContextTest, FormattedInsGeolocationMagneticVariation) {
    buf_[0]  = 0x00002000;
    buf_[1]  = 0x00000000;
    buf_[2]  = 0xFFFFFFFF;
    buf_[3]  = 0xFFFFFFFF;
    buf_[4]  = 0xFFFFFFFF;
    buf_[5]  = 0x7FFFFFFF;
    buf_[6]  = 0x7FFFFFFF;
    buf_[7]  = 0x7FFFFFFF;
    buf_[8]  = 0x7FFFFFFF;
    buf_[9]  = 0x7FFFFFFF;
    buf_[10] = 0x7FFFFFFF;
    buf_[11] = 0x00400000;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 12, &c_), 12);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_,
                      {{"has.formatted_ins_geolocation", true}, {"formatted_ins_geolocation.magnetic_variation", 1.0}});
}

TEST_F(ReadIfContextTest, FormattedInsGeolocationReserved) {
    buf_[0]  = 0x00002000;
    buf_[1]  = 0xF0000000;
    buf_[2]  = 0xFFFFFFFF;
    buf_[3]  = 0xFFFFFFFF;
    buf_[4]  = 0xFFFFFFFF;
    buf_[5]  = 0x7FFFFFFF;
    buf_[6]  = 0x7FFFFFFF;
    buf_[7]  = 0x7FFFFFFF;
    buf_[8]  = 0x7FFFFFFF;
    buf_[9]  = 0x7FFFFFFF;
    buf_[10] = 0x7FFFFFFF;
    buf_[11] = 0x7FFFFFFF;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 12, &c_), 12);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.formatted_ins_geolocation", true}});
}

TEST_F(ReadIfContextTest, EcefEphemerisTsi) {
    buf_[0]  = 0x00001000;
    buf_[1]  = 0x0C000000;
    buf_[2]  = 0xFFFFFFFF;
    buf_[3]  = 0xFFFFFFFF;
    buf_[4]  = 0xFFFFFFFF;
    buf_[5]  = 0x7FFFFFFF;
    buf_[6]  = 0x7FFFFFFF;
    buf_[7]  = 0x7FFFFFFF;
    buf_[8]  = 0x7FFFFFFF;
    buf_[9]  = 0x7FFFFFFF;
    buf_[10] = 0x7FFFFFFF;
    buf_[11] = 0x7FFFFFFF;
    buf_[12] = 0x7FFFFFFF;
    buf_[13] = 0x7FFFFFFF;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 14, &c_), 14);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.ecef_ephemeris", true}, {"ecef_ephemeris.tsi", VRT_TSI_OTHER}});
}

TEST_F(ReadIfContextTest, EcefEphemerisTsf) {
    buf_[0]  = 0x00001000;
    buf_[1]  = 0x03000000;
    buf_[2]  = 0xFFFFFFFF;
    buf_[3]  = 0xFFFFFFFF;
    buf_[4]  = 0xFFFFFFFF;
    buf_[5]  = 0x7FFFFFFF;
    buf_[6]  = 0x7FFFFFFF;
    buf_[7]  = 0x7FFFFFFF;
    buf_[8]  = 0x7FFFFFFF;
    buf_[9]  = 0x7FFFFFFF;
    buf_[10] = 0x7FFFFFFF;
    buf_[11] = 0x7FFFFFFF;
    buf_[12] = 0x7FFFFFFF;
    buf_[13] = 0x7FFFFFFF;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 14, &c_), 14);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.ecef_ephemeris", true}, {"ecef_ephemeris.tsf", VRT_TSF_FREE_RUNNING_COUNT}});
}

TEST_F(ReadIfContextTest, EcefEphemerisEcefOui) {
    buf_[0]  = 0x00001000;
    buf_[1]  = 0x00FFFFFF;
    buf_[2]  = 0xFFFFFFFF;
    buf_[3]  = 0xFFFFFFFF;
    buf_[4]  = 0xFFFFFFFF;
    buf_[5]  = 0x7FFFFFFF;
    buf_[6]  = 0x7FFFFFFF;
    buf_[7]  = 0x7FFFFFFF;
    buf_[8]  = 0x7FFFFFFF;
    buf_[9]  = 0x7FFFFFFF;
    buf_[10] = 0x7FFFFFFF;
    buf_[11] = 0x7FFFFFFF;
    buf_[12] = 0x7FFFFFFF;
    buf_[13] = 0x7FFFFFFF;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 14, &c_), 14);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.ecef_ephemeris", true}, {"ecef_ephemeris.oui", static_cast<uint32_t>(0x00FFFFFF)}});
}

TEST_F(ReadIfContextTest, EcefEphemerisIntegerSecondTimestamp) {
    buf_[0]  = 0x00001000;
    buf_[1]  = 0x04000000;
    buf_[2]  = 0xABABABAB;
    buf_[3]  = 0xFFFFFFFF;
    buf_[4]  = 0xFFFFFFFF;
    buf_[5]  = 0x7FFFFFFF;
    buf_[6]  = 0x7FFFFFFF;
    buf_[7]  = 0x7FFFFFFF;
    buf_[8]  = 0x7FFFFFFF;
    buf_[9]  = 0x7FFFFFFF;
    buf_[10] = 0x7FFFFFFF;
    buf_[11] = 0x7FFFFFFF;
    buf_[12] = 0x7FFFFFFF;
    buf_[13] = 0x7FFFFFFF;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 14, &c_), 14);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.ecef_ephemeris", true},
                           {"ecef_ephemeris.tsi", VRT_TSI_UTC},
                           {"ecef_ephemeris.integer_second_timestamp", static_cast<uint32_t>(0xABABABAB)}});
}

TEST_F(ReadIfContextTest, EcefEphemerisFractionalSecondTimestamp) {
    buf_[0]  = 0x00001000;
    buf_[1]  = 0x01000000;
    buf_[2]  = 0xFFFFFFFF;
    buf_[3]  = 0xABABABAB;
    buf_[4]  = 0xABBABBAB;
    buf_[5]  = 0x7FFFFFFF;
    buf_[6]  = 0x7FFFFFFF;
    buf_[7]  = 0x7FFFFFFF;
    buf_[8]  = 0x7FFFFFFF;
    buf_[9]  = 0x7FFFFFFF;
    buf_[10] = 0x7FFFFFFF;
    buf_[11] = 0x7FFFFFFF;
    buf_[12] = 0x7FFFFFFF;
    buf_[13] = 0x7FFFFFFF;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 14, &c_), 14);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.ecef_ephemeris", true},
                           {"ecef_ephemeris.tsf", VRT_TSF_SAMPLE_COUNT},
                           {"ecef_ephemeris.fractional_second_timestamp", static_cast<uint64_t>(0xABABABABABBABBAB)}});
}

TEST_F(ReadIfContextTest, EcefEphemerisPositionX) {
    buf_[0]  = 0x00001000;
    buf_[1]  = 0x00000000;
    buf_[2]  = 0xFFFFFFFF;
    buf_[3]  = 0xFFFFFFFF;
    buf_[4]  = 0xFFFFFFFF;
    buf_[5]  = 0x00000020;
    buf_[6]  = 0x7FFFFFFF;
    buf_[7]  = 0x7FFFFFFF;
    buf_[8]  = 0x7FFFFFFF;
    buf_[9]  = 0x7FFFFFFF;
    buf_[10] = 0x7FFFFFFF;
    buf_[11] = 0x7FFFFFFF;
    buf_[12] = 0x7FFFFFFF;
    buf_[13] = 0x7FFFFFFF;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 14, &c_), 14);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.ecef_ephemeris", true}, {"ecef_ephemeris.position_x", 1.0}});
}

TEST_F(ReadIfContextTest, EcefEphemerisPositionY) {
    buf_[0]  = 0x00001000;
    buf_[1]  = 0x00000000;
    buf_[2]  = 0xFFFFFFFF;
    buf_[3]  = 0xFFFFFFFF;
    buf_[4]  = 0xFFFFFFFF;
    buf_[5]  = 0x7FFFFFFF;
    buf_[6]  = 0x00000020;
    buf_[7]  = 0x7FFFFFFF;
    buf_[8]  = 0x7FFFFFFF;
    buf_[9]  = 0x7FFFFFFF;
    buf_[10] = 0x7FFFFFFF;
    buf_[11] = 0x7FFFFFFF;
    buf_[12] = 0x7FFFFFFF;
    buf_[13] = 0x7FFFFFFF;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 14, &c_), 14);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.ecef_ephemeris", true}, {"ecef_ephemeris.position_y", 1.0}});
}

TEST_F(ReadIfContextTest, EcefEphemerisPositionZ) {
    buf_[0]  = 0x00001000;
    buf_[1]  = 0x00000000;
    buf_[2]  = 0xFFFFFFFF;
    buf_[3]  = 0xFFFFFFFF;
    buf_[4]  = 0xFFFFFFFF;
    buf_[5]  = 0x7FFFFFFF;
    buf_[6]  = 0x7FFFFFFF;
    buf_[7]  = 0x00000020;
    buf_[8]  = 0x7FFFFFFF;
    buf_[9]  = 0x7FFFFFFF;
    buf_[10] = 0x7FFFFFFF;
    buf_[11] = 0x7FFFFFFF;
    buf_[12] = 0x7FFFFFFF;
    buf_[13] = 0x7FFFFFFF;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 14, &c_), 14);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.ecef_ephemeris", true}, {"ecef_ephemeris.position_z", 1.0}});
}

TEST_F(ReadIfContextTest, EcefEphemerisAttitudeAlpha) {
    buf_[0]  = 0x00001000;
    buf_[1]  = 0x00000000;
    buf_[2]  = 0xFFFFFFFF;
    buf_[3]  = 0xFFFFFFFF;
    buf_[4]  = 0xFFFFFFFF;
    buf_[5]  = 0x7FFFFFFF;
    buf_[6]  = 0x7FFFFFFF;
    buf_[7]  = 0x7FFFFFFF;
    buf_[8]  = 0x00400000;
    buf_[9]  = 0x7FFFFFFF;
    buf_[10] = 0x7FFFFFFF;
    buf_[11] = 0x7FFFFFFF;
    buf_[12] = 0x7FFFFFFF;
    buf_[13] = 0x7FFFFFFF;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 14, &c_), 14);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.ecef_ephemeris", true}, {"ecef_ephemeris.attitude_alpha", 1.0}});
}

TEST_F(ReadIfContextTest, EcefEphemerisAttitudeBeta) {
    buf_[0]  = 0x00001000;
    buf_[1]  = 0x00000000;
    buf_[2]  = 0xFFFFFFFF;
    buf_[3]  = 0xFFFFFFFF;
    buf_[4]  = 0xFFFFFFFF;
    buf_[5]  = 0x7FFFFFFF;
    buf_[6]  = 0x7FFFFFFF;
    buf_[7]  = 0x7FFFFFFF;
    buf_[8]  = 0x7FFFFFFF;
    buf_[9]  = 0x00400000;
    buf_[10] = 0x7FFFFFFF;
    buf_[11] = 0x7FFFFFFF;
    buf_[12] = 0x7FFFFFFF;
    buf_[13] = 0x7FFFFFFF;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 14, &c_), 14);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.ecef_ephemeris", true}, {"ecef_ephemeris.attitude_beta", 1.0}});
}

TEST_F(ReadIfContextTest, EcefEphemerisAttitudePhi) {
    buf_[0]  = 0x00001000;
    buf_[1]  = 0x00000000;
    buf_[2]  = 0xFFFFFFFF;
    buf_[3]  = 0xFFFFFFFF;
    buf_[4]  = 0xFFFFFFFF;
    buf_[5]  = 0x7FFFFFFF;
    buf_[6]  = 0x7FFFFFFF;
    buf_[7]  = 0x7FFFFFFF;
    buf_[8]  = 0x7FFFFFFF;
    buf_[9]  = 0x7FFFFFFF;
    buf_[10] = 0x00400000;
    buf_[11] = 0x7FFFFFFF;
    buf_[12] = 0x7FFFFFFF;
    buf_[13] = 0x7FFFFFFF;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 14, &c_), 14);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.ecef_ephemeris", true}, {"ecef_ephemeris.attitude_phi", 1.0}});
}

TEST_F(ReadIfContextTest, EcefEphemerisVelocityDx) {
    buf_[0]  = 0x00001000;
    buf_[1]  = 0x00000000;
    buf_[2]  = 0xFFFFFFFF;
    buf_[3]  = 0xFFFFFFFF;
    buf_[4]  = 0xFFFFFFFF;
    buf_[5]  = 0x7FFFFFFF;
    buf_[6]  = 0x7FFFFFFF;
    buf_[7]  = 0x7FFFFFFF;
    buf_[8]  = 0x7FFFFFFF;
    buf_[9]  = 0x7FFFFFFF;
    buf_[10] = 0x7FFFFFFF;
    buf_[11] = 0x00010000;
    buf_[12] = 0x7FFFFFFF;
    buf_[13] = 0x7FFFFFFF;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 14, &c_), 14);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.ecef_ephemeris", true}, {"ecef_ephemeris.velocity_dx", 1.0}});
}

TEST_F(ReadIfContextTest, EcefEphemerisVelocityDy) {
    buf_[0]  = 0x00001000;
    buf_[1]  = 0x00000000;
    buf_[2]  = 0xFFFFFFFF;
    buf_[3]  = 0xFFFFFFFF;
    buf_[4]  = 0xFFFFFFFF;
    buf_[5]  = 0x7FFFFFFF;
    buf_[6]  = 0x7FFFFFFF;
    buf_[7]  = 0x7FFFFFFF;
    buf_[8]  = 0x7FFFFFFF;
    buf_[9]  = 0x7FFFFFFF;
    buf_[10] = 0x7FFFFFFF;
    buf_[11] = 0x7FFFFFFF;
    buf_[12] = 0x00010000;
    buf_[13] = 0x7FFFFFFF;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 14, &c_), 14);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.ecef_ephemeris", true}, {"ecef_ephemeris.velocity_dy", 1.0}});
}

TEST_F(ReadIfContextTest, EcefEphemerisVelocityDz) {
    buf_[0]  = 0x00001000;
    buf_[1]  = 0x00000000;
    buf_[2]  = 0xFFFFFFFF;
    buf_[3]  = 0xFFFFFFFF;
    buf_[4]  = 0xFFFFFFFF;
    buf_[5]  = 0x7FFFFFFF;
    buf_[6]  = 0x7FFFFFFF;
    buf_[7]  = 0x7FFFFFFF;
    buf_[8]  = 0x7FFFFFFF;
    buf_[9]  = 0x7FFFFFFF;
    buf_[10] = 0x7FFFFFFF;
    buf_[11] = 0x7FFFFFFF;
    buf_[12] = 0x7FFFFFFF;
    buf_[13] = 0x00010000;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 14, &c_), 14);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.ecef_ephemeris", true}, {"ecef_ephemeris.velocity_dz", 1.0}});
}

TEST_F(ReadIfContextTest, EcefEphemerisReserved) {
    buf_[0]  = 0x00001000;
    buf_[1]  = 0xF0000000;
    buf_[2]  = 0xFFFFFFFF;
    buf_[3]  = 0xFFFFFFFF;
    buf_[4]  = 0xFFFFFFFF;
    buf_[5]  = 0x7FFFFFFF;
    buf_[6]  = 0x7FFFFFFF;
    buf_[7]  = 0x7FFFFFFF;
    buf_[8]  = 0x7FFFFFFF;
    buf_[9]  = 0x7FFFFFFF;
    buf_[10] = 0x7FFFFFFF;
    buf_[11] = 0x7FFFFFFF;
    buf_[12] = 0x7FFFFFFF;
    buf_[13] = 0x7FFFFFFF;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 14, &c_), 14);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.ecef_ephemeris", true}});
}

TEST_F(ReadIfContextTest, RelativeEphemerisTsi) {
    buf_[0]  = 0x00000800;
    buf_[1]  = 0x0C000000;
    buf_[2]  = 0xFFFFFFFF;
    buf_[3]  = 0xFFFFFFFF;
    buf_[4]  = 0xFFFFFFFF;
    buf_[5]  = 0x7FFFFFFF;
    buf_[6]  = 0x7FFFFFFF;
    buf_[7]  = 0x7FFFFFFF;
    buf_[8]  = 0x7FFFFFFF;
    buf_[9]  = 0x7FFFFFFF;
    buf_[10] = 0x7FFFFFFF;
    buf_[11] = 0x7FFFFFFF;
    buf_[12] = 0x7FFFFFFF;
    buf_[13] = 0x7FFFFFFF;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 14, &c_), 14);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.relative_ephemeris", true}, {"relative_ephemeris.tsi", VRT_TSI_OTHER}});
}

TEST_F(ReadIfContextTest, RelativeEphemerisTsf) {
    buf_[0]  = 0x00000800;
    buf_[1]  = 0x03000000;
    buf_[2]  = 0xFFFFFFFF;
    buf_[3]  = 0xFFFFFFFF;
    buf_[4]  = 0xFFFFFFFF;
    buf_[5]  = 0x7FFFFFFF;
    buf_[6]  = 0x7FFFFFFF;
    buf_[7]  = 0x7FFFFFFF;
    buf_[8]  = 0x7FFFFFFF;
    buf_[9]  = 0x7FFFFFFF;
    buf_[10] = 0x7FFFFFFF;
    buf_[11] = 0x7FFFFFFF;
    buf_[12] = 0x7FFFFFFF;
    buf_[13] = 0x7FFFFFFF;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 14, &c_), 14);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.relative_ephemeris", true}, {"relative_ephemeris.tsf", VRT_TSF_FREE_RUNNING_COUNT}});
}

TEST_F(ReadIfContextTest, RelativeEphemerisEcefOui) {
    buf_[0]  = 0x00000800;
    buf_[1]  = 0x00FFFFFF;
    buf_[2]  = 0xFFFFFFFF;
    buf_[3]  = 0xFFFFFFFF;
    buf_[4]  = 0xFFFFFFFF;
    buf_[5]  = 0x7FFFFFFF;
    buf_[6]  = 0x7FFFFFFF;
    buf_[7]  = 0x7FFFFFFF;
    buf_[8]  = 0x7FFFFFFF;
    buf_[9]  = 0x7FFFFFFF;
    buf_[10] = 0x7FFFFFFF;
    buf_[11] = 0x7FFFFFFF;
    buf_[12] = 0x7FFFFFFF;
    buf_[13] = 0x7FFFFFFF;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 14, &c_), 14);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(
        c_, {{"has.relative_ephemeris", true}, {"relative_ephemeris.oui", static_cast<uint32_t>(0x00FFFFFF)}});
}

TEST_F(ReadIfContextTest, RelativeEphemerisIntegerSecondTimestamp) {
    buf_[0]  = 0x00000800;
    buf_[1]  = 0x04000000;
    buf_[2]  = 0xABABABAB;
    buf_[3]  = 0xFFFFFFFF;
    buf_[4]  = 0xFFFFFFFF;
    buf_[5]  = 0x7FFFFFFF;
    buf_[6]  = 0x7FFFFFFF;
    buf_[7]  = 0x7FFFFFFF;
    buf_[8]  = 0x7FFFFFFF;
    buf_[9]  = 0x7FFFFFFF;
    buf_[10] = 0x7FFFFFFF;
    buf_[11] = 0x7FFFFFFF;
    buf_[12] = 0x7FFFFFFF;
    buf_[13] = 0x7FFFFFFF;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 14, &c_), 14);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.relative_ephemeris", true},
                           {"relative_ephemeris.tsi", VRT_TSI_UTC},
                           {"relative_ephemeris.integer_second_timestamp", static_cast<uint32_t>(0xABABABAB)}});
}

TEST_F(ReadIfContextTest, RelativeEphemerisFractionalSecondTimestamp) {
    buf_[0]  = 0x00000800;
    buf_[1]  = 0x01000000;
    buf_[2]  = 0xFFFFFFFF;
    buf_[3]  = 0xABABABAB;
    buf_[4]  = 0xABBABBAB;
    buf_[5]  = 0x7FFFFFFF;
    buf_[6]  = 0x7FFFFFFF;
    buf_[7]  = 0x7FFFFFFF;
    buf_[8]  = 0x7FFFFFFF;
    buf_[9]  = 0x7FFFFFFF;
    buf_[10] = 0x7FFFFFFF;
    buf_[11] = 0x7FFFFFFF;
    buf_[12] = 0x7FFFFFFF;
    buf_[13] = 0x7FFFFFFF;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 14, &c_), 14);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_,
                      {{"has.relative_ephemeris", true},
                       {"relative_ephemeris.tsf", VRT_TSF_SAMPLE_COUNT},
                       {"relative_ephemeris.fractional_second_timestamp", static_cast<uint64_t>(0xABABABABABBABBAB)}});
}

TEST_F(ReadIfContextTest, RelativeEphemerisPositionX) {
    buf_[0]  = 0x00000800;
    buf_[1]  = 0x00000000;
    buf_[2]  = 0xFFFFFFFF;
    buf_[3]  = 0xFFFFFFFF;
    buf_[4]  = 0xFFFFFFFF;
    buf_[5]  = 0x00000020;
    buf_[6]  = 0x7FFFFFFF;
    buf_[7]  = 0x7FFFFFFF;
    buf_[8]  = 0x7FFFFFFF;
    buf_[9]  = 0x7FFFFFFF;
    buf_[10] = 0x7FFFFFFF;
    buf_[11] = 0x7FFFFFFF;
    buf_[12] = 0x7FFFFFFF;
    buf_[13] = 0x7FFFFFFF;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 14, &c_), 14);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.relative_ephemeris", true}, {"relative_ephemeris.position_x", 1.0}});
}

TEST_F(ReadIfContextTest, RelativeEphemerisPositionY) {
    buf_[0]  = 0x00000800;
    buf_[1]  = 0x00000000;
    buf_[2]  = 0xFFFFFFFF;
    buf_[3]  = 0xFFFFFFFF;
    buf_[4]  = 0xFFFFFFFF;
    buf_[5]  = 0x7FFFFFFF;
    buf_[6]  = 0x00000020;
    buf_[7]  = 0x7FFFFFFF;
    buf_[8]  = 0x7FFFFFFF;
    buf_[9]  = 0x7FFFFFFF;
    buf_[10] = 0x7FFFFFFF;
    buf_[11] = 0x7FFFFFFF;
    buf_[12] = 0x7FFFFFFF;
    buf_[13] = 0x7FFFFFFF;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 14, &c_), 14);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.relative_ephemeris", true}, {"relative_ephemeris.position_y", 1.0}});
}

TEST_F(ReadIfContextTest, RelativeEphemerisPositionZ) {
    buf_[0]  = 0x00000800;
    buf_[1]  = 0x00000000;
    buf_[2]  = 0xFFFFFFFF;
    buf_[3]  = 0xFFFFFFFF;
    buf_[4]  = 0xFFFFFFFF;
    buf_[5]  = 0x7FFFFFFF;
    buf_[6]  = 0x7FFFFFFF;
    buf_[7]  = 0x00000020;
    buf_[8]  = 0x7FFFFFFF;
    buf_[9]  = 0x7FFFFFFF;
    buf_[10] = 0x7FFFFFFF;
    buf_[11] = 0x7FFFFFFF;
    buf_[12] = 0x7FFFFFFF;
    buf_[13] = 0x7FFFFFFF;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 14, &c_), 14);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.relative_ephemeris", true}, {"relative_ephemeris.position_z", 1.0}});
}

TEST_F(ReadIfContextTest, RelativeEphemerisAttitudeAlpha) {
    buf_[0]  = 0x00000800;
    buf_[1]  = 0x00000000;
    buf_[2]  = 0xFFFFFFFF;
    buf_[3]  = 0xFFFFFFFF;
    buf_[4]  = 0xFFFFFFFF;
    buf_[5]  = 0x7FFFFFFF;
    buf_[6]  = 0x7FFFFFFF;
    buf_[7]  = 0x7FFFFFFF;
    buf_[8]  = 0x00400000;
    buf_[9]  = 0x7FFFFFFF;
    buf_[10] = 0x7FFFFFFF;
    buf_[11] = 0x7FFFFFFF;
    buf_[12] = 0x7FFFFFFF;
    buf_[13] = 0x7FFFFFFF;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 14, &c_), 14);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.relative_ephemeris", true}, {"relative_ephemeris.attitude_alpha", 1.0}});
}

TEST_F(ReadIfContextTest, RelativeEphemerisAttitudeBeta) {
    buf_[0]  = 0x00000800;
    buf_[1]  = 0x00000000;
    buf_[2]  = 0xFFFFFFFF;
    buf_[3]  = 0xFFFFFFFF;
    buf_[4]  = 0xFFFFFFFF;
    buf_[5]  = 0x7FFFFFFF;
    buf_[6]  = 0x7FFFFFFF;
    buf_[7]  = 0x7FFFFFFF;
    buf_[8]  = 0x7FFFFFFF;
    buf_[9]  = 0x00400000;
    buf_[10] = 0x7FFFFFFF;
    buf_[11] = 0x7FFFFFFF;
    buf_[12] = 0x7FFFFFFF;
    buf_[13] = 0x7FFFFFFF;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 14, &c_), 14);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.relative_ephemeris", true}, {"relative_ephemeris.attitude_beta", 1.0}});
}

TEST_F(ReadIfContextTest, RelativeEphemerisAttitudePhi) {
    buf_[0]  = 0x00000800;
    buf_[1]  = 0x00000000;
    buf_[2]  = 0xFFFFFFFF;
    buf_[3]  = 0xFFFFFFFF;
    buf_[4]  = 0xFFFFFFFF;
    buf_[5]  = 0x7FFFFFFF;
    buf_[6]  = 0x7FFFFFFF;
    buf_[7]  = 0x7FFFFFFF;
    buf_[8]  = 0x7FFFFFFF;
    buf_[9]  = 0x7FFFFFFF;
    buf_[10] = 0x00400000;
    buf_[11] = 0x7FFFFFFF;
    buf_[12] = 0x7FFFFFFF;
    buf_[13] = 0x7FFFFFFF;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 14, &c_), 14);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.relative_ephemeris", true}, {"relative_ephemeris.attitude_phi", 1.0}});
}

TEST_F(ReadIfContextTest, RelativeEphemerisVelocityDx) {
    buf_[0]  = 0x00000800;
    buf_[1]  = 0x00000000;
    buf_[2]  = 0xFFFFFFFF;
    buf_[3]  = 0xFFFFFFFF;
    buf_[4]  = 0xFFFFFFFF;
    buf_[5]  = 0x7FFFFFFF;
    buf_[6]  = 0x7FFFFFFF;
    buf_[7]  = 0x7FFFFFFF;
    buf_[8]  = 0x7FFFFFFF;
    buf_[9]  = 0x7FFFFFFF;
    buf_[10] = 0x7FFFFFFF;
    buf_[11] = 0x00010000;
    buf_[12] = 0x7FFFFFFF;
    buf_[13] = 0x7FFFFFFF;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 14, &c_), 14);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.relative_ephemeris", true}, {"relative_ephemeris.velocity_dx", 1.0}});
}

TEST_F(ReadIfContextTest, RelativeEphemerisVelocityDy) {
    buf_[0]  = 0x00000800;
    buf_[1]  = 0x00000000;
    buf_[2]  = 0xFFFFFFFF;
    buf_[3]  = 0xFFFFFFFF;
    buf_[4]  = 0xFFFFFFFF;
    buf_[5]  = 0x7FFFFFFF;
    buf_[6]  = 0x7FFFFFFF;
    buf_[7]  = 0x7FFFFFFF;
    buf_[8]  = 0x7FFFFFFF;
    buf_[9]  = 0x7FFFFFFF;
    buf_[10] = 0x7FFFFFFF;
    buf_[11] = 0x7FFFFFFF;
    buf_[12] = 0x00010000;
    buf_[13] = 0x7FFFFFFF;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 14, &c_), 14);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.relative_ephemeris", true}, {"relative_ephemeris.velocity_dy", 1.0}});
}

TEST_F(ReadIfContextTest, RelativeEphemerisVelocityDz) {
    buf_[0]  = 0x00000800;
    buf_[1]  = 0x00000000;
    buf_[2]  = 0xFFFFFFFF;
    buf_[3]  = 0xFFFFFFFF;
    buf_[4]  = 0xFFFFFFFF;
    buf_[5]  = 0x7FFFFFFF;
    buf_[6]  = 0x7FFFFFFF;
    buf_[7]  = 0x7FFFFFFF;
    buf_[8]  = 0x7FFFFFFF;
    buf_[9]  = 0x7FFFFFFF;
    buf_[10] = 0x7FFFFFFF;
    buf_[11] = 0x7FFFFFFF;
    buf_[12] = 0x7FFFFFFF;
    buf_[13] = 0x00010000;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 14, &c_), 14);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.relative_ephemeris", true}, {"relative_ephemeris.velocity_dz", 1.0}});
}

TEST_F(ReadIfContextTest, RelativeEphemerisReserved) {
    buf_[0]  = 0x00000800;
    buf_[1]  = 0xF0000000;
    buf_[2]  = 0xFFFFFFFF;
    buf_[3]  = 0xFFFFFFFF;
    buf_[4]  = 0xFFFFFFFF;
    buf_[5]  = 0x7FFFFFFF;
    buf_[6]  = 0x7FFFFFFF;
    buf_[7]  = 0x7FFFFFFF;
    buf_[8]  = 0x7FFFFFFF;
    buf_[9]  = 0x7FFFFFFF;
    buf_[10] = 0x7FFFFFFF;
    buf_[11] = 0x7FFFFFFF;
    buf_[12] = 0x7FFFFFFF;
    buf_[13] = 0x7FFFFFFF;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 14, &c_), 14);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.relative_ephemeris", true}});
}

TEST_F(ReadIfContextTest, EphemerisReferenceIdentifier) {
    buf_[0] = 0x00000400;
    buf_[1] = 0xFFFFFFFF;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 2, &c_), 2);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_,
                      {{"has.ephemeris_reference_identifier", true}, {"ephemeris_reference_identifier", 0xFFFFFFFF}});
}

TEST_F(ReadIfContextTest, GpsAsciiOui) {
    buf_[0] = 0x00000200;
    buf_[1] = 0x00FFFFFF;
    buf_[2] = 0x00000000;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 3, &c_), 3);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.gps_ascii", true}, {"gps_ascii.oui", static_cast<uint32_t>(0x00FFFFFF)}});
}

TEST_F(ReadIfContextTest, GpsAsciiAscii) {
    buf_[0] = 0x00000200;
    buf_[1] = 0x00000000;
    buf_[2] = 0x00000003;
    std::string str("Raspberry c");
    std::memcpy(buf_.data() + 3, str.c_str(), 3 * 4);
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 6, &c_), 6);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.gps_ascii", true},
                           {"gps_ascii.number_of_words", static_cast<uint32_t>(0x00000003)},
                           {"gps_ascii.ascii", reinterpret_cast<const char*>(buf_.data() + 3)}});
    ASSERT_STREQ(str.c_str(), reinterpret_cast<const char*>(buf_.data() + 3));
}

TEST_F(ReadIfContextTest, GpsAsciiReserved) {
    buf_[0] = 0x00000200;
    buf_[1] = 0xFF000000;
    buf_[2] = 0x00000000;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 3, &c_), 3);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.gps_ascii", true}});
}

TEST_F(ReadIfContextTest, ContextAssocationListsEmpty) {
    buf_[0] = 0x00000100;
    buf_[1] = 0x00000000;
    buf_[2] = 0x00000000;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 3, &c_), 3);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.context_association_lists", true}});
}

TEST_F(ReadIfContextTest, ContextAssocationListsSource) {
    buf_[0] = 0x00000100;
    buf_[1] = 0xFFFF0000;
    buf_[2] = 0x00000000;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 514, &c_), 514);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.context_association_lists", true},
                           {"context_association_lists.source_list_size", static_cast<uint16_t>(0x01FF)},
                           {"context_association_lists.source_context_association_list",
                            const_cast<const uint32_t*>(buf_.data() + 3)}});
}

TEST_F(ReadIfContextTest, ContextAssocationListsSystem) {
    buf_[0] = 0x00000100;
    buf_[1] = 0x0000FFFF;
    buf_[2] = 0x00000000;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 514, &c_), 514);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.context_association_lists", true},
                           {"context_association_lists.system_list_size", static_cast<uint16_t>(0x01FF)},
                           {"context_association_lists.system_context_association_list",
                            const_cast<const uint32_t*>(buf_.data() + 3)}});
}

TEST_F(ReadIfContextTest, ContextAssocationListsVectorComponent) {
    buf_[0] = 0x00000100;
    buf_[1] = 0x00000000;
    buf_[2] = 0xFFFF0000;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 65538, &c_), 65538);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.context_association_lists", true},
                           {"context_association_lists.vector_component_list_size", static_cast<uint16_t>(0xFFFF)},
                           {"context_association_lists.vector_component_context_association_list",
                            const_cast<const uint32_t*>(buf_.data() + 3)}});
}

TEST_F(ReadIfContextTest, ContextAssocationListsHasChannelTag) {
    buf_[0] = 0x00000100;
    buf_[1] = 0x00000000;
    buf_[2] = 0x00008000;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 3, &c_), 3);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.context_association_lists", true},
                           {"context_association_lists.has.asynchronous_channel_tag_list", true}});
}

TEST_F(ReadIfContextTest, ContextAssocationListsContextAssociation) {
    buf_[0] = 0x00000100;
    buf_[1] = 0x00000000;
    buf_[2] = 0x00007FFF;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 32770, &c_), 32770);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.context_association_lists", true},
                           {"context_association_lists.asynchronous_channel_list_size", static_cast<uint16_t>(0x7FFF)},
                           {"context_association_lists.asynchronous_channel_context_association_list",
                            const_cast<const uint32_t*>(buf_.data() + 3)}});
}

TEST_F(ReadIfContextTest, ContextAssocationListsContextChannelTag) {
    buf_[0] = 0x00000100;
    buf_[1] = 0x00000000;
    buf_[2] = 0x00008003;
    ASSERT_EQ(vrt_read_if_context(buf_.data(), 9, &c_), 9);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"has.context_association_lists", true},
                           {"context_association_lists.has.asynchronous_channel_tag_list", true},
                           {"context_association_lists.asynchronous_channel_list_size", static_cast<uint16_t>(0x0003)},
                           {"context_association_lists.asynchronous_channel_context_association_list",
                            const_cast<const uint32_t*>(buf_.data() + 3)},
                           {"context_association_lists.asynchronous_channel_tag_list",
                            const_cast<const uint32_t*>(buf_.data() + 6)}});
}

TEST_F(ReadIfContextTest, EveryOther1) {
    buf_[0]  = 0xAAAAAA00; /* Context indicator field */
    buf_[1]  = 0x00000000;
    buf_[2]  = 0x00100000; /* Bandwidth */
    buf_[3]  = 0x00000000;
    buf_[4]  = 0x00100000; /* RF reference frequency */
    buf_[5]  = 0x00000000;
    buf_[6]  = 0x00100000; /* IF band offset */
    buf_[7]  = 0x00800080; /* Gain */
    buf_[8]  = 0x00000000;
    buf_[9]  = 0x00100000; /* Sample rate */
    buf_[10] = 0xFFFFFFFF; /* Timestamp calibration time */
    buf_[11] = 0x00FFFFFF;
    buf_[12] = 0x0000FFFF; /* Device identifier */
    buf_[13] = 0xD6FF0FFF;
    buf_[14] = 0xFFFFFFFF; /* Data packet payload format */
    buf_[15] = 0x0FFFFFFF;
    buf_[16] = 0xABABABAB;
    buf_[17] = 0xABABABAB;
    buf_[18] = 0xABABABAB;
    buf_[19] = 0x00400000;
    buf_[20] = 0x00400000;
    buf_[21] = 0x00000020;
    buf_[22] = 0x00010000;
    buf_[23] = 0x00400000;
    buf_[24] = 0x00400000;
    buf_[25] = 0x00400000; /* Formatted INS geolocation */
    buf_[26] = 0x0FFFFFFF;
    buf_[27] = 0xABABABAB;
    buf_[28] = 0xABABABAB;
    buf_[29] = 0xABABABAB;
    buf_[30] = 0x00000020;
    buf_[31] = 0x00000020;
    buf_[32] = 0x00000020;
    buf_[33] = 0x00400000;
    buf_[34] = 0x00400000;
    buf_[35] = 0x00400000;
    buf_[36] = 0x00010000;
    buf_[37] = 0x00010000;
    buf_[38] = 0x00010000; /* Relative ephemeris */
    buf_[39] = 0x00FFFFFF;
    buf_[40] = 0x00000003;
    buf_[41] = 0x5A5A5A5A;
    buf_[42] = 0x5A5A5A5A;
    buf_[43] = 0x5A5A5A5A; /* GPS ASCII */

    ASSERT_EQ(vrt_read_if_context(buf_.data(), 44, &c_), 44);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(c_, {{"context_field_change_indicator", true},
                           {"has.bandwidth", true},
                           {"has.rf_reference_frequency", true},
                           {"has.if_band_offset", true},
                           {"has.gain", true},
                           {"has.sample_rate", true},
                           {"has.timestamp_calibration_time", true},
                           {"has.device_identifier", true},
                           {"has.data_packet_payload_format", true},
                           {"has.formatted_ins_geolocation", true},
                           {"has.relative_ephemeris", true},
                           {"has.gps_ascii", true},

                           {"bandwidth", 1.0},
                           {"rf_reference_frequency", 1.0},
                           {"if_band_offset", 1.0},
                           {"gain.stage1", 1.0F},
                           {"gain.stage2", 1.0F},
                           {"sample_rate", 1.0},
                           {"timestamp_calibration_time", 0xFFFFFFFF},

                           {"device_identifier.oui", static_cast<uint32_t>(0x00FFFFFF)},
                           {"device_identifier.device_code", static_cast<uint16_t>(0xFFFF)},

                           {"data_packet_payload_format.packing_method", true},
                           {"data_packet_payload_format.real_or_complex", VRT_ROC_COMPLEX_POLAR},
                           {"data_packet_payload_format.data_item_format", VRT_DIF_UNSIGNED_VRT_6_BIT_EXPONENT},
                           {"data_packet_payload_format.sample_component_repeat", true},
                           {"data_packet_payload_format.event_tag_size", static_cast<uint8_t>(0x07)},
                           {"data_packet_payload_format.channel_tag_size", static_cast<uint8_t>(0x0F)},
                           {"data_packet_payload_format.item_packing_field_size", static_cast<uint8_t>(0x3F)},
                           {"data_packet_payload_format.data_item_size", static_cast<uint8_t>(0x3F)},
                           {"data_packet_payload_format.repeat_count", static_cast<uint16_t>(0xFFFF)},
                           {"data_packet_payload_format.vector_size", static_cast<uint16_t>(0xFFFF)},

                           {"formatted_ins_geolocation.tsi", VRT_TSI_OTHER},
                           {"formatted_ins_geolocation.tsf", VRT_TSF_FREE_RUNNING_COUNT},
                           {"formatted_ins_geolocation.oui", static_cast<uint32_t>(0x00FFFFFF)},
                           {"formatted_ins_geolocation.integer_second_timestamp", 0xABABABAB},
                           {"formatted_ins_geolocation.fractional_second_timestamp", 0xABABABABABABABAB},
                           {"formatted_ins_geolocation.latitude", 1.0},
                           {"formatted_ins_geolocation.longitude", 1.0},
                           {"formatted_ins_geolocation.altitude", 1.0},
                           {"formatted_ins_geolocation.speed_over_ground", 1.0},
                           {"formatted_ins_geolocation.heading_angle", 1.0},
                           {"formatted_ins_geolocation.track_angle", 1.0},
                           {"formatted_ins_geolocation.magnetic_variation", 1.0},

                           {"relative_ephemeris.tsi", VRT_TSI_OTHER},
                           {"relative_ephemeris.tsf", VRT_TSF_FREE_RUNNING_COUNT},
                           {"relative_ephemeris.oui", static_cast<uint32_t>(0x00FFFFFF)},
                           {"relative_ephemeris.integer_second_timestamp", 0xABABABAB},
                           {"relative_ephemeris.fractional_second_timestamp", 0xABABABABABABABAB},
                           {"relative_ephemeris.position_x", 1.0},
                           {"relative_ephemeris.position_y", 1.0},
                           {"relative_ephemeris.position_z", 1.0},
                           {"relative_ephemeris.attitude_alpha", 1.0},
                           {"relative_ephemeris.attitude_beta", 1.0},
                           {"relative_ephemeris.attitude_phi", 1.0},
                           {"relative_ephemeris.velocity_dx", 1.0},
                           {"relative_ephemeris.velocity_dy", 1.0},
                           {"relative_ephemeris.velocity_dz", 1.0},

                           {"gps_ascii.oui", static_cast<uint32_t>(0x00FFFFFF)},
                           {"gps_ascii.number_of_words", static_cast<uint32_t>(0x00000003)},
                           {"gps_ascii.ascii", reinterpret_cast<const char*>(buf_.data() + 41)}});
}

TEST_F(ReadIfContextTest, EveryOther2) {
    buf_[0]  = 0x55555500; /* Context indicator field */
    buf_[1]  = 0xFFFFFFFF; /* Reference point identifier */
    buf_[2]  = 0x00000000;
    buf_[3]  = 0x00100000; /* IF reference frequency */
    buf_[4]  = 0x00000000;
    buf_[5]  = 0x00100000; /* RF reference frequency offset */
    buf_[6]  = 0x00000080; /* Reference level */
    buf_[7]  = 0xFFFFFFFF; /* Over range count */
    buf_[8]  = 0xFFFFFFFF;
    buf_[9]  = 0xFFFFFFFF; /* Timestamp adjustment */
    buf_[10] = 0x00000040; /* Temperature */
    buf_[11] = 0xFF0FF0FF; /* State and event indicator */
    buf_[12] = 0x0FFFFFFF;
    buf_[13] = 0xABABABAB;
    buf_[14] = 0xABABABAB;
    buf_[15] = 0xABABABAB;
    buf_[16] = 0x00400000;
    buf_[17] = 0x00400000;
    buf_[18] = 0x00000020;
    buf_[19] = 0x00010000;
    buf_[20] = 0x00400000;
    buf_[21] = 0x00400000;
    buf_[22] = 0x00400000; /* Formatted GPS geolocation */
    buf_[23] = 0x0FFFFFFF;
    buf_[24] = 0xABABABAB;
    buf_[25] = 0xABABABAB;
    buf_[26] = 0xABABABAB;
    buf_[27] = 0x00000020;
    buf_[28] = 0x00000020;
    buf_[29] = 0x00000020;
    buf_[30] = 0x00400000;
    buf_[31] = 0x00400000;
    buf_[32] = 0x00400000;
    buf_[33] = 0x00010000;
    buf_[34] = 0x00010000;
    buf_[35] = 0x00010000; /* ECEF ephemeris */
    buf_[36] = 0xFFFFFFFF; /* Ephemeris reference identifier */
    buf_[37] = 0x00010001;
    buf_[38] = 0x00018001;
    buf_[39] = 0x5A5A5A5A;
    buf_[40] = 0x5A5A5A5A;
    buf_[41] = 0x5A5A5A5A;
    buf_[42] = 0x5A5A5A5A;
    buf_[43] = 0x5A5A5A5A; /* Context association lists */

    ASSERT_EQ(vrt_read_if_context(buf_.data(), 44, &c_), 44);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(
        c_,
        {{"has.reference_point_identifier", true},
         {"has.if_reference_frequency", true},
         {"has.rf_reference_frequency_offset", true},
         {"has.reference_level", true},
         {"has.over_range_count", true},
         {"has.timestamp_adjustment", true},
         {"has.temperature", true},
         {"has.state_and_event_indicators", true},
         {"has.formatted_gps_geolocation", true},
         {"has.ecef_ephemeris", true},
         {"has.ephemeris_reference_identifier", true},
         {"has.context_association_lists", true},

         {"reference_point_identifier", 0xFFFFFFFF},
         {"if_reference_frequency", 1.0},
         {"rf_reference_frequency_offset", 1.0},
         {"reference_level", 1.0F},
         {"over_range_count", 0xFFFFFFFF},
         {"timestamp_adjustment", 0xFFFFFFFFFFFFFFFF},
         {"temperature", 1.0F},

         {"state_and_event_indicators.has.calibrated_time", true},
         {"state_and_event_indicators.has.valid_data", true},
         {"state_and_event_indicators.has.reference_lock", true},
         {"state_and_event_indicators.has.agc_or_mgc", true},
         {"state_and_event_indicators.has.detected_signal", true},
         {"state_and_event_indicators.has.spectral_inversion", true},
         {"state_and_event_indicators.has.over_range", true},
         {"state_and_event_indicators.has.sample_loss", true},
         {"state_and_event_indicators.calibrated_time", true},
         {"state_and_event_indicators.valid_data", true},
         {"state_and_event_indicators.reference_lock", true},
         {"state_and_event_indicators.agc_or_mgc", true},
         {"state_and_event_indicators.detected_signal", true},
         {"state_and_event_indicators.spectral_inversion", true},
         {"state_and_event_indicators.over_range", true},
         {"state_and_event_indicators.sample_loss", true},
         {"state_and_event_indicators.user_defined7", true},
         {"state_and_event_indicators.user_defined6", true},
         {"state_and_event_indicators.user_defined5", true},
         {"state_and_event_indicators.user_defined4", true},
         {"state_and_event_indicators.user_defined3", true},
         {"state_and_event_indicators.user_defined2", true},
         {"state_and_event_indicators.user_defined1", true},
         {"state_and_event_indicators.user_defined0", true},

         {"formatted_gps_geolocation.tsi", VRT_TSI_OTHER},
         {"formatted_gps_geolocation.tsf", VRT_TSF_FREE_RUNNING_COUNT},
         {"formatted_gps_geolocation.oui", static_cast<uint32_t>(0x00FFFFFF)},
         {"formatted_gps_geolocation.integer_second_timestamp", 0xABABABAB},
         {"formatted_gps_geolocation.fractional_second_timestamp", 0xABABABABABABABAB},
         {"formatted_gps_geolocation.latitude", 1.0},
         {"formatted_gps_geolocation.longitude", 1.0},
         {"formatted_gps_geolocation.altitude", 1.0},
         {"formatted_gps_geolocation.speed_over_ground", 1.0},
         {"formatted_gps_geolocation.heading_angle", 1.0},
         {"formatted_gps_geolocation.track_angle", 1.0},
         {"formatted_gps_geolocation.magnetic_variation", 1.0},

         {"ecef_ephemeris.tsi", VRT_TSI_OTHER},
         {"ecef_ephemeris.tsf", VRT_TSF_FREE_RUNNING_COUNT},
         {"ecef_ephemeris.oui", static_cast<uint32_t>(0x00FFFFFF)},
         {"ecef_ephemeris.integer_second_timestamp", 0xABABABAB},
         {"ecef_ephemeris.fractional_second_timestamp", 0xABABABABABABABAB},
         {"ecef_ephemeris.position_x", 1.0},
         {"ecef_ephemeris.position_y", 1.0},
         {"ecef_ephemeris.position_z", 1.0},
         {"ecef_ephemeris.attitude_alpha", 1.0},
         {"ecef_ephemeris.attitude_beta", 1.0},
         {"ecef_ephemeris.attitude_phi", 1.0},
         {"ecef_ephemeris.velocity_dx", 1.0},
         {"ecef_ephemeris.velocity_dy", 1.0},
         {"ecef_ephemeris.velocity_dz", 1.0},

         {"ephemeris_reference_identifier", 0xFFFFFFFF},

         {"context_association_lists.has.asynchronous_channel_tag_list", true},
         {"context_association_lists.source_list_size", static_cast<uint16_t>(0x0001)},
         {"context_association_lists.system_list_size", static_cast<uint16_t>(0x0001)},
         {"context_association_lists.vector_component_list_size", static_cast<uint16_t>(0x0001)},
         {"context_association_lists.asynchronous_channel_list_size", static_cast<uint16_t>(0x0001)},
         {"context_association_lists.source_context_association_list", const_cast<const uint32_t*>(buf_.data() + 39)},
         {"context_association_lists.system_context_association_list", const_cast<const uint32_t*>(buf_.data() + 40)},
         {"context_association_lists.vector_component_context_association_list",
          const_cast<const uint32_t*>(buf_.data() + 41)},
         {"context_association_lists.asynchronous_channel_context_association_list",
          const_cast<const uint32_t*>(buf_.data() + 42)},
         {"context_association_lists.asynchronous_channel_tag_list", const_cast<const uint32_t*>(buf_.data() + 43)}});
}

TEST_F(ReadIfContextTest, All) {
    buf_[0]  = 0xFFFFFF00; /* Context indicator field */
    buf_[1]  = 0xFFFFFFFF; /* Reference point identifier */
    buf_[2]  = 0x00000000;
    buf_[3]  = 0x00100000; /* Bandwidth */
    buf_[4]  = 0x00000000;
    buf_[5]  = 0x00100000; /* IF reference frequency */
    buf_[6]  = 0x00000000;
    buf_[7]  = 0x00100000; /* RF reference frequency */
    buf_[8]  = 0x00000000;
    buf_[9]  = 0x00100000; /* RF reference frequency offset */
    buf_[10] = 0x00000000;
    buf_[11] = 0x00100000; /* IF band offset */
    buf_[12] = 0x00000080; /* Reference level */
    buf_[13] = 0x00800080; /* Gain */
    buf_[14] = 0xFFFFFFFF; /* Over range count */
    buf_[15] = 0x00000000;
    buf_[16] = 0x00100000; /* Sample rate */
    buf_[17] = 0xFFFFFFFF;
    buf_[18] = 0xFFFFFFFF; /* Timestamp adjustment */
    buf_[19] = 0xFFFFFFFF; /* Timestamp calibration time */
    buf_[20] = 0x00000040; /* Temperature */
    buf_[21] = 0x00FFFFFF;
    buf_[22] = 0x0000FFFF; /* Device identifier */
    buf_[23] = 0xFF0FF0FF; /* State and event indicator */
    buf_[24] = 0xD6FF0FFF;
    buf_[25] = 0xFFFFFFFF; /* Data packet payload format */
    buf_[26] = 0x0FFFFFFF;
    buf_[27] = 0xABABABAB;
    buf_[28] = 0xABABABAB;
    buf_[29] = 0xABABABAB;
    buf_[30] = 0x00400000;
    buf_[31] = 0x00400000;
    buf_[32] = 0x00000020;
    buf_[33] = 0x00010000;
    buf_[34] = 0x00400000;
    buf_[35] = 0x00400000;
    buf_[36] = 0x00400000; /* Formatted GPS geolocation */
    buf_[37] = 0x0FFFFFFF;
    buf_[38] = 0xABABABAB;
    buf_[39] = 0xABABABAB;
    buf_[40] = 0xABABABAB;
    buf_[41] = 0x00400000;
    buf_[42] = 0x00400000;
    buf_[43] = 0x00000020;
    buf_[44] = 0x00010000;
    buf_[45] = 0x00400000;
    buf_[46] = 0x00400000;
    buf_[47] = 0x00400000; /* Formatted INS geolocation */
    buf_[48] = 0x0FFFFFFF;
    buf_[49] = 0xABABABAB;
    buf_[50] = 0xABABABAB;
    buf_[51] = 0xABABABAB;
    buf_[52] = 0x00000020;
    buf_[53] = 0x00000020;
    buf_[54] = 0x00000020;
    buf_[55] = 0x00400000;
    buf_[56] = 0x00400000;
    buf_[57] = 0x00400000;
    buf_[58] = 0x00010000;
    buf_[59] = 0x00010000;
    buf_[60] = 0x00010000; /* ECEF ephemeris */
    buf_[61] = 0x0FFFFFFF;
    buf_[62] = 0xABABABAB;
    buf_[63] = 0xABABABAB;
    buf_[64] = 0xABABABAB;
    buf_[65] = 0x00000020;
    buf_[66] = 0x00000020;
    buf_[67] = 0x00000020;
    buf_[68] = 0x00400000;
    buf_[69] = 0x00400000;
    buf_[70] = 0x00400000;
    buf_[71] = 0x00010000;
    buf_[72] = 0x00010000;
    buf_[73] = 0x00010000; /* Relative ephemeris */
    buf_[74] = 0xFFFFFFFF; /* Ephemeris reference identifier */
    buf_[75] = 0x00FFFFFF;
    buf_[76] = 0x00000003;
    buf_[77] = 0x5A5A5A5A;
    buf_[78] = 0x5A5A5A5A;
    buf_[79] = 0x5A5A5A5A; /* GPS ASCII */
    buf_[80] = 0x00010001;
    buf_[81] = 0x00018001;
    buf_[82] = 0x5A5A5A5A;
    buf_[83] = 0x5A5A5A5A;
    buf_[84] = 0x5A5A5A5A;
    buf_[85] = 0x5A5A5A5A;
    buf_[86] = 0x5A5A5A5A; /* Context association lists */

    ASSERT_EQ(vrt_read_if_context(buf_.data(), 87, &c_), 87);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_if_context(
        c_,
        {{"context_field_change_indicator", true},
         {"has.reference_point_identifier", true},
         {"has.bandwidth", true},
         {"has.if_reference_frequency", true},
         {"has.rf_reference_frequency", true},
         {"has.rf_reference_frequency_offset", true},
         {"has.if_band_offset", true},
         {"has.reference_level", true},
         {"has.gain", true},
         {"has.over_range_count", true},
         {"has.sample_rate", true},
         {"has.timestamp_adjustment", true},
         {"has.timestamp_calibration_time", true},
         {"has.temperature", true},
         {"has.device_identifier", true},
         {"has.state_and_event_indicators", true},
         {"has.data_packet_payload_format", true},
         {"has.formatted_gps_geolocation", true},
         {"has.formatted_ins_geolocation", true},
         {"has.ecef_ephemeris", true},
         {"has.relative_ephemeris", true},
         {"has.ephemeris_reference_identifier", true},
         {"has.gps_ascii", true},
         {"has.context_association_lists", true},

         {"reference_point_identifier", 0xFFFFFFFF},
         {"bandwidth", 1.0},
         {"if_reference_frequency", 1.0},
         {"rf_reference_frequency", 1.0},
         {"rf_reference_frequency_offset", 1.0},
         {"if_band_offset", 1.0},
         {"reference_level", 1.0F},
         {"gain.stage1", 1.0F},
         {"gain.stage2", 1.0F},
         {"over_range_count", 0xFFFFFFFF},
         {"sample_rate", 1.0},
         {"timestamp_adjustment", 0xFFFFFFFFFFFFFFFF},
         {"timestamp_calibration_time", 0xFFFFFFFF},
         {"temperature", 1.0F},

         {"device_identifier.oui", static_cast<uint32_t>(0x00FFFFFF)},
         {"device_identifier.device_code", static_cast<uint16_t>(0xFFFF)},

         {"state_and_event_indicators.has.calibrated_time", true},
         {"state_and_event_indicators.has.valid_data", true},
         {"state_and_event_indicators.has.reference_lock", true},
         {"state_and_event_indicators.has.agc_or_mgc", true},
         {"state_and_event_indicators.has.detected_signal", true},
         {"state_and_event_indicators.has.spectral_inversion", true},
         {"state_and_event_indicators.has.over_range", true},
         {"state_and_event_indicators.has.sample_loss", true},
         {"state_and_event_indicators.calibrated_time", true},
         {"state_and_event_indicators.valid_data", true},
         {"state_and_event_indicators.reference_lock", true},
         {"state_and_event_indicators.agc_or_mgc", true},
         {"state_and_event_indicators.detected_signal", true},
         {"state_and_event_indicators.spectral_inversion", true},
         {"state_and_event_indicators.over_range", true},
         {"state_and_event_indicators.sample_loss", true},
         {"state_and_event_indicators.user_defined7", true},
         {"state_and_event_indicators.user_defined6", true},
         {"state_and_event_indicators.user_defined5", true},
         {"state_and_event_indicators.user_defined4", true},
         {"state_and_event_indicators.user_defined3", true},
         {"state_and_event_indicators.user_defined2", true},
         {"state_and_event_indicators.user_defined1", true},
         {"state_and_event_indicators.user_defined0", true},

         {"data_packet_payload_format.packing_method", true},
         {"data_packet_payload_format.real_or_complex", VRT_ROC_COMPLEX_POLAR},
         {"data_packet_payload_format.data_item_format", VRT_DIF_UNSIGNED_VRT_6_BIT_EXPONENT},
         {"data_packet_payload_format.sample_component_repeat", true},
         {"data_packet_payload_format.event_tag_size", static_cast<uint8_t>(0x07)},
         {"data_packet_payload_format.channel_tag_size", static_cast<uint8_t>(0x0F)},
         {"data_packet_payload_format.item_packing_field_size", static_cast<uint8_t>(0x3F)},
         {"data_packet_payload_format.data_item_size", static_cast<uint8_t>(0x3F)},
         {"data_packet_payload_format.repeat_count", static_cast<uint16_t>(0xFFFF)},
         {"data_packet_payload_format.vector_size", static_cast<uint16_t>(0xFFFF)},

         {"formatted_gps_geolocation.tsi", VRT_TSI_OTHER},
         {"formatted_gps_geolocation.tsf", VRT_TSF_FREE_RUNNING_COUNT},
         {"formatted_gps_geolocation.oui", static_cast<uint32_t>(0x00FFFFFF)},
         {"formatted_gps_geolocation.integer_second_timestamp", 0xABABABAB},
         {"formatted_gps_geolocation.fractional_second_timestamp", 0xABABABABABABABAB},
         {"formatted_gps_geolocation.latitude", 1.0},
         {"formatted_gps_geolocation.longitude", 1.0},
         {"formatted_gps_geolocation.altitude", 1.0},
         {"formatted_gps_geolocation.speed_over_ground", 1.0},
         {"formatted_gps_geolocation.heading_angle", 1.0},
         {"formatted_gps_geolocation.track_angle", 1.0},
         {"formatted_gps_geolocation.magnetic_variation", 1.0},

         {"formatted_ins_geolocation.tsi", VRT_TSI_OTHER},
         {"formatted_ins_geolocation.tsf", VRT_TSF_FREE_RUNNING_COUNT},
         {"formatted_ins_geolocation.oui", static_cast<uint32_t>(0x00FFFFFF)},
         {"formatted_ins_geolocation.integer_second_timestamp", 0xABABABAB},
         {"formatted_ins_geolocation.fractional_second_timestamp", 0xABABABABABABABAB},
         {"formatted_ins_geolocation.latitude", 1.0},
         {"formatted_ins_geolocation.longitude", 1.0},
         {"formatted_ins_geolocation.altitude", 1.0},
         {"formatted_ins_geolocation.speed_over_ground", 1.0},
         {"formatted_ins_geolocation.heading_angle", 1.0},
         {"formatted_ins_geolocation.track_angle", 1.0},
         {"formatted_ins_geolocation.magnetic_variation", 1.0},

         {"ecef_ephemeris.tsi", VRT_TSI_OTHER},
         {"ecef_ephemeris.tsf", VRT_TSF_FREE_RUNNING_COUNT},
         {"ecef_ephemeris.oui", static_cast<uint32_t>(0x00FFFFFF)},
         {"ecef_ephemeris.integer_second_timestamp", 0xABABABAB},
         {"ecef_ephemeris.fractional_second_timestamp", 0xABABABABABABABAB},
         {"ecef_ephemeris.position_x", 1.0},
         {"ecef_ephemeris.position_y", 1.0},
         {"ecef_ephemeris.position_z", 1.0},
         {"ecef_ephemeris.attitude_alpha", 1.0},
         {"ecef_ephemeris.attitude_beta", 1.0},
         {"ecef_ephemeris.attitude_phi", 1.0},
         {"ecef_ephemeris.velocity_dx", 1.0},
         {"ecef_ephemeris.velocity_dy", 1.0},
         {"ecef_ephemeris.velocity_dz", 1.0},

         {"relative_ephemeris.tsi", VRT_TSI_OTHER},
         {"relative_ephemeris.tsf", VRT_TSF_FREE_RUNNING_COUNT},
         {"relative_ephemeris.oui", static_cast<uint32_t>(0x00FFFFFF)},
         {"relative_ephemeris.integer_second_timestamp", 0xABABABAB},
         {"relative_ephemeris.fractional_second_timestamp", 0xABABABABABABABAB},
         {"relative_ephemeris.position_x", 1.0},
         {"relative_ephemeris.position_y", 1.0},
         {"relative_ephemeris.position_z", 1.0},
         {"relative_ephemeris.attitude_alpha", 1.0},
         {"relative_ephemeris.attitude_beta", 1.0},
         {"relative_ephemeris.attitude_phi", 1.0},
         {"relative_ephemeris.velocity_dx", 1.0},
         {"relative_ephemeris.velocity_dy", 1.0},
         {"relative_ephemeris.velocity_dz", 1.0},

         {"ephemeris_reference_identifier", 0xFFFFFFFF},

         {"gps_ascii.oui", static_cast<uint32_t>(0x00FFFFFF)},
         {"gps_ascii.number_of_words", static_cast<uint32_t>(0x00000003)},
         {"gps_ascii.ascii", reinterpret_cast<const char*>(buf_.data() + 77)},

         {"context_association_lists.has.asynchronous_channel_tag_list", true},
         {"context_association_lists.source_list_size", static_cast<uint16_t>(0x0001)},
         {"context_association_lists.system_list_size", static_cast<uint16_t>(0x0001)},
         {"context_association_lists.vector_component_list_size", static_cast<uint16_t>(0x0001)},
         {"context_association_lists.asynchronous_channel_list_size", static_cast<uint16_t>(0x0001)},
         {"context_association_lists.source_context_association_list", const_cast<const uint32_t*>(buf_.data() + 82)},
         {"context_association_lists.system_context_association_list", const_cast<const uint32_t*>(buf_.data() + 83)},
         {"context_association_lists.vector_component_context_association_list",
          const_cast<const uint32_t*>(buf_.data() + 84)},
         {"context_association_lists.asynchronous_channel_context_association_list",
          const_cast<const uint32_t*>(buf_.data() + 85)},
         {"context_association_lists.asynchronous_channel_tag_list", const_cast<const uint32_t*>(buf_.data() + 86)}});
}
