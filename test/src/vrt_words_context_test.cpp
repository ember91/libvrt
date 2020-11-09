#include <gtest/gtest.h>

#include <vrt/vrt_common.h>

class WordsContextTest : public ::testing::Test {
   protected:
    WordsContextTest() : c_() {}

    void SetUp() override { vrt_init_context(&c_); }

    vrt_context c_;
};

TEST_F(WordsContextTest, None) {
    ASSERT_EQ(vrt_words_context(&c_), 1);
}

TEST_F(WordsContextTest, ReferencePointIdentifier) {
    c_.has.reference_point_identifier = true;
    ASSERT_EQ(vrt_words_context(&c_), 2);
}

TEST_F(WordsContextTest, Bandwidth) {
    c_.has.bandwidth = true;
    ASSERT_EQ(vrt_words_context(&c_), 3);
}

TEST_F(WordsContextTest, IfReferenceFrequency) {
    c_.has.if_reference_frequency = true;
    ASSERT_EQ(vrt_words_context(&c_), 3);
}

TEST_F(WordsContextTest, RfReferenceFrequency) {
    c_.has.rf_reference_frequency = true;
    ASSERT_EQ(vrt_words_context(&c_), 3);
}

TEST_F(WordsContextTest, RfReferenceFrequencyOffset) {
    c_.has.rf_reference_frequency_offset = true;
    ASSERT_EQ(vrt_words_context(&c_), 3);
}

TEST_F(WordsContextTest, IfBandOffset) {
    c_.has.if_band_offset = true;
    ASSERT_EQ(vrt_words_context(&c_), 3);
}

TEST_F(WordsContextTest, ReferenceLevel) {
    c_.has.reference_level = true;
    ASSERT_EQ(vrt_words_context(&c_), 2);
}

TEST_F(WordsContextTest, Gain) {
    c_.has.gain = true;
    ASSERT_EQ(vrt_words_context(&c_), 2);
}

TEST_F(WordsContextTest, OverRangeCount) {
    c_.has.over_range_count = true;
    ASSERT_EQ(vrt_words_context(&c_), 2);
}

TEST_F(WordsContextTest, SampleRate) {
    c_.has.sample_rate = true;
    ASSERT_EQ(vrt_words_context(&c_), 3);
}

TEST_F(WordsContextTest, TimestampAdjustment) {
    c_.has.timestamp_adjustment = true;
    ASSERT_EQ(vrt_words_context(&c_), 3);
}

TEST_F(WordsContextTest, CalibrationTime) {
    c_.has.timestamp_calibration_time = true;
    ASSERT_EQ(vrt_words_context(&c_), 2);
}

TEST_F(WordsContextTest, Temperature) {
    c_.has.temperature = true;
    ASSERT_EQ(vrt_words_context(&c_), 2);
}

TEST_F(WordsContextTest, DeviceIdentifier) {
    c_.has.device_identifier = true;
    ASSERT_EQ(vrt_words_context(&c_), 3);
}

TEST_F(WordsContextTest, StateAndEventIndicators) {
    c_.has.state_and_event_indicators = true;
    ASSERT_EQ(vrt_words_context(&c_), 2);
}

TEST_F(WordsContextTest, DataPacketPayloadFormat) {
    c_.has.data_packet_payload_format = true;
    ASSERT_EQ(vrt_words_context(&c_), 3);
}

TEST_F(WordsContextTest, FormattedGpsGeolocation) {
    c_.has.formatted_gps_geolocation = true;
    ASSERT_EQ(vrt_words_context(&c_), 12);
}

TEST_F(WordsContextTest, FormattedInsGeolocation) {
    c_.has.formatted_ins_geolocation = true;
    ASSERT_EQ(vrt_words_context(&c_), 12);
}

TEST_F(WordsContextTest, EcefEphemeris) {
    c_.has.ecef_ephemeris = true;
    ASSERT_EQ(vrt_words_context(&c_), 14);
}

TEST_F(WordsContextTest, RelativeEphemeris) {
    c_.has.relative_ephemeris = true;
    ASSERT_EQ(vrt_words_context(&c_), 14);
}

TEST_F(WordsContextTest, GpsAsciiNoWords) {
    c_.has.gps_ascii = true;
    ASSERT_EQ(vrt_words_context(&c_), 3);
}

TEST_F(WordsContextTest, GpsAsciiWords) {
    c_.has.gps_ascii             = true;
    c_.gps_ascii.number_of_words = 9;
    ASSERT_EQ(vrt_words_context(&c_), 12);
}

TEST_F(WordsContextTest, ContextAssociationListsNoLists) {
    c_.has.context_association_lists = true;
    ASSERT_EQ(vrt_words_context(&c_), 3);
}

TEST_F(WordsContextTest, ContextAssociationListsSource) {
    c_.has.context_association_lists              = true;
    c_.context_association_lists.source_list_size = 6;
    ASSERT_EQ(vrt_words_context(&c_), 9);
}

TEST_F(WordsContextTest, ContextAssociationListsSystem) {
    c_.has.context_association_lists              = true;
    c_.context_association_lists.system_list_size = 5;
    ASSERT_EQ(vrt_words_context(&c_), 8);
}

TEST_F(WordsContextTest, ContextAssociationListsVectorComponent) {
    c_.has.context_association_lists                        = true;
    c_.context_association_lists.vector_component_list_size = 4;
    ASSERT_EQ(vrt_words_context(&c_), 7);
}

TEST_F(WordsContextTest, ContextAssociationListsAsynchronous) {
    c_.has.context_association_lists                            = true;
    c_.context_association_lists.asynchronous_channel_list_size = 3;
    ASSERT_EQ(vrt_words_context(&c_), 6);
}

TEST_F(WordsContextTest, ContextAssociationListsChannelTag) {
    c_.has.context_association_lists                               = true;
    c_.context_association_lists.has.asynchronous_channel_tag_list = true;
    c_.context_association_lists.asynchronous_channel_list_size    = 2;
    ASSERT_EQ(vrt_words_context(&c_), 7);
}

TEST_F(WordsContextTest, EveryOther1) {
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

    ASSERT_EQ(vrt_words_context(&c_), 47);
}

TEST_F(WordsContextTest, EveryOther2) {
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

    ASSERT_EQ(vrt_words_context(&c_), 62);
}

TEST_F(WordsContextTest, All) {
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

    ASSERT_EQ(vrt_words_context(&c_), 108);
}
