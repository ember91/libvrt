#include <gtest/gtest.h>

#include <vrt/vrt_types.h>
#include <vrt/vrt_util.h>

TEST(IsIfContextTest, IfDataWithout) {
    ASSERT_FALSE(vrt_is_context(VRT_PT_IF_DATA_WITHOUT_STREAM_ID));
}

TEST(IsIfContextTest, IfDataWith) {
    ASSERT_FALSE(vrt_is_context(VRT_PT_IF_DATA_WITH_STREAM_ID));
}

TEST(IsIfContextTest, ExtDataWithout) {
    ASSERT_FALSE(vrt_is_context(VRT_PT_EXT_DATA_WITHOUT_STREAM_ID));
}

TEST(IsIfContextTest, ExtDataWith) {
    ASSERT_FALSE(vrt_is_context(VRT_PT_EXT_DATA_WITH_STREAM_ID));
}

TEST(IsIfContextTest, IfContext) {
    ASSERT_TRUE(vrt_is_context(VRT_PT_IF_CONTEXT));
}

TEST(IsIfContextTest, ExtContext) {
    ASSERT_TRUE(vrt_is_context(VRT_PT_EXT_CONTEXT));
}
