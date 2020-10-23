#include <gtest/gtest.h>

#include <vrt/vrt_common.h>

class WordsTrailerTest : public ::testing::Test {
   protected:
    WordsTrailerTest() : h_() {}

    void SetUp() override { vrt_init_header(&h_); }

    vrt_header h_;
};

TEST_F(WordsTrailerTest, NoTrailer) {
    ASSERT_EQ(vrt_words_trailer(&h_), 0);
}

TEST_F(WordsTrailerTest, PacketType1) {
    h_.has.trailer = true;
    ASSERT_EQ(vrt_words_trailer(&h_), 1);
}

TEST_F(WordsTrailerTest, PacketType2) {
    h_.packet_type = VRT_PT_IF_DATA_WITH_STREAM_ID;
    h_.has.trailer = true;
    ASSERT_EQ(vrt_words_trailer(&h_), 1);
}

TEST_F(WordsTrailerTest, PacketType3) {
    h_.packet_type = VRT_PT_EXT_DATA_WITHOUT_STREAM_ID;
    h_.has.trailer = true;
    ASSERT_EQ(vrt_words_trailer(&h_), 1);
}

TEST_F(WordsTrailerTest, PacketType4) {
    h_.packet_type = VRT_PT_EXT_DATA_WITH_STREAM_ID;
    h_.has.trailer = true;
    ASSERT_EQ(vrt_words_trailer(&h_), 1);
}

TEST_F(WordsTrailerTest, PacketType5) {
    h_.packet_type = VRT_PT_IF_CONTEXT;
    h_.has.trailer = true;
    ASSERT_EQ(vrt_words_trailer(&h_), 0);
}

TEST_F(WordsTrailerTest, PacketType6) {
    h_.packet_type = VRT_PT_EXT_CONTEXT;
    h_.has.trailer = true;
    ASSERT_EQ(vrt_words_trailer(&h_), 0);
}
