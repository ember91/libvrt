#include <gtest/gtest.h>

#include <array>
#include <cstdint>
#include <limits>
#include <vector>

#include <vrt/vrt_error_code.h>
#include <vrt/vrt_init.h>
#include <vrt/vrt_types.h>
#include <vrt/vrt_write.h>

#include "hex.h"

class WritePacketTest : public ::testing::Test {
   protected:
    void SetUp() override {
        vrt_init_packet(&p_);
        buf_.fill(0xBAADF00D);
    }

    vrt_packet               p_{};
    std::array<uint32_t, 32> buf_{};
};

TEST_F(WritePacketTest, NegativeSizeBuffer) {
    ASSERT_EQ(vrt_write_packet(&p_, buf_.data(), -1, true), VRT_ERR_BUFFER_SIZE);
    ASSERT_EQ(vrt_write_packet(&p_, buf_.data(), -1, false), VRT_ERR_BUFFER_SIZE);
}

TEST_F(WritePacketTest, ZeroSizeBuffer) {
    ASSERT_EQ(vrt_write_packet(&p_, buf_.data(), 0, true), VRT_ERR_BUFFER_SIZE);
    ASSERT_EQ(vrt_write_packet(&p_, buf_.data(), 0, false), VRT_ERR_BUFFER_SIZE);
}

TEST_F(WritePacketTest, EmptyIfDataWithoutStreamId) {
    p_.header.packet_type = VRT_PT_IF_DATA_WITHOUT_STREAM_ID;
    ASSERT_EQ(vrt_write_packet(&p_, buf_.data(), 1, true), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000001));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}

TEST_F(WritePacketTest, EmptyIfDataWithStreamId) {
    p_.header.packet_type = VRT_PT_IF_DATA_WITH_STREAM_ID;
    p_.fields.stream_id   = 0xABABABAB;
    ASSERT_EQ(vrt_write_packet(&p_, buf_.data(), 2, true), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x10000002));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xABABABAB));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WritePacketTest, EmptyExtDataWithoutStreamId) {
    p_.header.packet_type = VRT_PT_EXT_DATA_WITHOUT_STREAM_ID;
    ASSERT_EQ(vrt_write_packet(&p_, buf_.data(), 1, true), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x20000001));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}

TEST_F(WritePacketTest, EmptyExtDataWithStreamId) {
    p_.header.packet_type = VRT_PT_EXT_DATA_WITH_STREAM_ID;
    p_.fields.stream_id   = 0xABABABAB;
    ASSERT_EQ(vrt_write_packet(&p_, buf_.data(), 2, true), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x30000002));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xABABABAB));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WritePacketTest, EmptyIfContext) {
    p_.header.packet_type = VRT_PT_IF_CONTEXT;
    p_.fields.stream_id   = 0xABABABAB;
    ASSERT_EQ(vrt_write_packet(&p_, buf_.data(), 3, true), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x40000003));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xABABABAB));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WritePacketTest, EmptyExtContext) {
    p_.header.packet_type = VRT_PT_EXT_CONTEXT;
    p_.fields.stream_id   = 0xABABABAB;
    ASSERT_EQ(vrt_write_packet(&p_, buf_.data(), 2, true), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x50000002));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xABABABAB));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WritePacketTest, BodyIfDataWithoutStreamId) {
    p_.header.packet_type        = VRT_PT_IF_DATA_WITHOUT_STREAM_ID;
    std::array<uint32_t, 3> body = {};
    body[0]                      = 0xCECECECE;
    body[1]                      = 0xFEFEFEFE;
    body[2]                      = 0xDEDEDEDE;
    p_.words_body                = body.size();
    p_.body                      = body.data();
    ASSERT_EQ(vrt_write_packet(&p_, buf_.data(), 4, true), 4);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000004));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xCECECECE));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xFEFEFEFE));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xDEDEDEDE));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xBAADF00D));
}

TEST_F(WritePacketTest, BodyIfDataWithStreamId) {
    p_.header.packet_type = VRT_PT_IF_DATA_WITH_STREAM_ID;
    p_.fields.stream_id   = 0xABABABAB;
    std::array<uint32_t, 3> body{};
    body[0]       = 0xCECECECE;
    body[1]       = 0xFEFEFEFE;
    body[2]       = 0xDEDEDEDE;
    p_.words_body = body.size();
    p_.body       = body.data();
    ASSERT_EQ(vrt_write_packet(&p_, buf_.data(), 5, true), 5);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x10000005));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xABABABAB));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xCECECECE));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xFEFEFEFE));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xDEDEDEDE));
    ASSERT_EQ(Hex(buf_[5]), Hex(0xBAADF00D));
}

TEST_F(WritePacketTest, BodyExtDataWithoutStreamId) {
    p_.header.packet_type = VRT_PT_EXT_DATA_WITHOUT_STREAM_ID;
    std::array<uint32_t, 3> body{};
    body[0]       = 0xCECECECE;
    body[1]       = 0xFEFEFEFE;
    body[2]       = 0xDEDEDEDE;
    p_.words_body = body.size();
    p_.body       = body.data();
    ASSERT_EQ(vrt_write_packet(&p_, buf_.data(), 4, true), 4);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x20000004));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xCECECECE));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xFEFEFEFE));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xDEDEDEDE));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xBAADF00D));
}

TEST_F(WritePacketTest, BodyExtDataWithStreamId) {
    p_.header.packet_type = VRT_PT_EXT_DATA_WITH_STREAM_ID;
    p_.fields.stream_id   = 0xABABABAB;
    std::array<uint32_t, 3> body{};
    body[0]       = 0xCECECECE;
    body[1]       = 0xFEFEFEFE;
    body[2]       = 0xDEDEDEDE;
    p_.words_body = body.size();
    p_.body       = body.data();
    ASSERT_EQ(vrt_write_packet(&p_, buf_.data(), 5, true), 5);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x30000005));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xABABABAB));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xCECECECE));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xFEFEFEFE));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xDEDEDEDE));
    ASSERT_EQ(Hex(buf_[5]), Hex(0xBAADF00D));
}

TEST_F(WritePacketTest, BodyIfContext) {
    p_.header.packet_type = VRT_PT_IF_CONTEXT;
    p_.fields.stream_id   = 0xABABABAB;
    std::array<uint32_t, 3> body{};
    body[0]       = 0xCECECECE;
    body[1]       = 0xFEFEFEFE;
    body[2]       = 0xDEDEDEDE;
    p_.words_body = body.size();
    p_.body       = body.data();
    ASSERT_EQ(vrt_write_packet(&p_, buf_.data(), 3, true), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x40000003));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xABABABAB));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WritePacketTest, BodyExtContext) {
    p_.header.packet_type = VRT_PT_EXT_CONTEXT;
    p_.fields.stream_id   = 0xABABABAB;
    std::array<uint32_t, 3> body{};
    body[0]       = 0xCECECECE;
    body[1]       = 0xFEFEFEFE;
    body[2]       = 0xDEDEDEDE;
    p_.words_body = body.size();
    p_.body       = body.data();
    ASSERT_EQ(vrt_write_packet(&p_, buf_.data(), 5, true), 5);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x50000005));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xABABABAB));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xCECECECE));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xFEFEFEFE));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xDEDEDEDE));
    ASSERT_EQ(Hex(buf_[5]), Hex(0xBAADF00D));
}

TEST_F(WritePacketTest, TrailerIfDataWithoutStreamId) {
    p_.header.packet_type      = VRT_PT_IF_DATA_WITHOUT_STREAM_ID;
    p_.header.has.trailer      = true;
    p_.trailer.has.sample_loss = true;
    p_.trailer.sample_loss     = true;
    ASSERT_EQ(vrt_write_packet(&p_, buf_.data(), 2, true), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x04000002));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x01001000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WritePacketTest, TrailerIfDataWithStreamId) {
    p_.header.packet_type      = VRT_PT_IF_DATA_WITH_STREAM_ID;
    p_.fields.stream_id        = 0xABABABAB;
    p_.header.has.trailer      = true;
    p_.trailer.has.sample_loss = true;
    p_.trailer.sample_loss     = true;
    ASSERT_EQ(vrt_write_packet(&p_, buf_.data(), 3, true), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x14000003));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xABABABAB));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x01001000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WritePacketTest, TrailerExtDataWithoutStreamId) {
    p_.header.packet_type      = VRT_PT_EXT_DATA_WITHOUT_STREAM_ID;
    p_.header.has.trailer      = true;
    p_.trailer.has.sample_loss = true;
    p_.trailer.sample_loss     = true;
    ASSERT_EQ(vrt_write_packet(&p_, buf_.data(), 2, true), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x24000002));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x01001000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WritePacketTest, TrailerExtDataWithStreamId) {
    p_.header.packet_type      = VRT_PT_EXT_DATA_WITH_STREAM_ID;
    p_.fields.stream_id        = 0xABABABAB;
    p_.header.has.trailer      = true;
    p_.trailer.has.sample_loss = true;
    p_.trailer.sample_loss     = true;
    ASSERT_EQ(vrt_write_packet(&p_, buf_.data(), 3, true), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x34000003));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xABABABAB));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x01001000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WritePacketTest, TrailerIfContext) {
    p_.header.packet_type      = VRT_PT_IF_CONTEXT;
    p_.fields.stream_id        = 0xABABABAB;
    p_.header.has.trailer      = true;
    p_.trailer.has.sample_loss = true;
    p_.trailer.sample_loss     = true;
    ASSERT_EQ(vrt_write_packet(&p_, buf_.data(), 3, true), VRT_ERR_TRAILER_IN_CONTEXT);
    ASSERT_EQ(vrt_write_packet(&p_, buf_.data(), 3, false), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x44000003));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xABABABAB));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WritePacketTest, TrailerExtContext) {
    p_.header.packet_type      = VRT_PT_EXT_CONTEXT;
    p_.fields.stream_id        = 0xABABABAB;
    p_.header.has.trailer      = true;
    p_.trailer.has.sample_loss = true;
    p_.trailer.sample_loss     = true;
    ASSERT_EQ(vrt_write_packet(&p_, buf_.data(), 2, true), VRT_ERR_TRAILER_IN_CONTEXT);
    ASSERT_EQ(vrt_write_packet(&p_, buf_.data(), 2, false), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x54000002));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xABABABAB));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WritePacketTest, ValidationBodyBufferSize) {
    p_.header.packet_count = static_cast<vrt_packet_type>(3);
    p_.words_body          = 5;
    ASSERT_EQ(vrt_write_packet(&p_, buf_.data(), 1, true), VRT_ERR_BUFFER_SIZE);
}

TEST_F(WritePacketTest, ValidationHeader) {
    p_.header.packet_count = static_cast<vrt_packet_type>(0xFF);
    ASSERT_EQ(vrt_write_packet(&p_, buf_.data(), 1, true), VRT_ERR_BOUNDS_PACKET_COUNT);
    ASSERT_EQ(vrt_write_packet(&p_, buf_.data(), 1, false), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x000F0001));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}

TEST_F(WritePacketTest, ValidationFields) {
    p_.header.tsf                          = VRT_TSF_REAL_TIME;
    p_.fields.fractional_seconds_timestamp = 0xFFFFFFFFFFFFFFFF;
    ASSERT_EQ(vrt_write_packet(&p_, buf_.data(), 3, true), VRT_ERR_BOUNDS_REAL_TIME);
    ASSERT_EQ(vrt_write_packet(&p_, buf_.data(), 3, false), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00200003));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WritePacketTest, ValidationTrailer) {
    p_.header.has.trailer                          = true;
    p_.trailer.has.associated_context_packet_count = true;
    p_.trailer.associated_context_packet_count     = 0xFF;
    ASSERT_EQ(vrt_write_packet(&p_, buf_.data(), 2, true), VRT_ERR_BOUNDS_ASSOCIATED_CONTEXT_PACKET_COUNT);
    ASSERT_EQ(vrt_write_packet(&p_, buf_.data(), 2, false), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x04000002));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x000000FF));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WritePacketTest, ValidationIfContext) {
    p_.header.packet_type         = VRT_PT_IF_CONTEXT;
    p_.fields.stream_id           = 0xABABABAB;
    p_.if_context.has.sample_rate = true;
    p_.if_context.sample_rate     = -1.0;
    ASSERT_EQ(vrt_write_packet(&p_, buf_.data(), 5, true), VRT_ERR_BOUNDS_SAMPLE_RATE);
    ASSERT_EQ(vrt_write_packet(&p_, buf_.data(), 5, false), 5);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x40000005));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xABABABAB));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x00200000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xFFF00000));
    ASSERT_EQ(Hex(buf_[5]), Hex(0xBAADF00D));
}

TEST_F(WritePacketTest, ValidationBoundsPacketSize) {
    /* Use heap allocated vectors instead here to avoid stack overflow */
    std::vector<uint32_t> buf_big;
    buf_big.resize(std::numeric_limits<uint16_t>::max() + 100);
    std::vector<uint32_t> body;
    body.resize(std::numeric_limits<uint16_t>::max());

    p_.header.packet_type = VRT_PT_EXT_CONTEXT;
    p_.words_body         = body.size();
    p_.body               = body.data();
    ASSERT_EQ(vrt_write_packet(&p_, buf_big.data(), buf_big.size(), true), VRT_ERR_BOUNDS_PACKET_SIZE);
}
