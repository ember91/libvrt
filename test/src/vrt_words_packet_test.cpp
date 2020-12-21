#include <gtest/gtest.h>

#include <vrt/vrt_init.h>
#include <vrt/vrt_types.h>
#include <vrt/vrt_words.h>

class WordsPacketTest : public ::testing::Test {
   protected:
    void SetUp() override { vrt_init_packet(&p_); }

    vrt_packet p_{};
};

TEST_F(WordsPacketTest, None) {
    ASSERT_EQ(vrt_words_packet(&p_), 1);
}

TEST_F(WordsPacketTest, Fields) {
    p_.header.has.class_id = true;
    ASSERT_EQ(vrt_words_packet(&p_), 3);
}

TEST_F(WordsPacketTest, Body) {
    p_.words_body = 45;
    ASSERT_EQ(vrt_words_packet(&p_), 46);
}

TEST_F(WordsPacketTest, IfContext) {
    p_.header.packet_type  = VRT_PT_IF_CONTEXT;
    p_.if_context.has.gain = true;
    ASSERT_EQ(vrt_words_packet(&p_), 4);
}

TEST_F(WordsPacketTest, TrailerIfDataWithoutStreamId) {
    p_.header.has.trailer = true;
    ASSERT_EQ(vrt_words_packet(&p_), 2);
}

TEST_F(WordsPacketTest, TrailerIfContext) {
    p_.header.packet_type = VRT_PT_IF_CONTEXT;
    p_.header.has.trailer = true;
    ASSERT_EQ(vrt_words_packet(&p_), 3);
}

TEST_F(WordsPacketTest, BodyIfDataWithoutStreamId) {
    p_.header.has.class_id = true;
    p_.header.has.trailer  = true;
    p_.words_body          = 45;
    ASSERT_EQ(vrt_words_packet(&p_), 49);
}

TEST_F(WordsPacketTest, BodyIfContext) {
    p_.header.packet_type         = VRT_PT_IF_CONTEXT;
    p_.header.has.class_id        = true;
    p_.header.has.trailer         = true;
    p_.words_body                 = 45;
    p_.if_context.has.sample_rate = true;
    ASSERT_EQ(vrt_words_packet(&p_), 7);
}
