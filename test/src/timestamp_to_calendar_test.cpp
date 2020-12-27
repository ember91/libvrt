#include <gtest/gtest.h>

#include <cstdint>

#include <../src/timestamp_to_calendar.h>
#include <vrt/vrt_error_code.h>
#include <vrt/vrt_init.h>
#include <vrt/vrt_types.h>

static const uint64_t PS_IN_S{1000000000000};

class TimestampToCalendarTest : public ::testing::Test {
   protected:
    vrt_calendar_time cal_{};
};

TEST_F(TimestampToCalendarTest, TsiNoneTsfNone) {
    ASSERT_EQ(timestamp_to_calendar(VRT_TSI_NONE, VRT_TSF_NONE, 0, 0, 0.0, &cal_), VRT_ERR_INVALID_TSI);
}

TEST_F(TimestampToCalendarTest, TsiUtcEpoch) {
    ASSERT_EQ(timestamp_to_calendar(VRT_TSI_UTC, VRT_TSF_NONE, 0, 0, 0.0, &cal_), 0);
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

TEST_F(TimestampToCalendarTest, TsiUtcNow) {
    ASSERT_EQ(timestamp_to_calendar(VRT_TSI_UTC, VRT_TSF_NONE, 1608751092, 0, 0.0, &cal_), 0);
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

TEST_F(TimestampToCalendarTest, TsiGpsEpoch) {
    ASSERT_EQ(timestamp_to_calendar(VRT_TSI_GPS, VRT_TSF_NONE, 0, 0, 0.0, &cal_), 0);
    ASSERT_EQ(cal_.year, 80);
    ASSERT_EQ(cal_.mday, 6);
    ASSERT_EQ(cal_.yday, 5);
    ASSERT_EQ(cal_.sec, 0);
    ASSERT_EQ(cal_.min, 0);
    ASSERT_EQ(cal_.hour, 0);
    ASSERT_EQ(cal_.mon, 0);
    ASSERT_EQ(cal_.wday, 0);
    ASSERT_EQ(cal_.ps, 0);
}

TEST_F(TimestampToCalendarTest, TsiGpsNow) {
    ASSERT_EQ(timestamp_to_calendar(VRT_TSI_GPS, VRT_TSF_NONE, 1292793456, 0, 0.0, &cal_), 0);
    ASSERT_EQ(cal_.year, 120);
    ASSERT_EQ(cal_.mday, 23);
    ASSERT_EQ(cal_.yday, 357);
    ASSERT_EQ(cal_.sec, 36);
    ASSERT_EQ(cal_.min, 17);
    ASSERT_EQ(cal_.hour, 21);
    ASSERT_EQ(cal_.mon, 11);
    ASSERT_EQ(cal_.wday, 3);
    ASSERT_EQ(cal_.ps, 0);
}

TEST_F(TimestampToCalendarTest, TsiOther) {
    ASSERT_EQ(timestamp_to_calendar(VRT_TSI_OTHER, VRT_TSF_NONE, 0, 0, 0.0, &cal_), VRT_ERR_INVALID_TSI);
}

TEST_F(TimestampToCalendarTest, TsfSampleCount) {
    ASSERT_EQ(timestamp_to_calendar(VRT_TSI_NONE, VRT_TSF_SAMPLE_COUNT, 0, 0, 0.0, &cal_), VRT_ERR_INVALID_TSI);
}

TEST_F(TimestampToCalendarTest, TsfRealTime) {
    ASSERT_EQ(timestamp_to_calendar(VRT_TSI_NONE, VRT_TSF_REAL_TIME, 0, 0, 0.0, &cal_), VRT_ERR_INVALID_TSI);
}

TEST_F(TimestampToCalendarTest, TsfFreeRunningCount) {
    ASSERT_EQ(timestamp_to_calendar(VRT_TSI_NONE, VRT_TSF_FREE_RUNNING_COUNT, 0, 0, 0.0, &cal_), VRT_ERR_INVALID_TSI);
}

TEST_F(TimestampToCalendarTest, TsiUtcTsfSampleCount) {
    ASSERT_EQ(timestamp_to_calendar(VRT_TSI_UTC, VRT_TSF_SAMPLE_COUNT, 1608751092, 8e6, 16e6, &cal_), 0);
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

TEST_F(TimestampToCalendarTest, TsiUtcTsfSampleCountInvalidSampleRate) {
    ASSERT_EQ(timestamp_to_calendar(VRT_TSI_UTC, VRT_TSF_SAMPLE_COUNT, 1608751092, 8e6, 0.0, &cal_),
              VRT_ERR_MISSING_SAMPLE_RATE);
}

TEST_F(TimestampToCalendarTest, TsiUtcTsfSampleCountBoundsSampleCount) {
    ASSERT_EQ(timestamp_to_calendar(VRT_TSI_UTC, VRT_TSF_SAMPLE_COUNT, 1608751092, 16e6, 16e6, &cal_),
              VRT_ERR_BOUNDS_SAMPLE_COUNT);
}

TEST_F(TimestampToCalendarTest, TsiUtcTsfRealTime) {
    ASSERT_EQ(timestamp_to_calendar(VRT_TSI_UTC, VRT_TSF_REAL_TIME, 1608751092, PS_IN_S / 2, 0.0, &cal_), 0);
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

TEST_F(TimestampToCalendarTest, TsiUtcTsfBoundsRealTime) {
    ASSERT_EQ(timestamp_to_calendar(VRT_TSI_UTC, VRT_TSF_REAL_TIME, 1608751092, PS_IN_S, 0.0, &cal_),
              VRT_ERR_BOUNDS_REAL_TIME);
}

TEST_F(TimestampToCalendarTest, TsiUtcTsfFreeRunningCount) {
    ASSERT_EQ(timestamp_to_calendar(VRT_TSI_UTC, VRT_TSF_FREE_RUNNING_COUNT, 1608751092, 0, 0.0, &cal_),
              VRT_ERR_INVALID_TSF);
}

TEST_F(TimestampToCalendarTest, TsiGpsTsfSampleCount) {
    ASSERT_EQ(timestamp_to_calendar(VRT_TSI_GPS, VRT_TSF_SAMPLE_COUNT, 1292793456, 8e6, 16e6, &cal_), 0);
    ASSERT_EQ(cal_.year, 120);
    ASSERT_EQ(cal_.mday, 23);
    ASSERT_EQ(cal_.yday, 357);
    ASSERT_EQ(cal_.sec, 36);
    ASSERT_EQ(cal_.min, 17);
    ASSERT_EQ(cal_.hour, 21);
    ASSERT_EQ(cal_.mon, 11);
    ASSERT_EQ(cal_.wday, 3);
    ASSERT_EQ(cal_.ps, PS_IN_S / 2);
}

TEST_F(TimestampToCalendarTest, TsiGpsTsfSampleCountInvalidSampleRate) {
    ASSERT_EQ(timestamp_to_calendar(VRT_TSI_GPS, VRT_TSF_SAMPLE_COUNT, 1292793456, 8e6, 0.0, &cal_),
              VRT_ERR_MISSING_SAMPLE_RATE);
}

TEST_F(TimestampToCalendarTest, TsiGpsTsfSampleCountBoundsSampleCount) {
    ASSERT_EQ(timestamp_to_calendar(VRT_TSI_GPS, VRT_TSF_SAMPLE_COUNT, 1292793456, 16e6, 16e6, &cal_),
              VRT_ERR_BOUNDS_SAMPLE_COUNT);
}

TEST_F(TimestampToCalendarTest, TsiGpsTsfRealTime) {
    ASSERT_EQ(timestamp_to_calendar(VRT_TSI_GPS, VRT_TSF_REAL_TIME, 1292793456, PS_IN_S / 2, 0.0, &cal_), 0);
    ASSERT_EQ(cal_.year, 120);
    ASSERT_EQ(cal_.mday, 23);
    ASSERT_EQ(cal_.yday, 357);
    ASSERT_EQ(cal_.sec, 36);
    ASSERT_EQ(cal_.min, 17);
    ASSERT_EQ(cal_.hour, 21);
    ASSERT_EQ(cal_.mon, 11);
    ASSERT_EQ(cal_.wday, 3);
    ASSERT_EQ(cal_.ps, PS_IN_S / 2);
}

TEST_F(TimestampToCalendarTest, TsiGpsTsfBoundsRealTime) {
    ASSERT_EQ(timestamp_to_calendar(VRT_TSI_GPS, VRT_TSF_REAL_TIME, 1292793456, PS_IN_S, 0.0, &cal_),
              VRT_ERR_BOUNDS_REAL_TIME);
}

TEST_F(TimestampToCalendarTest, TsiGpsTsfFreeRunningCount) {
    ASSERT_EQ(timestamp_to_calendar(VRT_TSI_GPS, VRT_TSF_FREE_RUNNING_COUNT, 1292793456, 0, 0.0, &cal_),
              VRT_ERR_INVALID_TSF);
}

TEST_F(TimestampToCalendarTest, TsiOtherTsfSampleCount) {
    ASSERT_EQ(timestamp_to_calendar(VRT_TSI_OTHER, VRT_TSF_SAMPLE_COUNT, 1000, 0, 0.0, &cal_), VRT_ERR_INVALID_TSI);
}

TEST_F(TimestampToCalendarTest, TsiOtherTsfRealTime) {
    ASSERT_EQ(timestamp_to_calendar(VRT_TSI_OTHER, VRT_TSF_REAL_TIME, 1000, 0, 16e6, &cal_), VRT_ERR_INVALID_TSI);
}

TEST_F(TimestampToCalendarTest, TsiOtherTsfRealTimeInvalidSampleRate) {
    ASSERT_EQ(timestamp_to_calendar(VRT_TSI_OTHER, VRT_TSF_REAL_TIME, 1000, 0, 0.0, &cal_), VRT_ERR_INVALID_TSI);
}

TEST_F(TimestampToCalendarTest, TsiOtherTsfFreeRunningCount) {
    ASSERT_EQ(timestamp_to_calendar(VRT_TSI_OTHER, VRT_TSF_FREE_RUNNING_COUNT, 1000, 0, 0.0, &cal_),
              VRT_ERR_INVALID_TSI);
}
