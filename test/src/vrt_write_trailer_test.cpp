#include <gtest/gtest.h>

#include <array>
#include <cstdint>

#include <vrt/vrt_error_code.h>
#include <vrt/vrt_init.h>
#include <vrt/vrt_types.h>
#include <vrt/vrt_write.h>

#include "hex.h"

class WriteTrailerTest : public ::testing::Test {
   protected:
    void SetUp() override {
        vrt_init_trailer(&t_);
        buf_.fill(0xBAADF00D);
    }

    vrt_trailer             t_{};
    std::array<uint32_t, 2> buf_{};
};

TEST_F(WriteTrailerTest, NegativeSizeBuffer) {
    ASSERT_EQ(vrt_write_trailer(&t_, buf_.data(), -1, true), VRT_ERR_BUFFER_SIZE);
    ASSERT_EQ(vrt_write_trailer(&t_, buf_.data(), -1, false), VRT_ERR_BUFFER_SIZE);
}

TEST_F(WriteTrailerTest, ZeroSizeBuffer) {
    ASSERT_EQ(vrt_write_trailer(&t_, buf_.data(), 0, true), VRT_ERR_BUFFER_SIZE);
    ASSERT_EQ(vrt_write_trailer(&t_, buf_.data(), 0, false), VRT_ERR_BUFFER_SIZE);
}

TEST_F(WriteTrailerTest, HasCalibratedTime) {
    t_.has.calibrated_time = true;
    ASSERT_EQ(vrt_write_trailer(&t_, buf_.data(), 1, true), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x80000000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}

TEST_F(WriteTrailerTest, HasValidData) {
    t_.has.valid_data = true;
    ASSERT_EQ(vrt_write_trailer(&t_, buf_.data(), 1, true), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x40000000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}

TEST_F(WriteTrailerTest, HasReferenceLock) {
    t_.has.reference_lock = true;
    ASSERT_EQ(vrt_write_trailer(&t_, buf_.data(), 1, true), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x20000000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}

TEST_F(WriteTrailerTest, HasAgcOrMgc) {
    t_.has.agc_or_mgc = true;
    ASSERT_EQ(vrt_write_trailer(&t_, buf_.data(), 1, true), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x10000000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}

TEST_F(WriteTrailerTest, HasDetectedSignal) {
    t_.has.detected_signal = true;
    ASSERT_EQ(vrt_write_trailer(&t_, buf_.data(), 1, true), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x08000000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}

TEST_F(WriteTrailerTest, HasSpectralInversion) {
    t_.has.spectral_inversion = true;
    ASSERT_EQ(vrt_write_trailer(&t_, buf_.data(), 1, true), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x04000000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}

TEST_F(WriteTrailerTest, HasOverRange) {
    t_.has.over_range = true;
    ASSERT_EQ(vrt_write_trailer(&t_, buf_.data(), 1, true), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x02000000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}

TEST_F(WriteTrailerTest, HasSampleLoss) {
    t_.has.sample_loss = true;
    ASSERT_EQ(vrt_write_trailer(&t_, buf_.data(), 1, true), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x01000000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}

TEST_F(WriteTrailerTest, HasUserDefined11) {
    t_.has.user_defined11 = true;
    ASSERT_EQ(vrt_write_trailer(&t_, buf_.data(), 1, true), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00800000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}

TEST_F(WriteTrailerTest, HasUserDefined10) {
    t_.has.user_defined10 = true;
    ASSERT_EQ(vrt_write_trailer(&t_, buf_.data(), 1, true), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00400000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}

TEST_F(WriteTrailerTest, HasUserDefined9) {
    t_.has.user_defined9 = true;
    ASSERT_EQ(vrt_write_trailer(&t_, buf_.data(), 1, true), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00200000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}

TEST_F(WriteTrailerTest, HasUserDefined8) {
    t_.has.user_defined8 = true;
    ASSERT_EQ(vrt_write_trailer(&t_, buf_.data(), 1, true), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00100000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}

TEST_F(WriteTrailerTest, CalibratedTime) {
    t_.calibrated_time = true;
    ASSERT_EQ(vrt_write_trailer(&t_, buf_.data(), 1, true), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}

TEST_F(WriteTrailerTest, ValidData) {
    t_.valid_data = true;
    ASSERT_EQ(vrt_write_trailer(&t_, buf_.data(), 1, true), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}

TEST_F(WriteTrailerTest, ReferenceLock) {
    t_.reference_lock = true;
    ASSERT_EQ(vrt_write_trailer(&t_, buf_.data(), 1, true), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}

TEST_F(WriteTrailerTest, AgcOrMgc) {
    t_.agc_or_mgc = VRT_AOM_AGC;
    ASSERT_EQ(vrt_write_trailer(&t_, buf_.data(), 1, true), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}

TEST_F(WriteTrailerTest, DetectedSignal) {
    t_.detected_signal = true;
    ASSERT_EQ(vrt_write_trailer(&t_, buf_.data(), 1, true), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}

TEST_F(WriteTrailerTest, SpectralInversion) {
    t_.spectral_inversion = true;
    ASSERT_EQ(vrt_write_trailer(&t_, buf_.data(), 1, true), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}

TEST_F(WriteTrailerTest, OverRange) {
    t_.over_range = true;
    ASSERT_EQ(vrt_write_trailer(&t_, buf_.data(), 1, true), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}

TEST_F(WriteTrailerTest, SampleLoss) {
    t_.sample_loss = true;
    ASSERT_EQ(vrt_write_trailer(&t_, buf_.data(), 1, true), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}

TEST_F(WriteTrailerTest, UserDefined11) {
    t_.user_defined11 = true;
    ASSERT_EQ(vrt_write_trailer(&t_, buf_.data(), 1, true), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}

TEST_F(WriteTrailerTest, UserDefined10) {
    t_.user_defined10 = true;
    ASSERT_EQ(vrt_write_trailer(&t_, buf_.data(), 1, true), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}

TEST_F(WriteTrailerTest, UserDefined9) {
    t_.user_defined9 = true;
    ASSERT_EQ(vrt_write_trailer(&t_, buf_.data(), 1, true), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}

TEST_F(WriteTrailerTest, UserDefined8) {
    t_.user_defined8 = true;
    ASSERT_EQ(vrt_write_trailer(&t_, buf_.data(), 1, true), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}

TEST_F(WriteTrailerTest, HasAssociatedPacketCount) {
    t_.has.associated_context_packet_count = true;
    ASSERT_EQ(vrt_write_trailer(&t_, buf_.data(), 1, true), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000080));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}

TEST_F(WriteTrailerTest, AssociatedPacketCount) {
    t_.associated_context_packet_count = 0xFF;
    ASSERT_EQ(vrt_write_trailer(&t_, buf_.data(), 1, true), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}

TEST_F(WriteTrailerTest, BothCalibratedTime) {
    t_.has.calibrated_time = true;
    t_.calibrated_time     = true;
    ASSERT_EQ(vrt_write_trailer(&t_, buf_.data(), 1, true), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x80080000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}

TEST_F(WriteTrailerTest, BothValidData) {
    t_.has.valid_data = true;
    t_.valid_data     = true;
    ASSERT_EQ(vrt_write_trailer(&t_, buf_.data(), 1, true), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x40040000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}

TEST_F(WriteTrailerTest, BothReferenceLock) {
    t_.has.reference_lock = true;
    t_.reference_lock     = true;
    ASSERT_EQ(vrt_write_trailer(&t_, buf_.data(), 1, true), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x20020000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}

TEST_F(WriteTrailerTest, BothAgcOrMgc) {
    t_.has.agc_or_mgc = true;
    t_.agc_or_mgc     = VRT_AOM_AGC;
    ASSERT_EQ(vrt_write_trailer(&t_, buf_.data(), 1, true), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x10010000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}

TEST_F(WriteTrailerTest, BothDetectedSignal) {
    t_.has.detected_signal = true;
    t_.detected_signal     = true;
    ASSERT_EQ(vrt_write_trailer(&t_, buf_.data(), 1, true), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x08008000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}

TEST_F(WriteTrailerTest, BothSpectralInversion) {
    t_.has.spectral_inversion = true;
    t_.spectral_inversion     = true;
    ASSERT_EQ(vrt_write_trailer(&t_, buf_.data(), 1, true), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x04004000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}

TEST_F(WriteTrailerTest, BothOverRange) {
    t_.has.over_range = true;
    t_.over_range     = true;
    ASSERT_EQ(vrt_write_trailer(&t_, buf_.data(), 1, true), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x02002000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}

TEST_F(WriteTrailerTest, BothSampleLoss) {
    t_.has.sample_loss = true;
    t_.sample_loss     = true;
    ASSERT_EQ(vrt_write_trailer(&t_, buf_.data(), 1, true), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x01001000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}

TEST_F(WriteTrailerTest, BothUserDefined11) {
    t_.has.user_defined11 = true;
    t_.user_defined11     = true;
    ASSERT_EQ(vrt_write_trailer(&t_, buf_.data(), 1, true), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00800800));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}

TEST_F(WriteTrailerTest, BothUserDefined10) {
    t_.has.user_defined10 = true;
    t_.user_defined10     = true;
    ASSERT_EQ(vrt_write_trailer(&t_, buf_.data(), 1, true), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00400400));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}

TEST_F(WriteTrailerTest, BothUserDefined9) {
    t_.has.user_defined9 = true;
    t_.user_defined9     = true;
    ASSERT_EQ(vrt_write_trailer(&t_, buf_.data(), 1, true), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00200200));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}

TEST_F(WriteTrailerTest, BothUserDefined8) {
    t_.has.user_defined8 = true;
    t_.user_defined8     = true;
    ASSERT_EQ(vrt_write_trailer(&t_, buf_.data(), 1, true), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00100100));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}

TEST_F(WriteTrailerTest, BothAssociatedContextPacketCount) {
    t_.has.associated_context_packet_count = true;
    t_.associated_context_packet_count     = 0x7E;
    ASSERT_EQ(vrt_write_trailer(&t_, buf_.data(), 1, true), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x000000FE));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}

TEST_F(WriteTrailerTest, BothAssociatedContextPacketCountInvalid) {
    t_.has.associated_context_packet_count = true;
    t_.associated_context_packet_count     = 0xFE;
    ASSERT_EQ(vrt_write_trailer(&t_, buf_.data(), 1, true), VRT_ERR_BOUNDS_ASSOCIATED_CONTEXT_PACKET_COUNT);
    ASSERT_EQ(vrt_write_trailer(&t_, buf_.data(), 1, false), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x000000FE));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}

TEST_F(WriteTrailerTest, EveryOther1) {
    t_.has.calibrated_time                 = true;
    t_.has.reference_lock                  = true;
    t_.has.detected_signal                 = true;
    t_.has.over_range                      = true;
    t_.has.user_defined11                  = true;
    t_.has.user_defined9                   = true;
    t_.calibrated_time                     = true;
    t_.reference_lock                      = true;
    t_.detected_signal                     = true;
    t_.over_range                          = true;
    t_.user_defined11                      = true;
    t_.user_defined9                       = true;
    t_.has.associated_context_packet_count = true;
    ASSERT_EQ(vrt_write_trailer(&t_, buf_.data(), 1, true), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0xAAAAAA80));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}

TEST_F(WriteTrailerTest, EveryOther2) {
    t_.has.valid_data                  = true;
    t_.has.agc_or_mgc                  = true;
    t_.has.spectral_inversion          = true;
    t_.has.sample_loss                 = true;
    t_.has.user_defined10              = true;
    t_.has.user_defined8               = true;
    t_.valid_data                      = true;
    t_.agc_or_mgc                      = VRT_AOM_AGC;
    t_.spectral_inversion              = true;
    t_.sample_loss                     = true;
    t_.user_defined10                  = true;
    t_.user_defined8                   = true;
    t_.associated_context_packet_count = 0xFF;
    ASSERT_EQ(vrt_write_trailer(&t_, buf_.data(), 1, true), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x55555500));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}

TEST_F(WriteTrailerTest, All) {
    t_.has.calibrated_time                 = true;
    t_.has.valid_data                      = true;
    t_.has.reference_lock                  = true;
    t_.has.agc_or_mgc                      = true;
    t_.has.detected_signal                 = true;
    t_.has.spectral_inversion              = true;
    t_.has.over_range                      = true;
    t_.has.sample_loss                     = true;
    t_.has.user_defined11                  = true;
    t_.has.user_defined10                  = true;
    t_.has.user_defined9                   = true;
    t_.has.user_defined8                   = true;
    t_.calibrated_time                     = true;
    t_.valid_data                          = true;
    t_.reference_lock                      = true;
    t_.agc_or_mgc                          = VRT_AOM_AGC;
    t_.detected_signal                     = true;
    t_.spectral_inversion                  = true;
    t_.over_range                          = true;
    t_.sample_loss                         = true;
    t_.user_defined11                      = true;
    t_.user_defined10                      = true;
    t_.user_defined9                       = true;
    t_.user_defined8                       = true;
    t_.has.associated_context_packet_count = true;
    t_.associated_context_packet_count     = 0x7F;
    ASSERT_EQ(vrt_write_trailer(&t_, buf_.data(), 1, true), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}
