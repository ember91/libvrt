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

class ReadHeaderTest : public ::testing::Test {
   protected:
    void SetUp() override {
        init_garbage_header(&h_);
        buf_.fill(0xBAADF00D);
    }

    vrt_header              h_{};
    std::array<uint32_t, 2> buf_{};
};

TEST_F(ReadHeaderTest, NegativeSizeBuffer) {
    ASSERT_EQ(vrt_read_header(buf_.data(), -1, &h_, true), VRT_ERR_BUFFER_SIZE);
    ASSERT_EQ(vrt_read_header(buf_.data(), -1, &h_, false), VRT_ERR_BUFFER_SIZE);
}

TEST_F(ReadHeaderTest, ZeroSizeBuffer) {
    ASSERT_EQ(vrt_read_header(buf_.data(), 0, &h_, true), VRT_ERR_BUFFER_SIZE);
    ASSERT_EQ(vrt_read_header(buf_.data(), 0, &h_, false), VRT_ERR_BUFFER_SIZE);
}

TEST_F(ReadHeaderTest, PacketType) {
    buf_[0] = 0x00000000;
    ASSERT_EQ(vrt_read_header(buf_.data(), 1, &h_, true), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_header(h_, {});
}

TEST_F(ReadHeaderTest, PacketTypeInvalid) {
    buf_[0] = 0x60000000;
    ASSERT_EQ(vrt_read_header(buf_.data(), 1, &h_, true), VRT_ERR_INVALID_PACKET_TYPE);
    ASSERT_EQ(vrt_read_header(buf_.data(), 1, &h_, false), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_header(h_, {{"packet_type", static_cast<vrt_packet_type>(0x6)}});
}

TEST_F(ReadHeaderTest, HasClassId) {
    buf_[0] = 0x08000000;
    ASSERT_EQ(vrt_read_header(buf_.data(), 1, &h_, true), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_header(h_, {{"has.class_id", true}});
}

TEST_F(ReadHeaderTest, HasTrailer) {
    buf_[0] = 0x04000000;
    ASSERT_EQ(vrt_read_header(buf_.data(), 1, &h_, true), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_header(h_, {{"has.trailer", true}});
}

TEST_F(ReadHeaderTest, HasTrailerInvalid) {
    buf_[0] = 0x44000000;
    ASSERT_EQ(vrt_read_header(buf_.data(), 1, &h_, true), VRT_ERR_TRAILER_IN_CONTEXT);
    ASSERT_EQ(vrt_read_header(buf_.data(), 1, &h_, false), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_header(h_, {{"packet_type", VRT_PT_IF_CONTEXT}, {"has.trailer", true}});
}

TEST_F(ReadHeaderTest, Tsm) {
    buf_[0] = 0x41000000;
    ASSERT_EQ(vrt_read_header(buf_.data(), 1, &h_, true), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_header(h_, {{"packet_type", VRT_PT_IF_CONTEXT}, {"tsm", VRT_TSM_COARSE}});
}

TEST_F(ReadHeaderTest, TsmInvalid) {
    buf_[0] = 0x01000000;
    ASSERT_EQ(vrt_read_header(buf_.data(), 1, &h_, true), VRT_ERR_TSM_IN_DATA);
    ASSERT_EQ(vrt_read_header(buf_.data(), 1, &h_, false), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_header(h_, {{"tsm", VRT_TSM_COARSE}});
}

TEST_F(ReadHeaderTest, Tsi) {
    buf_[0] = 0x00C00000;
    ASSERT_EQ(vrt_read_header(buf_.data(), 1, &h_, true), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_header(h_, {{"tsi", VRT_TSI_OTHER}});
}

TEST_F(ReadHeaderTest, Tsf) {
    buf_[0] = 0x00300000;
    ASSERT_EQ(vrt_read_header(buf_.data(), 1, &h_, true), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_header(h_, {{"tsf", VRT_TSF_FREE_RUNNING_COUNT}});
}

TEST_F(ReadHeaderTest, PacketCount) {
    buf_[0] = 0x000A0000;
    ASSERT_EQ(vrt_read_header(buf_.data(), 1, &h_, true), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_header(h_, {{"packet_count", static_cast<uint8_t>(0xA)}});
}

TEST_F(ReadHeaderTest, PacketSize) {
    buf_[0] = 0x0000FEDC;
    ASSERT_EQ(vrt_read_header(buf_.data(), 1, &h_, true), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_header(h_, {{"packet_size", static_cast<uint16_t>(0xFEDC)}});
}

TEST_F(ReadHeaderTest, Reserved) {
    buf_[0] = 0x02000000;
    ASSERT_EQ(vrt_read_header(buf_.data(), 1, &h_, true), VRT_ERR_RESERVED);
    ASSERT_EQ(vrt_read_header(buf_.data(), 1, &h_, false), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_header(h_, {});
}

TEST_F(ReadHeaderTest, EveryOther1) {
    buf_[0] = 0x0810ABCD;
    ASSERT_EQ(vrt_read_header(buf_.data(), 1, &h_, true), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_header(
        h_, {{"has.class_id", true}, {"tsf", VRT_TSF_SAMPLE_COUNT}, {"packet_size", static_cast<uint16_t>(0xABCD)}});
}

TEST_F(ReadHeaderTest, EveryOther2) {
    buf_[0] = 0x14850000;
    ASSERT_EQ(vrt_read_header(buf_.data(), 1, &h_, true), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_header(h_, {{"packet_type", VRT_PT_IF_DATA_WITH_STREAM_ID},
                       {"has.trailer", true},
                       {"tsi", VRT_TSI_GPS},
                       {"packet_count", static_cast<uint8_t>(0x5)}});
}

TEST_F(ReadHeaderTest, All) {
    buf_[0] = 0x597FBEDE;
    ASSERT_EQ(vrt_read_header(buf_.data(), 1, &h_, true), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_header(h_, {{"packet_type", VRT_PT_EXT_CONTEXT},
                       {"has.class_id", true},
                       {"tsm", VRT_TSM_COARSE},
                       {"tsi", VRT_TSI_UTC},
                       {"tsf", VRT_TSF_FREE_RUNNING_COUNT},
                       {"packet_count", static_cast<uint8_t>(0xF)},
                       {"packet_size", static_cast<uint16_t>(0xBEDE)}});
}
