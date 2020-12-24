#include <gtest/gtest.h>

#include <cmath>
#include <cstdint>

/* This is not nice, but whatever... */
#include <../src/vrt_fixed_point.h>

#include "hex.h"

TEST(FixedPointTest, I16ToFloat) {
    /* Examples from the standard. Note that the smaller resolution numbers are modified since they are rounded in the
     * standard. Also, the maximum numbers are almost the negation of the minimum numbers. */
    /* Figure 7.1.5.9-2 & 7.1.5.10-1 */
    ASSERT_FLOAT_EQ(vrt_fixed_point_i16_to_float(static_cast<int16_t>(0xFF80), 7), -1.0F);
    ASSERT_FLOAT_EQ(vrt_fixed_point_i16_to_float(static_cast<int16_t>(0x0080), 7), 1.0F);
    ASSERT_FLOAT_EQ(vrt_fixed_point_i16_to_float(static_cast<int16_t>(0x8000), 7), -std::exp2f(8));
    ASSERT_FLOAT_EQ(vrt_fixed_point_i16_to_float(static_cast<int16_t>(0x7FFF), 7), std::exp2f(8) - std::exp2f(-7));
    ASSERT_FLOAT_EQ(vrt_fixed_point_i16_to_float(static_cast<int16_t>(0xFFFF), 7), -std::exp2f(-7));
    ASSERT_FLOAT_EQ(vrt_fixed_point_i16_to_float(static_cast<int16_t>(0x0001), 7), std::exp2f(-7));
    /* Figure 7.1.5.15-1 */
    ASSERT_FLOAT_EQ(vrt_fixed_point_i16_to_float(static_cast<int16_t>(0xFFC0), 6), -1.0F);
    ASSERT_FLOAT_EQ(vrt_fixed_point_i16_to_float(static_cast<int16_t>(0x0040), 6), 1.0F);
    ASSERT_FLOAT_EQ(vrt_fixed_point_i16_to_float(static_cast<int16_t>(0x8000), 6), -std::exp2f(9));
    ASSERT_FLOAT_EQ(vrt_fixed_point_i16_to_float(static_cast<int16_t>(0x7FFF), 6), std::exp2f(9) - std::exp2f(-6));
    ASSERT_FLOAT_EQ(vrt_fixed_point_i16_to_float(static_cast<int16_t>(0xFFFF), 6), -std::exp2f(-6));
    ASSERT_FLOAT_EQ(vrt_fixed_point_i16_to_float(static_cast<int16_t>(0x0001), 6), std::exp2f(-6));
}

TEST(FixedPointTest, I32ToDouble) {
    /* Examples from the standard. Note that the smaller resolution numbers are modified since they are rounded in the
     * standard. */
    /* Figure 7.1.5.19-2 */
    ASSERT_DOUBLE_EQ(vrt_fixed_point_i32_to_double(static_cast<int32_t>(0x80000000), 22), -std::exp2(9));
    ASSERT_DOUBLE_EQ(vrt_fixed_point_i32_to_double(static_cast<int32_t>(0x7FFFFFFF), 22),
                     std::exp2(9) - std::exp2(-22));
    ASSERT_DOUBLE_EQ(vrt_fixed_point_i32_to_double(static_cast<int32_t>(0xFFFFFFFF), 22), -std::exp2(-22));
    ASSERT_DOUBLE_EQ(vrt_fixed_point_i32_to_double(static_cast<int32_t>(0x00000001), 22), std::exp2(-22));
    /* Figure 7.1.5.19-3 */
    ASSERT_DOUBLE_EQ(vrt_fixed_point_i32_to_double(static_cast<int32_t>(0x80000000), 5), -std::exp2(26));
    ASSERT_DOUBLE_EQ(vrt_fixed_point_i32_to_double(static_cast<int32_t>(0x7FFFFFFF), 5), std::exp2(26) - std::exp2(-5));
    ASSERT_DOUBLE_EQ(vrt_fixed_point_i32_to_double(static_cast<int32_t>(0xFFFFFFFF), 5), -std::exp2(-5));
    ASSERT_DOUBLE_EQ(vrt_fixed_point_i32_to_double(static_cast<int32_t>(0x00000001), 5), std::exp2(-5));
}

TEST(FixedPointTest, U32ToDouble) {
    /* Examples from the standard. Note that the smaller resolution numbers are modified since they are rounded in the
     * standard. */
    /* Figure 7.1.5.19-4 */
    /* Note: Observation 7.1.5.19-5 seems to have a spelling error. 65636 should be 65536. */
    ASSERT_DOUBLE_EQ(vrt_fixed_point_u32_to_double(0x00000000U, 16), 0.0);
    ASSERT_DOUBLE_EQ(vrt_fixed_point_u32_to_double(0xFFFFFFFFU, 16), std::exp2(16) - std::exp2(-16));
    ASSERT_DOUBLE_EQ(vrt_fixed_point_u32_to_double(0x00000001U, 16), std::exp2(-16));
}

TEST(FixedPointTest, I64ToDouble) {
    /* Examples from the standard. Note that the smaller resolution numbers are modified since they are rounded in the
     * standard. */
    /* Figure 7.1.5.4-1 */
    ASSERT_DOUBLE_EQ(vrt_fixed_point_i64_to_double(static_cast<int64_t>(0xFFFFFFFFFFF00000), 20), -1.0);
    ASSERT_DOUBLE_EQ(vrt_fixed_point_i64_to_double(static_cast<int64_t>(0x0000000000100000), 20), 1.0);
    /* Note that these tests would work if double had 64 bits of precision */
    // ASSERT_DOUBLE_EQ(vrt_fixed_point_i64_to_double(static_cast<int64_t>(0x8000000000000000), 20), -std::exp2(43));
    // ASSERT_DOUBLE_EQ(vrt_fixed_point_i64_to_double(static_cast<int64_t>(0x7FFFFFFFFFFFFFFF), 20), std::exp2(43)
    // - std::exp2(-20));
    ASSERT_DOUBLE_EQ(vrt_fixed_point_i64_to_double(static_cast<int64_t>(0xFFFFFFFFFFFFFFFF), 20), -std::exp2(-20));
    ASSERT_DOUBLE_EQ(vrt_fixed_point_i64_to_double(static_cast<int64_t>(0x0000000000000001), 20), std::exp2(-20));
}

TEST(FixedPointTest, FloatToI16) {
    /* Examples from the standard. Note that the smaller resolution numbers are modified since they are rounded in the
     * standard. Also, the maximum numbers are almost the negation of the minimum numbers. */
    /* Figure 7.1.5.9-2 & 7.1.5.10-1 */
    ASSERT_EQ(Hex(vrt_float_to_fixed_point_i16(-1.0F, 7)), Hex(static_cast<int16_t>(0xFF80)));
    ASSERT_EQ(Hex(vrt_float_to_fixed_point_i16(1.0F, 7)), Hex(static_cast<int16_t>(0x0080)));
    ASSERT_EQ(Hex(vrt_float_to_fixed_point_i16(-std::exp2f(8), 7)), Hex(static_cast<int16_t>(0x8000)));
    ASSERT_EQ(Hex(vrt_float_to_fixed_point_i16(std::exp2f(8) - std::exp2f(-7), 7)), Hex(static_cast<int16_t>(0x7FFF)));
    ASSERT_EQ(Hex(vrt_float_to_fixed_point_i16(-std::exp2f(-7), 7)), Hex(static_cast<int16_t>(0xFFFF)));
    ASSERT_EQ(Hex(vrt_float_to_fixed_point_i16(std::exp2f(-7), 7)), Hex(static_cast<int16_t>(0x0001)));
    /* Figure 7.1.5.15-1 */
    ASSERT_EQ(Hex(vrt_float_to_fixed_point_i16(-1.0F, 6)), Hex(static_cast<int16_t>(0xFFC0)));
    ASSERT_EQ(Hex(vrt_float_to_fixed_point_i16(1.0F, 6)), Hex(static_cast<int16_t>(0x0040)));
    ASSERT_EQ(Hex(vrt_float_to_fixed_point_i16(-std::exp2f(9), 6)), Hex(static_cast<int16_t>(0x8000)));
    ASSERT_EQ(Hex(vrt_float_to_fixed_point_i16(std::exp2f(9) - std::exp2f(-6), 6)), Hex(static_cast<int16_t>(0x7FFF)));
    ASSERT_EQ(Hex(vrt_float_to_fixed_point_i16(-std::exp2f(-6), 6)), Hex(static_cast<int16_t>(0xFFFF)));
    ASSERT_EQ(Hex(vrt_float_to_fixed_point_i16(std::exp2f(-6), 6)), Hex(static_cast<int16_t>(0x0001)));
}

TEST(FixedPointTest, DoubleToI32) {
    /* Examples from the standard. Note that the smaller resolution numbers are modified since they are rounded in the
     * standard. */
    /* Figure 7.1.5.19-2 */
    ASSERT_EQ(Hex(vrt_double_to_fixed_point_i32(-std::exp2(9), 22)), Hex(static_cast<int32_t>(0x80000000)));
    ASSERT_EQ(Hex(vrt_double_to_fixed_point_i32(std::exp2(9) - std::exp2(-22), 22)),
              Hex(static_cast<int32_t>(0x7FFFFFFF)));
    ASSERT_EQ(Hex(vrt_double_to_fixed_point_i32(-std::exp2(-22), 22)), Hex(static_cast<int32_t>(0xFFFFFFFF)));
    ASSERT_EQ(Hex(vrt_double_to_fixed_point_i32(std::exp2(-22), 22)), Hex(static_cast<int32_t>(0x00000001)));
    /* Figure 7.1.5.19-3 & 7.1.5.21-3a */
    ASSERT_EQ(Hex(vrt_double_to_fixed_point_i32(-std::exp2(26), 5)), Hex(static_cast<int32_t>(0x80000000)));
    ASSERT_EQ(Hex(vrt_double_to_fixed_point_i32(std::exp2(26) - std::exp2(-5), 5)),
              Hex(static_cast<int32_t>(0x7FFFFFFF)));
    ASSERT_EQ(Hex(vrt_double_to_fixed_point_i32(-std::exp2(-5), 5)), Hex(static_cast<int32_t>(0xFFFFFFFF)));
    ASSERT_EQ(Hex(vrt_double_to_fixed_point_i32(std::exp2(-5), 5)), Hex(static_cast<int32_t>(0x00000001)));
    /* Figure 7.1.5.21-3b */
    ASSERT_EQ(Hex(vrt_double_to_fixed_point_i32(-std::exp2(9), 22)), Hex(static_cast<int32_t>(0x80000000)));
    ASSERT_EQ(Hex(vrt_double_to_fixed_point_i32(512 - std::exp2(-22), 22)), Hex(static_cast<int32_t>(0x7FFFFFFF)));
    ASSERT_EQ(Hex(vrt_double_to_fixed_point_i32(-std::exp2(-22), 22)), Hex(static_cast<int32_t>(0xFFFFFFFF)));
    ASSERT_EQ(Hex(vrt_double_to_fixed_point_i32(std::exp2(-22), 22)), Hex(static_cast<int32_t>(0x00000001)));
    /* Figure 7.1.5.21-3c */
    ASSERT_EQ(Hex(vrt_double_to_fixed_point_i32(-std::exp2(15), 16)), Hex(static_cast<int32_t>(0x80000000)));
    ASSERT_EQ(Hex(vrt_double_to_fixed_point_i32(std::exp2(15) - std::exp2(-16), 16)),
              Hex(static_cast<int32_t>(0x7FFFFFFF)));
    ASSERT_EQ(Hex(vrt_double_to_fixed_point_i32(-std::exp2(-16), 16)), Hex(static_cast<int32_t>(0xFFFFFFFF)));
    ASSERT_EQ(Hex(vrt_double_to_fixed_point_i32(std::exp2(-16), 16)), Hex(static_cast<int32_t>(0x00000001)));
}

TEST(FixedPointTest, DoubleToU32) {
    /* Examples from the standard. Note that the smaller resolution numbers are modified since they are rounded in the
     * standard. */
    /* Figure 7.1.5.19-4 */
    /* Note: Observation 7.1.5.19-5 seems to have a spelling error. 65636 should be 65536. */
    ASSERT_EQ(Hex(vrt_double_to_fixed_point_u32(0.0, 16)), Hex(0x00000000U));
    ASSERT_EQ(Hex(vrt_double_to_fixed_point_u32(std::exp2(16) - std::exp2(-16), 16)), Hex(0xFFFFFFFFU));
    ASSERT_EQ(Hex(vrt_double_to_fixed_point_u32(std::exp2(-16), 16)), Hex(0x00000001U));
}

TEST(FixedPointTest, DoubleToI64) {
    /* Examples from the standard. Note that the smaller resolution numbers are modified since they are rounded in the
     * standard. */
    /* Figure 7.1.5.4-1 */
    ASSERT_EQ(Hex(vrt_double_to_fixed_point_i64(-1.0, 20)), Hex(static_cast<int64_t>(0xFFFFFFFFFFF00000)));
    ASSERT_EQ(Hex(vrt_double_to_fixed_point_i64(1.0, 20)), Hex(static_cast<int64_t>(0x0000000000100000)));
    /* Note that these tests would work if double had 64 bits of precision */
    // ASSERT_EQ(Hex(vrt_double_to_fixed_point_i64(-std::exp2(43), 20)), Hex(0x8000000000000000));
    // ASSERT_EQ(Hex(vrt_double_to_fixed_point_i64(std::exp2(43) - std::exp2(-20), 20)), Hex(0x7FFFFFFFFFFFFFFF));
    ASSERT_EQ(Hex(vrt_double_to_fixed_point_i64(-std::exp2(-20), 20)), Hex(static_cast<int64_t>(0xFFFFFFFFFFFFFFFF)));
    ASSERT_EQ(Hex(vrt_double_to_fixed_point_i64(std::exp2(-20), 20)), Hex(static_cast<int64_t>(0x0000000000000001)));
}
