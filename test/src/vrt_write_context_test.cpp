#include <gtest/gtest.h>

#include <array>
#include <cstring>

#include <vrt/vrt_common.h>
#include <vrt/vrt_write.h>

#include "hex.h"

class WriteContextTest : public ::testing::Test {
   protected:
    WriteContextTest() : c_(), buf_() {}

    void SetUp() override {
        vrt_init_context(&c_);
        buf_.fill(0xBAADF00D);
    }

    vrt_context                c_;
    std::array<uint32_t, 1024> buf_;
};

TEST_F(WriteContextTest, None) {
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 0), VRT_ERR);
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 1), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, ContextFieldChange) {
    c_.context_field_change_indicator = true;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 1), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x80000000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, ReferencePointIdentifier) {
    c_.has.reference_point_identifier = true;
    c_.reference_point_identifier     = 0xFEFEFEFE;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 2), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x40000000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xFEFEFEFE));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, Bandwidth) {
    c_.has.bandwidth = true;
    c_.bandwidth     = 4097.0;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 3), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x20000000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000001));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x00100000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, IfReferenceFrequency) {
    c_.has.if_reference_frequency = true;
    c_.if_reference_frequency     = 4097.0;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 3), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x10000000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000001));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x00100000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, RfReferenceFrequency) {
    c_.has.rf_reference_frequency = true;
    c_.rf_reference_frequency     = 4097.0;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 3), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x08000000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000001));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x00100000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, RfReferenceFrequencyOffset) {
    c_.has.rf_reference_frequency_offset = true;
    c_.rf_reference_frequency_offset     = 4097.0;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 3), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x04000000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000001));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x00100000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, IfBandOffset) {
    c_.has.if_band_offset = true;
    c_.if_band_offset     = 4097.0;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 3), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x02000000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000001));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x00100000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, ReferenceLevel) {
    c_.has.reference_level = true;
    c_.reference_level     = -1.0F;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 2), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x01000000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x0000FF80));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, Gain) {
    c_.has.gain    = true;
    c_.gain.stage1 = -1.0F;
    c_.gain.stage2 = 1.0F;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 2), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00800000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x0080FF80));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, OverRangeCount) {
    c_.has.over_range_count = true;
    c_.over_range_count     = 0xFEFEFEFE;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 2), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00400000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xFEFEFEFE));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, SampleRate) {
    c_.has.sample_rate = true;
    c_.sample_rate     = 4097.0;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 3), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00200000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000001));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x00100000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, TimestampAdjustment) {
    c_.has.timestamp_adjustment = true;
    c_.timestamp_adjustment     = 0xABABABABBEBEBEBE;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 3), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00100000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xABABABAB));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBEBEBEBE));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, TimestampCalibrationTime) {
    c_.has.timestamp_calibration_time = true;
    c_.timestamp_calibration_time     = 0xABABABAB;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 2), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00080000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xABABABAB));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, Temperature) {
    c_.has.temperature = true;
    c_.temperature     = -1.0F;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 2), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00040000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x0000FFC0));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, DeviceIdentifier) {
    c_.has.device_identifier         = true;
    c_.device_identifier.oui         = 0xFFFFFFFF;
    c_.device_identifier.device_code = 0xABAF;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 3), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00020000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00FFFFFF));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x0000ABAF));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, StateAndEventIndicatorsHasCalibratedTime) {
    c_.has.state_and_event_indicators                 = true;
    c_.state_and_event_indicators.has.calibrated_time = true;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 2), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x80000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, StateAndEventIndicatorsHasValidData) {
    c_.has.state_and_event_indicators            = true;
    c_.state_and_event_indicators.has.valid_data = true;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 2), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x40000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, StateAndEventIndicatorsHasReferenceLock) {
    c_.has.state_and_event_indicators                = true;
    c_.state_and_event_indicators.has.reference_lock = true;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 2), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x20000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, StateAndEventIndicatorsHasAgcOrMgc) {
    c_.has.state_and_event_indicators            = true;
    c_.state_and_event_indicators.has.agc_or_mgc = true;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 2), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x10000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, StateAndEventIndicatorsHasDetectedSignal) {
    c_.has.state_and_event_indicators                 = true;
    c_.state_and_event_indicators.has.detected_signal = true;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 2), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x08000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, StateAndEventIndicatorsHasSpectralInversion) {
    c_.has.state_and_event_indicators                    = true;
    c_.state_and_event_indicators.has.spectral_inversion = true;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 2), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x04000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, StateAndEventIndicatorsHasOverRange) {
    c_.has.state_and_event_indicators            = true;
    c_.state_and_event_indicators.has.over_range = true;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 2), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x02000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, StateAndEventIndicatorsHasSampleLoss) {
    c_.has.state_and_event_indicators             = true;
    c_.state_and_event_indicators.has.sample_loss = true;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 2), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x01000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, StateAndEventIndicatorsCalibratedTime) {
    c_.has.state_and_event_indicators             = true;
    c_.state_and_event_indicators.calibrated_time = true;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 2), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, StateAndEventIndicatorsValidData) {
    c_.has.state_and_event_indicators        = true;
    c_.state_and_event_indicators.valid_data = true;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 2), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, StateAndEventIndicatorsReferenceLock) {
    c_.has.state_and_event_indicators            = true;
    c_.state_and_event_indicators.reference_lock = true;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 2), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, StateAndEventIndicatorsAgcOrMgc) {
    c_.has.state_and_event_indicators        = true;
    c_.state_and_event_indicators.agc_or_mgc = true;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 2), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, StateAndEventIndicatorsDetectedSignal) {
    c_.has.state_and_event_indicators             = true;
    c_.state_and_event_indicators.detected_signal = true;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 2), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, StateAndEventIndicatorsSpectralInversion) {
    c_.has.state_and_event_indicators                = true;
    c_.state_and_event_indicators.spectral_inversion = true;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 2), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, StateAndEventIndicatorsOverRange) {
    c_.has.state_and_event_indicators        = true;
    c_.state_and_event_indicators.over_range = true;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 2), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, StateAndEventIndicatorsSampleLoss) {
    c_.has.state_and_event_indicators         = true;
    c_.state_and_event_indicators.sample_loss = true;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 2), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, BothStateAndEventIndicatorsCalibratedTime) {
    c_.has.state_and_event_indicators                 = true;
    c_.state_and_event_indicators.has.calibrated_time = true;
    c_.state_and_event_indicators.calibrated_time     = true;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 2), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x80080000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, BothStateAndEventIndicatorsValidData) {
    c_.has.state_and_event_indicators            = true;
    c_.state_and_event_indicators.has.valid_data = true;
    c_.state_and_event_indicators.valid_data     = true;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 2), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x40040000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, BothStateAndEventIndicatorsReferenceLock) {
    c_.has.state_and_event_indicators                = true;
    c_.state_and_event_indicators.has.reference_lock = true;
    c_.state_and_event_indicators.reference_lock     = true;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 2), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x20020000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, BothStateAndEventIndicatorsAgcOrMgc) {
    c_.has.state_and_event_indicators            = true;
    c_.state_and_event_indicators.has.agc_or_mgc = true;
    c_.state_and_event_indicators.agc_or_mgc     = true;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 2), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x10010000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, BothStateAndEventIndicatorsDetectedSignal) {
    c_.has.state_and_event_indicators                 = true;
    c_.state_and_event_indicators.has.detected_signal = true;
    c_.state_and_event_indicators.detected_signal     = true;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 2), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x08008000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, BothStateAndEventIndicatorsSpectralInversion) {
    c_.has.state_and_event_indicators                    = true;
    c_.state_and_event_indicators.has.spectral_inversion = true;
    c_.state_and_event_indicators.spectral_inversion     = true;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 2), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x04004000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, BothStateAndEventIndicatorsOverRange) {
    c_.has.state_and_event_indicators            = true;
    c_.state_and_event_indicators.has.over_range = true;
    c_.state_and_event_indicators.over_range     = true;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 2), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x02002000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, BothStateAndEventIndicatorsSampleLoss) {
    c_.has.state_and_event_indicators             = true;
    c_.state_and_event_indicators.has.sample_loss = true;
    c_.state_and_event_indicators.sample_loss     = true;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 2), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x01001000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, BothStateAndEventIndicatorsUserDefined7) {
    c_.has.state_and_event_indicators           = true;
    c_.state_and_event_indicators.user_defined7 = true;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 2), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000080));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, BothStateAndEventIndicatorsUserDefined6) {
    c_.has.state_and_event_indicators           = true;
    c_.state_and_event_indicators.user_defined6 = true;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 2), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000040));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, BothStateAndEventIndicatorsUserDefined5) {
    c_.has.state_and_event_indicators           = true;
    c_.state_and_event_indicators.user_defined5 = true;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 2), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000020));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, BothStateAndEventIndicatorsUserDefined4) {
    c_.has.state_and_event_indicators           = true;
    c_.state_and_event_indicators.user_defined4 = true;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 2), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000010));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, BothStateAndEventIndicatorsUserDefined3) {
    c_.has.state_and_event_indicators           = true;
    c_.state_and_event_indicators.user_defined3 = true;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 2), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000008));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, BothStateAndEventIndicatorsUserDefined2) {
    c_.has.state_and_event_indicators           = true;
    c_.state_and_event_indicators.user_defined2 = true;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 2), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000004));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, BothStateAndEventIndicatorsUserDefined1) {
    c_.has.state_and_event_indicators           = true;
    c_.state_and_event_indicators.user_defined1 = true;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 2), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000002));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, BothStateAndEventIndicatorsUserDefined0) {
    c_.has.state_and_event_indicators           = true;
    c_.state_and_event_indicators.user_defined0 = true;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 2), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000001));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, DataPacketPayloadFormatPackingMethod) {
    c_.has.data_packet_payload_format            = true;
    c_.data_packet_payload_format.packing_method = true;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 3), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00008000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x80000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, DataPacketPayloadFormatRealOrComplex1) {
    c_.has.data_packet_payload_format             = true;
    c_.data_packet_payload_format.real_or_complex = VRT_ROC_COMPLEX_POLAR;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 3), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00008000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x40000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, DataPacketPayloadFormatRealOrComplex2) {
    c_.has.data_packet_payload_format             = true;
    c_.data_packet_payload_format.real_or_complex = static_cast<vrt_real_complex>(3);
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 3), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00008000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x60000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, DataPacketPayloadFormatDataItemFormat1) {
    c_.has.data_packet_payload_format              = true;
    c_.data_packet_payload_format.data_item_format = VRT_DIF_UNSIGNED_VRT_6_BIT_EXPONENT;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 3), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00008000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x16000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, DataPacketPayloadFormatDataItemFormat2) {
    c_.has.data_packet_payload_format              = true;
    c_.data_packet_payload_format.data_item_format = static_cast<vrt_data_item_format>(31);
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 3), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00008000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x1F000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, DataPacketPayloadFormatSampleComponentRepeat) {
    c_.has.data_packet_payload_format                     = true;
    c_.data_packet_payload_format.sample_component_repeat = true;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 3), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00008000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00800000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, DataPacketPayloadFormatEventTagSize) {
    c_.has.data_packet_payload_format            = true;
    c_.data_packet_payload_format.event_tag_size = 0xFF;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 3), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00008000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00700000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, DataPacketPayloadFormatChannelTagSize) {
    c_.has.data_packet_payload_format              = true;
    c_.data_packet_payload_format.channel_tag_size = 0xFF;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 3), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00008000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x000F0000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, DataPacketPayloadFormatItemPackingFieldSize) {
    c_.has.data_packet_payload_format                     = true;
    c_.data_packet_payload_format.item_packing_field_size = 0xFF;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 3), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00008000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000FC0));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, DataPacketPayloadFormatDataItemSize) {
    c_.has.data_packet_payload_format            = true;
    c_.data_packet_payload_format.data_item_size = 0xFF;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 3), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00008000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x0000003F));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, DataPacketPayloadFormatRepeatCount) {
    c_.has.data_packet_payload_format          = true;
    c_.data_packet_payload_format.repeat_count = 0xFFFF;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 3), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00008000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xFFFF0000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, DataPacketPayloadFormatVectorSize) {
    c_.has.data_packet_payload_format         = true;
    c_.data_packet_payload_format.vector_size = 0xFFFF;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 3), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00008000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x0000FFFF));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, FormattedGpsGeolocationTsi) {
    c_.has.formatted_gps_geolocation = true;
    c_.formatted_gps_geolocation.tsi = VRT_TSI_OTHER;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 12), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x0C000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[5]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[6]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[7]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[8]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[9]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[10]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[11]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[12]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, FormattedGpsGeolocationTsf) {
    c_.has.formatted_gps_geolocation = true;
    c_.formatted_gps_geolocation.tsf = VRT_TSF_FREE_RUNNING_COUNT;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 12), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x03000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[5]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[6]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[7]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[8]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[9]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[10]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[11]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[12]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, FormattedGpsGeolocationOui) {
    c_.has.formatted_gps_geolocation = true;
    c_.formatted_gps_geolocation.oui = 0xFFFFFFFF;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 12), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00FFFFFF));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[5]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[6]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[7]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[8]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[9]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[10]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[11]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[12]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, FormattedGpsGeolocationIntegerSecondTimestamp) {
    c_.has.formatted_gps_geolocation                      = true;
    c_.formatted_gps_geolocation.tsi                      = VRT_TSI_UTC;
    c_.formatted_gps_geolocation.integer_second_timestamp = 0xABABABAB;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 12), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x04000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xABABABAB));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[5]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[6]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[7]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[8]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[9]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[10]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[11]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[12]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, FormattedGpsGeolocationFractionalSecondTimestamp) {
    c_.has.formatted_gps_geolocation                         = true;
    c_.formatted_gps_geolocation.tsf                         = VRT_TSF_REAL_TIME;
    c_.formatted_gps_geolocation.fractional_second_timestamp = 0xABABABABBABABABA;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 12), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x02000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xABABABAB));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xBABABABA));
    ASSERT_EQ(Hex(buf_[5]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[6]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[7]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[8]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[9]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[10]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[11]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[12]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, FormattedGpsGeolocationLatitude) {
    c_.has.formatted_gps_geolocation      = true;
    c_.formatted_gps_geolocation.latitude = 1.0;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 12), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[5]), Hex(0x00400000));
    ASSERT_EQ(Hex(buf_[6]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[7]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[8]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[9]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[10]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[11]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[12]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, FormattedGpsGeolocationLongitude) {
    c_.has.formatted_gps_geolocation       = true;
    c_.formatted_gps_geolocation.longitude = 1.0;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 12), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[5]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[6]), Hex(0x00400000));
    ASSERT_EQ(Hex(buf_[7]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[8]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[9]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[10]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[11]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[12]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, FormattedGpsGeolocationAltitude) {
    c_.has.formatted_gps_geolocation      = true;
    c_.formatted_gps_geolocation.altitude = 1.0;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 12), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[5]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[6]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[7]), Hex(0x00000020));
    ASSERT_EQ(Hex(buf_[8]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[9]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[10]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[11]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[12]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, FormattedGpsGeolocationSpeedOverGround) {
    c_.has.formatted_gps_geolocation               = true;
    c_.formatted_gps_geolocation.speed_over_ground = 1.0;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 12), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[5]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[6]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[7]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[8]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[9]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[10]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[11]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[12]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, FormattedGpsGeolocationHeadingAngle) {
    c_.has.formatted_gps_geolocation           = true;
    c_.formatted_gps_geolocation.heading_angle = 1.0;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 12), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[5]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[6]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[7]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[8]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[9]), Hex(0x00400000));
    ASSERT_EQ(Hex(buf_[10]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[11]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[12]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, FormattedGpsGeolocationTrackAngle) {
    c_.has.formatted_gps_geolocation         = true;
    c_.formatted_gps_geolocation.track_angle = 1.0;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 12), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[5]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[6]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[7]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[8]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[9]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[10]), Hex(0x00400000));
    ASSERT_EQ(Hex(buf_[11]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[12]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, FormattedGpsGeolocationMagneticVariation) {
    c_.has.formatted_gps_geolocation                = true;
    c_.formatted_gps_geolocation.magnetic_variation = 1.0;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 12), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[5]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[6]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[7]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[8]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[9]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[10]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[11]), Hex(0x00400000));
    ASSERT_EQ(Hex(buf_[12]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, FormattedInsGeolocationTsi) {
    c_.has.formatted_ins_geolocation = true;
    c_.formatted_ins_geolocation.tsi = VRT_TSI_OTHER;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 12), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x0C000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[5]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[6]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[7]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[8]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[9]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[10]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[11]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[12]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, FormattedInsGeolocationTsf) {
    c_.has.formatted_ins_geolocation = true;
    c_.formatted_ins_geolocation.tsf = VRT_TSF_FREE_RUNNING_COUNT;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 12), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x03000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[5]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[6]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[7]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[8]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[9]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[10]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[11]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[12]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, FormattedInsGeolocationOui) {
    c_.has.formatted_ins_geolocation = true;
    c_.formatted_ins_geolocation.oui = 0xFFFFFFFF;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 12), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00FFFFFF));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[5]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[6]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[7]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[8]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[9]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[10]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[11]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[12]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, FormattedInsGeolocationIntegerSecondTimestamp) {
    c_.has.formatted_ins_geolocation                      = true;
    c_.formatted_ins_geolocation.tsi                      = VRT_TSI_UTC;
    c_.formatted_ins_geolocation.integer_second_timestamp = 0xABABABAB;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 12), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x04000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xABABABAB));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[5]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[6]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[7]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[8]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[9]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[10]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[11]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[12]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, FormattedInsGeolocationFractionalSecondTimestamp) {
    c_.has.formatted_ins_geolocation                         = true;
    c_.formatted_ins_geolocation.tsf                         = VRT_TSF_REAL_TIME;
    c_.formatted_ins_geolocation.fractional_second_timestamp = 0xABABABABBABABABA;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 12), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x02000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xABABABAB));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xBABABABA));
    ASSERT_EQ(Hex(buf_[5]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[6]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[7]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[8]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[9]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[10]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[11]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[12]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, FormattedInsGeolocationLatitude) {
    c_.has.formatted_ins_geolocation      = true;
    c_.formatted_ins_geolocation.latitude = 1.0;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 12), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[5]), Hex(0x00400000));
    ASSERT_EQ(Hex(buf_[6]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[7]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[8]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[9]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[10]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[11]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[12]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, FormattedInsGeolocationLongitude) {
    c_.has.formatted_ins_geolocation       = true;
    c_.formatted_ins_geolocation.longitude = 1.0;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 12), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[5]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[6]), Hex(0x00400000));
    ASSERT_EQ(Hex(buf_[7]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[8]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[9]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[10]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[11]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[12]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, FormattedInsGeolocationAltitude) {
    c_.has.formatted_ins_geolocation      = true;
    c_.formatted_ins_geolocation.altitude = 1.0;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 12), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[5]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[6]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[7]), Hex(0x00000020));
    ASSERT_EQ(Hex(buf_[8]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[9]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[10]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[11]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[12]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, FormattedInsGeolocationSpeedOverGround) {
    c_.has.formatted_ins_geolocation               = true;
    c_.formatted_ins_geolocation.speed_over_ground = 1.0;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 12), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[5]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[6]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[7]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[8]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[9]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[10]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[11]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[12]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, FormattedInsGeolocationHeadingAngle) {
    c_.has.formatted_ins_geolocation           = true;
    c_.formatted_ins_geolocation.heading_angle = 1.0;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 12), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[5]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[6]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[7]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[8]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[9]), Hex(0x00400000));
    ASSERT_EQ(Hex(buf_[10]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[11]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[12]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, FormattedInsGeolocationTrackAngle) {
    c_.has.formatted_ins_geolocation         = true;
    c_.formatted_ins_geolocation.track_angle = 1.0;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 12), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[5]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[6]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[7]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[8]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[9]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[10]), Hex(0x00400000));
    ASSERT_EQ(Hex(buf_[11]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[12]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, FormattedInsGeolocationMagneticVariation) {
    c_.has.formatted_ins_geolocation                = true;
    c_.formatted_ins_geolocation.magnetic_variation = 1.0;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 12), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[5]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[6]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[7]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[8]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[9]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[10]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[11]), Hex(0x00400000));
    ASSERT_EQ(Hex(buf_[12]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, EcefEphemerisTsi) {
    c_.has.ecef_ephemeris = true;
    c_.ecef_ephemeris.tsi = VRT_TSI_OTHER;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 14), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x0C000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[5]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[6]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[7]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[8]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[9]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[10]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[11]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[12]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[13]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[14]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, EcefEphemerisTsf) {
    c_.has.ecef_ephemeris = true;
    c_.ecef_ephemeris.tsf = VRT_TSF_FREE_RUNNING_COUNT;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 14), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x03000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[5]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[6]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[7]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[8]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[9]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[10]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[11]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[12]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[13]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[14]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, EcefEphemerisOui) {
    c_.has.ecef_ephemeris = true;
    c_.ecef_ephemeris.oui = 0xFFFFFFFF;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 14), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00FFFFFF));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[5]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[6]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[7]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[8]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[9]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[10]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[11]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[12]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[13]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[14]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, EcefEphemerisIntegerSecondTimestamp) {
    c_.has.ecef_ephemeris                      = true;
    c_.ecef_ephemeris.tsi                      = VRT_TSI_UTC;
    c_.ecef_ephemeris.integer_second_timestamp = 0xABABABAB;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 14), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x04000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xABABABAB));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[5]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[6]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[7]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[8]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[9]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[10]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[11]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[12]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[13]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[14]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, EcefEphemerisFractionalSecondTimestamp) {
    c_.has.ecef_ephemeris                         = true;
    c_.ecef_ephemeris.tsf                         = VRT_TSF_REAL_TIME;
    c_.ecef_ephemeris.fractional_second_timestamp = 0xABABABABBABABABA;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 14), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x02000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xABABABAB));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xBABABABA));
    ASSERT_EQ(Hex(buf_[5]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[6]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[7]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[8]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[9]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[10]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[11]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[12]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[13]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[14]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, EcefEphemerisPositionX) {
    c_.has.ecef_ephemeris        = true;
    c_.ecef_ephemeris.position_x = 1.0;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 14), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[5]), Hex(0x00000020));
    ASSERT_EQ(Hex(buf_[6]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[7]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[8]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[9]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[10]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[11]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[12]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[13]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[14]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, EcefEphemerisPositionY) {
    c_.has.ecef_ephemeris        = true;
    c_.ecef_ephemeris.position_y = 1.0;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 14), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[5]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[6]), Hex(0x00000020));
    ASSERT_EQ(Hex(buf_[7]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[8]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[9]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[10]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[11]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[12]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[13]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[14]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, EcefEphemerisPositionZ) {
    c_.has.ecef_ephemeris        = true;
    c_.ecef_ephemeris.position_z = 1.0;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 14), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[5]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[6]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[7]), Hex(0x00000020));
    ASSERT_EQ(Hex(buf_[8]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[9]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[10]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[11]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[12]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[13]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[14]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, EcefEphemerisAltitudeAlpha) {
    c_.has.ecef_ephemeris            = true;
    c_.ecef_ephemeris.altitude_alpha = 1.0;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 14), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[5]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[6]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[7]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[8]), Hex(0x00400000));
    ASSERT_EQ(Hex(buf_[9]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[10]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[11]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[12]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[13]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[14]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, EcefEphemerisAltitudeBeta) {
    c_.has.ecef_ephemeris           = true;
    c_.ecef_ephemeris.altitude_beta = 1.0;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 14), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[5]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[6]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[7]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[8]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[9]), Hex(0x00400000));
    ASSERT_EQ(Hex(buf_[10]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[11]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[12]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[13]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[14]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, EcefEphemerisAltitudePhi) {
    c_.has.ecef_ephemeris          = true;
    c_.ecef_ephemeris.altitude_phi = 1.0;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 14), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[5]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[6]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[7]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[8]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[9]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[10]), Hex(0x00400000));
    ASSERT_EQ(Hex(buf_[11]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[12]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[13]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[14]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, EcefEphemerisVelocityDx) {
    c_.has.ecef_ephemeris         = true;
    c_.ecef_ephemeris.velocity_dx = 1.0;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 14), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[5]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[6]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[7]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[8]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[9]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[10]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[11]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[12]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[13]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[14]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, EcefEphemerisVelocityDy) {
    c_.has.ecef_ephemeris         = true;
    c_.ecef_ephemeris.velocity_dy = 1.0;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 14), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[5]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[6]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[7]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[8]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[9]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[10]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[11]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[12]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[13]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[14]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, EcefEphemerisVelocityDz) {
    c_.has.ecef_ephemeris         = true;
    c_.ecef_ephemeris.velocity_dz = 1.0;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 14), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[5]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[6]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[7]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[8]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[9]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[10]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[11]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[12]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[13]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[14]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, RelativeEphemerisTsi) {
    c_.has.relative_ephemeris = true;
    c_.relative_ephemeris.tsi = VRT_TSI_OTHER;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 14), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x0C000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[5]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[6]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[7]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[8]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[9]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[10]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[11]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[12]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[13]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[14]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, RelativeEphemerisTsf) {
    c_.has.relative_ephemeris = true;
    c_.relative_ephemeris.tsf = VRT_TSF_FREE_RUNNING_COUNT;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 14), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x03000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[5]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[6]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[7]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[8]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[9]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[10]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[11]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[12]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[13]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[14]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, RelativeEphemerisOui) {
    c_.has.relative_ephemeris = true;
    c_.relative_ephemeris.oui = 0xFFFFFFFF;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 14), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00FFFFFF));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[5]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[6]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[7]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[8]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[9]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[10]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[11]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[12]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[13]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[14]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, RelativeEphemerisIntegerSecondTimestamp) {
    c_.has.relative_ephemeris                      = true;
    c_.relative_ephemeris.tsi                      = VRT_TSI_UTC;
    c_.relative_ephemeris.integer_second_timestamp = 0xABABABAB;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 14), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x04000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xABABABAB));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[5]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[6]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[7]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[8]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[9]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[10]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[11]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[12]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[13]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[14]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, RelativeEphemerisFractionalSecondTimestamp) {
    c_.has.relative_ephemeris                         = true;
    c_.relative_ephemeris.tsf                         = VRT_TSF_REAL_TIME;
    c_.relative_ephemeris.fractional_second_timestamp = 0xABABABABBABABABA;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 14), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x02000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xABABABAB));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xBABABABA));
    ASSERT_EQ(Hex(buf_[5]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[6]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[7]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[8]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[9]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[10]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[11]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[12]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[13]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[14]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, RelativeEphemerisPositionX) {
    c_.has.relative_ephemeris        = true;
    c_.relative_ephemeris.position_x = 1.0;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 14), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[5]), Hex(0x00000020));
    ASSERT_EQ(Hex(buf_[6]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[7]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[8]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[9]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[10]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[11]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[12]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[13]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[14]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, RelativeEphemerisPositionY) {
    c_.has.relative_ephemeris        = true;
    c_.relative_ephemeris.position_y = 1.0;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 14), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[5]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[6]), Hex(0x00000020));
    ASSERT_EQ(Hex(buf_[7]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[8]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[9]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[10]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[11]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[12]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[13]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[14]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, RelativeEphemerisPositionZ) {
    c_.has.relative_ephemeris        = true;
    c_.relative_ephemeris.position_z = 1.0;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 14), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[5]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[6]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[7]), Hex(0x00000020));
    ASSERT_EQ(Hex(buf_[8]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[9]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[10]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[11]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[12]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[13]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[14]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, RelativeEphemerisAltitudeAlpha) {
    c_.has.relative_ephemeris            = true;
    c_.relative_ephemeris.altitude_alpha = 1.0;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 14), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[5]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[6]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[7]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[8]), Hex(0x00400000));
    ASSERT_EQ(Hex(buf_[9]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[10]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[11]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[12]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[13]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[14]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, RelativeEphemerisAltitudeBeta) {
    c_.has.relative_ephemeris           = true;
    c_.relative_ephemeris.altitude_beta = 1.0;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 14), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[5]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[6]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[7]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[8]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[9]), Hex(0x00400000));
    ASSERT_EQ(Hex(buf_[10]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[11]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[12]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[13]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[14]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, RelativeEphemerisAltitudePhi) {
    c_.has.relative_ephemeris          = true;
    c_.relative_ephemeris.altitude_phi = 1.0;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 14), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[5]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[6]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[7]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[8]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[9]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[10]), Hex(0x00400000));
    ASSERT_EQ(Hex(buf_[11]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[12]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[13]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[14]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, RelativeEphemerisVelocityDx) {
    c_.has.relative_ephemeris         = true;
    c_.relative_ephemeris.velocity_dx = 1.0;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 14), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[5]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[6]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[7]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[8]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[9]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[10]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[11]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[12]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[13]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[14]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, RelativeEphemerisVelocityDy) {
    c_.has.relative_ephemeris         = true;
    c_.relative_ephemeris.velocity_dy = 1.0;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 14), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[5]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[6]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[7]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[8]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[9]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[10]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[11]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[12]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[13]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[14]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, RelativeEphemerisVelocityDz) {
    c_.has.relative_ephemeris         = true;
    c_.relative_ephemeris.velocity_dz = 1.0;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 14), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[5]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[6]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[7]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[8]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[9]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[10]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[11]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[12]), Hex(0x7FFFFFFF));
    ASSERT_EQ(Hex(buf_[13]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[14]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, EphemerisReferenceIdentifier) {
    c_.has.ephemeris_reference_identifier = true;
    c_.ephemeris_reference_identifier     = 0xFEFEFEFE;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 2), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000400));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xFEFEFEFE));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, GpsAsciiOui) {
    c_.has.gps_ascii = true;
    c_.gps_ascii.oui = 0xFFFFFFFF;
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 3), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000200));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00FFFFFF));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, GpsAsciiAscii) {
    c_.has.gps_ascii             = true;
    c_.gps_ascii.number_of_words = 5;
    c_.gps_ascii.ascii           = "What's your name?";
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 8), 8);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000200));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x00000005));
    ASSERT_EQ(std::memcmp(c_.gps_ascii.ascii, buf_.data() + 3, sizeof(uint32_t) * c_.gps_ascii.number_of_words), 0);
    ASSERT_EQ(Hex(buf_[8]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, ContextAssociationListsSource) {
    c_.has.context_association_lists                             = true;
    c_.context_association_lists.source_list_size                = 0xFFFF;
    std::array<uint32_t, 0x1FF> l                                = {};
    l[0]                                                         = 0xABABABAB;
    l[0x1FE]                                                     = 0xCBCBCBCB;
    c_.context_association_lists.source_context_association_list = l.data();
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 0x202), 0x202);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000100));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x01FF0000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xABABABAB));
    ASSERT_EQ(Hex(buf_[0x201]), Hex(0xCBCBCBCB));
    ASSERT_EQ(Hex(buf_[0x202]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, ContextAssociationListsSystem) {
    c_.has.context_association_lists                             = true;
    c_.context_association_lists.system_list_size                = 0xFFFF;
    std::array<uint32_t, 0x1FF> l                                = {};
    l[0]                                                         = 0xABABABAB;
    l[0x1FE]                                                     = 0xCBCBCBCB;
    c_.context_association_lists.system_context_association_list = l.data();
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 0x202), 0x202);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000100));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x000001FF));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xABABABAB));
    ASSERT_EQ(Hex(buf_[0x201]), Hex(0xCBCBCBCB));
    ASSERT_EQ(Hex(buf_[0x202]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, ContextAssociationListsVectorComponent) {
    c_.has.context_association_lists                                       = true;
    c_.context_association_lists.vector_component_list_size                = 0x1FF;
    std::array<uint32_t, 0x1FF> l                                          = {};
    l[0]                                                                   = 0xABABABAB;
    l[0x1FE]                                                               = 0xCBCBCBCB;
    c_.context_association_lists.vector_component_context_association_list = l.data();
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 0x202), 0x202);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000100));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x01FF0000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xABABABAB));
    ASSERT_EQ(Hex(buf_[0x201]), Hex(0xCBCBCBCB));
    ASSERT_EQ(Hex(buf_[0x202]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, ContextAssociationListsAsynchronous) {
    c_.has.context_association_lists                                           = true;
    c_.context_association_lists.asynchronous_channel_list_size                = 0x1FF;
    std::array<uint32_t, 0x1FF> l                                              = {};
    l[0]                                                                       = 0xABABABAB;
    l[0x1FE]                                                                   = 0xCBCBCBCB;
    c_.context_association_lists.asynchronous_channel_context_association_list = l.data();
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 0x202), 0x202);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000100));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x000001FF));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xABABABAB));
    ASSERT_EQ(Hex(buf_[0x201]), Hex(0xCBCBCBCB));
    ASSERT_EQ(Hex(buf_[0x202]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, ContextAssociationListsAsynchronousTag) {
    c_.has.context_association_lists                                           = true;
    c_.context_association_lists.asynchronous_channel_list_size                = 3;
    c_.context_association_lists.has.asynchronous_channel_tag_list             = true;
    std::array<uint32_t, 3> l1                                                 = {};
    l1[0]                                                                      = 0xABABABAB;
    l1[1]                                                                      = 0xBCBCBCBC;
    l1[2]                                                                      = 0xCDCDCDCD;
    std::array<uint32_t, 3> l2                                                 = {};
    l2[0]                                                                      = 0xABABABAB;
    l2[1]                                                                      = 0xBCBCBCBC;
    l2[2]                                                                      = 0xCDCDCDCD;
    c_.context_association_lists.asynchronous_channel_context_association_list = l1.data();
    c_.context_association_lists.asynchronous_channel_tag_list                 = l2.data();
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 9), 9);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000100));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x00008003));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xABABABAB));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xBCBCBCBC));
    ASSERT_EQ(Hex(buf_[5]), Hex(0xCDCDCDCD));
    ASSERT_EQ(Hex(buf_[6]), Hex(0xABABABAB));
    ASSERT_EQ(Hex(buf_[7]), Hex(0xBCBCBCBC));
    ASSERT_EQ(Hex(buf_[8]), Hex(0xCDCDCDCD));
    ASSERT_EQ(Hex(buf_[9]), Hex(0xBAADF00D));
}

TEST_F(WriteContextTest, EveryOther1) {
    c_.context_field_change_indicator = true;
    c_.has.bandwidth                  = true;
    c_.has.rf_reference_frequency     = true;
    c_.has.if_band_offset             = true;
    c_.has.gain                       = true;
    c_.has.sample_rate                = true;
    c_.has.timestamp_calibration_time = true;
    c_.has.device_identifier          = true;
    c_.has.data_packet_payload_format = true;
    c_.has.formatted_ins_geolocation  = true;
    c_.has.relative_ephemeris         = true;
    c_.has.gps_ascii                  = true;

    c_.bandwidth                     = -1.0;
    c_.rf_reference_frequency        = -1.0;
    c_.if_band_offset                = -1.0;
    c_.gain.stage1                   = -1.0F;
    c_.gain.stage2                   = -1.0F;
    c_.sample_rate                   = -1.0;
    c_.timestamp_calibration_time    = 0xBABABABA;
    c_.device_identifier.oui         = 0xFEBEFEBE;
    c_.device_identifier.device_code = 0xBEBA;

    c_.data_packet_payload_format.packing_method          = true;
    c_.data_packet_payload_format.real_or_complex         = VRT_ROC_COMPLEX_POLAR;
    c_.data_packet_payload_format.data_item_format        = VRT_DIF_UNSIGNED_VRT_6_BIT_EXPONENT;
    c_.data_packet_payload_format.sample_component_repeat = true;
    c_.data_packet_payload_format.event_tag_size          = 0xFF;
    c_.data_packet_payload_format.channel_tag_size        = 0xFF;
    c_.data_packet_payload_format.item_packing_field_size = 0xFF;
    c_.data_packet_payload_format.data_item_size          = 0xFF;
    c_.data_packet_payload_format.repeat_count            = 0xFFFF;
    c_.data_packet_payload_format.vector_size             = 0xFFFF;

    c_.formatted_ins_geolocation.tsi                         = VRT_TSI_OTHER;
    c_.formatted_ins_geolocation.tsf                         = VRT_TSF_FREE_RUNNING_COUNT;
    c_.formatted_ins_geolocation.oui                         = 0xFFFFFFFF;
    c_.formatted_ins_geolocation.integer_second_timestamp    = 0xABABBABA;
    c_.formatted_ins_geolocation.fractional_second_timestamp = 0xBEBEBABACECECBCB;
    c_.formatted_ins_geolocation.latitude                    = -1.0;
    c_.formatted_ins_geolocation.longitude                   = -1.0;
    c_.formatted_ins_geolocation.altitude                    = -1.0;
    c_.formatted_ins_geolocation.speed_over_ground           = -1.0;
    c_.formatted_ins_geolocation.heading_angle               = -1.0;
    c_.formatted_ins_geolocation.track_angle                 = -1.0;
    c_.formatted_ins_geolocation.magnetic_variation          = -1.0;

    c_.relative_ephemeris.tsi                         = VRT_TSI_OTHER;
    c_.relative_ephemeris.tsf                         = VRT_TSF_FREE_RUNNING_COUNT;
    c_.relative_ephemeris.oui                         = 0xFFFFFFFF;
    c_.relative_ephemeris.integer_second_timestamp    = 0xABABBABA;
    c_.relative_ephemeris.fractional_second_timestamp = 0xBEBEBABACECECBCB;
    c_.relative_ephemeris.position_x                  = -1.0;
    c_.relative_ephemeris.position_y                  = -1.0;
    c_.relative_ephemeris.position_z                  = -1.0;
    c_.relative_ephemeris.altitude_alpha              = -1.0;
    c_.relative_ephemeris.altitude_beta               = -1.0;
    c_.relative_ephemeris.altitude_phi                = -1.0;
    c_.relative_ephemeris.velocity_dx                 = -1.0;
    c_.relative_ephemeris.velocity_dy                 = -1.0;
    c_.relative_ephemeris.velocity_dz                 = -1.0;

    c_.gps_ascii.oui             = 0xFFFFFFFF;
    c_.gps_ascii.number_of_words = 3;
    c_.gps_ascii.ascii           = "Lorem ipsum!";

    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 43), VRT_ERR);
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 44), 44);
    ASSERT_EQ(Hex(buf_[0]), Hex(0xAAAAAA00)); /* Context indicators */
    ASSERT_EQ(Hex(buf_[1]), Hex(0xFFFFFFFF)); /* Bandwidth */
    ASSERT_EQ(Hex(buf_[2]), Hex(0xFFF00000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xFFFFFFFF)); /* RF reference frequency */
    ASSERT_EQ(Hex(buf_[4]), Hex(0xFFF00000));
    ASSERT_EQ(Hex(buf_[5]), Hex(0xFFFFFFFF)); /* IF band offset */
    ASSERT_EQ(Hex(buf_[6]), Hex(0xFFF00000));
    ASSERT_EQ(Hex(buf_[7]), Hex(0xFF80FF80)); /* Gain stage 1 & 2 */
    ASSERT_EQ(Hex(buf_[8]), Hex(0xFFFFFFFF)); /* Sample rate */
    ASSERT_EQ(Hex(buf_[9]), Hex(0xFFF00000));
    ASSERT_EQ(Hex(buf_[10]), Hex(0xBABABABA)); /* Timestamp calibration time */
    ASSERT_EQ(Hex(buf_[11]), Hex(0x00BEFEBE)); /* Device identifier.oui */
    ASSERT_EQ(Hex(buf_[12]), Hex(0x0000BEBA)); /* Device identifier.device_code */
    ASSERT_EQ(Hex(buf_[13]), Hex(0xD6FF0FFF)); /* Data packet payload format */
    ASSERT_EQ(Hex(buf_[14]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[15]), Hex(0x0FFFFFFF)); /* Formatted INS geolocation */
    ASSERT_EQ(Hex(buf_[16]), Hex(0xABABBABA));
    ASSERT_EQ(Hex(buf_[17]), Hex(0xBEBEBABA));
    ASSERT_EQ(Hex(buf_[18]), Hex(0xCECECBCB));
    ASSERT_EQ(Hex(buf_[19]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[20]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[21]), Hex(0xFFFFFFE0));
    ASSERT_EQ(Hex(buf_[22]), Hex(0xFFFF0000));
    ASSERT_EQ(Hex(buf_[23]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[24]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[25]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[26]), Hex(0x0FFFFFFF)); /* Relative ephemeris */
    ASSERT_EQ(Hex(buf_[27]), Hex(0xABABBABA));
    ASSERT_EQ(Hex(buf_[28]), Hex(0xBEBEBABA));
    ASSERT_EQ(Hex(buf_[29]), Hex(0xCECECBCB));
    ASSERT_EQ(Hex(buf_[30]), Hex(0xFFFFFFE0));
    ASSERT_EQ(Hex(buf_[31]), Hex(0xFFFFFFE0));
    ASSERT_EQ(Hex(buf_[32]), Hex(0xFFFFFFE0));
    ASSERT_EQ(Hex(buf_[33]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[34]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[35]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[36]), Hex(0xFFFF0000));
    ASSERT_EQ(Hex(buf_[37]), Hex(0xFFFF0000));
    ASSERT_EQ(Hex(buf_[38]), Hex(0xFFFF0000));
    ASSERT_EQ(Hex(buf_[39]), Hex(0x00FFFFFF)); /* GPS ASCII */
    ASSERT_EQ(Hex(buf_[40]), Hex(0x00000003));
    ASSERT_EQ(Hex(buf_[41]), Hex(0x65726F4C));
    ASSERT_EQ(Hex(buf_[42]), Hex(0x7069206D));
    ASSERT_EQ(Hex(buf_[43]), Hex(0x216D7573));
}

TEST_F(WriteContextTest, EveryOther2) {
    c_.has.reference_point_identifier     = true;
    c_.has.if_reference_frequency         = true;
    c_.has.rf_reference_frequency_offset  = true;
    c_.has.reference_level                = true;
    c_.has.over_range_count               = true;
    c_.has.timestamp_adjustment           = true;
    c_.has.temperature                    = true;
    c_.has.state_and_event_indicators     = true;
    c_.has.formatted_gps_geolocation      = true;
    c_.has.ecef_ephemeris                 = true;
    c_.has.ephemeris_reference_identifier = true;
    c_.has.context_association_lists      = true;

    c_.reference_point_identifier    = 0xACACACAC;
    c_.if_reference_frequency        = -1.0;
    c_.rf_reference_frequency_offset = -1.0;
    c_.reference_level               = -1.0F;
    c_.over_range_count              = 0xBDBDBDBD;
    c_.timestamp_adjustment          = 0xABCABCABCABCABCA;
    c_.temperature                   = -1.0F;

    c_.state_and_event_indicators.has.calibrated_time    = true;
    c_.state_and_event_indicators.has.valid_data         = true;
    c_.state_and_event_indicators.has.reference_lock     = true;
    c_.state_and_event_indicators.has.agc_or_mgc         = true;
    c_.state_and_event_indicators.has.detected_signal    = true;
    c_.state_and_event_indicators.has.spectral_inversion = true;
    c_.state_and_event_indicators.has.over_range         = true;
    c_.state_and_event_indicators.has.sample_loss        = true;
    c_.state_and_event_indicators.calibrated_time        = true;
    c_.state_and_event_indicators.valid_data             = true;
    c_.state_and_event_indicators.reference_lock         = true;
    c_.state_and_event_indicators.agc_or_mgc             = true;
    c_.state_and_event_indicators.detected_signal        = true;
    c_.state_and_event_indicators.spectral_inversion     = true;
    c_.state_and_event_indicators.over_range             = true;
    c_.state_and_event_indicators.sample_loss            = true;
    c_.state_and_event_indicators.user_defined7          = true;
    c_.state_and_event_indicators.user_defined6          = true;
    c_.state_and_event_indicators.user_defined5          = true;
    c_.state_and_event_indicators.user_defined4          = true;
    c_.state_and_event_indicators.user_defined3          = true;
    c_.state_and_event_indicators.user_defined2          = true;
    c_.state_and_event_indicators.user_defined1          = true;
    c_.state_and_event_indicators.user_defined0          = true;

    c_.formatted_gps_geolocation.tsi                         = VRT_TSI_OTHER;
    c_.formatted_gps_geolocation.tsf                         = VRT_TSF_FREE_RUNNING_COUNT;
    c_.formatted_gps_geolocation.oui                         = 0xFFFFFFFF;
    c_.formatted_gps_geolocation.integer_second_timestamp    = 0xABABBABA;
    c_.formatted_gps_geolocation.fractional_second_timestamp = 0xBEBEBABACECECBCB;
    c_.formatted_gps_geolocation.latitude                    = -1.0;
    c_.formatted_gps_geolocation.longitude                   = -1.0;
    c_.formatted_gps_geolocation.altitude                    = -1.0;
    c_.formatted_gps_geolocation.speed_over_ground           = -1.0;
    c_.formatted_gps_geolocation.heading_angle               = -1.0;
    c_.formatted_gps_geolocation.track_angle                 = -1.0;
    c_.formatted_gps_geolocation.magnetic_variation          = -1.0;

    c_.ecef_ephemeris.tsi                         = VRT_TSI_OTHER;
    c_.ecef_ephemeris.tsf                         = VRT_TSF_FREE_RUNNING_COUNT;
    c_.ecef_ephemeris.oui                         = 0xFFFFFFFF;
    c_.ecef_ephemeris.integer_second_timestamp    = 0xABABBABA;
    c_.ecef_ephemeris.fractional_second_timestamp = 0xBEBEBABACECECBCB;
    c_.ecef_ephemeris.position_x                  = -1.0;
    c_.ecef_ephemeris.position_y                  = -1.0;
    c_.ecef_ephemeris.position_z                  = -1.0;
    c_.ecef_ephemeris.altitude_alpha              = -1.0;
    c_.ecef_ephemeris.altitude_beta               = -1.0;
    c_.ecef_ephemeris.altitude_phi                = -1.0;
    c_.ecef_ephemeris.velocity_dx                 = -1.0;
    c_.ecef_ephemeris.velocity_dy                 = -1.0;
    c_.ecef_ephemeris.velocity_dz                 = -1.0;

    c_.ephemeris_reference_identifier = 0xDEDEDEDE;

    std::array<uint32_t, 2> l1                                                 = {0x12345678, 0x23456789};
    std::array<uint32_t, 2> l2                                                 = {0x3456789A, 0x456789AB};
    std::array<uint32_t, 2> l3                                                 = {0x56789ABC, 0x6789ABCD};
    std::array<uint32_t, 2> l4                                                 = {0x789ABCDE, 0x89ABCDEF};
    std::array<uint32_t, 2> l5                                                 = {0x9ABCDEF0, 0xABCDEF01};
    c_.context_association_lists.has.asynchronous_channel_tag_list             = true;
    c_.context_association_lists.source_list_size                              = l1.size();
    c_.context_association_lists.system_list_size                              = l2.size();
    c_.context_association_lists.vector_component_list_size                    = l3.size();
    c_.context_association_lists.asynchronous_channel_list_size                = l4.size();
    c_.context_association_lists.source_context_association_list               = l1.data();
    c_.context_association_lists.system_context_association_list               = l2.data();
    c_.context_association_lists.vector_component_context_association_list     = l3.data();
    c_.context_association_lists.asynchronous_channel_context_association_list = l4.data();
    c_.context_association_lists.asynchronous_channel_tag_list                 = l5.data();

    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 48), VRT_ERR);
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 49), 49);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x55555500)); /* Context indicators */
    ASSERT_EQ(Hex(buf_[1]), Hex(0xACACACAC)); /* Reference point identifier */
    ASSERT_EQ(Hex(buf_[2]), Hex(0xFFFFFFFF)); /* IF reference frequency */
    ASSERT_EQ(Hex(buf_[3]), Hex(0xFFF00000));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xFFFFFFFF)); /* RF reference frequency offset */
    ASSERT_EQ(Hex(buf_[5]), Hex(0xFFF00000));
    ASSERT_EQ(Hex(buf_[6]), Hex(0x0000FF80)); /* Reference level */
    ASSERT_EQ(Hex(buf_[7]), Hex(0xBDBDBDBD)); /* Over-range count */
    ASSERT_EQ(Hex(buf_[8]), Hex(0xABCABCAB)); /* Timestamp adjustment */
    ASSERT_EQ(Hex(buf_[9]), Hex(0xCABCABCA));
    ASSERT_EQ(Hex(buf_[10]), Hex(0x0000FFC0)); /* Temperature */
    ASSERT_EQ(Hex(buf_[11]), Hex(0xFF0FF0FF)); /* State and event indicators */
    ASSERT_EQ(Hex(buf_[12]), Hex(0x0FFFFFFF)); /* Formatted GPS geolocation */
    ASSERT_EQ(Hex(buf_[13]), Hex(0xABABBABA));
    ASSERT_EQ(Hex(buf_[14]), Hex(0xBEBEBABA));
    ASSERT_EQ(Hex(buf_[15]), Hex(0xCECECBCB));
    ASSERT_EQ(Hex(buf_[16]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[17]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[18]), Hex(0xFFFFFFE0));
    ASSERT_EQ(Hex(buf_[19]), Hex(0xFFFF0000));
    ASSERT_EQ(Hex(buf_[20]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[21]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[22]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[23]), Hex(0x0FFFFFFF)); /* ECEF ephemeris */
    ASSERT_EQ(Hex(buf_[24]), Hex(0xABABBABA));
    ASSERT_EQ(Hex(buf_[25]), Hex(0xBEBEBABA));
    ASSERT_EQ(Hex(buf_[26]), Hex(0xCECECBCB));
    ASSERT_EQ(Hex(buf_[27]), Hex(0xFFFFFFE0));
    ASSERT_EQ(Hex(buf_[28]), Hex(0xFFFFFFE0));
    ASSERT_EQ(Hex(buf_[29]), Hex(0xFFFFFFE0));
    ASSERT_EQ(Hex(buf_[30]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[31]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[32]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[33]), Hex(0xFFFF0000));
    ASSERT_EQ(Hex(buf_[34]), Hex(0xFFFF0000));
    ASSERT_EQ(Hex(buf_[35]), Hex(0xFFFF0000));
    ASSERT_EQ(Hex(buf_[36]), Hex(0xDEDEDEDE)); /* Ephemeris reference identifier */
    ASSERT_EQ(Hex(buf_[37]), Hex(0x00020002)); /* Context association lists */
    ASSERT_EQ(Hex(buf_[38]), Hex(0x00028002));
    ASSERT_EQ(Hex(buf_[39]), Hex(0x12345678));
    ASSERT_EQ(Hex(buf_[40]), Hex(0x23456789));
    ASSERT_EQ(Hex(buf_[41]), Hex(0x3456789A));
    ASSERT_EQ(Hex(buf_[42]), Hex(0x456789AB));
    ASSERT_EQ(Hex(buf_[43]), Hex(0x56789ABC));
    ASSERT_EQ(Hex(buf_[44]), Hex(0x6789ABCD));
    ASSERT_EQ(Hex(buf_[45]), Hex(0x789ABCDE));
    ASSERT_EQ(Hex(buf_[46]), Hex(0x89ABCDEF));
    ASSERT_EQ(Hex(buf_[47]), Hex(0x9ABCDEF0));
    ASSERT_EQ(Hex(buf_[48]), Hex(0xABCDEF01));
}

TEST_F(WriteContextTest, All) {
    c_.context_field_change_indicator     = true;
    c_.has.reference_point_identifier     = true;
    c_.has.bandwidth                      = true;
    c_.has.if_reference_frequency         = true;
    c_.has.rf_reference_frequency         = true;
    c_.has.rf_reference_frequency_offset  = true;
    c_.has.if_band_offset                 = true;
    c_.has.reference_level                = true;
    c_.has.gain                           = true;
    c_.has.over_range_count               = true;
    c_.has.sample_rate                    = true;
    c_.has.timestamp_adjustment           = true;
    c_.has.timestamp_calibration_time     = true;
    c_.has.temperature                    = true;
    c_.has.device_identifier              = true;
    c_.has.state_and_event_indicators     = true;
    c_.has.data_packet_payload_format     = true;
    c_.has.formatted_gps_geolocation      = true;
    c_.has.formatted_ins_geolocation      = true;
    c_.has.ecef_ephemeris                 = true;
    c_.has.relative_ephemeris             = true;
    c_.has.ephemeris_reference_identifier = true;
    c_.has.gps_ascii                      = true;
    c_.has.context_association_lists      = true;

    c_.reference_point_identifier    = 0xACACACAC;
    c_.bandwidth                     = -1.0;
    c_.if_reference_frequency        = -1.0;
    c_.rf_reference_frequency        = -1.0;
    c_.rf_reference_frequency_offset = -1.0;
    c_.if_band_offset                = -1.0;
    c_.reference_level               = -1.0F;
    c_.gain.stage1                   = -1.0F;
    c_.gain.stage2                   = -1.0F;
    c_.over_range_count              = 0xBDBDBDBD;
    c_.sample_rate                   = -1.0;
    c_.timestamp_adjustment          = 0xABCABCABCABCABCA;
    c_.timestamp_calibration_time    = 0xBABABABA;
    c_.temperature                   = -1.0F;
    c_.device_identifier.oui         = 0xFEBEFEBE;
    c_.device_identifier.device_code = 0xBEBA;

    c_.state_and_event_indicators.has.calibrated_time    = true;
    c_.state_and_event_indicators.has.valid_data         = true;
    c_.state_and_event_indicators.has.reference_lock     = true;
    c_.state_and_event_indicators.has.agc_or_mgc         = true;
    c_.state_and_event_indicators.has.detected_signal    = true;
    c_.state_and_event_indicators.has.spectral_inversion = true;
    c_.state_and_event_indicators.has.over_range         = true;
    c_.state_and_event_indicators.has.sample_loss        = true;
    c_.state_and_event_indicators.calibrated_time        = true;
    c_.state_and_event_indicators.valid_data             = true;
    c_.state_and_event_indicators.reference_lock         = true;
    c_.state_and_event_indicators.agc_or_mgc             = true;
    c_.state_and_event_indicators.detected_signal        = true;
    c_.state_and_event_indicators.spectral_inversion     = true;
    c_.state_and_event_indicators.over_range             = true;
    c_.state_and_event_indicators.sample_loss            = true;
    c_.state_and_event_indicators.user_defined7          = true;
    c_.state_and_event_indicators.user_defined6          = true;
    c_.state_and_event_indicators.user_defined5          = true;
    c_.state_and_event_indicators.user_defined4          = true;
    c_.state_and_event_indicators.user_defined3          = true;
    c_.state_and_event_indicators.user_defined2          = true;
    c_.state_and_event_indicators.user_defined1          = true;
    c_.state_and_event_indicators.user_defined0          = true;

    c_.data_packet_payload_format.packing_method          = true;
    c_.data_packet_payload_format.real_or_complex         = VRT_ROC_COMPLEX_POLAR;
    c_.data_packet_payload_format.data_item_format        = VRT_DIF_UNSIGNED_VRT_6_BIT_EXPONENT;
    c_.data_packet_payload_format.sample_component_repeat = true;
    c_.data_packet_payload_format.event_tag_size          = 0xFF;
    c_.data_packet_payload_format.channel_tag_size        = 0xFF;
    c_.data_packet_payload_format.item_packing_field_size = 0xFF;
    c_.data_packet_payload_format.data_item_size          = 0xFF;
    c_.data_packet_payload_format.repeat_count            = 0xFFFF;
    c_.data_packet_payload_format.vector_size             = 0xFFFF;

    c_.formatted_gps_geolocation.tsi                         = VRT_TSI_OTHER;
    c_.formatted_gps_geolocation.tsf                         = VRT_TSF_FREE_RUNNING_COUNT;
    c_.formatted_gps_geolocation.oui                         = 0xFFFFFFFF;
    c_.formatted_gps_geolocation.integer_second_timestamp    = 0xABABBABA;
    c_.formatted_gps_geolocation.fractional_second_timestamp = 0xBEBEBABACECECBCB;
    c_.formatted_gps_geolocation.latitude                    = -1.0;
    c_.formatted_gps_geolocation.longitude                   = -1.0;
    c_.formatted_gps_geolocation.altitude                    = -1.0;
    c_.formatted_gps_geolocation.speed_over_ground           = -1.0;
    c_.formatted_gps_geolocation.heading_angle               = -1.0;
    c_.formatted_gps_geolocation.track_angle                 = -1.0;
    c_.formatted_gps_geolocation.magnetic_variation          = -1.0;

    c_.formatted_ins_geolocation.tsi                         = VRT_TSI_OTHER;
    c_.formatted_ins_geolocation.tsf                         = VRT_TSF_FREE_RUNNING_COUNT;
    c_.formatted_ins_geolocation.oui                         = 0xFFFFFFFF;
    c_.formatted_ins_geolocation.integer_second_timestamp    = 0xABABBABA;
    c_.formatted_ins_geolocation.fractional_second_timestamp = 0xBEBEBABACECECBCB;
    c_.formatted_ins_geolocation.latitude                    = -1.0;
    c_.formatted_ins_geolocation.longitude                   = -1.0;
    c_.formatted_ins_geolocation.altitude                    = -1.0;
    c_.formatted_ins_geolocation.speed_over_ground           = -1.0;
    c_.formatted_ins_geolocation.heading_angle               = -1.0;
    c_.formatted_ins_geolocation.track_angle                 = -1.0;
    c_.formatted_ins_geolocation.magnetic_variation          = -1.0;

    c_.ecef_ephemeris.tsi                         = VRT_TSI_OTHER;
    c_.ecef_ephemeris.tsf                         = VRT_TSF_FREE_RUNNING_COUNT;
    c_.ecef_ephemeris.oui                         = 0xFFFFFFFF;
    c_.ecef_ephemeris.integer_second_timestamp    = 0xABABBABA;
    c_.ecef_ephemeris.fractional_second_timestamp = 0xBEBEBABACECECBCB;
    c_.ecef_ephemeris.position_x                  = -1.0;
    c_.ecef_ephemeris.position_y                  = -1.0;
    c_.ecef_ephemeris.position_z                  = -1.0;
    c_.ecef_ephemeris.altitude_alpha              = -1.0;
    c_.ecef_ephemeris.altitude_beta               = -1.0;
    c_.ecef_ephemeris.altitude_phi                = -1.0;
    c_.ecef_ephemeris.velocity_dx                 = -1.0;
    c_.ecef_ephemeris.velocity_dy                 = -1.0;
    c_.ecef_ephemeris.velocity_dz                 = -1.0;

    c_.relative_ephemeris.tsi                         = VRT_TSI_OTHER;
    c_.relative_ephemeris.tsf                         = VRT_TSF_FREE_RUNNING_COUNT;
    c_.relative_ephemeris.oui                         = 0xFFFFFFFF;
    c_.relative_ephemeris.integer_second_timestamp    = 0xABABBABA;
    c_.relative_ephemeris.fractional_second_timestamp = 0xBEBEBABACECECBCB;
    c_.relative_ephemeris.position_x                  = -1.0;
    c_.relative_ephemeris.position_y                  = -1.0;
    c_.relative_ephemeris.position_z                  = -1.0;
    c_.relative_ephemeris.altitude_alpha              = -1.0;
    c_.relative_ephemeris.altitude_beta               = -1.0;
    c_.relative_ephemeris.altitude_phi                = -1.0;
    c_.relative_ephemeris.velocity_dx                 = -1.0;
    c_.relative_ephemeris.velocity_dy                 = -1.0;
    c_.relative_ephemeris.velocity_dz                 = -1.0;

    c_.ephemeris_reference_identifier = 0xDEDEDEDE;

    c_.gps_ascii.oui             = 0xFFFFFFFF;
    c_.gps_ascii.number_of_words = 3;
    c_.gps_ascii.ascii           = "Lorem ipsum!";

    std::array<uint32_t, 2> l1                                                 = {0x12345678, 0x23456789};
    std::array<uint32_t, 2> l2                                                 = {0x3456789A, 0x456789AB};
    std::array<uint32_t, 2> l3                                                 = {0x56789ABC, 0x6789ABCD};
    std::array<uint32_t, 2> l4                                                 = {0x789ABCDE, 0x89ABCDEF};
    std::array<uint32_t, 2> l5                                                 = {0x9ABCDEF0, 0xABCDEF01};
    c_.context_association_lists.has.asynchronous_channel_tag_list             = true;
    c_.context_association_lists.source_list_size                              = l1.size();
    c_.context_association_lists.system_list_size                              = l2.size();
    c_.context_association_lists.vector_component_list_size                    = l3.size();
    c_.context_association_lists.asynchronous_channel_list_size                = l4.size();
    c_.context_association_lists.source_context_association_list               = l1.data();
    c_.context_association_lists.system_context_association_list               = l2.data();
    c_.context_association_lists.vector_component_context_association_list     = l3.data();
    c_.context_association_lists.asynchronous_channel_context_association_list = l4.data();
    c_.context_association_lists.asynchronous_channel_tag_list                 = l5.data();

    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 91), VRT_ERR);
    ASSERT_EQ(vrt_write_context(&c_, buf_.data(), 92), 92);
    ASSERT_EQ(Hex(buf_[0]), Hex(0xFFFFFF00)); /* Context indicators */
    ASSERT_EQ(Hex(buf_[1]), Hex(0xACACACAC)); /* Reference point identifier */
    ASSERT_EQ(Hex(buf_[2]), Hex(0xFFFFFFFF)); /* Bandwidth */
    ASSERT_EQ(Hex(buf_[3]), Hex(0xFFF00000));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xFFFFFFFF)); /* IF reference frequency */
    ASSERT_EQ(Hex(buf_[5]), Hex(0xFFF00000));
    ASSERT_EQ(Hex(buf_[6]), Hex(0xFFFFFFFF)); /* RF reference frequency */
    ASSERT_EQ(Hex(buf_[7]), Hex(0xFFF00000));
    ASSERT_EQ(Hex(buf_[8]), Hex(0xFFFFFFFF)); /* RF reference frequency offset */
    ASSERT_EQ(Hex(buf_[9]), Hex(0xFFF00000));
    ASSERT_EQ(Hex(buf_[10]), Hex(0xFFFFFFFF)); /* IF band offset */
    ASSERT_EQ(Hex(buf_[11]), Hex(0xFFF00000));
    ASSERT_EQ(Hex(buf_[12]), Hex(0x0000FF80)); /* Reference level */
    ASSERT_EQ(Hex(buf_[13]), Hex(0xFF80FF80)); /* Gain stage 1 & 2 */
    ASSERT_EQ(Hex(buf_[14]), Hex(0xBDBDBDBD)); /* Over-range count */
    ASSERT_EQ(Hex(buf_[15]), Hex(0xFFFFFFFF)); /* Sample rate */
    ASSERT_EQ(Hex(buf_[16]), Hex(0xFFF00000));
    ASSERT_EQ(Hex(buf_[17]), Hex(0xABCABCAB)); /* Timestamp adjustment */
    ASSERT_EQ(Hex(buf_[18]), Hex(0xCABCABCA));
    ASSERT_EQ(Hex(buf_[19]), Hex(0xBABABABA)); /* Timestamp calibration time */
    ASSERT_EQ(Hex(buf_[20]), Hex(0x0000FFC0)); /* Temperature */
    ASSERT_EQ(Hex(buf_[21]), Hex(0x00BEFEBE)); /* Device identifier.oui */
    ASSERT_EQ(Hex(buf_[22]), Hex(0x0000BEBA)); /* Device identifier.device_code */
    ASSERT_EQ(Hex(buf_[23]), Hex(0xFF0FF0FF)); /* State and event indicators */
    ASSERT_EQ(Hex(buf_[24]), Hex(0xD6FF0FFF)); /* Data packet payload format */
    ASSERT_EQ(Hex(buf_[25]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[26]), Hex(0x0FFFFFFF)); /* Formatted GPS geolocation */
    ASSERT_EQ(Hex(buf_[27]), Hex(0xABABBABA));
    ASSERT_EQ(Hex(buf_[28]), Hex(0xBEBEBABA));
    ASSERT_EQ(Hex(buf_[29]), Hex(0xCECECBCB));
    ASSERT_EQ(Hex(buf_[30]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[31]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[32]), Hex(0xFFFFFFE0));
    ASSERT_EQ(Hex(buf_[33]), Hex(0xFFFF0000));
    ASSERT_EQ(Hex(buf_[34]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[35]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[36]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[37]), Hex(0x0FFFFFFF)); /* Formatted INS geolocation */
    ASSERT_EQ(Hex(buf_[38]), Hex(0xABABBABA));
    ASSERT_EQ(Hex(buf_[39]), Hex(0xBEBEBABA));
    ASSERT_EQ(Hex(buf_[40]), Hex(0xCECECBCB));
    ASSERT_EQ(Hex(buf_[41]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[42]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[43]), Hex(0xFFFFFFE0));
    ASSERT_EQ(Hex(buf_[44]), Hex(0xFFFF0000));
    ASSERT_EQ(Hex(buf_[45]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[46]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[47]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[48]), Hex(0x0FFFFFFF)); /* ECEF ephemeris */
    ASSERT_EQ(Hex(buf_[49]), Hex(0xABABBABA));
    ASSERT_EQ(Hex(buf_[50]), Hex(0xBEBEBABA));
    ASSERT_EQ(Hex(buf_[51]), Hex(0xCECECBCB));
    ASSERT_EQ(Hex(buf_[52]), Hex(0xFFFFFFE0));
    ASSERT_EQ(Hex(buf_[53]), Hex(0xFFFFFFE0));
    ASSERT_EQ(Hex(buf_[54]), Hex(0xFFFFFFE0));
    ASSERT_EQ(Hex(buf_[55]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[56]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[57]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[58]), Hex(0xFFFF0000));
    ASSERT_EQ(Hex(buf_[59]), Hex(0xFFFF0000));
    ASSERT_EQ(Hex(buf_[60]), Hex(0xFFFF0000));
    ASSERT_EQ(Hex(buf_[61]), Hex(0x0FFFFFFF)); /* Relative ephemeris */
    ASSERT_EQ(Hex(buf_[62]), Hex(0xABABBABA));
    ASSERT_EQ(Hex(buf_[63]), Hex(0xBEBEBABA));
    ASSERT_EQ(Hex(buf_[64]), Hex(0xCECECBCB));
    ASSERT_EQ(Hex(buf_[65]), Hex(0xFFFFFFE0));
    ASSERT_EQ(Hex(buf_[66]), Hex(0xFFFFFFE0));
    ASSERT_EQ(Hex(buf_[67]), Hex(0xFFFFFFE0));
    ASSERT_EQ(Hex(buf_[68]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[69]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[70]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[71]), Hex(0xFFFF0000));
    ASSERT_EQ(Hex(buf_[72]), Hex(0xFFFF0000));
    ASSERT_EQ(Hex(buf_[73]), Hex(0xFFFF0000));
    ASSERT_EQ(Hex(buf_[74]), Hex(0xDEDEDEDE)); /* Ephemeris reference identifier */
    ASSERT_EQ(Hex(buf_[75]), Hex(0x00FFFFFF)); /* GPS ASCII */
    ASSERT_EQ(Hex(buf_[76]), Hex(0x00000003));
    ASSERT_EQ(Hex(buf_[77]), Hex(0x65726F4C));
    ASSERT_EQ(Hex(buf_[78]), Hex(0x7069206D));
    ASSERT_EQ(Hex(buf_[79]), Hex(0x216D7573));
    ASSERT_EQ(Hex(buf_[80]), Hex(0x00020002)); /* Context association lists */
    ASSERT_EQ(Hex(buf_[81]), Hex(0x00028002));
    ASSERT_EQ(Hex(buf_[82]), Hex(0x12345678));
    ASSERT_EQ(Hex(buf_[83]), Hex(0x23456789));
    ASSERT_EQ(Hex(buf_[84]), Hex(0x3456789A));
    ASSERT_EQ(Hex(buf_[85]), Hex(0x456789AB));
    ASSERT_EQ(Hex(buf_[86]), Hex(0x56789ABC));
    ASSERT_EQ(Hex(buf_[87]), Hex(0x6789ABCD));
    ASSERT_EQ(Hex(buf_[88]), Hex(0x789ABCDE));
    ASSERT_EQ(Hex(buf_[89]), Hex(0x89ABCDEF));
    ASSERT_EQ(Hex(buf_[90]), Hex(0x9ABCDEF0));
    ASSERT_EQ(Hex(buf_[91]), Hex(0xABCDEF01));
}
