#include <gtest/gtest.h>

#include <vrt/vrt_common.h>
#include <vrt/vrt_validate.h>

class ValidateTrailerTest : public ::testing::Test {
   protected:
    ValidateTrailerTest() : t_() {}

    void SetUp() override { vrt_init_trailer(&t_); }

    vrt_trailer t_;
};

TEST_F(ValidateTrailerTest, None) {
    ASSERT_EQ(vrt_validate_trailer(&t_), 0);
}

TEST_F(ValidateTrailerTest, AssociatedPacketCount) {
    t_.associated_context_packet_count = 0xFF;
    ASSERT_EQ(vrt_validate_trailer(&t_), 0);
}

TEST_F(ValidateTrailerTest, AssociatedPacketCountValid) {
    t_.has.associated_context_packet_count = true;
    t_.associated_context_packet_count     = 0x7F;
    ASSERT_EQ(vrt_validate_trailer(&t_), 0);
}

TEST_F(ValidateTrailerTest, AssociatedPacketCountInvalid) {
    t_.has.associated_context_packet_count = true;
    t_.associated_context_packet_count     = 0xFF;
    ASSERT_EQ(vrt_validate_trailer(&t_), VRT_ERR_ASSOCIATED_CONTEXT_PACKET_COUNT);
}
