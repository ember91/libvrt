#include <gtest/gtest.h>

#include <cstdint>

#include <vrt/vrt_error_code.h>
#include <vrt/vrt_init.h>
#include <vrt/vrt_time.h>
#include <vrt/vrt_types.h>

static const uint64_t PS_IN_S{1000000000000};

class TimeCalendarFieldsTest : public ::testing::Test {
   protected:
    void SetUp() override {
        vrt_init_header(&h_);
        vrt_init_fields(&f_);
    }

    vrt_header        h_{};
    vrt_fields        f_{};
    vrt_calendar_time cal_{};
};

TEST_F(TimeCalendarFieldsTest, TsiNone) {
    ASSERT_EQ(vrt_time_calendar_fields(&h_, &f_, 0.0, &cal_), VRT_ERR_INVALID_TSI);
}

TEST_F(TimeCalendarFieldsTest, TsiUtcEpoch) {
    h_.tsi                       = VRT_TSI_UTC;
    f_.integer_seconds_timestamp = 0;
    ASSERT_EQ(vrt_time_calendar_fields(&h_, &f_, 0.0, &cal_), 0);
    ASSERT_EQ(cal_.year, 70);
    ASSERT_EQ(cal_.mday, 1);
    ASSERT_EQ(cal_.yday, 0);
    ASSERT_EQ(cal_.sec, 0);
    ASSERT_EQ(cal_.min, 0);
    ASSERT_EQ(cal_.hour, 0);
    ASSERT_EQ(cal_.mon, 0);
    ASSERT_EQ(cal_.wday, 4);
    ASSERT_EQ(cal_.ps, 0);
}

TEST_F(TimeCalendarFieldsTest, TsiUtcTsfSampleCount) {
    h_.tsi                          = VRT_TSI_UTC;
    f_.integer_seconds_timestamp    = 1608751092;
    h_.tsf                          = VRT_TSF_SAMPLE_COUNT;
    f_.fractional_seconds_timestamp = 8e6;
    ASSERT_EQ(vrt_time_calendar_fields(&h_, &f_, 16e6, &cal_), 0);
    ASSERT_EQ(cal_.year, 120);
    ASSERT_EQ(cal_.mday, 23);
    ASSERT_EQ(cal_.yday, 357);
    ASSERT_EQ(cal_.sec, 12);
    ASSERT_EQ(cal_.min, 18);
    ASSERT_EQ(cal_.hour, 19);
    ASSERT_EQ(cal_.mon, 11);
    ASSERT_EQ(cal_.wday, 3);
    ASSERT_EQ(cal_.ps, PS_IN_S / 2);
}
