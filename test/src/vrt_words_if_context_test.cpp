#include <gtest/gtest.h>

#include <vrt/vrt_init.h>
#include <vrt/vrt_types.h>
#include <vrt/vrt_words.h>

class WordsIfContextTest : public ::testing::Test {
   protected:
    void SetUp() override { vrt_init_if_context(&c_); }

    vrt_if_context c_{};
};

TEST_F(WordsIfContextTest, None) {
    ASSERT_EQ(vrt_words_if_context(&c_), 1);
}

TEST_F(WordsIfContextTest, ReferencePointIdentifier) {
    c_.has.reference_point_identifier = true;
    ASSERT_EQ(vrt_words_if_context(&c_), 2);
}

TEST_F(WordsIfContextTest, Bandwidth) {
    c_.has.bandwidth = true;
    ASSERT_EQ(vrt_words_if_context(&c_), 3);
}

TEST_F(WordsIfContextTest, IfReferenceFrequency) {
    c_.has.if_reference_frequency = true;
    ASSERT_EQ(vrt_words_if_context(&c_), 3);
}

TEST_F(WordsIfContextTest, RfReferenceFrequency) {
    c_.has.rf_reference_frequency = true;
    ASSERT_EQ(vrt_words_if_context(&c_), 3);
}

TEST_F(WordsIfContextTest, RfReferenceFrequencyOffset) {
    c_.has.rf_reference_frequency_offset = true;
    ASSERT_EQ(vrt_words_if_context(&c_), 3);
}

TEST_F(WordsIfContextTest, IfBandOffset) {
    c_.has.if_band_offset = true;
    ASSERT_EQ(vrt_words_if_context(&c_), 3);
}

TEST_F(WordsIfContextTest, ReferenceLevel) {
    c_.has.reference_level = true;
    ASSERT_EQ(vrt_words_if_context(&c_), 2);
}

TEST_F(WordsIfContextTest, Gain) {
    c_.has.gain = true;
    ASSERT_EQ(vrt_words_if_context(&c_), 2);
}

TEST_F(WordsIfContextTest, OverRangeCount) {
    c_.has.over_range_count = true;
    ASSERT_EQ(vrt_words_if_context(&c_), 2);
}

TEST_F(WordsIfContextTest, SampleRate) {
    c_.has.sample_rate = true;
    ASSERT_EQ(vrt_words_if_context(&c_), 3);
}

TEST_F(WordsIfContextTest, TimestampAdjustment) {
    c_.has.timestamp_adjustment = true;
    ASSERT_EQ(vrt_words_if_context(&c_), 3);
}

TEST_F(WordsIfContextTest, CalibrationTime) {
    c_.has.timestamp_calibration_time = true;
    ASSERT_EQ(vrt_words_if_context(&c_), 2);
}

TEST_F(WordsIfContextTest, Temperature) {
    c_.has.temperature = true;
    ASSERT_EQ(vrt_words_if_context(&c_), 2);
}

TEST_F(WordsIfContextTest, DeviceIdentifier) {
    c_.has.device_identifier = true;
    ASSERT_EQ(vrt_words_if_context(&c_), 3);
}

TEST_F(WordsIfContextTest, StateAndEventIndicators) {
    c_.has.state_and_event_indicators = true;
    ASSERT_EQ(vrt_words_if_context(&c_), 2);
}

TEST_F(WordsIfContextTest, DataPacketPayloadFormat) {
    c_.has.data_packet_payload_format = true;
    ASSERT_EQ(vrt_words_if_context(&c_), 3);
}

TEST_F(WordsIfContextTest, FormattedGpsGeolocation) {
    c_.has.formatted_gps_geolocation = true;
    ASSERT_EQ(vrt_words_if_context(&c_), 12);
}

TEST_F(WordsIfContextTest, FormattedInsGeolocation) {
    c_.has.formatted_ins_geolocation = true;
    ASSERT_EQ(vrt_words_if_context(&c_), 12);
}

TEST_F(WordsIfContextTest, EcefEphemeris) {
    c_.has.ecef_ephemeris = true;
    ASSERT_EQ(vrt_words_if_context(&c_), 14);
}

TEST_F(WordsIfContextTest, RelativeEphemeris) {
    c_.has.relative_ephemeris = true;
    ASSERT_EQ(vrt_words_if_context(&c_), 14);
}

TEST_F(WordsIfContextTest, GpsAsciiNoWords) {
    c_.has.gps_ascii = true;
    ASSERT_EQ(vrt_words_if_context(&c_), 3);
}

TEST_F(WordsIfContextTest, GpsAsciiWords) {
    c_.has.gps_ascii             = true;
    c_.gps_ascii.number_of_words = 9;
    ASSERT_EQ(vrt_words_if_context(&c_), 12);
}

TEST_F(WordsIfContextTest, ContextAssociationListsNoLists) {
    c_.has.context_association_lists = true;
    ASSERT_EQ(vrt_words_if_context(&c_), 3);
}

TEST_F(WordsIfContextTest, ContextAssociationListsSource) {
    c_.has.context_association_lists              = true;
    c_.context_association_lists.source_list_size = 6;
    ASSERT_EQ(vrt_words_if_context(&c_), 9);
}

TEST_F(WordsIfContextTest, ContextAssociationListsSystem) {
    c_.has.context_association_lists              = true;
    c_.context_association_lists.system_list_size = 5;
    ASSERT_EQ(vrt_words_if_context(&c_), 8);
}

TEST_F(WordsIfContextTest, ContextAssociationListsVectorComponent) {
    c_.has.context_association_lists                        = true;
    c_.context_association_lists.vector_component_list_size = 4;
    ASSERT_EQ(vrt_words_if_context(&c_), 7);
}

TEST_F(WordsIfContextTest, ContextAssociationListsAsynchronous) {
    c_.has.context_association_lists                            = true;
    c_.context_association_lists.asynchronous_channel_list_size = 3;
    ASSERT_EQ(vrt_words_if_context(&c_), 6);
}

TEST_F(WordsIfContextTest, ContextAssociationListsChannelTag) {
    c_.has.context_association_lists                               = true;
    c_.context_association_lists.has.asynchronous_channel_tag_list = true;
    c_.context_association_lists.asynchronous_channel_list_size    = 2;
    ASSERT_EQ(vrt_words_if_context(&c_), 7);
}

TEST_F(WordsIfContextTest, EveryOther1) {
    c_.has.reference_point_identifier    = true;
    c_.has.if_reference_frequency        = true;
    c_.has.rf_reference_frequency_offset = true;
    c_.has.reference_level               = true;
    c_.has.over_range_count              = true;
    c_.has.timestamp_adjustment          = true;
    c_.has.temperature                   = true;
    c_.has.state_and_event_indicators    = true;
    c_.has.formatted_gps_geolocation     = true;
    c_.has.ecef_ephemeris                = true;
    c_.has.gps_ascii                     = true;
    c_.gps_ascii.number_of_words         = 9;

    ASSERT_EQ(vrt_words_if_context(&c_), 47);
}

TEST_F(WordsIfContextTest, EveryOther2) {
    c_.has.bandwidth                                               = true;
    c_.has.rf_reference_frequency                                  = true;
    c_.has.if_band_offset                                          = true;
    c_.has.gain                                                    = true;
    c_.has.sample_rate                                             = true;
    c_.has.timestamp_calibration_time                              = true;
    c_.has.device_identifier                                       = true;
    c_.has.data_packet_payload_format                              = true;
    c_.has.formatted_ins_geolocation                               = true;
    c_.has.relative_ephemeris                                      = true;
    c_.has.context_association_lists                               = true;
    c_.context_association_lists.source_list_size                  = 6;
    c_.context_association_lists.system_list_size                  = 5;
    c_.context_association_lists.vector_component_list_size        = 4;
    c_.context_association_lists.has.asynchronous_channel_tag_list = true;
    c_.context_association_lists.asynchronous_channel_list_size    = 3;

    ASSERT_EQ(vrt_words_if_context(&c_), 62);
}

TEST_F(WordsIfContextTest, All) {
    c_.has.reference_point_identifier                              = true;
    c_.has.bandwidth                                               = true;
    c_.has.if_reference_frequency                                  = true;
    c_.has.rf_reference_frequency                                  = true;
    c_.has.rf_reference_frequency_offset                           = true;
    c_.has.if_band_offset                                          = true;
    c_.has.reference_level                                         = true;
    c_.has.gain                                                    = true;
    c_.has.over_range_count                                        = true;
    c_.has.sample_rate                                             = true;
    c_.has.timestamp_adjustment                                    = true;
    c_.has.timestamp_calibration_time                              = true;
    c_.has.temperature                                             = true;
    c_.has.device_identifier                                       = true;
    c_.has.state_and_event_indicators                              = true;
    c_.has.data_packet_payload_format                              = true;
    c_.has.formatted_gps_geolocation                               = true;
    c_.has.formatted_ins_geolocation                               = true;
    c_.has.ecef_ephemeris                                          = true;
    c_.has.relative_ephemeris                                      = true;
    c_.has.gps_ascii                                               = true;
    c_.gps_ascii.number_of_words                                   = 9;
    c_.has.context_association_lists                               = true;
    c_.context_association_lists.source_list_size                  = 6;
    c_.context_association_lists.system_list_size                  = 5;
    c_.context_association_lists.vector_component_list_size        = 4;
    c_.context_association_lists.has.asynchronous_channel_tag_list = true;
    c_.context_association_lists.asynchronous_channel_list_size    = 3;

    ASSERT_EQ(vrt_words_if_context(&c_), 108);
}
