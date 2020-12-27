#include <gtest/gtest.h>

#include <cstdint>

#include <vrt/vrt_error_code.h>
#include <vrt/vrt_init.h>
#include <vrt/vrt_time.h>
#include <vrt/vrt_types.h>

static const uint64_t PS_IN_S{1000000000000};

class TimeCalendarCalibrationTest : public ::testing::Test {
   protected:
    void SetUp() override {
        vrt_init_header(&h_);
        vrt_init_if_context(&c_);
    }

    vrt_header        h_{};
    vrt_if_context    c_{};
    vrt_calendar_time cal_{};
};

TEST_F(TimeCalendarCalibrationTest, UnexpectedField) {
    h_.packet_type = VRT_PT_IF_CONTEXT;
    ASSERT_EQ(vrt_time_calendar_calibration(&h_, &c_, &cal_), VRT_ERR_EXPECTED_FIELD);
}

TEST_F(TimeCalendarCalibrationTest, TsiNone) {
    h_.packet_type                    = VRT_PT_IF_CONTEXT;
    c_.has.timestamp_calibration_time = true;
    ASSERT_EQ(vrt_time_calendar_calibration(&h_, &c_, &cal_), VRT_ERR_INVALID_TSI);
}

TEST_F(TimeCalendarCalibrationTest, TsiUtcNow) {
    h_.packet_type                    = VRT_PT_IF_CONTEXT;
    c_.has.timestamp_calibration_time = true;
    h_.tsi                            = VRT_TSI_UTC;
    c_.timestamp_calibration_time     = 1608751092;
    ASSERT_EQ(vrt_time_calendar_calibration(&h_, &c_, &cal_), 0);
    ASSERT_EQ(cal_.year, 120);
    ASSERT_EQ(cal_.mday, 23);
    ASSERT_EQ(cal_.yday, 357);
    ASSERT_EQ(cal_.sec, 12);
    ASSERT_EQ(cal_.min, 18);
    ASSERT_EQ(cal_.hour, 19);
    ASSERT_EQ(cal_.mon, 11);
    ASSERT_EQ(cal_.wday, 3);
    ASSERT_EQ(cal_.ps, 0);
}
