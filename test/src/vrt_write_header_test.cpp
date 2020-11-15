#include <gtest/gtest.h>

#include <array>

#include <vrt/vrt_common.h>
#include <vrt/vrt_write.h>

#include "hex.h"

class WriteHeaderTest : public ::testing::Test {
   protected:
    WriteHeaderTest() : h_(), buf_() {}

    void SetUp() override {
        vrt_init_header(&h_);
        buf_.fill(0xBAADF00D);
    }

    vrt_header              h_;
    std::array<uint32_t, 2> buf_;
};

TEST_F(WriteHeaderTest, ZeroSizeBuffer) {
    ASSERT_EQ(vrt_write_header(&h_, buf_.data(), 0), VRT_ERR_BUF_SIZE);
}

TEST_F(WriteHeaderTest, PacketType1) {
    h_.packet_type = VRT_PT_EXT_CONTEXT;
    ASSERT_EQ(vrt_write_header(&h_, buf_.data(), 1), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x50000000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}

TEST_F(WriteHeaderTest, PacketType2) {
    h_.packet_type = static_cast<vrt_packet_type>(0xFFFFFFFF);
    ASSERT_EQ(vrt_write_header(&h_, buf_.data(), 1), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0xF0000000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}

TEST_F(WriteHeaderTest, ClassId) {
    h_.has.class_id = true;
    ASSERT_EQ(vrt_write_header(&h_, buf_.data(), 1), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x08000000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}

TEST_F(WriteHeaderTest, HasTrailer) {
    h_.has.trailer = true;
    ASSERT_EQ(vrt_write_header(&h_, buf_.data(), 1), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x04000000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}

TEST_F(WriteHeaderTest, Tsm) {
    h_.tsm = VRT_TSM_COARSE;
    ASSERT_EQ(vrt_write_header(&h_, buf_.data(), 1), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x01000000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}

TEST_F(WriteHeaderTest, Tsi1) {
    h_.tsi = VRT_TSI_OTHER;
    ASSERT_EQ(vrt_write_header(&h_, buf_.data(), 1), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00C00000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}

TEST_F(WriteHeaderTest, Tsi2) {
    h_.tsi = static_cast<vrt_tsi>(0xFFFFFFFF);
    ASSERT_EQ(vrt_write_header(&h_, buf_.data(), 1), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00C00000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}

TEST_F(WriteHeaderTest, Tsf1) {
    h_.tsf = VRT_TSF_FREE_RUNNING_COUNT;
    ASSERT_EQ(vrt_write_header(&h_, buf_.data(), 1), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00300000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}

TEST_F(WriteHeaderTest, Tsf2) {
    h_.tsf = static_cast<vrt_tsf>(0xFFFFFFFF);
    ASSERT_EQ(vrt_write_header(&h_, buf_.data(), 1), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00300000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}

TEST_F(WriteHeaderTest, PacketCount) {
    h_.packet_count = 0xFF;
    ASSERT_EQ(vrt_write_header(&h_, buf_.data(), 1), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x000F0000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}

TEST_F(WriteHeaderTest, PacketSize) {
    h_.packet_size = 0xFEDC;
    ASSERT_EQ(vrt_write_header(&h_, buf_.data(), 1), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x0000FEDC));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}

TEST_F(WriteHeaderTest, EveryOther1) {
    h_.packet_type  = VRT_PT_IF_CONTEXT;
    h_.has.class_id = false;
    h_.has.trailer  = true;
    h_.tsm          = VRT_TSM_FINE;
    h_.tsi          = VRT_TSI_UTC;
    h_.tsf          = VRT_TSF_NONE;
    h_.packet_count = 0xF;
    h_.packet_size  = 0x0000;
    ASSERT_EQ(vrt_write_header(&h_, buf_.data(), 1), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x444F0000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}

TEST_F(WriteHeaderTest, EveryOther2) {
    h_.packet_type  = VRT_PT_IF_DATA_WITHOUT_STREAM_ID;
    h_.has.class_id = true;
    h_.has.trailer  = true;
    h_.tsm          = VRT_TSM_FINE;
    h_.tsi          = VRT_TSI_NONE;
    h_.tsf          = VRT_TSF_SAMPLE_COUNT;
    h_.packet_count = 0x0;
    h_.packet_size  = 0xFFFF;
    ASSERT_EQ(vrt_write_header(&h_, buf_.data(), 1), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x0C10FFFF));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}

TEST_F(WriteHeaderTest, All) {
    h_.packet_type  = VRT_PT_EXT_DATA_WITH_STREAM_ID;
    h_.has.class_id = true;
    h_.has.trailer  = true;
    h_.tsm          = VRT_TSM_FINE;
    h_.tsi          = VRT_TSI_OTHER;
    h_.tsf          = VRT_TSF_SAMPLE_COUNT;
    h_.packet_count = 0xC;
    h_.packet_size  = 0xBA98;
    ASSERT_EQ(vrt_write_header(&h_, buf_.data(), 1), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x3CDCBA98));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}
