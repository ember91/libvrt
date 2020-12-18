#include <gtest/gtest.h>

#include <any>
#include <array>
#include <cstdint>
#include <string>

#include <vrt/vrt_error_code.h>
#include <vrt/vrt_read.h>
#include <vrt/vrt_types.h>

#include "init_garbage.h"
#include "read_assertions.h"

class ReadTrailerTest : public ::testing::Test {
   protected:
    void SetUp() override {
        init_garbage_trailer(&t_);
        buf_.fill(0xBAADF00D);
    }

    vrt_trailer             t_{};
    std::array<uint32_t, 2> buf_{};
};

TEST_F(ReadTrailerTest, NegativeSizeBuffer) {
    ASSERT_EQ(vrt_read_trailer(buf_.data(), -1, &t_), VRT_ERR_BUFFER_SIZE);
}

TEST_F(ReadTrailerTest, ZeroSizeBuffer) {
    ASSERT_EQ(vrt_read_trailer(buf_.data(), 0, &t_), VRT_ERR_BUFFER_SIZE);
}

TEST_F(ReadTrailerTest, None) {
    buf_[0] = 0x00000000;
    ASSERT_EQ(vrt_read_trailer(buf_.data(), 1, &t_), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_trailer(t_, {});
}

TEST_F(ReadTrailerTest, HasCalibratedTime) {
    buf_[0] = 0x80000000;
    ASSERT_EQ(vrt_read_trailer(buf_.data(), 1, &t_), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_trailer(t_, {{"has.calibrated_time", true}});
}

TEST_F(ReadTrailerTest, HasValidData) {
    buf_[0] = 0x40000000;
    ASSERT_EQ(vrt_read_trailer(buf_.data(), 1, &t_), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_trailer(t_, {{"has.valid_data", true}});
}

TEST_F(ReadTrailerTest, HasReferenceLock) {
    buf_[0] = 0x20000000;
    ASSERT_EQ(vrt_read_trailer(buf_.data(), 1, &t_), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_trailer(t_, {{"has.reference_lock", true}});
}

TEST_F(ReadTrailerTest, HasAgcOrMgc) {
    buf_[0] = 0x10000000;
    ASSERT_EQ(vrt_read_trailer(buf_.data(), 1, &t_), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_trailer(t_, {{"has.agc_or_mgc", true}});
}

TEST_F(ReadTrailerTest, HasDetectedSignal) {
    buf_[0] = 0x08000000;
    ASSERT_EQ(vrt_read_trailer(buf_.data(), 1, &t_), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_trailer(t_, {{"has.detected_signal", true}});
}

TEST_F(ReadTrailerTest, HasSpectralInversion) {
    buf_[0] = 0x04000000;
    ASSERT_EQ(vrt_read_trailer(buf_.data(), 1, &t_), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_trailer(t_, {{"has.spectral_inversion", true}});
}

TEST_F(ReadTrailerTest, hasOverRange) {
    buf_[0] = 0x02000000;
    ASSERT_EQ(vrt_read_trailer(buf_.data(), 1, &t_), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_trailer(t_, {{"has.over_range", true}});
}

TEST_F(ReadTrailerTest, HasSampleLoss) {
    buf_[0] = 0x01000000;
    ASSERT_EQ(vrt_read_trailer(buf_.data(), 1, &t_), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_trailer(t_, {{"has.sample_loss", true}});
}

TEST_F(ReadTrailerTest, HasUserDefined11) {
    buf_[0] = 0x00800000;
    ASSERT_EQ(vrt_read_trailer(buf_.data(), 1, &t_), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_trailer(t_, {{"has.user_defined11", true}});
}

TEST_F(ReadTrailerTest, HasUserDefined10) {
    buf_[0] = 0x00400000;
    ASSERT_EQ(vrt_read_trailer(buf_.data(), 1, &t_), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_trailer(t_, {{"has.user_defined10", true}});
}

TEST_F(ReadTrailerTest, HasUserDefined9) {
    buf_[0] = 0x00200000;
    ASSERT_EQ(vrt_read_trailer(buf_.data(), 1, &t_), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_trailer(t_, {{"has.user_defined9", true}});
}

TEST_F(ReadTrailerTest, HasUserDefined8) {
    buf_[0] = 0x00100000;
    ASSERT_EQ(vrt_read_trailer(buf_.data(), 1, &t_), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_trailer(t_, {{"has.user_defined8", true}});
}

TEST_F(ReadTrailerTest, CalibratedTime) {
    buf_[0] = 0x00080000;
    ASSERT_EQ(vrt_read_trailer(buf_.data(), 1, &t_), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_trailer(t_, {});
}

TEST_F(ReadTrailerTest, ValidData) {
    buf_[0] = 0x00040000;
    ASSERT_EQ(vrt_read_trailer(buf_.data(), 1, &t_), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_trailer(t_, {});
}

TEST_F(ReadTrailerTest, ReferenceLock) {
    buf_[0] = 0x00020000;
    ASSERT_EQ(vrt_read_trailer(buf_.data(), 1, &t_), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_trailer(t_, {});
}

TEST_F(ReadTrailerTest, AgcOrMgc) {
    buf_[0] = 0x00010000;
    ASSERT_EQ(vrt_read_trailer(buf_.data(), 1, &t_), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_trailer(t_, {});
}

TEST_F(ReadTrailerTest, DetectedSignal) {
    buf_[0] = 0x00008000;
    ASSERT_EQ(vrt_read_trailer(buf_.data(), 1, &t_), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_trailer(t_, {});
}

TEST_F(ReadTrailerTest, SpectralInversion) {
    buf_[0] = 0x00004000;
    ASSERT_EQ(vrt_read_trailer(buf_.data(), 1, &t_), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_trailer(t_, {});
}

TEST_F(ReadTrailerTest, OverRange) {
    buf_[0] = 0x00002000;
    ASSERT_EQ(vrt_read_trailer(buf_.data(), 1, &t_), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_trailer(t_, {});
}

TEST_F(ReadTrailerTest, SampleLoss) {
    buf_[0] = 0x00001000;
    ASSERT_EQ(vrt_read_trailer(buf_.data(), 1, &t_), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_trailer(t_, {});
}

TEST_F(ReadTrailerTest, UserDefined11) {
    buf_[0] = 0x00000800;
    ASSERT_EQ(vrt_read_trailer(buf_.data(), 1, &t_), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_trailer(t_, {});
}

TEST_F(ReadTrailerTest, UserDefined10) {
    buf_[0] = 0x00000400;
    ASSERT_EQ(vrt_read_trailer(buf_.data(), 1, &t_), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_trailer(t_, {});
}

TEST_F(ReadTrailerTest, UserDefined9) {
    buf_[0] = 0x00000200;
    ASSERT_EQ(vrt_read_trailer(buf_.data(), 1, &t_), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_trailer(t_, {});
}

TEST_F(ReadTrailerTest, UserDefined8) {
    buf_[0] = 0x00000100;
    ASSERT_EQ(vrt_read_trailer(buf_.data(), 1, &t_), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_trailer(t_, {});
}

TEST_F(ReadTrailerTest, BothCalibratedTime) {
    buf_[0] = 0x80080000;
    ASSERT_EQ(vrt_read_trailer(buf_.data(), 1, &t_), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_trailer(t_, {{"has.calibrated_time", true}, {"calibrated_time", true}});
}

TEST_F(ReadTrailerTest, BothValidData) {
    buf_[0] = 0x40040000;
    ASSERT_EQ(vrt_read_trailer(buf_.data(), 1, &t_), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_trailer(t_, {{"has.valid_data", true}, {"valid_data", true}});
}

TEST_F(ReadTrailerTest, BothReferenceLock) {
    buf_[0] = 0x20020000;
    ASSERT_EQ(vrt_read_trailer(buf_.data(), 1, &t_), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_trailer(t_, {{"has.reference_lock", true}, {"reference_lock", true}});
}

TEST_F(ReadTrailerTest, BothAgcOrMgc) {
    buf_[0] = 0x10010000;
    ASSERT_EQ(vrt_read_trailer(buf_.data(), 1, &t_), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_trailer(t_, {{"has.agc_or_mgc", true}, {"agc_or_mgc", VRT_AOM_AGC}});
}

TEST_F(ReadTrailerTest, BothDetectedSignal) {
    buf_[0] = 0x08008000;
    ASSERT_EQ(vrt_read_trailer(buf_.data(), 1, &t_), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_trailer(t_, {{"has.detected_signal", true}, {"detected_signal", true}});
}

TEST_F(ReadTrailerTest, BothSpectralInversion) {
    buf_[0] = 0x04004000;
    ASSERT_EQ(vrt_read_trailer(buf_.data(), 1, &t_), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_trailer(t_, {{"has.spectral_inversion", true}, {"spectral_inversion", true}});
}

TEST_F(ReadTrailerTest, BothOverRange) {
    buf_[0] = 0x02002000;
    ASSERT_EQ(vrt_read_trailer(buf_.data(), 1, &t_), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_trailer(t_, {{"has.over_range", true}, {"over_range", true}});
}

TEST_F(ReadTrailerTest, BothSampleLoss) {
    buf_[0] = 0x01001000;
    ASSERT_EQ(vrt_read_trailer(buf_.data(), 1, &t_), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_trailer(t_, {{"has.sample_loss", true}, {"sample_loss", true}});
}

TEST_F(ReadTrailerTest, BothUserDefined11) {
    buf_[0] = 0x00800800;
    ASSERT_EQ(vrt_read_trailer(buf_.data(), 1, &t_), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_trailer(t_, {{"has.user_defined11", true}, {"user_defined11", true}});
}

TEST_F(ReadTrailerTest, BothUserDefined10) {
    buf_[0] = 0x00400400;
    ASSERT_EQ(vrt_read_trailer(buf_.data(), 1, &t_), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_trailer(t_, {{"has.user_defined10", true}, {"user_defined10", true}});
}

TEST_F(ReadTrailerTest, BothUserDefined9) {
    buf_[0] = 0x00200200;
    ASSERT_EQ(vrt_read_trailer(buf_.data(), 1, &t_), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_trailer(t_, {{"has.user_defined9", true}, {"user_defined9", true}});
}

TEST_F(ReadTrailerTest, BothUserDefined8) {
    buf_[0] = 0x00100100;
    ASSERT_EQ(vrt_read_trailer(buf_.data(), 1, &t_), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_trailer(t_, {{"has.user_defined8", true}, {"user_defined8", true}});
}

TEST_F(ReadTrailerTest, HasAssociatedContextPacketCount) {
    buf_[0] = 0x00000080;
    ASSERT_EQ(vrt_read_trailer(buf_.data(), 1, &t_), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_trailer(t_, {{"has.associated_context_packet_count", true}});
}

TEST_F(ReadTrailerTest, AssociatedContextPacketCount) {
    buf_[0] = 0x0000007E;
    ASSERT_EQ(vrt_read_trailer(buf_.data(), 1, &t_), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_trailer(t_, {});
}

TEST_F(ReadTrailerTest, BothAssociatedContextPacketCount) {
    buf_[0] = 0x000000FF;
    ASSERT_EQ(vrt_read_trailer(buf_.data(), 1, &t_), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_trailer(t_, {{"has.associated_context_packet_count", true},
                        {"associated_context_packet_count", static_cast<uint8_t>(0x7F)}});
}

TEST_F(ReadTrailerTest, EveryOther1) {
    buf_[0] = 0x55555555;
    ASSERT_EQ(vrt_read_trailer(buf_.data(), 1, &t_), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_trailer(t_, {{"has.valid_data", true},
                        {"valid_data", true},
                        {"has.agc_or_mgc", true},
                        {"agc_or_mgc", VRT_AOM_AGC},
                        {"has.spectral_inversion", true},
                        {"spectral_inversion", true},
                        {"has.sample_loss", true},
                        {"sample_loss", true},
                        {"has.user_defined10", true},
                        {"user_defined10", true},
                        {"has.user_defined8", true},
                        {"user_defined8", true}});
}

TEST_F(ReadTrailerTest, EveryOther2) {
    buf_[0] = 0xAAAAAAAA;
    ASSERT_EQ(vrt_read_trailer(buf_.data(), 1, &t_), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_trailer(t_, {{"has.calibrated_time", true},
                        {"calibrated_time", true},
                        {"has.reference_lock", true},
                        {"reference_lock", true},
                        {"has.detected_signal", true},
                        {"detected_signal", true},
                        {"has.over_range", true},
                        {"over_range", true},
                        {"has.user_defined11", true},
                        {"user_defined11", true},
                        {"has.user_defined9", true},
                        {"user_defined9", true},
                        {"has.associated_context_packet_count", true},
                        {"associated_context_packet_count", static_cast<uint8_t>(0x2A)}});
}

TEST_F(ReadTrailerTest, All) {
    buf_[0] = 0xFFFFFFFE;
    ASSERT_EQ(vrt_read_trailer(buf_.data(), 1, &t_), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_trailer(t_, {{"has.calibrated_time", true},
                        {"calibrated_time", true},
                        {"has.valid_data", true},
                        {"valid_data", true},
                        {"has.reference_lock", true},
                        {"reference_lock", true},
                        {"has.agc_or_mgc", true},
                        {"agc_or_mgc", VRT_AOM_AGC},
                        {"has.detected_signal", true},
                        {"detected_signal", true},
                        {"has.spectral_inversion", true},
                        {"spectral_inversion", true},
                        {"has.over_range", true},
                        {"over_range", true},
                        {"has.sample_loss", true},
                        {"sample_loss", true},
                        {"has.user_defined11", true},
                        {"user_defined11", true},
                        {"has.user_defined10", true},
                        {"user_defined10", true},
                        {"has.user_defined9", true},
                        {"user_defined9", true},
                        {"has.user_defined8", true},
                        {"user_defined8", true},
                        {"has.associated_context_packet_count", true},
                        {"associated_context_packet_count", static_cast<uint8_t>(0x7E)}});
}
