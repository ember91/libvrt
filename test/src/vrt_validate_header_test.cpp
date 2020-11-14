#include <gtest/gtest.h>

#include <vrt/vrt_common.h>
#include <vrt/vrt_validate.h>

class ValidateHeaderTest : public ::testing::Test {
   protected:
    ValidateHeaderTest() : h_() {}

    void SetUp() override { vrt_init_header(&h_); }

    vrt_header h_;
};

TEST_F(ValidateHeaderTest, None) {
    ASSERT_EQ(vrt_validate_header(&h_), 0);
}

TEST_F(ValidateHeaderTest, PacketTypeValid) {
    h_.packet_type = static_cast<vrt_packet_type>(0);
    ASSERT_EQ(vrt_validate_header(&h_), 0);
    h_.packet_type = static_cast<vrt_packet_type>(5);
    ASSERT_EQ(vrt_validate_header(&h_), 0);
}

TEST_F(ValidateHeaderTest, PacketType) {
    h_.packet_type = static_cast<vrt_packet_type>(-1);
    ASSERT_EQ(vrt_validate_header(&h_), VRT_ERR_PACKET_TYPE);
    h_.packet_type = static_cast<vrt_packet_type>(6);
    ASSERT_EQ(vrt_validate_header(&h_), VRT_ERR_PACKET_TYPE);
}

TEST_F(ValidateHeaderTest, TrailerValid) {
    h_.packet_type = VRT_PT_IF_DATA_WITHOUT_STREAM_ID;
    h_.has.trailer = true;
    ASSERT_EQ(vrt_validate_header(&h_), 0);
}

TEST_F(ValidateHeaderTest, TrailerInvalid) {
    h_.packet_type = VRT_PT_IF_CONTEXT;
    h_.has.trailer = true;
    ASSERT_EQ(vrt_validate_header(&h_), VRT_ERR_TRAILER);
}

TEST_F(ValidateHeaderTest, TsmValid) {
    h_.packet_type = VRT_PT_EXT_CONTEXT;
    h_.tsm         = true;
    ASSERT_EQ(vrt_validate_header(&h_), 0);
}

TEST_F(ValidateHeaderTest, TsmInvalid) {
    h_.packet_type = VRT_PT_EXT_DATA_WITH_STREAM_ID;
    h_.tsm         = true;
    ASSERT_EQ(vrt_validate_header(&h_), VRT_ERR_TSM);
}

TEST_F(ValidateHeaderTest, TsiValid) {
    h_.tsi = static_cast<vrt_tsi>(0);
    ASSERT_EQ(vrt_validate_header(&h_), 0);
    h_.tsi = static_cast<vrt_tsi>(3);
    ASSERT_EQ(vrt_validate_header(&h_), 0);
}

TEST_F(ValidateHeaderTest, TsiInvalid) {
    h_.tsi = static_cast<vrt_tsi>(-1);
    ASSERT_EQ(vrt_validate_header(&h_), VRT_ERR_TSI);
    h_.tsi = static_cast<vrt_tsi>(4);
    ASSERT_EQ(vrt_validate_header(&h_), VRT_ERR_TSI);
}

TEST_F(ValidateHeaderTest, TsfValid) {
    h_.tsf = static_cast<vrt_tsf>(0);
    ASSERT_EQ(vrt_validate_header(&h_), 0);
    h_.tsf = static_cast<vrt_tsf>(3);
    ASSERT_EQ(vrt_validate_header(&h_), 0);
}

TEST_F(ValidateHeaderTest, TsfInvalid) {
    h_.tsf = static_cast<vrt_tsf>(-1);
    ASSERT_EQ(vrt_validate_header(&h_), VRT_ERR_TSF);
    h_.tsf = static_cast<vrt_tsf>(4);
    ASSERT_EQ(vrt_validate_header(&h_), VRT_ERR_TSF);
}

TEST_F(ValidateHeaderTest, PacketCountValid) {
    h_.packet_count = 0x0F;
}

TEST_F(ValidateHeaderTest, PacketCountInvalid) {
    h_.packet_count = 0x10;
}
