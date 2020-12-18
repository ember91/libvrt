#include <gtest/gtest.h>

#include <vrt/vrt_types.h>
#include <vrt/vrt_util.h>

TEST(HasStreamIdTest, IfDataWithout) {
    vrt_header h{};
    h.packet_type = VRT_PT_IF_DATA_WITHOUT_STREAM_ID;
    ASSERT_FALSE(vrt_has_stream_id(&h));
}

TEST(HasStreamIdTest, IfDataWith) {
    vrt_header h{};
    h.packet_type = VRT_PT_IF_DATA_WITH_STREAM_ID;
    ASSERT_TRUE(vrt_has_stream_id(&h));
}

TEST(HasStreamIdTest, ExtDataWithout) {
    vrt_header h{};
    h.packet_type = VRT_PT_EXT_DATA_WITHOUT_STREAM_ID;
    ASSERT_FALSE(vrt_has_stream_id(&h));
}

TEST(HasStreamIdTest, ExtDataWith) {
    vrt_header h{};
    h.packet_type = VRT_PT_EXT_DATA_WITH_STREAM_ID;
    ASSERT_TRUE(vrt_has_stream_id(&h));
}

TEST(HasStreamIdTest, IfContext) {
    vrt_header h{};
    h.packet_type = VRT_PT_IF_CONTEXT;
    ASSERT_TRUE(vrt_has_stream_id(&h));
}

TEST(HasStreamIdTest, ExtContext) {
    vrt_header h{};
    h.packet_type = VRT_PT_EXT_CONTEXT;
    ASSERT_TRUE(vrt_has_stream_id(&h));
}
