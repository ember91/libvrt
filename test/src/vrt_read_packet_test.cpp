#include <gtest/gtest.h>

#include <any>
#include <array>
#include <cstdint>
#include <cstring>
#include <map>
#include <string>

#include <vrt/vrt_error_code.h>
#include <vrt/vrt_read.h>
#include <vrt/vrt_types.h>

#include "hex.h"
#include "init_garbage.h"
#include "read_util.h"

class ReadPacketTest : public ::testing::Test {
   protected:
    ReadPacketTest() : p_(), buf_() {}

    void SetUp() override {
        init_garbage_packet(&p_);
        buf_.fill(0xBAADF00D);
    }

    vrt_packet               p_;
    std::array<uint32_t, 16> buf_;
};

TEST_F(ReadPacketTest, NegativeSizeBuffer) {
    ASSERT_EQ(vrt_read_packet(buf_.data(), -1, &p_, true), VRT_ERR_BUFFER_SIZE);
    ASSERT_EQ(vrt_read_packet(buf_.data(), -1, &p_, false), VRT_ERR_BUFFER_SIZE);
}

TEST_F(ReadPacketTest, ZeroSizeBuffer) {
    ASSERT_EQ(vrt_read_packet(buf_.data(), 0, &p_, true), VRT_ERR_BUFFER_SIZE);
    ASSERT_EQ(vrt_read_packet(buf_.data(), 0, &p_, false), VRT_ERR_BUFFER_SIZE);
}

TEST_F(ReadPacketTest, EmptyIfDataWithoutStreamId) {
    buf_[0] = 0x00000001;
    ASSERT_EQ(vrt_read_packet(buf_.data(), 1, &p_, true), 1);
    ASSERT_EQ(Hex(p_.header.packet_type), Hex(VRT_PT_IF_DATA_WITHOUT_STREAM_ID));
    ASSERT_EQ(p_.header.packet_size, 1);
}

TEST_F(ReadPacketTest, EmptyIfDataWithStreamId) {
    buf_[0] = 0x10000002;
    buf_[1] = 0xABABABAB;
    ASSERT_EQ(vrt_read_packet(buf_.data(), 2, &p_, true), 2);
    ASSERT_EQ(Hex(p_.header.packet_type), Hex(VRT_PT_IF_DATA_WITH_STREAM_ID));
    ASSERT_EQ(p_.header.packet_size, 2);
    ASSERT_EQ(Hex(p_.fields.stream_id), Hex(0xABABABAB));
}

TEST_F(ReadPacketTest, EmptyExtDataWithoutStreamId) {
    buf_[0] = 0x20000001;
    ASSERT_EQ(vrt_read_packet(buf_.data(), 1, &p_, true), 1);
    ASSERT_EQ(Hex(p_.header.packet_type), Hex(VRT_PT_EXT_DATA_WITHOUT_STREAM_ID));
    ASSERT_EQ(p_.header.packet_size, 1);
}

TEST_F(ReadPacketTest, EmptyExtDataWithStreamId) {
    buf_[0] = 0x30000002;
    buf_[1] = 0xABABABAB;
    ASSERT_EQ(vrt_read_packet(buf_.data(), 2, &p_, true), 2);
    ASSERT_EQ(Hex(p_.header.packet_type), Hex(VRT_PT_EXT_DATA_WITH_STREAM_ID));
    ASSERT_EQ(p_.header.packet_size, 2);
    ASSERT_EQ(Hex(p_.fields.stream_id), Hex(0xABABABAB));
}

TEST_F(ReadPacketTest, EmptyIfContext) {
    buf_[0] = 0x40000003;
    buf_[1] = 0xABABABAB;
    buf_[2] = 0x00000000;
    ASSERT_EQ(vrt_read_packet(buf_.data(), 3, &p_, true), 3);
    ASSERT_EQ(Hex(p_.header.packet_type), Hex(VRT_PT_IF_CONTEXT));
    ASSERT_EQ(p_.header.packet_size, 3);
    ASSERT_EQ(Hex(p_.fields.stream_id), Hex(0xABABABAB));
}

TEST_F(ReadPacketTest, EmptyExtContext) {
    buf_[0] = 0x50000002;
    buf_[1] = 0xABABABAB;
    ASSERT_EQ(vrt_read_packet(buf_.data(), 2, &p_, true), 2);
    ASSERT_EQ(Hex(p_.header.packet_type), Hex(VRT_PT_EXT_CONTEXT));
    ASSERT_EQ(p_.header.packet_size, 2);
    ASSERT_EQ(Hex(p_.fields.stream_id), Hex(0xABABABAB));
}

TEST_F(ReadPacketTest, BodyIfDataWithoutStreamId) {
    buf_[0] = 0x00000004;
    buf_[1] = 0xCECECECE;
    buf_[2] = 0xFEFEFEFE;
    buf_[3] = 0xDEDEDEDE;
    ASSERT_EQ(vrt_read_packet(buf_.data(), 4, &p_, true), 4);
    ASSERT_EQ(Hex(p_.header.packet_type), Hex(VRT_PT_IF_DATA_WITHOUT_STREAM_ID));
    ASSERT_EQ(p_.header.packet_size, 4);
    ASSERT_EQ(p_.words_body, 3);
    uint32_t* b{reinterpret_cast<uint32_t*>(p_.body)};
    ASSERT_EQ(Hex(b[0]), Hex(0xCECECECE));
    ASSERT_EQ(Hex(b[1]), Hex(0xFEFEFEFE));
    ASSERT_EQ(Hex(b[2]), Hex(0xDEDEDEDE));
}

TEST_F(ReadPacketTest, BodyIfDataWithStreamId) {
    buf_[0] = 0x10000005;
    buf_[1] = 0xABABABAB;
    buf_[2] = 0xCECECECE;
    buf_[3] = 0xFEFEFEFE;
    buf_[4] = 0xDEDEDEDE;
    ASSERT_EQ(vrt_read_packet(buf_.data(), 5, &p_, true), 5);
    ASSERT_EQ(Hex(p_.header.packet_type), Hex(VRT_PT_IF_DATA_WITH_STREAM_ID));
    ASSERT_EQ(p_.header.packet_size, 5);
    ASSERT_EQ(Hex(p_.fields.stream_id), Hex(0xABABABAB));
    ASSERT_EQ(p_.words_body, 3);
    uint32_t* b{reinterpret_cast<uint32_t*>(p_.body)};
    ASSERT_EQ(Hex(b[0]), Hex(0xCECECECE));
    ASSERT_EQ(Hex(b[1]), Hex(0xFEFEFEFE));
    ASSERT_EQ(Hex(b[2]), Hex(0xDEDEDEDE));
}

TEST_F(ReadPacketTest, BodyExtDataWithoutStreamId) {
    buf_[0] = 0x20000004;
    buf_[1] = 0xCECECECE;
    buf_[2] = 0xFEFEFEFE;
    buf_[3] = 0xDEDEDEDE;
    ASSERT_EQ(vrt_read_packet(buf_.data(), 4, &p_, true), 4);
    ASSERT_EQ(Hex(p_.header.packet_type), Hex(VRT_PT_EXT_DATA_WITHOUT_STREAM_ID));
    ASSERT_EQ(p_.header.packet_size, 4);
    ASSERT_EQ(p_.words_body, 3);
    uint32_t* b{reinterpret_cast<uint32_t*>(p_.body)};
    ASSERT_EQ(Hex(b[0]), Hex(0xCECECECE));
    ASSERT_EQ(Hex(b[1]), Hex(0xFEFEFEFE));
    ASSERT_EQ(Hex(b[2]), Hex(0xDEDEDEDE));
}

TEST_F(ReadPacketTest, BodyExtDataWithStreamId) {
    buf_[0] = 0x30000005;
    buf_[1] = 0xABABABAB;
    buf_[2] = 0xCECECECE;
    buf_[3] = 0xFEFEFEFE;
    buf_[4] = 0xDEDEDEDE;
    ASSERT_EQ(vrt_read_packet(buf_.data(), 5, &p_, true), 5);
    ASSERT_EQ(Hex(p_.header.packet_type), Hex(VRT_PT_EXT_DATA_WITH_STREAM_ID));
    ASSERT_EQ(p_.header.packet_size, 5);
    ASSERT_EQ(Hex(p_.fields.stream_id), Hex(0xABABABAB));
    ASSERT_EQ(p_.words_body, 3);
    uint32_t* b{reinterpret_cast<uint32_t*>(p_.body)};
    ASSERT_EQ(Hex(b[0]), Hex(0xCECECECE));
    ASSERT_EQ(Hex(b[1]), Hex(0xFEFEFEFE));
    ASSERT_EQ(Hex(b[2]), Hex(0xDEDEDEDE));
}

TEST_F(ReadPacketTest, BodyIfContext) {
    buf_[0] = 0x40000003;
    buf_[1] = 0xABABABAB;
    buf_[2] = 0x00000000;
    ASSERT_EQ(vrt_read_packet(buf_.data(), 3, &p_, true), 3);
    ASSERT_EQ(Hex(p_.header.packet_type), Hex(VRT_PT_IF_CONTEXT));
    ASSERT_EQ(p_.header.packet_size, 3);
    ASSERT_EQ(Hex(p_.fields.stream_id), Hex(0xABABABAB));
    ASSERT_EQ(p_.words_body, 0);
    ASSERT_EQ(p_.body, nullptr);
}

TEST_F(ReadPacketTest, BodyExtContext) {
    buf_[0] = 0x50000005;
    buf_[1] = 0xABABABAB;
    buf_[2] = 0xCECECECE;
    buf_[3] = 0xFEFEFEFE;
    buf_[4] = 0xDEDEDEDE;
    ASSERT_EQ(vrt_read_packet(buf_.data(), 5, &p_, true), 5);
    ASSERT_EQ(Hex(p_.header.packet_type), Hex(VRT_PT_EXT_CONTEXT));
    ASSERT_EQ(p_.header.packet_size, 5);
    ASSERT_EQ(Hex(p_.fields.stream_id), Hex(0xABABABAB));
    ASSERT_EQ(p_.words_body, 3);
    uint32_t* b{reinterpret_cast<uint32_t*>(p_.body)};
    ASSERT_EQ(Hex(b[0]), Hex(0xCECECECE));
    ASSERT_EQ(Hex(b[1]), Hex(0xFEFEFEFE));
    ASSERT_EQ(Hex(b[2]), Hex(0xDEDEDEDE));
}

TEST_F(ReadPacketTest, TrailerIfDataWithoutStreamId) {
    buf_[0] = 0x04000002;
    buf_[1] = 0x01001000;
    ASSERT_EQ(vrt_read_packet(buf_.data(), 2, &p_, true), 2);
    ASSERT_EQ(Hex(p_.header.packet_type), Hex(VRT_PT_IF_DATA_WITHOUT_STREAM_ID));
    ASSERT_TRUE(p_.header.has.trailer);
    ASSERT_EQ(p_.header.packet_size, 2);
    ASSERT_TRUE(p_.trailer.has.sample_loss);
    ASSERT_EQ(p_.trailer.sample_loss, true);
}

TEST_F(ReadPacketTest, TrailerIfDataWithStreamId) {
    buf_[0] = 0x14000003;
    buf_[1] = 0xABABABAB;
    buf_[2] = 0x01001000;
    ASSERT_EQ(vrt_read_packet(buf_.data(), 3, &p_, true), 3);
    ASSERT_EQ(Hex(p_.header.packet_type), Hex(VRT_PT_IF_DATA_WITH_STREAM_ID));
    ASSERT_TRUE(p_.header.has.trailer);
    ASSERT_EQ(p_.header.packet_size, 3);
    ASSERT_EQ(Hex(p_.fields.stream_id), Hex(0xABABABAB));
    ASSERT_TRUE(p_.trailer.has.sample_loss);
    ASSERT_EQ(p_.trailer.sample_loss, true);
}

TEST_F(ReadPacketTest, TrailerExtDataWithoutStreamId) {
    buf_[0] = 0x24000002;
    buf_[1] = 0x01001000;
    ASSERT_EQ(vrt_read_packet(buf_.data(), 2, &p_, true), 2);
    ASSERT_EQ(Hex(p_.header.packet_type), Hex(VRT_PT_EXT_DATA_WITHOUT_STREAM_ID));
    ASSERT_TRUE(p_.header.has.trailer);
    ASSERT_EQ(p_.header.packet_size, 2);
    ASSERT_TRUE(p_.trailer.has.sample_loss);
    ASSERT_TRUE(p_.trailer.sample_loss);
}

TEST_F(ReadPacketTest, TrailerExtDataWithStreamId) {
    buf_[0] = 0x34000003;
    buf_[1] = 0xABABABAB;
    buf_[2] = 0x01001000;
    ASSERT_EQ(vrt_read_packet(buf_.data(), 3, &p_, true), 3);
    ASSERT_EQ(Hex(p_.header.packet_type), Hex(VRT_PT_EXT_DATA_WITH_STREAM_ID));
    ASSERT_TRUE(p_.header.has.trailer);
    ASSERT_EQ(p_.header.packet_size, 3);
    ASSERT_EQ(Hex(p_.fields.stream_id), Hex(0xABABABAB));
    ASSERT_TRUE(p_.trailer.has.sample_loss);
    ASSERT_TRUE(p_.trailer.sample_loss);
}

TEST_F(ReadPacketTest, TrailerIfContext) {
    buf_[0] = 0x44000003;
    buf_[1] = 0xABABABAB;
    buf_[2] = 0x00000000;
    ASSERT_EQ(vrt_read_packet(buf_.data(), 3, &p_, true), VRT_ERR_TRAILER_IN_CONTEXT);
    ASSERT_EQ(vrt_read_packet(buf_.data(), 3, &p_, false), 3);
    ASSERT_EQ(Hex(p_.header.packet_type), Hex(VRT_PT_IF_CONTEXT));
    ASSERT_TRUE(p_.header.has.trailer);
    ASSERT_EQ(p_.header.packet_size, 3);
    ASSERT_EQ(Hex(p_.fields.stream_id), Hex(0xABABABAB));
}

TEST_F(ReadPacketTest, TrailerExtContext) {
    buf_[0] = 0x54000003;
    buf_[1] = 0xABABABAB;
    buf_[2] = 0x00000000;
    ASSERT_EQ(vrt_read_packet(buf_.data(), 3, &p_, true), VRT_ERR_TRAILER_IN_CONTEXT);
    ASSERT_EQ(vrt_read_packet(buf_.data(), 3, &p_, false), 3);
    ASSERT_EQ(Hex(p_.header.packet_type), Hex(VRT_PT_EXT_CONTEXT));
    ASSERT_TRUE(p_.header.has.trailer);
    ASSERT_EQ(p_.header.packet_size, 3);
    ASSERT_EQ(Hex(p_.fields.stream_id), Hex(0xABABABAB));
}

TEST_F(ReadPacketTest, ValidationHeader) {
    buf_[0] = 0x02000001;
    ASSERT_EQ(vrt_read_packet(buf_.data(), 1, &p_, true), VRT_ERR_RESERVED);
    ASSERT_EQ(vrt_read_packet(buf_.data(), 1, &p_, false), 1);
    ASSERT_EQ(Hex(p_.header.packet_type), Hex(VRT_PT_IF_DATA_WITHOUT_STREAM_ID));
    ASSERT_EQ(p_.header.packet_size, 1);
}

TEST_F(ReadPacketTest, ValidationFields) {
    buf_[0] = 0x00200003;
    buf_[1] = 0xFFFFFFFF;
    buf_[2] = 0xFFFFFFFF;
    ASSERT_EQ(vrt_read_packet(buf_.data(), 3, &p_, true), VRT_ERR_BOUNDS_REAL_TIME);
    ASSERT_EQ(vrt_read_packet(buf_.data(), 3, &p_, false), 3);
    ASSERT_EQ(Hex(p_.header.packet_type), Hex(VRT_PT_IF_DATA_WITHOUT_STREAM_ID));
    ASSERT_EQ(Hex(p_.header.tsf), Hex(VRT_TSF_REAL_TIME));
    ASSERT_EQ(p_.header.packet_size, 3);
    ASSERT_EQ(Hex(p_.fields.fractional_seconds_timestamp), Hex(0xFFFFFFFFFFFFFFFF));
}

TEST_F(ReadPacketTest, ValidationIfContext) {
    buf_[0] = 0x40000005;
    buf_[1] = 0xABABABAB;
    buf_[2] = 0x00200000;
    buf_[3] = 0xFFFFFFFF;
    buf_[4] = 0xFFF00000;
    ASSERT_EQ(vrt_read_packet(buf_.data(), 5, &p_, true), VRT_ERR_BOUNDS_SAMPLE_RATE);
    ASSERT_EQ(vrt_read_packet(buf_.data(), 5, &p_, false), 5);
    ASSERT_EQ(Hex(p_.header.packet_type), Hex(VRT_PT_IF_CONTEXT));
    ASSERT_EQ(Hex(p_.fields.stream_id), Hex(0xABABABAB));
    ASSERT_EQ(p_.header.packet_size, 5);
    ASSERT_TRUE(p_.if_context.has.sample_rate);
    ASSERT_DOUBLE_EQ(p_.if_context.sample_rate, -1.0);
}

TEST_F(ReadPacketTest, ValidationPacketSizeMismatch) {
    buf_[0] = 0x40000004;
    buf_[1] = 0xABABABAB;
    buf_[2] = 0x00000000;
    ASSERT_EQ(vrt_read_packet(buf_.data(), 3, &p_, true), VRT_ERR_PACKET_SIZE_MISMATCH);
    ASSERT_EQ(vrt_read_packet(buf_.data(), 3, &p_, false), 3);
    ASSERT_EQ(Hex(p_.header.packet_type), Hex(VRT_PT_IF_CONTEXT));
    ASSERT_EQ(Hex(p_.fields.stream_id), Hex(0xABABABAB));
    ASSERT_EQ(p_.header.packet_size, 4);
}
