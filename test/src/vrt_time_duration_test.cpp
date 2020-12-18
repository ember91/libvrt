#include <gtest/gtest.h>

#include <cstdint>

#include <vrt/vrt_error_code.h>
#include <vrt/vrt_init.h>
#include <vrt/vrt_time.h>
#include <vrt/vrt_types.h>

static const uint64_t PS_IN_S{1000000000000};

class TimeDurationTest : public ::testing::Test {
   protected:
    void SetUp() override {
        vrt_init_packet(&p1_);
        vrt_init_packet(&p2_);
    }

    vrt_packet p1_{};
    vrt_packet p2_{};
    vrt_time   dur_{};
};

TEST_F(TimeDurationTest, None) {
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 0.0, &dur_), 0);
}

TEST_F(TimeDurationTest, MismatchTsi) {
    p1_.header.tsi = VRT_TSI_UTC;
    p2_.header.tsi = VRT_TSI_GPS;
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 0.0, &dur_), VRT_ERR_MISMATCH_TIME_TYPES);
}

TEST_F(TimeDurationTest, MismatchTsf) {
    p1_.header.tsf = VRT_TSF_SAMPLE_COUNT;
    p2_.header.tsf = VRT_TSF_FREE_RUNNING_COUNT;
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 0.0, &dur_), VRT_ERR_MISMATCH_TIME_TYPES);
}

TEST_F(TimeDurationTest, Utc) {
    p1_.header.tsi                          = VRT_TSI_UTC;
    p2_.header.tsi                          = VRT_TSI_UTC;
    p1_.fields.integer_seconds_timestamp    = 5;
    p2_.fields.integer_seconds_timestamp    = 2;
    p1_.fields.fractional_seconds_timestamp = 5000;
    p2_.fields.fractional_seconds_timestamp = 2000;
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 0.0, &dur_), 0);
    ASSERT_EQ(dur_.s, 3);
    ASSERT_EQ(dur_.ps, 0);
}

TEST_F(TimeDurationTest, UtcNegative) {
    p1_.header.tsi                          = VRT_TSI_UTC;
    p2_.header.tsi                          = VRT_TSI_UTC;
    p1_.fields.integer_seconds_timestamp    = 2;
    p2_.fields.integer_seconds_timestamp    = 5;
    p1_.fields.fractional_seconds_timestamp = 5000;
    p2_.fields.fractional_seconds_timestamp = 2000;
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 0.0, &dur_), 0);
    ASSERT_EQ(dur_.s, -3);
    ASSERT_EQ(dur_.ps, 0);
}

TEST_F(TimeDurationTest, Gps) {
    p1_.header.tsi                          = VRT_TSI_GPS;
    p2_.header.tsi                          = VRT_TSI_GPS;
    p1_.fields.integer_seconds_timestamp    = 5;
    p2_.fields.integer_seconds_timestamp    = 2;
    p1_.fields.fractional_seconds_timestamp = 5000;
    p2_.fields.fractional_seconds_timestamp = 2000;
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 0.0, &dur_), 0);
    ASSERT_EQ(dur_.s, 3);
    ASSERT_EQ(dur_.ps, 0);
}

TEST_F(TimeDurationTest, GpsNegative) {
    p1_.header.tsi                          = VRT_TSI_GPS;
    p2_.header.tsi                          = VRT_TSI_GPS;
    p1_.fields.integer_seconds_timestamp    = 2;
    p2_.fields.integer_seconds_timestamp    = 5;
    p1_.fields.fractional_seconds_timestamp = 5000;
    p2_.fields.fractional_seconds_timestamp = 2000;
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 0.0, &dur_), 0);
    ASSERT_EQ(dur_.s, -3);
    ASSERT_EQ(dur_.ps, 0);
}

TEST_F(TimeDurationTest, Other) {
    p1_.header.tsi                          = VRT_TSI_OTHER;
    p2_.header.tsi                          = VRT_TSI_OTHER;
    p1_.fields.integer_seconds_timestamp    = 5;
    p2_.fields.integer_seconds_timestamp    = 2;
    p1_.fields.fractional_seconds_timestamp = 5000;
    p2_.fields.fractional_seconds_timestamp = 2000;
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 0.0, &dur_), 0);
    ASSERT_EQ(dur_.s, 3);
    ASSERT_EQ(dur_.ps, 0);
}

TEST_F(TimeDurationTest, OtherNegative) {
    p1_.header.tsi                          = VRT_TSI_OTHER;
    p2_.header.tsi                          = VRT_TSI_OTHER;
    p1_.fields.integer_seconds_timestamp    = 2;
    p2_.fields.integer_seconds_timestamp    = 5;
    p1_.fields.fractional_seconds_timestamp = 5000;
    p2_.fields.fractional_seconds_timestamp = 2000;
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 0.0, &dur_), 0);
    ASSERT_EQ(dur_.s, -3);
    ASSERT_EQ(dur_.ps, 0);
}

TEST_F(TimeDurationTest, SampleCount) {
    p1_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p2_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p1_.fields.integer_seconds_timestamp    = 2;
    p2_.fields.integer_seconds_timestamp    = 5;
    p1_.fields.fractional_seconds_timestamp = 750;
    p2_.fields.fractional_seconds_timestamp = 250;
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 0.0, &dur_), VRT_ERR_MISSING_SAMPLE_RATE);
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 1000.0, &dur_), 0);
    ASSERT_EQ(dur_.s, 0);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDurationTest, SampleCountNegative) {
    p1_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p2_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p1_.fields.integer_seconds_timestamp    = 2;
    p2_.fields.integer_seconds_timestamp    = 5;
    p1_.fields.fractional_seconds_timestamp = 250;
    p2_.fields.fractional_seconds_timestamp = 750;
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 0.0, &dur_), VRT_ERR_MISSING_SAMPLE_RATE);
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 1000.0, &dur_), 0);
    ASSERT_EQ(dur_.s, -1);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDurationTest, SampleCountOutsideBounds1) {
    p1_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p2_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p1_.fields.fractional_seconds_timestamp = 1000;
    p2_.fields.fractional_seconds_timestamp = 750;
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 1000.0, &dur_), VRT_ERR_BOUNDS_SAMPLE_COUNT);
}

TEST_F(TimeDurationTest, SampleCountOutsideBounds2) {
    p1_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p2_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p1_.fields.fractional_seconds_timestamp = 750;
    p2_.fields.fractional_seconds_timestamp = 1000;
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 1000.0, &dur_), VRT_ERR_BOUNDS_SAMPLE_COUNT);
}

TEST_F(TimeDurationTest, RealTime) {
    p1_.header.tsf                          = VRT_TSF_REAL_TIME;
    p2_.header.tsf                          = VRT_TSF_REAL_TIME;
    p1_.fields.integer_seconds_timestamp    = 8;
    p2_.fields.integer_seconds_timestamp    = 12;
    p1_.fields.fractional_seconds_timestamp = PS_IN_S * 3 / 4;
    p2_.fields.fractional_seconds_timestamp = PS_IN_S / 4;
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 0.0, &dur_), 0);
    ASSERT_EQ(dur_.s, 0);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDurationTest, RealTimeNegative) {
    p1_.header.tsf                          = VRT_TSF_REAL_TIME;
    p2_.header.tsf                          = VRT_TSF_REAL_TIME;
    p1_.fields.integer_seconds_timestamp    = 8;
    p2_.fields.integer_seconds_timestamp    = 12;
    p1_.fields.fractional_seconds_timestamp = PS_IN_S / 4;
    p2_.fields.fractional_seconds_timestamp = PS_IN_S * 3 / 4;
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 0.0, &dur_), 0);
    ASSERT_EQ(dur_.s, -1);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDurationTest, RealTimeBounds1) {
    p1_.header.tsf                          = VRT_TSF_REAL_TIME;
    p2_.header.tsf                          = VRT_TSF_REAL_TIME;
    p1_.fields.fractional_seconds_timestamp = PS_IN_S;
    p2_.fields.fractional_seconds_timestamp = PS_IN_S / 4;
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 0.0, &dur_), VRT_ERR_BOUNDS_REAL_TIME);
}

TEST_F(TimeDurationTest, RealTimeBounds2) {
    p1_.header.tsf                          = VRT_TSF_REAL_TIME;
    p2_.header.tsf                          = VRT_TSF_REAL_TIME;
    p1_.fields.fractional_seconds_timestamp = PS_IN_S / 4;
    p2_.fields.fractional_seconds_timestamp = PS_IN_S;
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 0.0, &dur_), VRT_ERR_BOUNDS_REAL_TIME);
}

TEST_F(TimeDurationTest, FreeRunningCount) {
    p1_.header.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    p2_.header.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    p1_.fields.integer_seconds_timestamp    = 2;
    p2_.fields.integer_seconds_timestamp    = 5;
    p1_.fields.fractional_seconds_timestamp = 3000;
    p2_.fields.fractional_seconds_timestamp = 500;
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 0.0, &dur_), VRT_ERR_MISSING_SAMPLE_RATE);
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 1000.0, &dur_), 0);
    ASSERT_EQ(dur_.s, 2);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDurationTest, FreeRunningCountNegative) {
    p1_.header.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    p2_.header.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    p1_.fields.integer_seconds_timestamp    = 2;
    p2_.fields.integer_seconds_timestamp    = 5;
    p1_.fields.fractional_seconds_timestamp = 500;
    p2_.fields.fractional_seconds_timestamp = 3000;
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 0.0, &dur_), VRT_ERR_MISSING_SAMPLE_RATE);
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 1000.0, &dur_), 0);
    ASSERT_EQ(dur_.s, -3);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDurationTest, UtcSampleCount) {
    p1_.header.tsi                          = VRT_TSI_UTC;
    p2_.header.tsi                          = VRT_TSI_UTC;
    p1_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p2_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p1_.fields.integer_seconds_timestamp    = 5;
    p2_.fields.integer_seconds_timestamp    = 2;
    p1_.fields.fractional_seconds_timestamp = 750;
    p2_.fields.fractional_seconds_timestamp = 250;
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 0.0, &dur_), VRT_ERR_MISSING_SAMPLE_RATE);
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 1000.0, &dur_), 0);
    ASSERT_EQ(dur_.s, 3);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDurationTest, UtcSampleCountNegative) {
    p1_.header.tsi                          = VRT_TSI_UTC;
    p2_.header.tsi                          = VRT_TSI_UTC;
    p1_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p2_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p1_.fields.integer_seconds_timestamp    = 2;
    p2_.fields.integer_seconds_timestamp    = 5;
    p1_.fields.fractional_seconds_timestamp = 250;
    p2_.fields.fractional_seconds_timestamp = 750;
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 0.0, &dur_), VRT_ERR_MISSING_SAMPLE_RATE);
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 1000.0, &dur_), 0);
    ASSERT_EQ(dur_.s, -4);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDurationTest, UtcSampleCountOutsideBounds1) {
    p1_.header.tsi                          = VRT_TSI_UTC;
    p2_.header.tsi                          = VRT_TSI_UTC;
    p1_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p2_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p1_.fields.fractional_seconds_timestamp = 1000;
    p2_.fields.fractional_seconds_timestamp = 750;
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 1000.0, &dur_), VRT_ERR_BOUNDS_SAMPLE_COUNT);
}

TEST_F(TimeDurationTest, UtcSampleCountOutsideBounds2) {
    p1_.header.tsi                          = VRT_TSI_UTC;
    p2_.header.tsi                          = VRT_TSI_UTC;
    p1_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p2_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p1_.fields.fractional_seconds_timestamp = 750;
    p2_.fields.fractional_seconds_timestamp = 1000;
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 1000.0, &dur_), VRT_ERR_BOUNDS_SAMPLE_COUNT);
}

TEST_F(TimeDurationTest, UtcRealTime) {
    p1_.header.tsi                          = VRT_TSI_UTC;
    p2_.header.tsi                          = VRT_TSI_UTC;
    p1_.header.tsf                          = VRT_TSF_REAL_TIME;
    p2_.header.tsf                          = VRT_TSF_REAL_TIME;
    p1_.fields.integer_seconds_timestamp    = 12;
    p2_.fields.integer_seconds_timestamp    = 8;
    p1_.fields.fractional_seconds_timestamp = PS_IN_S * 3 / 4;
    p2_.fields.fractional_seconds_timestamp = PS_IN_S / 4;
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 0.0, &dur_), 0);
    ASSERT_EQ(dur_.s, 4);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDurationTest, UtcRealTimeNegative) {
    p1_.header.tsi                          = VRT_TSI_UTC;
    p2_.header.tsi                          = VRT_TSI_UTC;
    p1_.header.tsf                          = VRT_TSF_REAL_TIME;
    p2_.header.tsf                          = VRT_TSF_REAL_TIME;
    p1_.fields.integer_seconds_timestamp    = 8;
    p2_.fields.integer_seconds_timestamp    = 12;
    p1_.fields.fractional_seconds_timestamp = PS_IN_S / 4;
    p2_.fields.fractional_seconds_timestamp = PS_IN_S * 3 / 4;
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 0.0, &dur_), 0);
    ASSERT_EQ(dur_.s, -5);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDurationTest, UtcRealTimeBounds1) {
    p1_.header.tsi                          = VRT_TSI_UTC;
    p2_.header.tsi                          = VRT_TSI_UTC;
    p1_.header.tsf                          = VRT_TSF_REAL_TIME;
    p2_.header.tsf                          = VRT_TSF_REAL_TIME;
    p1_.fields.fractional_seconds_timestamp = PS_IN_S;
    p2_.fields.fractional_seconds_timestamp = PS_IN_S / 4;
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 0.0, &dur_), VRT_ERR_BOUNDS_REAL_TIME);
}

TEST_F(TimeDurationTest, UtcRealTimeBounds2) {
    p1_.header.tsi                          = VRT_TSI_UTC;
    p2_.header.tsi                          = VRT_TSI_UTC;
    p1_.header.tsf                          = VRT_TSF_REAL_TIME;
    p2_.header.tsf                          = VRT_TSF_REAL_TIME;
    p1_.fields.fractional_seconds_timestamp = PS_IN_S / 4;
    p2_.fields.fractional_seconds_timestamp = PS_IN_S;
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 0.0, &dur_), VRT_ERR_BOUNDS_REAL_TIME);
}

TEST_F(TimeDurationTest, UtcFreeRunningCount) {
    p1_.header.tsi                          = VRT_TSI_UTC;
    p2_.header.tsi                          = VRT_TSI_UTC;
    p1_.header.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    p2_.header.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    p1_.fields.integer_seconds_timestamp    = 4;
    p2_.fields.integer_seconds_timestamp    = 2;
    p1_.fields.fractional_seconds_timestamp = 3000;
    p2_.fields.fractional_seconds_timestamp = 500;
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 0.0, &dur_), VRT_ERR_MISSING_SAMPLE_RATE);
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 1000.0, &dur_), 0);
    ASSERT_EQ(dur_.s, 2);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDurationTest, UtcFreeRunningCountNegative) {
    p1_.header.tsi                          = VRT_TSI_UTC;
    p2_.header.tsi                          = VRT_TSI_UTC;
    p1_.header.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    p2_.header.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    p1_.fields.integer_seconds_timestamp    = 2;
    p2_.fields.integer_seconds_timestamp    = 4;
    p1_.fields.fractional_seconds_timestamp = 500;
    p2_.fields.fractional_seconds_timestamp = 3000;
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 0.0, &dur_), VRT_ERR_MISSING_SAMPLE_RATE);
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 1000.0, &dur_), 0);
    ASSERT_EQ(dur_.s, -3);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDurationTest, UtcFreeRunningCountMismatch) {
    p1_.header.tsi                          = VRT_TSI_UTC;
    p2_.header.tsi                          = VRT_TSI_UTC;
    p1_.header.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    p2_.header.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    p1_.fields.integer_seconds_timestamp    = 2;
    p2_.fields.integer_seconds_timestamp    = 100;
    p1_.fields.fractional_seconds_timestamp = 500;
    p2_.fields.fractional_seconds_timestamp = 3000;
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 1000.0, &dur_), VRT_ERR_INTEGER_SECONDS_MISMATCH);
}

TEST_F(TimeDurationTest, GpsSampleCount) {
    p1_.header.tsi                          = VRT_TSI_GPS;
    p2_.header.tsi                          = VRT_TSI_GPS;
    p1_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p2_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p1_.fields.integer_seconds_timestamp    = 5;
    p2_.fields.integer_seconds_timestamp    = 2;
    p1_.fields.fractional_seconds_timestamp = 750;
    p2_.fields.fractional_seconds_timestamp = 250;
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 0.0, &dur_), VRT_ERR_MISSING_SAMPLE_RATE);
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 1000.0, &dur_), 0);
    ASSERT_EQ(dur_.s, 3);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDurationTest, GpsSampleCountNegative) {
    p1_.header.tsi                          = VRT_TSI_GPS;
    p2_.header.tsi                          = VRT_TSI_GPS;
    p1_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p2_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p1_.fields.integer_seconds_timestamp    = 2;
    p2_.fields.integer_seconds_timestamp    = 5;
    p1_.fields.fractional_seconds_timestamp = 250;
    p2_.fields.fractional_seconds_timestamp = 750;
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 0.0, &dur_), VRT_ERR_MISSING_SAMPLE_RATE);
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 1000.0, &dur_), 0);
    ASSERT_EQ(dur_.s, -4);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDurationTest, GpsSampleCountOutsideBounds1) {
    p1_.header.tsi                          = VRT_TSI_GPS;
    p2_.header.tsi                          = VRT_TSI_GPS;
    p1_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p2_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p1_.fields.fractional_seconds_timestamp = 1000;
    p2_.fields.fractional_seconds_timestamp = 750;
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 1000.0, &dur_), VRT_ERR_BOUNDS_SAMPLE_COUNT);
}

TEST_F(TimeDurationTest, GpsSampleCountOutsideBounds2) {
    p1_.header.tsi                          = VRT_TSI_GPS;
    p2_.header.tsi                          = VRT_TSI_GPS;
    p1_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p2_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p1_.fields.fractional_seconds_timestamp = 750;
    p2_.fields.fractional_seconds_timestamp = 1000;
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 1000.0, &dur_), VRT_ERR_BOUNDS_SAMPLE_COUNT);
}

TEST_F(TimeDurationTest, GpsRealTime) {
    p1_.header.tsi                          = VRT_TSI_GPS;
    p2_.header.tsi                          = VRT_TSI_GPS;
    p1_.header.tsf                          = VRT_TSF_REAL_TIME;
    p2_.header.tsf                          = VRT_TSF_REAL_TIME;
    p1_.fields.integer_seconds_timestamp    = 12;
    p2_.fields.integer_seconds_timestamp    = 8;
    p1_.fields.fractional_seconds_timestamp = PS_IN_S * 3 / 4;
    p2_.fields.fractional_seconds_timestamp = PS_IN_S / 4;
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 0.0, &dur_), 0);
    ASSERT_EQ(dur_.s, 4);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDurationTest, GpsRealTimeNegative) {
    p1_.header.tsi                          = VRT_TSI_GPS;
    p2_.header.tsi                          = VRT_TSI_GPS;
    p1_.header.tsf                          = VRT_TSF_REAL_TIME;
    p2_.header.tsf                          = VRT_TSF_REAL_TIME;
    p1_.fields.integer_seconds_timestamp    = 8;
    p2_.fields.integer_seconds_timestamp    = 12;
    p1_.fields.fractional_seconds_timestamp = PS_IN_S / 4;
    p2_.fields.fractional_seconds_timestamp = PS_IN_S * 3 / 4;
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 0.0, &dur_), 0);
    ASSERT_EQ(dur_.s, -5);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDurationTest, GpsRealTimeBounds1) {
    p1_.header.tsi                          = VRT_TSI_GPS;
    p2_.header.tsi                          = VRT_TSI_GPS;
    p1_.header.tsf                          = VRT_TSF_REAL_TIME;
    p2_.header.tsf                          = VRT_TSF_REAL_TIME;
    p1_.fields.fractional_seconds_timestamp = PS_IN_S;
    p2_.fields.fractional_seconds_timestamp = PS_IN_S / 4;
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 0.0, &dur_), VRT_ERR_BOUNDS_REAL_TIME);
}

TEST_F(TimeDurationTest, GpsRealTimeBounds2) {
    p1_.header.tsi                          = VRT_TSI_GPS;
    p2_.header.tsi                          = VRT_TSI_GPS;
    p1_.header.tsf                          = VRT_TSF_REAL_TIME;
    p2_.header.tsf                          = VRT_TSF_REAL_TIME;
    p1_.fields.fractional_seconds_timestamp = PS_IN_S / 4;
    p2_.fields.fractional_seconds_timestamp = PS_IN_S;
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 0.0, &dur_), VRT_ERR_BOUNDS_REAL_TIME);
}

TEST_F(TimeDurationTest, GpsFreeRunningCount) {
    p1_.header.tsi                          = VRT_TSI_GPS;
    p2_.header.tsi                          = VRT_TSI_GPS;
    p1_.header.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    p2_.header.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    p1_.fields.integer_seconds_timestamp    = 4;
    p2_.fields.integer_seconds_timestamp    = 2;
    p1_.fields.fractional_seconds_timestamp = 3000;
    p2_.fields.fractional_seconds_timestamp = 500;
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 0.0, &dur_), VRT_ERR_MISSING_SAMPLE_RATE);
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 1000.0, &dur_), 0);
    ASSERT_EQ(dur_.s, 2);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDurationTest, GpsFreeRunningCountNegative) {
    p1_.header.tsi                          = VRT_TSI_GPS;
    p2_.header.tsi                          = VRT_TSI_GPS;
    p1_.header.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    p2_.header.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    p1_.fields.integer_seconds_timestamp    = 2;
    p2_.fields.integer_seconds_timestamp    = 4;
    p1_.fields.fractional_seconds_timestamp = 500;
    p2_.fields.fractional_seconds_timestamp = 3000;
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 0.0, &dur_), VRT_ERR_MISSING_SAMPLE_RATE);
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 1000.0, &dur_), 0);
    ASSERT_EQ(dur_.s, -3);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDurationTest, GpsFreeRunningCountMismatch) {
    p1_.header.tsi                          = VRT_TSI_GPS;
    p2_.header.tsi                          = VRT_TSI_GPS;
    p1_.header.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    p2_.header.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    p1_.fields.integer_seconds_timestamp    = 2;
    p2_.fields.integer_seconds_timestamp    = 100;
    p1_.fields.fractional_seconds_timestamp = 500;
    p2_.fields.fractional_seconds_timestamp = 3000;
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 1000.0, &dur_), VRT_ERR_INTEGER_SECONDS_MISMATCH);
}

TEST_F(TimeDurationTest, OtherSampleCount) {
    p1_.header.tsi                          = VRT_TSI_OTHER;
    p2_.header.tsi                          = VRT_TSI_OTHER;
    p1_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p2_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p1_.fields.integer_seconds_timestamp    = 5;
    p2_.fields.integer_seconds_timestamp    = 2;
    p1_.fields.fractional_seconds_timestamp = 750;
    p2_.fields.fractional_seconds_timestamp = 250;
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 0.0, &dur_), VRT_ERR_MISSING_SAMPLE_RATE);
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 1000.0, &dur_), 0);
    ASSERT_EQ(dur_.s, 3);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDurationTest, OtherSampleCountNegative) {
    p1_.header.tsi                          = VRT_TSI_OTHER;
    p2_.header.tsi                          = VRT_TSI_OTHER;
    p1_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p2_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p1_.fields.integer_seconds_timestamp    = 2;
    p2_.fields.integer_seconds_timestamp    = 5;
    p1_.fields.fractional_seconds_timestamp = 250;
    p2_.fields.fractional_seconds_timestamp = 750;
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 0.0, &dur_), VRT_ERR_MISSING_SAMPLE_RATE);
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 1000.0, &dur_), 0);
    ASSERT_EQ(dur_.s, -4);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDurationTest, OtherSampleCountOutsideBounds1) {
    p1_.header.tsi                          = VRT_TSI_OTHER;
    p2_.header.tsi                          = VRT_TSI_OTHER;
    p1_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p2_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p1_.fields.fractional_seconds_timestamp = 1000;
    p2_.fields.fractional_seconds_timestamp = 750;
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 1000.0, &dur_), VRT_ERR_BOUNDS_SAMPLE_COUNT);
}

TEST_F(TimeDurationTest, OtherSampleCountOutsideBounds2) {
    p1_.header.tsi                          = VRT_TSI_OTHER;
    p2_.header.tsi                          = VRT_TSI_OTHER;
    p1_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p2_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p1_.fields.fractional_seconds_timestamp = 750;
    p2_.fields.fractional_seconds_timestamp = 1000;
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 1000.0, &dur_), VRT_ERR_BOUNDS_SAMPLE_COUNT);
}

TEST_F(TimeDurationTest, OtherRealTime) {
    p1_.header.tsi                          = VRT_TSI_OTHER;
    p2_.header.tsi                          = VRT_TSI_OTHER;
    p1_.header.tsf                          = VRT_TSF_REAL_TIME;
    p2_.header.tsf                          = VRT_TSF_REAL_TIME;
    p1_.fields.integer_seconds_timestamp    = 12;
    p2_.fields.integer_seconds_timestamp    = 8;
    p1_.fields.fractional_seconds_timestamp = PS_IN_S * 3 / 4;
    p2_.fields.fractional_seconds_timestamp = PS_IN_S / 4;
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 0.0, &dur_), 0);
    ASSERT_EQ(dur_.s, 4);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDurationTest, OtherRealTimeNegative) {
    p1_.header.tsi                          = VRT_TSI_OTHER;
    p2_.header.tsi                          = VRT_TSI_OTHER;
    p1_.header.tsf                          = VRT_TSF_REAL_TIME;
    p2_.header.tsf                          = VRT_TSF_REAL_TIME;
    p1_.fields.integer_seconds_timestamp    = 8;
    p2_.fields.integer_seconds_timestamp    = 12;
    p1_.fields.fractional_seconds_timestamp = PS_IN_S / 4;
    p2_.fields.fractional_seconds_timestamp = PS_IN_S * 3 / 4;
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 0.0, &dur_), 0);
    ASSERT_EQ(dur_.s, -5);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDurationTest, OtherRealTimeBounds1) {
    p1_.header.tsi                          = VRT_TSI_OTHER;
    p2_.header.tsi                          = VRT_TSI_OTHER;
    p1_.header.tsf                          = VRT_TSF_REAL_TIME;
    p2_.header.tsf                          = VRT_TSF_REAL_TIME;
    p1_.fields.fractional_seconds_timestamp = PS_IN_S;
    p2_.fields.fractional_seconds_timestamp = PS_IN_S / 4;
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 0.0, &dur_), VRT_ERR_BOUNDS_REAL_TIME);
}

TEST_F(TimeDurationTest, OtherRealTimeBounds2) {
    p1_.header.tsi                          = VRT_TSI_OTHER;
    p2_.header.tsi                          = VRT_TSI_OTHER;
    p1_.header.tsf                          = VRT_TSF_REAL_TIME;
    p2_.header.tsf                          = VRT_TSF_REAL_TIME;
    p1_.fields.fractional_seconds_timestamp = PS_IN_S / 4;
    p2_.fields.fractional_seconds_timestamp = PS_IN_S;
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 0.0, &dur_), VRT_ERR_BOUNDS_REAL_TIME);
}

TEST_F(TimeDurationTest, OtherFreeRunningCount) {
    p1_.header.tsi                          = VRT_TSI_OTHER;
    p2_.header.tsi                          = VRT_TSI_OTHER;
    p1_.header.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    p2_.header.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    p1_.fields.integer_seconds_timestamp    = 4;
    p2_.fields.integer_seconds_timestamp    = 2;
    p1_.fields.fractional_seconds_timestamp = 3000;
    p2_.fields.fractional_seconds_timestamp = 500;
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 0.0, &dur_), VRT_ERR_MISSING_SAMPLE_RATE);
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 1000.0, &dur_), 0);
    ASSERT_EQ(dur_.s, 2);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDurationTest, OtherFreeRunningCountNegative) {
    p1_.header.tsi                          = VRT_TSI_OTHER;
    p2_.header.tsi                          = VRT_TSI_OTHER;
    p1_.header.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    p2_.header.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    p1_.fields.integer_seconds_timestamp    = 2;
    p2_.fields.integer_seconds_timestamp    = 4;
    p1_.fields.fractional_seconds_timestamp = 500;
    p2_.fields.fractional_seconds_timestamp = 3000;
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 0.0, &dur_), VRT_ERR_MISSING_SAMPLE_RATE);
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 1000.0, &dur_), 0);
    ASSERT_EQ(dur_.s, -3);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDurationTest, OtherFreeRunningCountMismatch) {
    p1_.header.tsi                          = VRT_TSI_OTHER;
    p2_.header.tsi                          = VRT_TSI_OTHER;
    p1_.header.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    p2_.header.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    p1_.fields.integer_seconds_timestamp    = 2;
    p2_.fields.integer_seconds_timestamp    = 100;
    p1_.fields.fractional_seconds_timestamp = 500;
    p2_.fields.fractional_seconds_timestamp = 3000;
    ASSERT_EQ(vrt_time_duration(&p1_, &p2_, 1000.0, &dur_), VRT_ERR_INTEGER_SECONDS_MISMATCH);
}
