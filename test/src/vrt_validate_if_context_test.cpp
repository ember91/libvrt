#include <gtest/gtest.h>

#include <vrt/vrt_common.h>
#include <vrt/vrt_validate.h>

#include "../../src/vrt_fixed_point.h"

class ValidateIfContextTest : public ::testing::Test {
   protected:
    ValidateIfContextTest() : c_() {}

    void SetUp() override { vrt_init_if_context(&c_); }

    vrt_if_context c_;
};

TEST_F(ValidateIfContextTest, None) {
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
}

TEST_F(ValidateIfContextTest, BandwidthValid) {
    c_.bandwidth = -1.0;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.bandwidth = true;
    c_.bandwidth     = 0;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
}

TEST_F(ValidateIfContextTest, BandwidthInvalid) {
    c_.has.bandwidth = true;
    c_.bandwidth     = -1.0;
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_BANDWIDTH);
}

TEST_F(ValidateIfContextTest, SampleRateValid) {
    c_.sample_rate = -1.0;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.sample_rate = true;
    c_.sample_rate     = 0;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
}

TEST_F(ValidateIfContextTest, SampleRateInvalid) {
    c_.has.sample_rate = true;
    c_.sample_rate     = -1.0;
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_SAMPLE_RATE);
}

TEST_F(ValidateIfContextTest, TemperatureValid) {
    c_.temperature = -274;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.temperature = true;
    c_.temperature     = -273.15;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
}

TEST_F(ValidateIfContextTest, TemperatureInvalid) {
    c_.has.temperature = true;
    c_.temperature     = -274;
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_TEMPERATURE);
}

TEST_F(ValidateIfContextTest, DeviceIdentifierOuiValid) {
    c_.device_identifier.oui = 0x01FFFFFF;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.device_identifier = true;
    c_.device_identifier.oui = 0x00FFFFFF;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
}

TEST_F(ValidateIfContextTest, DeviceIdentifierOuiInvalid) {
    c_.has.device_identifier = true;
    c_.device_identifier.oui = 0x01FFFFFF;
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_OUI);
}

TEST_F(ValidateIfContextTest, RealOrComplexValid) {
    c_.data_packet_payload_format.real_or_complex = static_cast<vrt_real_complex>(-1);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.data_packet_payload_format.real_or_complex = static_cast<vrt_real_complex>(3);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.data_packet_payload_format             = true;
    c_.data_packet_payload_format.real_or_complex = static_cast<vrt_real_complex>(0);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.data_packet_payload_format             = true;
    c_.data_packet_payload_format.real_or_complex = static_cast<vrt_real_complex>(2);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
}

TEST_F(ValidateIfContextTest, RealOrComplexInvalid) {
    c_.has.data_packet_payload_format             = true;
    c_.data_packet_payload_format.real_or_complex = static_cast<vrt_real_complex>(-1);
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_REAL_OR_COMPLEX);
    c_.data_packet_payload_format.real_or_complex = static_cast<vrt_real_complex>(3);
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_REAL_OR_COMPLEX);
}

TEST_F(ValidateIfContextTest, DataItemFormatValid) {
    c_.data_packet_payload_format.data_item_format = static_cast<vrt_data_item_format>(-1);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.data_packet_payload_format.data_item_format = static_cast<vrt_data_item_format>(0x07);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.data_packet_payload_format.data_item_format = static_cast<vrt_data_item_format>(0x0D);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.data_packet_payload_format.data_item_format = static_cast<vrt_data_item_format>(0x17);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.data_packet_payload_format              = true;
    c_.data_packet_payload_format.data_item_format = static_cast<vrt_data_item_format>(0x00);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.data_packet_payload_format              = true;
    c_.data_packet_payload_format.data_item_format = static_cast<vrt_data_item_format>(0x06);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.data_packet_payload_format              = true;
    c_.data_packet_payload_format.data_item_format = static_cast<vrt_data_item_format>(0x0E);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.data_packet_payload_format              = true;
    c_.data_packet_payload_format.data_item_format = static_cast<vrt_data_item_format>(0x16);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
}

TEST_F(ValidateIfContextTest, DataItemFormatInvalid) {
    c_.has.data_packet_payload_format              = true;
    c_.data_packet_payload_format.data_item_format = static_cast<vrt_data_item_format>(-1);
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_DATA_ITEM_FORMAT);
    c_.data_packet_payload_format.data_item_format = static_cast<vrt_data_item_format>(0x07);
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_DATA_ITEM_FORMAT);
    c_.data_packet_payload_format.data_item_format = static_cast<vrt_data_item_format>(0x0D);
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_DATA_ITEM_FORMAT);
    c_.data_packet_payload_format.data_item_format = static_cast<vrt_data_item_format>(0x17);
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_DATA_ITEM_FORMAT);
}

TEST_F(ValidateIfContextTest, EventTagSizeValid) {
    c_.data_packet_payload_format.event_tag_size = 0x08;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.data_packet_payload_format            = true;
    c_.data_packet_payload_format.event_tag_size = 0x07;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
}

TEST_F(ValidateIfContextTest, EventTagSizeInvalid) {
    c_.has.data_packet_payload_format            = true;
    c_.data_packet_payload_format.event_tag_size = 0x08;
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_EVENT_TAG_SIZE);
}

TEST_F(ValidateIfContextTest, ChannelTagSizeValid) {
    c_.data_packet_payload_format.channel_tag_size = 0x10;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.data_packet_payload_format              = true;
    c_.data_packet_payload_format.channel_tag_size = 0x0F;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
}

TEST_F(ValidateIfContextTest, ChannelTagSizeInvalid) {
    c_.has.data_packet_payload_format              = true;
    c_.data_packet_payload_format.channel_tag_size = 0x10;
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_CHANNEL_TAG_SIZE);
}

TEST_F(ValidateIfContextTest, ItemPackingFieldSizeValid) {
    c_.data_packet_payload_format.item_packing_field_size = 0x4F;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.data_packet_payload_format                     = true;
    c_.data_packet_payload_format.item_packing_field_size = 0x3F;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
}

TEST_F(ValidateIfContextTest, ItemPackingFieldSizeInvalid) {
    c_.has.data_packet_payload_format                     = true;
    c_.data_packet_payload_format.item_packing_field_size = 0x4F;
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_ITEM_PACKING_FIELD_SIZE);
}

TEST_F(ValidateIfContextTest, DataItemSizeValid) {
    c_.data_packet_payload_format.data_item_size = 0x4F;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.data_packet_payload_format            = true;
    c_.data_packet_payload_format.data_item_size = 0x3F;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
}

TEST_F(ValidateIfContextTest, DataItemSizeInvalid) {
    c_.has.data_packet_payload_format            = true;
    c_.data_packet_payload_format.data_item_size = 0x4F;
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_DATA_ITEM_SIZE);
}

TEST_F(ValidateIfContextTest, FormattedGpsGeolocationTsiValid) {
    c_.formatted_gps_geolocation.tsi = static_cast<vrt_tsi>(-1);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.formatted_gps_geolocation.tsi = static_cast<vrt_tsi>(4);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.formatted_gps_geolocation = true;
    c_.formatted_gps_geolocation.tsi = static_cast<vrt_tsi>(0);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.formatted_gps_geolocation = true;
    c_.formatted_gps_geolocation.tsi = static_cast<vrt_tsi>(3);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
}

TEST_F(ValidateIfContextTest, FormattedGpsGeolocationTsiInvalid) {
    c_.has.formatted_gps_geolocation = true;
    c_.formatted_gps_geolocation.tsi = static_cast<vrt_tsi>(-1);
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_TSI);
    c_.formatted_gps_geolocation.tsi = static_cast<vrt_tsi>(4);
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_TSI);
}

TEST_F(ValidateIfContextTest, FormattedGpsGeolocationTsfValid) {
    c_.formatted_gps_geolocation.tsf = static_cast<vrt_tsf>(-1);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.formatted_gps_geolocation.tsf = static_cast<vrt_tsf>(4);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.formatted_gps_geolocation = true;
    c_.formatted_gps_geolocation.tsf = static_cast<vrt_tsf>(0);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.formatted_gps_geolocation = true;
    c_.formatted_gps_geolocation.tsf = static_cast<vrt_tsf>(3);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
}

TEST_F(ValidateIfContextTest, FormattedGpsGeolocationTsfInvalid) {
    c_.has.formatted_gps_geolocation = true;
    c_.formatted_gps_geolocation.tsf = static_cast<vrt_tsf>(-1);
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_TSF);
    c_.formatted_gps_geolocation.tsf = static_cast<vrt_tsf>(4);
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_TSF);
}

TEST_F(ValidateIfContextTest, FormattedGpsGeolocationOuiValid) {
    c_.formatted_gps_geolocation.oui = 0x01FFFFFF;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.formatted_gps_geolocation = true;
    c_.formatted_gps_geolocation.oui = 0x00FFFFFF;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
}

TEST_F(ValidateIfContextTest, FormattedGpsGeolocationOuiInvalid) {
    c_.has.formatted_gps_geolocation = true;
    c_.formatted_gps_geolocation.oui = 0x01FFFFFF;
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_OUI);
}

TEST_F(ValidateIfContextTest, FormattedGpsGeolocationIntegerSecondTimestampValid) {
    c_.formatted_gps_geolocation.tsi                      = VRT_TSI_UNDEFINED;
    c_.formatted_gps_geolocation.integer_second_timestamp = 0x7FFFFFFF;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.formatted_gps_geolocation                      = true;
    c_.formatted_gps_geolocation.tsi                      = VRT_TSI_UNDEFINED;
    c_.formatted_gps_geolocation.integer_second_timestamp = 0xFFFFFFFF;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.formatted_gps_geolocation                      = true;
    c_.formatted_gps_geolocation.tsi                      = VRT_TSI_UTC;
    c_.formatted_gps_geolocation.integer_second_timestamp = 0x7FFFFFFF;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
}

TEST_F(ValidateIfContextTest, FormattedGpsGeolocationIntegerSecondTimestampInvalid) {
    c_.has.formatted_gps_geolocation                      = true;
    c_.formatted_gps_geolocation.tsi                      = VRT_TSI_UNDEFINED;
    c_.formatted_gps_geolocation.integer_second_timestamp = 0x7FFFFFFF;
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_INTEGER_SECOND_TIMESTAMP);
}

TEST_F(ValidateIfContextTest, FormattedGpsGeolocationFractionalSecondTimestampValid) {
    c_.formatted_gps_geolocation.tsf                         = VRT_TSF_UNDEFINED;
    c_.formatted_gps_geolocation.fractional_second_timestamp = 0x7FFFFFFFFFFFFFFF;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.formatted_gps_geolocation                         = true;
    c_.formatted_gps_geolocation.tsf                         = VRT_TSF_UNDEFINED;
    c_.formatted_gps_geolocation.fractional_second_timestamp = 0xFFFFFFFFFFFFFFFF;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.formatted_gps_geolocation                         = true;
    c_.formatted_gps_geolocation.tsf                         = VRT_TSF_FREE_RUNNING_COUNT;
    c_.formatted_gps_geolocation.fractional_second_timestamp = 0x7FFFFFFFFFFFFFFF;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
}

TEST_F(ValidateIfContextTest, FormattedGpsGeolocationFractionalSecondTimestampInvalid) {
    c_.has.formatted_gps_geolocation                         = true;
    c_.formatted_gps_geolocation.tsf                         = VRT_TSF_UNDEFINED;
    c_.formatted_gps_geolocation.fractional_second_timestamp = 0x7FFFFFFFFFFFFFFF;
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_FRACTIONAL_SECOND_TIMESTAMP);
}

TEST_F(ValidateIfContextTest, FormattedGpsGeolocationLatitudeValid) {
    c_.formatted_gps_geolocation.latitude = -91.0;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.formatted_gps_geolocation.latitude = 91.0;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.formatted_gps_geolocation.latitude = vrt_fixed_point_i32_to_double(0x7FFFFFFE, 22);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.formatted_gps_geolocation      = true;
    c_.formatted_gps_geolocation.latitude = -90.0;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.formatted_gps_geolocation      = true;
    c_.formatted_gps_geolocation.latitude = 90.0;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.formatted_gps_geolocation      = true;
    c_.formatted_gps_geolocation.latitude = vrt_fixed_point_i32_to_double(0x7FFFFFFF, 22);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
}

TEST_F(ValidateIfContextTest, FormattedGpsGeolocationLatitudeInvalid) {
    c_.has.formatted_gps_geolocation      = true;
    c_.formatted_gps_geolocation.latitude = -91.0;
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_LATITUDE);
    c_.formatted_gps_geolocation.latitude = 91.0;
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_LATITUDE);
    c_.formatted_gps_geolocation.latitude = vrt_fixed_point_i32_to_double(0x7FFFFFFE, 22);
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_LATITUDE);
}

TEST_F(ValidateIfContextTest, FormattedGpsGeolocationLongitudeValid) {
    c_.formatted_gps_geolocation.longitude = -181.0;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.formatted_gps_geolocation.longitude = 181.0;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.formatted_gps_geolocation.longitude = vrt_fixed_point_i32_to_double(0x7FFFFFFE, 22);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.formatted_gps_geolocation       = true;
    c_.formatted_gps_geolocation.longitude = -180.0;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.formatted_gps_geolocation       = true;
    c_.formatted_gps_geolocation.longitude = 180.0;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.formatted_gps_geolocation       = true;
    c_.formatted_gps_geolocation.longitude = vrt_fixed_point_i32_to_double(0x7FFFFFFF, 22);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
}

TEST_F(ValidateIfContextTest, FormattedGpsGeolocationLongitudeInvalid) {
    c_.has.formatted_gps_geolocation       = true;
    c_.formatted_gps_geolocation.longitude = -181.0;
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_LONGITUDE);
    c_.formatted_gps_geolocation.longitude = 181.0;
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_LONGITUDE);
    c_.formatted_gps_geolocation.longitude = vrt_fixed_point_i32_to_double(0x7FFFFFFE, 22);
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_LONGITUDE);
}

TEST_F(ValidateIfContextTest, FormattedGpsGeolocationHeadingAngleValid) {
    c_.formatted_gps_geolocation.heading_angle = -1.0;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.formatted_gps_geolocation.heading_angle = 360;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.formatted_gps_geolocation.heading_angle = vrt_fixed_point_i32_to_double(0x7FFFFFFE, 22);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.formatted_gps_geolocation           = true;
    c_.formatted_gps_geolocation.heading_angle = 0;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.formatted_gps_geolocation           = true;
    c_.formatted_gps_geolocation.heading_angle = 359;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.formatted_gps_geolocation           = true;
    c_.formatted_gps_geolocation.heading_angle = vrt_fixed_point_i32_to_double(0x7FFFFFFF, 22);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
}

TEST_F(ValidateIfContextTest, FormattedGpsGeolocationHeadingAngleInvalid) {
    c_.has.formatted_gps_geolocation           = true;
    c_.formatted_gps_geolocation.heading_angle = -1.0;
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_HEADING_ANGLE);
    c_.formatted_gps_geolocation.heading_angle = 360;
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_HEADING_ANGLE);
    c_.formatted_gps_geolocation.heading_angle = vrt_fixed_point_i32_to_double(0x7FFFFFFE, 22);
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_HEADING_ANGLE);
}

TEST_F(ValidateIfContextTest, FormattedGpsGeolocationTrackAngleValid) {
    c_.formatted_gps_geolocation.track_angle = -1.0;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.formatted_gps_geolocation.track_angle = 360;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.formatted_gps_geolocation.track_angle = vrt_fixed_point_i32_to_double(0x7FFFFFFE, 22);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.formatted_gps_geolocation         = true;
    c_.formatted_gps_geolocation.track_angle = 0;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.formatted_gps_geolocation         = true;
    c_.formatted_gps_geolocation.track_angle = 359;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.formatted_gps_geolocation         = true;
    c_.formatted_gps_geolocation.track_angle = vrt_fixed_point_i32_to_double(0x7FFFFFFF, 22);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
}

TEST_F(ValidateIfContextTest, FormattedGpsGeolocationTrackAngleInvalid) {
    c_.has.formatted_gps_geolocation         = true;
    c_.formatted_gps_geolocation.track_angle = -1.0;
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_TRACK_ANGLE);
    c_.formatted_gps_geolocation.track_angle = 360;
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_TRACK_ANGLE);
    c_.formatted_gps_geolocation.track_angle = vrt_fixed_point_i32_to_double(0x7FFFFFFE, 22);
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_TRACK_ANGLE);
}

TEST_F(ValidateIfContextTest, FormattedGpsGeolocationMagneticVariationValid) {
    c_.formatted_gps_geolocation.magnetic_variation = -181;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.formatted_gps_geolocation.magnetic_variation = 181;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.formatted_gps_geolocation.magnetic_variation = vrt_fixed_point_i32_to_double(0x7FFFFFFE, 22);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.formatted_gps_geolocation                = true;
    c_.formatted_gps_geolocation.magnetic_variation = -180;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.formatted_gps_geolocation                = true;
    c_.formatted_gps_geolocation.magnetic_variation = 180;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.formatted_gps_geolocation                = true;
    c_.formatted_gps_geolocation.magnetic_variation = vrt_fixed_point_i32_to_double(0x7FFFFFFF, 22);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
}

TEST_F(ValidateIfContextTest, FormattedGpsGeolocationMagneticVariationInvalid) {
    c_.has.formatted_gps_geolocation                = true;
    c_.formatted_gps_geolocation.magnetic_variation = -181;
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_MAGNETIC_VARIATION);
    c_.formatted_gps_geolocation.magnetic_variation = 181;
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_MAGNETIC_VARIATION);
    c_.formatted_gps_geolocation.magnetic_variation = vrt_fixed_point_i32_to_double(0x7FFFFFFE, 22);
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_MAGNETIC_VARIATION);
}

TEST_F(ValidateIfContextTest, FormattedInsGeolocationTsiValid) {
    c_.formatted_ins_geolocation.tsi = static_cast<vrt_tsi>(-1);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.formatted_ins_geolocation.tsi = static_cast<vrt_tsi>(4);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.formatted_ins_geolocation = true;
    c_.formatted_ins_geolocation.tsi = static_cast<vrt_tsi>(0);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.formatted_ins_geolocation = true;
    c_.formatted_ins_geolocation.tsi = static_cast<vrt_tsi>(3);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
}

TEST_F(ValidateIfContextTest, FormattedInsGeolocationTsiInvalid) {
    c_.has.formatted_ins_geolocation = true;
    c_.formatted_ins_geolocation.tsi = static_cast<vrt_tsi>(-1);
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_TSI);
    c_.formatted_ins_geolocation.tsi = static_cast<vrt_tsi>(4);
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_TSI);
}

TEST_F(ValidateIfContextTest, FormattedInsGeolocationTsfValid) {
    c_.formatted_ins_geolocation.tsf = static_cast<vrt_tsf>(-1);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.formatted_ins_geolocation.tsf = static_cast<vrt_tsf>(4);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.formatted_ins_geolocation = true;
    c_.formatted_ins_geolocation.tsf = static_cast<vrt_tsf>(0);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.formatted_ins_geolocation = true;
    c_.formatted_ins_geolocation.tsf = static_cast<vrt_tsf>(3);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
}

TEST_F(ValidateIfContextTest, FormattedInsGeolocationTsfInvalid) {
    c_.has.formatted_ins_geolocation = true;
    c_.formatted_ins_geolocation.tsf = static_cast<vrt_tsf>(-1);
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_TSF);
    c_.formatted_ins_geolocation.tsf = static_cast<vrt_tsf>(4);
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_TSF);
}

TEST_F(ValidateIfContextTest, FormattedInsGeolocationOuiValid) {
    c_.formatted_ins_geolocation.oui = 0x01FFFFFF;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.formatted_ins_geolocation = true;
    c_.formatted_ins_geolocation.oui = 0x00FFFFFF;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
}

TEST_F(ValidateIfContextTest, FormattedInsGeolocationOuiInvalid) {
    c_.has.formatted_ins_geolocation = true;
    c_.formatted_ins_geolocation.oui = 0x01FFFFFF;
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_OUI);
}

TEST_F(ValidateIfContextTest, FormattedInsGeolocationIntegerSecondTimestampValid) {
    c_.formatted_ins_geolocation.tsi                      = VRT_TSI_UNDEFINED;
    c_.formatted_ins_geolocation.integer_second_timestamp = 0x7FFFFFFF;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.formatted_ins_geolocation                      = true;
    c_.formatted_ins_geolocation.tsi                      = VRT_TSI_UNDEFINED;
    c_.formatted_ins_geolocation.integer_second_timestamp = 0xFFFFFFFF;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.formatted_ins_geolocation                      = true;
    c_.formatted_ins_geolocation.tsi                      = VRT_TSI_UTC;
    c_.formatted_ins_geolocation.integer_second_timestamp = 0x7FFFFFFF;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
}

TEST_F(ValidateIfContextTest, FormattedInsGeolocationIntegerSecondTimestampInvalid) {
    c_.has.formatted_ins_geolocation                      = true;
    c_.formatted_ins_geolocation.tsi                      = VRT_TSI_UNDEFINED;
    c_.formatted_ins_geolocation.integer_second_timestamp = 0x7FFFFFFF;
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_INTEGER_SECOND_TIMESTAMP);
}

TEST_F(ValidateIfContextTest, FormattedInsGeolocationFractionalSecondTimestampValid) {
    c_.formatted_ins_geolocation.tsf                         = VRT_TSF_UNDEFINED;
    c_.formatted_ins_geolocation.fractional_second_timestamp = 0x7FFFFFFFFFFFFFFF;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.formatted_ins_geolocation                         = true;
    c_.formatted_ins_geolocation.tsf                         = VRT_TSF_UNDEFINED;
    c_.formatted_ins_geolocation.fractional_second_timestamp = 0xFFFFFFFFFFFFFFFF;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.formatted_ins_geolocation                         = true;
    c_.formatted_ins_geolocation.tsf                         = VRT_TSF_FREE_RUNNING_COUNT;
    c_.formatted_ins_geolocation.fractional_second_timestamp = 0x7FFFFFFFFFFFFFFF;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
}

TEST_F(ValidateIfContextTest, FormattedInsGeolocationFractionalSecondTimestampInvalid) {
    c_.has.formatted_ins_geolocation                         = true;
    c_.formatted_ins_geolocation.tsf                         = VRT_TSF_UNDEFINED;
    c_.formatted_ins_geolocation.fractional_second_timestamp = 0x7FFFFFFFFFFFFFFF;
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_FRACTIONAL_SECOND_TIMESTAMP);
}

TEST_F(ValidateIfContextTest, FormattedInsGeolocationLatitudeValid) {
    c_.formatted_ins_geolocation.latitude = -91.0;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.formatted_ins_geolocation.latitude = 91.0;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.formatted_ins_geolocation.latitude = vrt_fixed_point_i32_to_double(0x7FFFFFFE, 22);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.formatted_ins_geolocation      = true;
    c_.formatted_ins_geolocation.latitude = -90.0;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.formatted_ins_geolocation      = true;
    c_.formatted_ins_geolocation.latitude = 90.0;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.formatted_ins_geolocation      = true;
    c_.formatted_ins_geolocation.latitude = vrt_fixed_point_i32_to_double(0x7FFFFFFF, 22);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
}

TEST_F(ValidateIfContextTest, FormattedInsGeolocationLatitudeInvalid) {
    c_.has.formatted_ins_geolocation      = true;
    c_.formatted_ins_geolocation.latitude = -91.0;
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_LATITUDE);
    c_.formatted_ins_geolocation.latitude = 91.0;
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_LATITUDE);
    c_.formatted_ins_geolocation.latitude = vrt_fixed_point_i32_to_double(0x7FFFFFFE, 22);
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_LATITUDE);
}

TEST_F(ValidateIfContextTest, FormattedInsGeolocationLongitudeValid) {
    c_.formatted_ins_geolocation.longitude = -181.0;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.formatted_ins_geolocation.longitude = 181.0;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.formatted_ins_geolocation.longitude = vrt_fixed_point_i32_to_double(0x7FFFFFFE, 22);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.formatted_ins_geolocation       = true;
    c_.formatted_ins_geolocation.longitude = -180.0;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.formatted_ins_geolocation       = true;
    c_.formatted_ins_geolocation.longitude = 180.0;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.formatted_ins_geolocation       = true;
    c_.formatted_ins_geolocation.longitude = vrt_fixed_point_i32_to_double(0x7FFFFFFF, 22);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
}

TEST_F(ValidateIfContextTest, FormattedInsGeolocationLongitudeInvalid) {
    c_.has.formatted_ins_geolocation       = true;
    c_.formatted_ins_geolocation.longitude = -181.0;
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_LONGITUDE);
    c_.formatted_ins_geolocation.longitude = 181.0;
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_LONGITUDE);
    c_.formatted_ins_geolocation.longitude = vrt_fixed_point_i32_to_double(0x7FFFFFFE, 22);
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_LONGITUDE);
}

TEST_F(ValidateIfContextTest, FormattedInsGeolocationHeadingAngleValid) {
    c_.formatted_ins_geolocation.heading_angle = -1.0;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.formatted_ins_geolocation.heading_angle = 360;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.formatted_ins_geolocation.heading_angle = vrt_fixed_point_i32_to_double(0x7FFFFFFE, 22);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.formatted_ins_geolocation           = true;
    c_.formatted_ins_geolocation.heading_angle = 0;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.formatted_ins_geolocation           = true;
    c_.formatted_ins_geolocation.heading_angle = 359;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.formatted_ins_geolocation           = true;
    c_.formatted_ins_geolocation.heading_angle = vrt_fixed_point_i32_to_double(0x7FFFFFFF, 22);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
}

TEST_F(ValidateIfContextTest, FormattedInsGeolocationHeadingAngleInvalid) {
    c_.has.formatted_ins_geolocation           = true;
    c_.formatted_ins_geolocation.heading_angle = -1.0;
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_HEADING_ANGLE);
    c_.formatted_ins_geolocation.heading_angle = 360;
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_HEADING_ANGLE);
    c_.formatted_ins_geolocation.heading_angle = vrt_fixed_point_i32_to_double(0x7FFFFFFE, 22);
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_HEADING_ANGLE);
}

TEST_F(ValidateIfContextTest, FormattedInsGeolocationTrackAngleValid) {
    c_.formatted_ins_geolocation.track_angle = -1.0;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.formatted_ins_geolocation.track_angle = 360;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.formatted_ins_geolocation.track_angle = vrt_fixed_point_i32_to_double(0x7FFFFFFE, 22);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.formatted_ins_geolocation         = true;
    c_.formatted_ins_geolocation.track_angle = 0;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.formatted_ins_geolocation         = true;
    c_.formatted_ins_geolocation.track_angle = 359;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.formatted_ins_geolocation         = true;
    c_.formatted_ins_geolocation.track_angle = vrt_fixed_point_i32_to_double(0x7FFFFFFF, 22);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
}

TEST_F(ValidateIfContextTest, FormattedInsGeolocationTrackAngleInvalid) {
    c_.has.formatted_ins_geolocation         = true;
    c_.formatted_ins_geolocation.track_angle = -1.0;
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_TRACK_ANGLE);
    c_.formatted_ins_geolocation.track_angle = 360;
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_TRACK_ANGLE);
    c_.formatted_ins_geolocation.track_angle = vrt_fixed_point_i32_to_double(0x7FFFFFFE, 22);
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_TRACK_ANGLE);
}

TEST_F(ValidateIfContextTest, FormattedInsGeolocationMagneticVariationValid) {
    c_.formatted_ins_geolocation.magnetic_variation = -181;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.formatted_ins_geolocation.magnetic_variation = 181;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.formatted_ins_geolocation.magnetic_variation = vrt_fixed_point_i32_to_double(0x7FFFFFFE, 22);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.formatted_ins_geolocation                = true;
    c_.formatted_ins_geolocation.magnetic_variation = -180;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.formatted_ins_geolocation                = true;
    c_.formatted_ins_geolocation.magnetic_variation = 180;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.formatted_ins_geolocation                = true;
    c_.formatted_ins_geolocation.magnetic_variation = vrt_fixed_point_i32_to_double(0x7FFFFFFF, 22);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
}

TEST_F(ValidateIfContextTest, FormattedInsGeolocationMagneticVariationInvalid) {
    c_.has.formatted_ins_geolocation                = true;
    c_.formatted_ins_geolocation.magnetic_variation = -181;
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_MAGNETIC_VARIATION);
    c_.formatted_ins_geolocation.magnetic_variation = 181;
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_MAGNETIC_VARIATION);
    c_.formatted_ins_geolocation.magnetic_variation = vrt_fixed_point_i32_to_double(0x7FFFFFFE, 22);
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_MAGNETIC_VARIATION);
}

TEST_F(ValidateIfContextTest, EcefEphemerisTsiValid) {
    c_.ecef_ephemeris.tsi = static_cast<vrt_tsi>(-1);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.ecef_ephemeris.tsi = static_cast<vrt_tsi>(4);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.ecef_ephemeris = true;
    c_.ecef_ephemeris.tsi = static_cast<vrt_tsi>(0);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.ecef_ephemeris = true;
    c_.ecef_ephemeris.tsi = static_cast<vrt_tsi>(3);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
}

TEST_F(ValidateIfContextTest, EcefEphemerisTsiInvalid) {
    c_.has.ecef_ephemeris = true;
    c_.ecef_ephemeris.tsi = static_cast<vrt_tsi>(-1);
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_TSI);
    c_.ecef_ephemeris.tsi = static_cast<vrt_tsi>(4);
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_TSI);
}

TEST_F(ValidateIfContextTest, EcefEphemerisTsfValid) {
    c_.ecef_ephemeris.tsf = static_cast<vrt_tsf>(-1);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.ecef_ephemeris.tsf = static_cast<vrt_tsf>(4);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.ecef_ephemeris = true;
    c_.ecef_ephemeris.tsf = static_cast<vrt_tsf>(0);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.ecef_ephemeris = true;
    c_.ecef_ephemeris.tsf = static_cast<vrt_tsf>(3);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
}

TEST_F(ValidateIfContextTest, EcefEphemerisTsfInvalid) {
    c_.has.ecef_ephemeris = true;
    c_.ecef_ephemeris.tsf = static_cast<vrt_tsf>(-1);
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_TSF);
    c_.ecef_ephemeris.tsf = static_cast<vrt_tsf>(4);
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_TSF);
}

TEST_F(ValidateIfContextTest, EcefEphemerisOuiValid) {
    c_.ecef_ephemeris.oui = 0x01FFFFFF;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.ecef_ephemeris = true;
    c_.ecef_ephemeris.oui = 0x00FFFFFF;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
}

TEST_F(ValidateIfContextTest, EcefEphemerisOuiInvalid) {
    c_.has.ecef_ephemeris = true;
    c_.ecef_ephemeris.oui = 0x01FFFFFF;
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_OUI);
}

TEST_F(ValidateIfContextTest, EcefEphemerisIntegerSecondTimestampValid) {
    c_.ecef_ephemeris.tsi                      = VRT_TSI_UNDEFINED;
    c_.ecef_ephemeris.integer_second_timestamp = 0x7FFFFFFF;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.ecef_ephemeris                      = true;
    c_.ecef_ephemeris.tsi                      = VRT_TSI_UNDEFINED;
    c_.ecef_ephemeris.integer_second_timestamp = 0xFFFFFFFF;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.ecef_ephemeris                      = true;
    c_.ecef_ephemeris.tsi                      = VRT_TSI_UTC;
    c_.ecef_ephemeris.integer_second_timestamp = 0x7FFFFFFF;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
}

TEST_F(ValidateIfContextTest, EcefEphemerisIntegerSecondTimestampInvalid) {
    c_.has.ecef_ephemeris                      = true;
    c_.ecef_ephemeris.tsi                      = VRT_TSI_UNDEFINED;
    c_.ecef_ephemeris.integer_second_timestamp = 0x7FFFFFFF;
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_INTEGER_SECOND_TIMESTAMP);
}

TEST_F(ValidateIfContextTest, EcefEphemerisFractionalSecondTimestampValid) {
    c_.ecef_ephemeris.tsf                         = VRT_TSF_UNDEFINED;
    c_.ecef_ephemeris.fractional_second_timestamp = 0x7FFFFFFFFFFFFFFF;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.ecef_ephemeris                         = true;
    c_.ecef_ephemeris.tsf                         = VRT_TSF_UNDEFINED;
    c_.ecef_ephemeris.fractional_second_timestamp = 0xFFFFFFFFFFFFFFFF;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.ecef_ephemeris                         = true;
    c_.ecef_ephemeris.tsf                         = VRT_TSF_FREE_RUNNING_COUNT;
    c_.ecef_ephemeris.fractional_second_timestamp = 0x7FFFFFFFFFFFFFFF;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
}

TEST_F(ValidateIfContextTest, EcefEphemerisFractionalSecondTimestampInvalid) {
    c_.has.ecef_ephemeris                         = true;
    c_.ecef_ephemeris.tsf                         = VRT_TSF_UNDEFINED;
    c_.ecef_ephemeris.fractional_second_timestamp = 0x7FFFFFFFFFFFFFFF;
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_FRACTIONAL_SECOND_TIMESTAMP);
}

TEST_F(ValidateIfContextTest, RelativeEphemerisTsiValid) {
    c_.relative_ephemeris.tsi = static_cast<vrt_tsi>(-1);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.relative_ephemeris.tsi = static_cast<vrt_tsi>(4);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.relative_ephemeris = true;
    c_.relative_ephemeris.tsi = static_cast<vrt_tsi>(0);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.relative_ephemeris = true;
    c_.relative_ephemeris.tsi = static_cast<vrt_tsi>(3);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
}

TEST_F(ValidateIfContextTest, RelativeEphemerisTsiInvalid) {
    c_.has.relative_ephemeris = true;
    c_.relative_ephemeris.tsi = static_cast<vrt_tsi>(-1);
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_TSI);
    c_.relative_ephemeris.tsi = static_cast<vrt_tsi>(4);
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_TSI);
}

TEST_F(ValidateIfContextTest, RelativeEphemerisTsfValid) {
    c_.relative_ephemeris.tsf = static_cast<vrt_tsf>(-1);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.relative_ephemeris.tsf = static_cast<vrt_tsf>(4);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.relative_ephemeris = true;
    c_.relative_ephemeris.tsf = static_cast<vrt_tsf>(0);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.relative_ephemeris = true;
    c_.relative_ephemeris.tsf = static_cast<vrt_tsf>(3);
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
}

TEST_F(ValidateIfContextTest, RelativeEphemerisTsfInvalid) {
    c_.has.relative_ephemeris = true;
    c_.relative_ephemeris.tsf = static_cast<vrt_tsf>(-1);
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_TSF);
    c_.relative_ephemeris.tsf = static_cast<vrt_tsf>(4);
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_TSF);
}

TEST_F(ValidateIfContextTest, RelativeEphemerisOuiValid) {
    c_.relative_ephemeris.oui = 0x01FFFFFF;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.relative_ephemeris = true;
    c_.relative_ephemeris.oui = 0x00FFFFFF;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
}

TEST_F(ValidateIfContextTest, RelativeEphemerisOuiInvalid) {
    c_.has.relative_ephemeris = true;
    c_.relative_ephemeris.oui = 0x01FFFFFF;
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_OUI);
}

TEST_F(ValidateIfContextTest, RelativeEphemerisIntegerSecondTimestampValid) {
    c_.relative_ephemeris.tsi                      = VRT_TSI_UNDEFINED;
    c_.relative_ephemeris.integer_second_timestamp = 0x7FFFFFFF;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.relative_ephemeris                      = true;
    c_.relative_ephemeris.tsi                      = VRT_TSI_UNDEFINED;
    c_.relative_ephemeris.integer_second_timestamp = 0xFFFFFFFF;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.relative_ephemeris                      = true;
    c_.relative_ephemeris.tsi                      = VRT_TSI_UTC;
    c_.relative_ephemeris.integer_second_timestamp = 0x7FFFFFFF;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
}

TEST_F(ValidateIfContextTest, RelativeEphemerisIntegerSecondTimestampInvalid) {
    c_.has.relative_ephemeris                      = true;
    c_.relative_ephemeris.tsi                      = VRT_TSI_UNDEFINED;
    c_.relative_ephemeris.integer_second_timestamp = 0x7FFFFFFF;
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_INTEGER_SECOND_TIMESTAMP);
}

TEST_F(ValidateIfContextTest, RelativeEphemerisFractionalSecondTimestampValid) {
    c_.relative_ephemeris.tsf                         = VRT_TSF_UNDEFINED;
    c_.relative_ephemeris.fractional_second_timestamp = 0x7FFFFFFFFFFFFFFF;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.relative_ephemeris                         = true;
    c_.relative_ephemeris.tsf                         = VRT_TSF_UNDEFINED;
    c_.relative_ephemeris.fractional_second_timestamp = 0xFFFFFFFFFFFFFFFF;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.relative_ephemeris                         = true;
    c_.relative_ephemeris.tsf                         = VRT_TSF_FREE_RUNNING_COUNT;
    c_.relative_ephemeris.fractional_second_timestamp = 0x7FFFFFFFFFFFFFFF;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
}

TEST_F(ValidateIfContextTest, RelativeEphemerisFractionalSecondTimestampInvalid) {
    c_.has.relative_ephemeris                         = true;
    c_.relative_ephemeris.tsf                         = VRT_TSF_UNDEFINED;
    c_.relative_ephemeris.fractional_second_timestamp = 0x7FFFFFFFFFFFFFFF;
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_FRACTIONAL_SECOND_TIMESTAMP);
}

TEST_F(ValidateIfContextTest, SourceListSizeValid) {
    c_.context_association_lists.source_list_size = 0x0200;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.context_association_lists              = true;
    c_.context_association_lists.source_list_size = 0x01FF;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
}

TEST_F(ValidateIfContextTest, SourceListSizeInvalid) {
    c_.has.context_association_lists              = true;
    c_.context_association_lists.source_list_size = 0x0200;
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_SOURCE_LIST_SIZE);
}

TEST_F(ValidateIfContextTest, SystemListSizeValid) {
    c_.context_association_lists.system_list_size = 0x0200;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.context_association_lists              = true;
    c_.context_association_lists.system_list_size = 0x01FF;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
}

TEST_F(ValidateIfContextTest, SystemListSizeInvalid) {
    c_.has.context_association_lists              = true;
    c_.context_association_lists.system_list_size = 0x0200;
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_SYSTEM_LIST_SIZE);
}

TEST_F(ValidateIfContextTest, ChannelListSizeValid) {
    c_.context_association_lists.asynchronous_channel_list_size = 0xFFFF;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
    c_.has.context_association_lists                            = true;
    c_.context_association_lists.asynchronous_channel_list_size = 0x7FFF;
    ASSERT_EQ(vrt_validate_if_context(&c_), 0);
}

TEST_F(ValidateIfContextTest, ChannelListSizeInvalid) {
    c_.has.context_association_lists                            = true;
    c_.context_association_lists.asynchronous_channel_list_size = 0xFFFF;
    ASSERT_EQ(vrt_validate_if_context(&c_), VRT_ERR_CHANNEL_LIST_SIZE);
}
