#include <gtest/gtest.h>

#include <vrt/vrt_types.h>
#include <vrt/vrt_util.h>

TEST(HasStreamIdTest, IfDataWithout) {
    ASSERT_FALSE(vrt_has_stream_id(VRT_PT_IF_DATA_WITHOUT_STREAM_ID));
}

TEST(HasStreamIdTest, IfDataWith) {
    ASSERT_TRUE(vrt_has_stream_id(VRT_PT_IF_DATA_WITH_STREAM_ID));
}

TEST(HasStreamIdTest, ExtDataWithout) {
    ASSERT_FALSE(vrt_has_stream_id(VRT_PT_EXT_DATA_WITHOUT_STREAM_ID));
}

TEST(HasStreamIdTest, ExtDataWith) {
    ASSERT_TRUE(vrt_has_stream_id(VRT_PT_EXT_DATA_WITH_STREAM_ID));
}

TEST(HasStreamIdTest, IfContext) {
    ASSERT_TRUE(vrt_has_stream_id(VRT_PT_IF_CONTEXT));
}

TEST(HasStreamIdTest, ExtContext) {
    ASSERT_TRUE(vrt_has_stream_id(VRT_PT_EXT_CONTEXT));
}
