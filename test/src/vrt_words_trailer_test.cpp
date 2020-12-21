#include <gtest/gtest.h>

#include <vrt/vrt_init.h>
#include <vrt/vrt_types.h>
#include <vrt/vrt_words.h>

class WordsTrailerTest : public ::testing::Test {
   protected:
    void SetUp() override { vrt_init_header(&h_); }

    vrt_header h_{};
};

TEST_F(WordsTrailerTest, NoTrailer) {
    ASSERT_EQ(vrt_words_trailer(&h_), 0);
}

TEST_F(WordsTrailerTest, IfDataWithoutStreamId) {
    h_.has.trailer = true;
    ASSERT_EQ(vrt_words_trailer(&h_), 1);
}

TEST_F(WordsTrailerTest, IfDataWithStreamId) {
    h_.packet_type = VRT_PT_IF_DATA_WITH_STREAM_ID;
    h_.has.trailer = true;
    ASSERT_EQ(vrt_words_trailer(&h_), 1);
}

TEST_F(WordsTrailerTest, ExtDataWithoutStreamId) {
    h_.packet_type = VRT_PT_EXT_DATA_WITHOUT_STREAM_ID;
    h_.has.trailer = true;
    ASSERT_EQ(vrt_words_trailer(&h_), 1);
}

TEST_F(WordsTrailerTest, ExtDataWithStreamId) {
    h_.packet_type = VRT_PT_EXT_DATA_WITH_STREAM_ID;
    h_.has.trailer = true;
    ASSERT_EQ(vrt_words_trailer(&h_), 1);
}

TEST_F(WordsTrailerTest, IfContext) {
    h_.packet_type = VRT_PT_IF_CONTEXT;
    h_.has.trailer = true;
    ASSERT_EQ(vrt_words_trailer(&h_), 0);
}

TEST_F(WordsTrailerTest, ExtContext) {
    h_.packet_type = VRT_PT_EXT_CONTEXT;
    h_.has.trailer = true;
    ASSERT_EQ(vrt_words_trailer(&h_), 0);
}
