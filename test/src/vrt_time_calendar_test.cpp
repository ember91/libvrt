#include <gtest/gtest.h>

#include <cstdint>

#include <vrt/vrt_error_code.h>
#include <vrt/vrt_init.h>
#include <vrt/vrt_time.h>
#include <vrt/vrt_types.h>

static const uint64_t PS_IN_S{1000000000000};

class TimeCalendarTest : public ::testing::Test {
   protected:
    void SetUp() override { vrt_init_packet(&p_); }

    vrt_packet        p_{};
    vrt_calendar_time cal_{};
};

TEST_F(TimeCalendarTest, TsiNone) {
    ASSERT_EQ(vrt_time_calendar(&p_, 0.0, &cal_), VRT_ERR_INVALID_TSI);
}

TEST_F(TimeCalendarTest, TsiUtcEpoch) {
    p_.header.tsi                       = VRT_TSI_UTC;
    p_.fields.integer_seconds_timestamp = 0;
    ASSERT_EQ(vrt_time_calendar(&p_, 0.0, &cal_), 0);
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

TEST_F(TimeCalendarTest, TsiUtcNow) {
    p_.header.tsi                       = VRT_TSI_UTC;
    p_.fields.integer_seconds_timestamp = 1608751092;
    ASSERT_EQ(vrt_time_calendar(&p_, 0.0, &cal_), 0);
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

TEST_F(TimeCalendarTest, TsiGpsEpoch) {
    p_.header.tsi                       = VRT_TSI_GPS;
    p_.fields.integer_seconds_timestamp = 0;
    ASSERT_EQ(vrt_time_calendar(&p_, 0.0, &cal_), 0);
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

TEST_F(TimeCalendarTest, TsiGpsNow) {
    p_.header.tsi                       = VRT_TSI_GPS;
    p_.fields.integer_seconds_timestamp = 1292793456;
    ASSERT_EQ(vrt_time_calendar(&p_, 0.0, &cal_), 0);
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

TEST_F(TimeCalendarTest, TsiOther) {
    p_.header.tsi = VRT_TSI_OTHER;
    ASSERT_EQ(vrt_time_calendar(&p_, 0.0, &cal_), VRT_ERR_INVALID_TSI);
}

TEST_F(TimeCalendarTest, TsfSampleCount) {
    p_.header.tsf = VRT_TSF_SAMPLE_COUNT;
    ASSERT_EQ(vrt_time_calendar(&p_, 0.0, &cal_), VRT_ERR_INVALID_TSI);
}

TEST_F(TimeCalendarTest, TsfRealTime) {
    p_.header.tsf = VRT_TSF_REAL_TIME;
    ASSERT_EQ(vrt_time_calendar(&p_, 0.0, &cal_), VRT_ERR_INVALID_TSI);
}

TEST_F(TimeCalendarTest, TsfFreeRunningCount) {
    p_.header.tsf = VRT_TSF_FREE_RUNNING_COUNT;
    ASSERT_EQ(vrt_time_calendar(&p_, 0.0, &cal_), VRT_ERR_INVALID_TSI);
}

TEST_F(TimeCalendarTest, TsiUtcTsfSampleCount) {
    p_.header.tsi                          = VRT_TSI_UTC;
    p_.fields.integer_seconds_timestamp    = 1608751092;
    p_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p_.fields.fractional_seconds_timestamp = 8e6;
    ASSERT_EQ(vrt_time_calendar(&p_, 16e6, &cal_), 0);
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

TEST_F(TimeCalendarTest, TsiUtcTsfSampleCountInvalidSampleRate) {
    p_.header.tsi                          = VRT_TSI_UTC;
    p_.fields.integer_seconds_timestamp    = 1608751092;
    p_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p_.fields.fractional_seconds_timestamp = 8e6;
    ASSERT_EQ(vrt_time_calendar(&p_, 0.0, &cal_), VRT_ERR_MISSING_SAMPLE_RATE);
}

TEST_F(TimeCalendarTest, TsiUtcTsfRealTime) {
    p_.header.tsi                          = VRT_TSI_UTC;
    p_.fields.integer_seconds_timestamp    = 1608751092;
    p_.header.tsf                          = VRT_TSF_REAL_TIME;
    p_.fields.fractional_seconds_timestamp = PS_IN_S / 2;
    ASSERT_EQ(vrt_time_calendar(&p_, 0.0, &cal_), 0);
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

TEST_F(TimeCalendarTest, TsiUtcTsfRealTimeBounds) {
    p_.header.tsi                          = VRT_TSI_UTC;
    p_.fields.integer_seconds_timestamp    = 1608751092;
    p_.header.tsf                          = VRT_TSF_REAL_TIME;
    p_.fields.fractional_seconds_timestamp = PS_IN_S;
    ASSERT_EQ(vrt_time_calendar(&p_, 0.0, &cal_), VRT_ERR_BOUNDS_REAL_TIME);
}

TEST_F(TimeCalendarTest, TsiUtcTsfFreeRunningCount) {
    p_.header.tsi                       = VRT_TSI_UTC;
    p_.fields.integer_seconds_timestamp = 1608751092;
    p_.header.tsf                       = VRT_TSF_FREE_RUNNING_COUNT;
    ASSERT_EQ(vrt_time_calendar(&p_, 0.0, &cal_), VRT_ERR_INVALID_TSF);
}

TEST_F(TimeCalendarTest, TsiGpsTsfSampleCount) {
    p_.header.tsi                          = VRT_TSI_GPS;
    p_.fields.integer_seconds_timestamp    = 1292793456;
    p_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p_.fields.fractional_seconds_timestamp = 8e6;
    ASSERT_EQ(vrt_time_calendar(&p_, 16e6, &cal_), 0);
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

TEST_F(TimeCalendarTest, TsiGpsTsfSampleCountInvalidSampleRate) {
    p_.header.tsi                          = VRT_TSI_GPS;
    p_.fields.integer_seconds_timestamp    = 1292793456;
    p_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p_.fields.fractional_seconds_timestamp = 8e6;
    ASSERT_EQ(vrt_time_calendar(&p_, 0.0, &cal_), VRT_ERR_MISSING_SAMPLE_RATE);
}

TEST_F(TimeCalendarTest, TsiGpsTsfRealTime) {
    p_.header.tsi                          = VRT_TSI_GPS;
    p_.fields.integer_seconds_timestamp    = 1292793456;
    p_.header.tsf                          = VRT_TSF_REAL_TIME;
    p_.fields.fractional_seconds_timestamp = PS_IN_S / 2;
    ASSERT_EQ(vrt_time_calendar(&p_, 0.0, &cal_), 0);
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

TEST_F(TimeCalendarTest, TsiGpsTsfRealTimeBounds) {
    p_.header.tsi                          = VRT_TSI_GPS;
    p_.fields.integer_seconds_timestamp    = 1292793456;
    p_.header.tsf                          = VRT_TSF_REAL_TIME;
    p_.fields.fractional_seconds_timestamp = PS_IN_S;
    ASSERT_EQ(vrt_time_calendar(&p_, 0.0, &cal_), VRT_ERR_BOUNDS_REAL_TIME);
}

TEST_F(TimeCalendarTest, TsiGpsTsfFreeRunningCount) {
    p_.header.tsi                       = VRT_TSI_GPS;
    p_.fields.integer_seconds_timestamp = 1292793456;
    p_.header.tsf                       = VRT_TSF_FREE_RUNNING_COUNT;
    ASSERT_EQ(vrt_time_calendar(&p_, 0.0, &cal_), VRT_ERR_INVALID_TSF);
}

TEST_F(TimeCalendarTest, TsiOtherTsfSampleCount) {
    p_.header.tsi                       = VRT_TSI_OTHER;
    p_.fields.integer_seconds_timestamp = 1000;
    p_.header.tsf                       = VRT_TSF_SAMPLE_COUNT;
    ASSERT_EQ(vrt_time_calendar(&p_, 0.0, &cal_), VRT_ERR_INVALID_TSI);
}

TEST_F(TimeCalendarTest, TsiOtherTsfRealTime) {
    p_.header.tsi                       = VRT_TSI_OTHER;
    p_.fields.integer_seconds_timestamp = 1000;
    p_.header.tsf                       = VRT_TSF_REAL_TIME;
    ASSERT_EQ(vrt_time_calendar(&p_, 16e6, &cal_), VRT_ERR_INVALID_TSI);
}

TEST_F(TimeCalendarTest, TsiOtherTsfRealTimeInvalidSampleRate) {
    p_.header.tsi                       = VRT_TSI_OTHER;
    p_.fields.integer_seconds_timestamp = 1000;
    p_.header.tsf                       = VRT_TSF_REAL_TIME;
    ASSERT_EQ(vrt_time_calendar(&p_, 0.0, &cal_), VRT_ERR_INVALID_TSI);
}

TEST_F(TimeCalendarTest, TsiOtherTsfFreeRunningCount) {
    p_.header.tsi                       = VRT_TSI_OTHER;
    p_.fields.integer_seconds_timestamp = 1000;
    p_.header.tsf                       = VRT_TSF_FREE_RUNNING_COUNT;
    ASSERT_EQ(vrt_time_calendar(&p_, 0.0, &cal_), VRT_ERR_INVALID_TSI);
}
