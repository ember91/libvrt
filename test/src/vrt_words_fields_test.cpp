#include <gtest/gtest.h>

#include <vrt/vrt_init.h>
#include <vrt/vrt_types.h>
#include <vrt/vrt_words.h>

class WordsFieldsTest : public ::testing::Test {
   protected:
    void SetUp() override { vrt_init_header(&h_); }

    vrt_header h_{};
};

TEST_F(WordsFieldsTest, IfDataWithoutStreamId) {
    ASSERT_EQ(vrt_words_fields(&h_), 0);
    h_.packet_type = VRT_PT_IF_DATA_WITHOUT_STREAM_ID;
}

TEST_F(WordsFieldsTest, IfDataWithStreamId) {
    h_.packet_type = VRT_PT_IF_DATA_WITH_STREAM_ID;
    ASSERT_EQ(vrt_words_fields(&h_), 1);
}

TEST_F(WordsFieldsTest, ExtDataWithoutStreamId) {
    h_.packet_type = VRT_PT_EXT_DATA_WITHOUT_STREAM_ID;
    ASSERT_EQ(vrt_words_fields(&h_), 0);
}

TEST_F(WordsFieldsTest, ExtDataWithStreamId) {
    h_.packet_type = VRT_PT_EXT_DATA_WITH_STREAM_ID;
    ASSERT_EQ(vrt_words_fields(&h_), 1);
}

TEST_F(WordsFieldsTest, IfContext) {
    h_.packet_type = VRT_PT_IF_CONTEXT;
    ASSERT_EQ(vrt_words_fields(&h_), 1);
}

TEST_F(WordsFieldsTest, ExtContext) {
    h_.packet_type = VRT_PT_EXT_CONTEXT;
    ASSERT_EQ(vrt_words_fields(&h_), 1);
}

TEST_F(WordsFieldsTest, ClassId) {
    h_.has.class_id = true;
    ASSERT_EQ(vrt_words_fields(&h_), 2);
}

TEST_F(WordsFieldsTest, Tsi) {
    h_.tsi = VRT_TSI_UTC;
    ASSERT_EQ(vrt_words_fields(&h_), 1);
}

TEST_F(WordsFieldsTest, Tsf) {
    h_.tsf = VRT_TSF_REAL_TIME;
    ASSERT_EQ(vrt_words_fields(&h_), 2);
}

TEST_F(WordsFieldsTest, EveryOther1) {
    h_.packet_type = VRT_PT_IF_DATA_WITH_STREAM_ID;
    h_.tsi         = VRT_TSI_UTC;
    ASSERT_EQ(vrt_words_fields(&h_), 2);
}

TEST_F(WordsFieldsTest, EveryOther2) {
    h_.has.class_id = true;
    h_.tsf          = VRT_TSF_FREE_RUNNING_COUNT;
    ASSERT_EQ(vrt_words_fields(&h_), 4);
}

TEST_F(WordsFieldsTest, All) {
    h_.packet_type  = VRT_PT_IF_DATA_WITH_STREAM_ID;
    h_.has.class_id = true;
    h_.tsi          = VRT_TSI_GPS;
    h_.tsf          = VRT_TSF_SAMPLE_COUNT;
    ASSERT_EQ(vrt_words_fields(&h_), 6);
}
