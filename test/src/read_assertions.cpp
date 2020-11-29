#include "read_assertions.h"

#include <cstdint>
#include <sstream>
#include <stdexcept>
#include <typeinfo>

#include <gtest/gtest.h>
#include <vrt/vrt_types.h>

#include "../../src/vrt_fixed_point.h"
#include "hex.h"

/**
 * Test helper function. Get value as type from map if any, otherwise return default value. If not possible to cast to
 * type, throw an exception.
 *
 * \tparam T       Type of value.
 * \param values   Lists non-default values as string->value. May remove an object if there's a hit.
 * \param name     Field name.
 * \param def      Default value.
 *
 * \return Value from map if there is a hit, or default if no hit.
 *
 * \throw std::runtime_error If key is present with missing value, or if the value could not be cast to type T.
 *
 * \note It is unfortunately not possible to use googletest macros inside templated code.
 */
template <typename T>
static T get_val(std::map<std::string, std::any>* values, const std::string& name, T def) {
    T val2 = def;

    auto it = values->find(name);
    if (it != values->end()) {
        if (!it->second.has_value()) {
            std::stringstream ss;
            ss << "Field '" << name << "' with assumed type '" << typeid(T).name() << "' is missing its value'";
            throw std::runtime_error(ss.str());
        }
        try {
            val2 = std::any_cast<T>(it->second);
        } catch (const std::bad_cast&) {
            std::stringstream     ss;
            const std::type_info& info = it->second.type();
            ss << "Failed to cast field '" << name << "' from type '" << typeid(info).name() << "' to '"
               << typeid(T).name() << "'";
            throw std::runtime_error(ss.str());
        }

        /* Remove so it is possible to test for unused keys later */
        values->erase(it);
    }

    return val2;
}

/**
 * Check for remaining keys in map, and fail if so.
 *
 * \param values Map to check for remaining elements.
 */
static void check_remaining(const std::map<std::string, std::any>& values) {
    if (!values.empty()) {
        std::stringstream ss;
        if (values.size() == 1) {
            ss << "Unknown map key '" << values.begin()->first << "'";
        } else if (values.size() > 1) {
            ss << "Unknown map keys:\n";
            for (const auto& kv : values) {
                ss << kv.first << "\n";
            }
        }
        FAIL() << ss.str();
    }
}

/**
 * Assert header values.
 *
 * \param h      Header.
 * \param values Lists non-default values, i.e. values that differ from the vrt_init_header() state, as field name ->
 *               field value.
 */
void assert_header(const vrt_header& h, const std::map<std::string, std::any>& values) {
    std::map<std::string, std::any> val_cp(values);
    ASSERT_EQ(Hex(h.packet_type),
              Hex(get_val<vrt_packet_type>(&val_cp, "packet_type", VRT_PT_IF_DATA_WITHOUT_STREAM_ID)));
    ASSERT_EQ(h.has.class_id, get_val<bool>(&val_cp, "has.class_id", false));
    ASSERT_EQ(h.has.trailer, get_val<bool>(&val_cp, "has.trailer", false));
    ASSERT_EQ(h.tsm, get_val<vrt_tsm>(&val_cp, "tsm", VRT_TSM_FINE));
    ASSERT_EQ(Hex(h.tsi), Hex(get_val<vrt_tsi>(&val_cp, "tsi", VRT_TSI_NONE)));
    ASSERT_EQ(Hex(h.tsf), Hex(get_val<vrt_tsf>(&val_cp, "tsf", VRT_TSF_NONE)));
    ASSERT_EQ(Hex(h.packet_count), Hex(get_val<uint8_t>(&val_cp, "packet_count", 0)));
    ASSERT_EQ(Hex(h.packet_size), Hex(get_val<uint16_t>(&val_cp, "packet_size", 0)));

    check_remaining(val_cp);
}

/**
 * Assert fields section values.
 *
 * \param f      Fields.
 * \param values Lists non-default values, i.e. values that differ from the vrt_init_fields() state, as field name ->
 *               field value.
 */
void assert_fields(const vrt_fields& f, const std::map<std::string, std::any>& values) {
    std::map<std::string, std::any> val_cp(values);
    ASSERT_EQ(Hex(f.stream_id), Hex(get_val<uint32_t>(&val_cp, "stream_id", 0)));
    ASSERT_EQ(Hex(f.class_id.oui), Hex(get_val<uint32_t>(&val_cp, "class_id.oui", 0)));
    ASSERT_EQ(Hex(f.class_id.information_class_code),
              Hex(get_val<uint16_t>(&val_cp, "class_id.information_class_code", 0)));
    ASSERT_EQ(Hex(f.class_id.packet_class_code), Hex(get_val<uint16_t>(&val_cp, "class_id.packet_class_code", 0)));
    ASSERT_EQ(Hex(f.integer_seconds_timestamp), Hex(get_val<uint32_t>(&val_cp, "integer_seconds_timestamp", 0)));
    ASSERT_EQ(Hex(f.fractional_seconds_timestamp), Hex(get_val<uint64_t>(&val_cp, "fractional_seconds_timestamp", 0)));

    check_remaining(val_cp);
}

/**
 * Assert trailer values.
 *
 * \param t      Trailer.
 * \param values Lists non-default values, i.e. values that differ from the vrt_init_trailer() state, as field name ->
 *               field value.
 */
void assert_trailer(const vrt_trailer& t, const std::map<std::string, std::any>& values) {
    std::map<std::string, std::any> val_cp(values);
    ASSERT_EQ(t.has.calibrated_time, get_val<bool>(&val_cp, "has.calibrated_time", false));
    ASSERT_EQ(t.has.valid_data, get_val<bool>(&val_cp, "has.valid_data", false));
    ASSERT_EQ(t.has.reference_lock, get_val<bool>(&val_cp, "has.reference_lock", false));
    ASSERT_EQ(t.has.agc_or_mgc, get_val<bool>(&val_cp, "has.agc_or_mgc", false));
    ASSERT_EQ(t.has.detected_signal, get_val<bool>(&val_cp, "has.detected_signal", false));
    ASSERT_EQ(t.has.spectral_inversion, get_val<bool>(&val_cp, "has.spectral_inversion", false));
    ASSERT_EQ(t.has.over_range, get_val<bool>(&val_cp, "has.over_range", false));
    ASSERT_EQ(t.has.sample_loss, get_val<bool>(&val_cp, "has.sample_loss", false));
    ASSERT_EQ(t.has.user_defined11, get_val<bool>(&val_cp, "has.user_defined11", false));
    ASSERT_EQ(t.has.user_defined10, get_val<bool>(&val_cp, "has.user_defined10", false));
    ASSERT_EQ(t.has.user_defined9, get_val<bool>(&val_cp, "has.user_defined9", false));
    ASSERT_EQ(t.has.user_defined8, get_val<bool>(&val_cp, "has.user_defined8", false));
    ASSERT_EQ(t.calibrated_time, get_val<bool>(&val_cp, "calibrated_time", false));
    ASSERT_EQ(t.valid_data, get_val<bool>(&val_cp, "valid_data", false));
    ASSERT_EQ(t.reference_lock, get_val<bool>(&val_cp, "reference_lock", false));
    ASSERT_EQ(t.agc_or_mgc, get_val<vrt_agc_or_mgc>(&val_cp, "agc_or_mgc", VRT_AOM_MGC));
    ASSERT_EQ(t.detected_signal, get_val<bool>(&val_cp, "detected_signal", false));
    ASSERT_EQ(t.spectral_inversion, get_val<bool>(&val_cp, "spectral_inversion", false));
    ASSERT_EQ(t.over_range, get_val<bool>(&val_cp, "over_range", false));
    ASSERT_EQ(t.sample_loss, get_val<bool>(&val_cp, "sample_loss", false));
    ASSERT_EQ(t.user_defined11, get_val<bool>(&val_cp, "user_defined11", false));
    ASSERT_EQ(t.user_defined10, get_val<bool>(&val_cp, "user_defined10", false));
    ASSERT_EQ(t.user_defined9, get_val<bool>(&val_cp, "user_defined9", false));
    ASSERT_EQ(t.user_defined8, get_val<bool>(&val_cp, "user_defined8", false));
    ASSERT_EQ(t.has.associated_context_packet_count,
              get_val<bool>(&val_cp, "has.associated_context_packet_count", false));
    ASSERT_EQ(Hex(t.associated_context_packet_count),
              Hex(get_val<uint8_t>(&val_cp, "associated_context_packet_count", 0)));

    check_remaining(val_cp);
}

static void assert_formatted_geolocation(const vrt_formatted_geolocation& g,
                                         std::map<std::string, std::any>* val_cp,
                                         const std::string&               base) {
    ASSERT_EQ(Hex(g.tsi), Hex(get_val<vrt_tsi>(val_cp, base + "tsi", VRT_TSI_UNDEFINED)));
    ASSERT_EQ(Hex(g.tsf), Hex(get_val<vrt_tsf>(val_cp, base + "tsf", VRT_TSF_UNDEFINED)));
    ASSERT_EQ(Hex(g.oui), Hex(get_val<uint32_t>(val_cp, base + "oui", 0)));
    ASSERT_EQ(Hex(g.integer_second_timestamp),
              Hex(get_val<uint32_t>(val_cp, base + "integer_second_timestamp", 0xFFFFFFFF)));
    ASSERT_EQ(Hex(g.fractional_second_timestamp),
              Hex(get_val<uint64_t>(val_cp, base + "fractional_second_timestamp", 0xFFFFFFFFFFFFFFFF)));
    ASSERT_EQ(g.has.latitude, get_val<bool>(val_cp, base + "has.latitude", false));
    ASSERT_EQ(g.has.longitude, get_val<bool>(val_cp, base + "has.longitude", false));
    ASSERT_EQ(g.has.altitude, get_val<bool>(val_cp, base + "has.altitude", false));
    ASSERT_EQ(g.has.speed_over_ground, get_val<bool>(val_cp, base + "has.speed_over_ground", false));
    ASSERT_EQ(g.has.heading_angle, get_val<bool>(val_cp, base + "has.heading_angle", false));
    ASSERT_EQ(g.has.track_angle, get_val<bool>(val_cp, base + "has.track_angle", false));
    ASSERT_EQ(g.has.magnetic_variation, get_val<bool>(val_cp, base + "has.magnetic_variation", false));
    ASSERT_DOUBLE_EQ(g.latitude,
                     get_val<double>(val_cp, base + "latitude", vrt_fixed_point_i32_to_double(0x7FFFFFFF, 22)));
    ASSERT_EQ(g.longitude, get_val<double>(val_cp, base + "longitude", vrt_fixed_point_i32_to_double(0x7FFFFFFF, 22)));
    ASSERT_DOUBLE_EQ(g.altitude,
                     get_val<double>(val_cp, base + "altitude", vrt_fixed_point_i32_to_double(0x7FFFFFFF, 5)));
    ASSERT_DOUBLE_EQ(g.speed_over_ground, get_val<double>(val_cp, base + "speed_over_ground",
                                                          vrt_fixed_point_u32_to_double(0x7FFFFFFF, 16)));
    ASSERT_DOUBLE_EQ(g.heading_angle,
                     get_val<double>(val_cp, base + "heading_angle", vrt_fixed_point_i32_to_double(0x7FFFFFFF, 22)));
    ASSERT_DOUBLE_EQ(g.track_angle,
                     get_val<double>(val_cp, base + "track_angle", vrt_fixed_point_i32_to_double(0x7FFFFFFF, 22)));
    ASSERT_DOUBLE_EQ(g.magnetic_variation, get_val<double>(val_cp, base + "magnetic_variation",
                                                           vrt_fixed_point_i32_to_double(0x7FFFFFFF, 22)));
}

static void assert_ephemeris(const vrt_ephemeris& e, std::map<std::string, std::any>* val_cp, const std::string& base) {
    ASSERT_EQ(Hex(e.tsi), Hex(get_val<vrt_tsi>(val_cp, base + "tsi", VRT_TSI_UNDEFINED)));
    ASSERT_EQ(Hex(e.tsf), Hex(get_val<vrt_tsf>(val_cp, base + "tsf", VRT_TSF_UNDEFINED)));
    ASSERT_EQ(Hex(e.oui), Hex(get_val<uint32_t>(val_cp, base + "oui", 0)));
    ASSERT_EQ(Hex(e.integer_second_timestamp),
              Hex(get_val<uint32_t>(val_cp, base + "integer_second_timestamp", 0xFFFFFFFF)));
    ASSERT_EQ(Hex(e.fractional_second_timestamp),
              Hex(get_val<uint64_t>(val_cp, base + "fractional_second_timestamp", 0xFFFFFFFFFFFFFFFF)));
    ASSERT_EQ(e.has.position_x, get_val<bool>(val_cp, base + "has.position_x", false));
    ASSERT_EQ(e.has.position_y, get_val<bool>(val_cp, base + "has.position_y", false));
    ASSERT_EQ(e.has.position_z, get_val<bool>(val_cp, base + "has.position_z", false));
    ASSERT_EQ(e.has.attitude_alpha, get_val<bool>(val_cp, base + "has.attitude_alpha", false));
    ASSERT_EQ(e.has.attitude_beta, get_val<bool>(val_cp, base + "has.attitude_beta", false));
    ASSERT_EQ(e.has.attitude_phi, get_val<bool>(val_cp, base + "has.attitude_phi", false));
    ASSERT_EQ(e.has.velocity_dx, get_val<bool>(val_cp, base + "has.velocity_dx", false));
    ASSERT_EQ(e.has.velocity_dy, get_val<bool>(val_cp, base + "has.velocity_dy", false));
    ASSERT_EQ(e.has.velocity_dz, get_val<bool>(val_cp, base + "has.velocity_dz", false));
    ASSERT_DOUBLE_EQ(e.position_x,
                     get_val<double>(val_cp, base + "position_x", vrt_fixed_point_i32_to_double(0x7FFFFFFF, 5)));
    ASSERT_DOUBLE_EQ(e.position_y,
                     get_val<double>(val_cp, base + "position_y", vrt_fixed_point_i32_to_double(0x7FFFFFFF, 5)));
    ASSERT_DOUBLE_EQ(e.position_z,
                     get_val<double>(val_cp, base + "position_z", vrt_fixed_point_i32_to_double(0x7FFFFFFF, 5)));
    ASSERT_DOUBLE_EQ(e.attitude_alpha,
                     get_val<double>(val_cp, base + "attitude_alpha", vrt_fixed_point_i32_to_double(0x7FFFFFFF, 22)));
    ASSERT_DOUBLE_EQ(e.attitude_beta,
                     get_val<double>(val_cp, base + "attitude_beta", vrt_fixed_point_i32_to_double(0x7FFFFFFF, 22)));
    ASSERT_DOUBLE_EQ(e.attitude_phi,
                     get_val<double>(val_cp, base + "attitude_phi", vrt_fixed_point_i32_to_double(0x7FFFFFFF, 22)));
    ASSERT_DOUBLE_EQ(e.velocity_dx,
                     get_val<double>(val_cp, base + "velocity_dx", vrt_fixed_point_i32_to_double(0x7FFFFFFF, 16)));
    ASSERT_DOUBLE_EQ(e.velocity_dy,
                     get_val<double>(val_cp, base + "velocity_dy", vrt_fixed_point_i32_to_double(0x7FFFFFFF, 16)));
    ASSERT_DOUBLE_EQ(e.velocity_dz,
                     get_val<double>(val_cp, base + "velocity_dz", vrt_fixed_point_i32_to_double(0x7FFFFFFF, 16)));
}

/**
 * Assert IF context values.
 *
 * \param c      IF context.
 * \param values Lists non-default values, i.e. values that differ from the vrt_init_if_context() state, as field name
 *               -> field value.
 */
void assert_if_context(const vrt_if_context& c, const std::map<std::string, std::any>& values) {
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
              get_val<vrt_agc_or_mgc>(&val_cp, "state_and_event_indicators.agc_or_mgc", VRT_AOM_MGC));
    ASSERT_EQ(c.state_and_event_indicators.detected_signal,
              get_val<bool>(&val_cp, "state_and_event_indicators.detected_signal", false));
    ASSERT_EQ(c.state_and_event_indicators.spectral_inversion,
              get_val<bool>(&val_cp, "state_and_event_indicators.spectral_inversion", false));
    ASSERT_EQ(c.state_and_event_indicators.over_range,
              get_val<bool>(&val_cp, "state_and_event_indicators.over_range", false));
    ASSERT_EQ(c.state_and_event_indicators.sample_loss,
              get_val<bool>(&val_cp, "state_and_event_indicators.sample_loss", false));
    ASSERT_EQ(c.state_and_event_indicators.user_defined,
              get_val<uint8_t>(&val_cp, "state_and_event_indicators.user_defined", 0));

    ASSERT_EQ(
        c.data_packet_payload_format.packing_method,
        get_val<vrt_packing_method>(&val_cp, "data_packet_payload_format.packing_method", VRT_PM_PROCESSING_EFFICIENT));
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

    assert_formatted_geolocation(c.formatted_gps_geolocation, &val_cp, "formatted_gps_geolocation.");
    assert_formatted_geolocation(c.formatted_ins_geolocation, &val_cp, "formatted_ins_geolocation.");

    assert_ephemeris(c.ecef_ephemeris, &val_cp, "ecef_ephemeris.");
    assert_ephemeris(c.relative_ephemeris, &val_cp, "relative_ephemeris.");

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
