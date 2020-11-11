#include <gtest/gtest.h>

#include <vrt/vrt_common.h>

TEST(IsContextTest, IfDataWithout) {
    ASSERT_FALSE(vrt_is_context(VRT_PT_IF_DATA_WITHOUT_STREAM_ID));
}

TEST(IsContextTest, IfDataWith) {
    ASSERT_FALSE(vrt_is_context(VRT_PT_IF_DATA_WITH_STREAM_ID));
}

TEST(IsContextTest, ExtDataWithout) {
    ASSERT_FALSE(vrt_is_context(VRT_PT_EXT_DATA_WITHOUT_STREAM_ID));
}

TEST(IsContextTest, ExtDataWith) {
    ASSERT_FALSE(vrt_is_context(VRT_PT_EXT_DATA_WITH_STREAM_ID));
}

TEST(IsContextTest, IfContext) {
    ASSERT_TRUE(vrt_is_context(VRT_PT_IF_CONTEXT));
}

TEST(IsContextTest, ExtContext) {
    ASSERT_TRUE(vrt_is_context(VRT_PT_EXT_CONTEXT));
}
