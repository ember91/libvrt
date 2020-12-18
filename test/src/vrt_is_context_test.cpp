#include <gtest/gtest.h>

#include <vrt/vrt_types.h>
#include <vrt/vrt_util.h>

TEST(IsIfContextTest, IfDataWithout) {
    vrt_header h{};
    h.packet_type = VRT_PT_IF_DATA_WITHOUT_STREAM_ID;
    ASSERT_FALSE(vrt_is_context(&h));
}

TEST(IsIfContextTest, IfDataWith) {
    vrt_header h{};
    h.packet_type = VRT_PT_IF_DATA_WITH_STREAM_ID;
    ASSERT_FALSE(vrt_is_context(&h));
}

TEST(IsIfContextTest, ExtDataWithout) {
    vrt_header h{};
    h.packet_type = VRT_PT_EXT_DATA_WITHOUT_STREAM_ID;
    ASSERT_FALSE(vrt_is_context(&h));
}

TEST(IsIfContextTest, ExtDataWith) {
    vrt_header h{};
    h.packet_type = VRT_PT_EXT_DATA_WITH_STREAM_ID;
    ASSERT_FALSE(vrt_is_context(&h));
}

TEST(IsIfContextTest, IfContext) {
    vrt_header h{};
    h.packet_type = VRT_PT_IF_CONTEXT;
    ASSERT_TRUE(vrt_is_context(&h));
}

TEST(IsIfContextTest, ExtContext) {
    vrt_header h{};
    h.packet_type = VRT_PT_EXT_CONTEXT;
    ASSERT_TRUE(vrt_is_context(&h));
}
