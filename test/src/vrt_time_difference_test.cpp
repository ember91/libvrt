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
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 0.0, &dur_), 0);
}

TEST_F(TimeDurationTest, MismatchTsi) {
    p2_.header.tsi = VRT_TSI_UTC;
    p1_.header.tsi = VRT_TSI_GPS;
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 0.0, &dur_), VRT_ERR_MISMATCH_TIME_TYPES);
}

TEST_F(TimeDurationTest, MismatchTsf) {
    p2_.header.tsf = VRT_TSF_SAMPLE_COUNT;
    p1_.header.tsf = VRT_TSF_FREE_RUNNING_COUNT;
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 0.0, &dur_), VRT_ERR_MISMATCH_TIME_TYPES);
}

TEST_F(TimeDurationTest, UtcPositive) {
    p2_.header.tsi                          = VRT_TSI_UTC;
    p1_.header.tsi                          = VRT_TSI_UTC;
    p2_.fields.integer_seconds_timestamp    = 5;
    p1_.fields.integer_seconds_timestamp    = 2;
    p2_.fields.fractional_seconds_timestamp = 5000;
    p1_.fields.fractional_seconds_timestamp = 2000;
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 0.0, &dur_), 0);
    ASSERT_EQ(dur_.s, 3);
    ASSERT_EQ(dur_.ps, 0);
}

TEST_F(TimeDurationTest, UtcNegative) {
    p2_.header.tsi                          = VRT_TSI_UTC;
    p1_.header.tsi                          = VRT_TSI_UTC;
    p2_.fields.integer_seconds_timestamp    = 2;
    p1_.fields.integer_seconds_timestamp    = 5;
    p2_.fields.fractional_seconds_timestamp = 5000;
    p1_.fields.fractional_seconds_timestamp = 2000;
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 0.0, &dur_), 0);
    ASSERT_EQ(dur_.s, -3);
    ASSERT_EQ(dur_.ps, 0);
}

TEST_F(TimeDurationTest, GpsPositive) {
    p2_.header.tsi                          = VRT_TSI_GPS;
    p1_.header.tsi                          = VRT_TSI_GPS;
    p2_.fields.integer_seconds_timestamp    = 5;
    p1_.fields.integer_seconds_timestamp    = 2;
    p2_.fields.fractional_seconds_timestamp = 5000;
    p1_.fields.fractional_seconds_timestamp = 2000;
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 0.0, &dur_), 0);
    ASSERT_EQ(dur_.s, 3);
    ASSERT_EQ(dur_.ps, 0);
}

TEST_F(TimeDurationTest, GpsNegative) {
    p2_.header.tsi                          = VRT_TSI_GPS;
    p1_.header.tsi                          = VRT_TSI_GPS;
    p2_.fields.integer_seconds_timestamp    = 2;
    p1_.fields.integer_seconds_timestamp    = 5;
    p2_.fields.fractional_seconds_timestamp = 5000;
    p1_.fields.fractional_seconds_timestamp = 2000;
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 0.0, &dur_), 0);
    ASSERT_EQ(dur_.s, -3);
    ASSERT_EQ(dur_.ps, 0);
}

TEST_F(TimeDurationTest, OtherPositive) {
    p2_.header.tsi                          = VRT_TSI_OTHER;
    p1_.header.tsi                          = VRT_TSI_OTHER;
    p2_.fields.integer_seconds_timestamp    = 5;
    p1_.fields.integer_seconds_timestamp    = 2;
    p2_.fields.fractional_seconds_timestamp = 5000;
    p1_.fields.fractional_seconds_timestamp = 2000;
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 0.0, &dur_), 0);
    ASSERT_EQ(dur_.s, 3);
    ASSERT_EQ(dur_.ps, 0);
}

TEST_F(TimeDurationTest, OtherNegative) {
    p2_.header.tsi                          = VRT_TSI_OTHER;
    p1_.header.tsi                          = VRT_TSI_OTHER;
    p2_.fields.integer_seconds_timestamp    = 2;
    p1_.fields.integer_seconds_timestamp    = 5;
    p2_.fields.fractional_seconds_timestamp = 5000;
    p1_.fields.fractional_seconds_timestamp = 2000;
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 0.0, &dur_), 0);
    ASSERT_EQ(dur_.s, -3);
    ASSERT_EQ(dur_.ps, 0);
}

TEST_F(TimeDurationTest, SampleCountPositive) {
    p2_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p1_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p2_.fields.integer_seconds_timestamp    = 2;
    p1_.fields.integer_seconds_timestamp    = 5;
    p2_.fields.fractional_seconds_timestamp = 750;
    p1_.fields.fractional_seconds_timestamp = 250;
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 0.0, &dur_), VRT_ERR_MISSING_SAMPLE_RATE);
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 1000.0, &dur_), 0);
    ASSERT_EQ(dur_.s, 0);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDurationTest, SampleCountNegative) {
    p2_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p1_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p2_.fields.integer_seconds_timestamp    = 2;
    p1_.fields.integer_seconds_timestamp    = 5;
    p2_.fields.fractional_seconds_timestamp = 250;
    p1_.fields.fractional_seconds_timestamp = 750;
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 0.0, &dur_), VRT_ERR_MISSING_SAMPLE_RATE);
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 1000.0, &dur_), 0);
    ASSERT_EQ(dur_.s, -1);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDurationTest, SampleCountOutsideBounds1) {
    p2_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p1_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p2_.fields.fractional_seconds_timestamp = 1000;
    p1_.fields.fractional_seconds_timestamp = 750;
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 1000.0, &dur_), VRT_ERR_BOUNDS_SAMPLE_COUNT);
}

TEST_F(TimeDurationTest, SampleCountOutsideBounds2) {
    p2_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p1_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p2_.fields.fractional_seconds_timestamp = 750;
    p1_.fields.fractional_seconds_timestamp = 1000;
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 1000.0, &dur_), VRT_ERR_BOUNDS_SAMPLE_COUNT);
}

TEST_F(TimeDurationTest, RealTimePositive) {
    p2_.header.tsf                          = VRT_TSF_REAL_TIME;
    p1_.header.tsf                          = VRT_TSF_REAL_TIME;
    p2_.fields.integer_seconds_timestamp    = 8;
    p1_.fields.integer_seconds_timestamp    = 12;
    p2_.fields.fractional_seconds_timestamp = PS_IN_S * 3 / 4;
    p1_.fields.fractional_seconds_timestamp = PS_IN_S / 4;
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 0.0, &dur_), 0);
    ASSERT_EQ(dur_.s, 0);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDurationTest, RealTimeNegative) {
    p2_.header.tsf                          = VRT_TSF_REAL_TIME;
    p1_.header.tsf                          = VRT_TSF_REAL_TIME;
    p2_.fields.integer_seconds_timestamp    = 8;
    p1_.fields.integer_seconds_timestamp    = 12;
    p2_.fields.fractional_seconds_timestamp = PS_IN_S / 4;
    p1_.fields.fractional_seconds_timestamp = PS_IN_S * 3 / 4;
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 0.0, &dur_), 0);
    ASSERT_EQ(dur_.s, -1);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDurationTest, RealTimeBounds1) {
    p2_.header.tsf                          = VRT_TSF_REAL_TIME;
    p1_.header.tsf                          = VRT_TSF_REAL_TIME;
    p2_.fields.fractional_seconds_timestamp = PS_IN_S;
    p1_.fields.fractional_seconds_timestamp = PS_IN_S / 4;
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 0.0, &dur_), VRT_ERR_BOUNDS_REAL_TIME);
}

TEST_F(TimeDurationTest, RealTimeBounds2) {
    p2_.header.tsf                          = VRT_TSF_REAL_TIME;
    p1_.header.tsf                          = VRT_TSF_REAL_TIME;
    p2_.fields.fractional_seconds_timestamp = PS_IN_S / 4;
    p1_.fields.fractional_seconds_timestamp = PS_IN_S;
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 0.0, &dur_), VRT_ERR_BOUNDS_REAL_TIME);
}

TEST_F(TimeDurationTest, FreeRunningCountPositive) {
    p2_.header.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    p1_.header.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    p2_.fields.integer_seconds_timestamp    = 2;
    p1_.fields.integer_seconds_timestamp    = 5;
    p2_.fields.fractional_seconds_timestamp = 3000;
    p1_.fields.fractional_seconds_timestamp = 500;
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 0.0, &dur_), VRT_ERR_MISSING_SAMPLE_RATE);
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 1000.0, &dur_), 0);
    ASSERT_EQ(dur_.s, 2);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDurationTest, FreeRunningCountNegative) {
    p2_.header.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    p1_.header.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    p2_.fields.integer_seconds_timestamp    = 2;
    p1_.fields.integer_seconds_timestamp    = 5;
    p2_.fields.fractional_seconds_timestamp = 500;
    p1_.fields.fractional_seconds_timestamp = 3000;
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 0.0, &dur_), VRT_ERR_MISSING_SAMPLE_RATE);
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 1000.0, &dur_), 0);
    ASSERT_EQ(dur_.s, -3);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDurationTest, UtcSampleCountPositive) {
    p2_.header.tsi                          = VRT_TSI_UTC;
    p1_.header.tsi                          = VRT_TSI_UTC;
    p2_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p1_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p2_.fields.integer_seconds_timestamp    = 5;
    p1_.fields.integer_seconds_timestamp    = 2;
    p2_.fields.fractional_seconds_timestamp = 750;
    p1_.fields.fractional_seconds_timestamp = 250;
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 0.0, &dur_), VRT_ERR_MISSING_SAMPLE_RATE);
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 1000.0, &dur_), 0);
    ASSERT_EQ(dur_.s, 3);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDurationTest, UtcSampleCountNegative) {
    p2_.header.tsi                          = VRT_TSI_UTC;
    p1_.header.tsi                          = VRT_TSI_UTC;
    p2_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p1_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p2_.fields.integer_seconds_timestamp    = 2;
    p1_.fields.integer_seconds_timestamp    = 5;
    p2_.fields.fractional_seconds_timestamp = 250;
    p1_.fields.fractional_seconds_timestamp = 750;
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 0.0, &dur_), VRT_ERR_MISSING_SAMPLE_RATE);
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 1000.0, &dur_), 0);
    ASSERT_EQ(dur_.s, -4);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDurationTest, UtcSampleCountOutsideBounds1) {
    p2_.header.tsi                          = VRT_TSI_UTC;
    p1_.header.tsi                          = VRT_TSI_UTC;
    p2_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p1_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p2_.fields.fractional_seconds_timestamp = 1000;
    p1_.fields.fractional_seconds_timestamp = 750;
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 1000.0, &dur_), VRT_ERR_BOUNDS_SAMPLE_COUNT);
}

TEST_F(TimeDurationTest, UtcSampleCountOutsideBounds2) {
    p2_.header.tsi                          = VRT_TSI_UTC;
    p1_.header.tsi                          = VRT_TSI_UTC;
    p2_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p1_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p2_.fields.fractional_seconds_timestamp = 750;
    p1_.fields.fractional_seconds_timestamp = 1000;
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 1000.0, &dur_), VRT_ERR_BOUNDS_SAMPLE_COUNT);
}

TEST_F(TimeDurationTest, UtcRealTimePositive) {
    p2_.header.tsi                          = VRT_TSI_UTC;
    p1_.header.tsi                          = VRT_TSI_UTC;
    p2_.header.tsf                          = VRT_TSF_REAL_TIME;
    p1_.header.tsf                          = VRT_TSF_REAL_TIME;
    p2_.fields.integer_seconds_timestamp    = 12;
    p1_.fields.integer_seconds_timestamp    = 8;
    p2_.fields.fractional_seconds_timestamp = PS_IN_S * 3 / 4;
    p1_.fields.fractional_seconds_timestamp = PS_IN_S / 4;
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 0.0, &dur_), 0);
    ASSERT_EQ(dur_.s, 4);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDurationTest, UtcRealTimeNegative) {
    p2_.header.tsi                          = VRT_TSI_UTC;
    p1_.header.tsi                          = VRT_TSI_UTC;
    p2_.header.tsf                          = VRT_TSF_REAL_TIME;
    p1_.header.tsf                          = VRT_TSF_REAL_TIME;
    p2_.fields.integer_seconds_timestamp    = 8;
    p1_.fields.integer_seconds_timestamp    = 12;
    p2_.fields.fractional_seconds_timestamp = PS_IN_S / 4;
    p1_.fields.fractional_seconds_timestamp = PS_IN_S * 3 / 4;
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 0.0, &dur_), 0);
    ASSERT_EQ(dur_.s, -5);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDurationTest, UtcRealTimeBounds1) {
    p2_.header.tsi                          = VRT_TSI_UTC;
    p1_.header.tsi                          = VRT_TSI_UTC;
    p2_.header.tsf                          = VRT_TSF_REAL_TIME;
    p1_.header.tsf                          = VRT_TSF_REAL_TIME;
    p2_.fields.fractional_seconds_timestamp = PS_IN_S;
    p1_.fields.fractional_seconds_timestamp = PS_IN_S / 4;
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 0.0, &dur_), VRT_ERR_BOUNDS_REAL_TIME);
}

TEST_F(TimeDurationTest, UtcRealTimeBounds2) {
    p2_.header.tsi                          = VRT_TSI_UTC;
    p1_.header.tsi                          = VRT_TSI_UTC;
    p2_.header.tsf                          = VRT_TSF_REAL_TIME;
    p1_.header.tsf                          = VRT_TSF_REAL_TIME;
    p2_.fields.fractional_seconds_timestamp = PS_IN_S / 4;
    p1_.fields.fractional_seconds_timestamp = PS_IN_S;
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 0.0, &dur_), VRT_ERR_BOUNDS_REAL_TIME);
}

TEST_F(TimeDurationTest, UtcFreeRunningCountPositive) {
    p2_.header.tsi                          = VRT_TSI_UTC;
    p1_.header.tsi                          = VRT_TSI_UTC;
    p2_.header.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    p1_.header.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    p2_.fields.integer_seconds_timestamp    = 4;
    p1_.fields.integer_seconds_timestamp    = 2;
    p2_.fields.fractional_seconds_timestamp = 3000;
    p1_.fields.fractional_seconds_timestamp = 500;
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 0.0, &dur_), VRT_ERR_MISSING_SAMPLE_RATE);
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 1000.0, &dur_), 0);
    ASSERT_EQ(dur_.s, 2);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDurationTest, UtcFreeRunningCountNegative) {
    p2_.header.tsi                          = VRT_TSI_UTC;
    p1_.header.tsi                          = VRT_TSI_UTC;
    p2_.header.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    p1_.header.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    p2_.fields.integer_seconds_timestamp    = 2;
    p1_.fields.integer_seconds_timestamp    = 4;
    p2_.fields.fractional_seconds_timestamp = 500;
    p1_.fields.fractional_seconds_timestamp = 3000;
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 0.0, &dur_), VRT_ERR_MISSING_SAMPLE_RATE);
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 1000.0, &dur_), 0);
    ASSERT_EQ(dur_.s, -3);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDurationTest, UtcFreeRunningCountMismatch) {
    p2_.header.tsi                          = VRT_TSI_UTC;
    p1_.header.tsi                          = VRT_TSI_UTC;
    p2_.header.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    p1_.header.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    p2_.fields.integer_seconds_timestamp    = 2;
    p1_.fields.integer_seconds_timestamp    = 100;
    p2_.fields.fractional_seconds_timestamp = 500;
    p1_.fields.fractional_seconds_timestamp = 3000;
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 1000.0, &dur_), VRT_ERR_INTEGER_SECONDS_MISMATCH);
}

TEST_F(TimeDurationTest, GpsSampleCountPositive) {
    p2_.header.tsi                          = VRT_TSI_GPS;
    p1_.header.tsi                          = VRT_TSI_GPS;
    p2_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p1_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p2_.fields.integer_seconds_timestamp    = 5;
    p1_.fields.integer_seconds_timestamp    = 2;
    p2_.fields.fractional_seconds_timestamp = 750;
    p1_.fields.fractional_seconds_timestamp = 250;
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 0.0, &dur_), VRT_ERR_MISSING_SAMPLE_RATE);
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 1000.0, &dur_), 0);
    ASSERT_EQ(dur_.s, 3);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDurationTest, GpsSampleCountNegative) {
    p2_.header.tsi                          = VRT_TSI_GPS;
    p1_.header.tsi                          = VRT_TSI_GPS;
    p2_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p1_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p2_.fields.integer_seconds_timestamp    = 2;
    p1_.fields.integer_seconds_timestamp    = 5;
    p2_.fields.fractional_seconds_timestamp = 250;
    p1_.fields.fractional_seconds_timestamp = 750;
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 0.0, &dur_), VRT_ERR_MISSING_SAMPLE_RATE);
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 1000.0, &dur_), 0);
    ASSERT_EQ(dur_.s, -4);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDurationTest, GpsSampleCountOutsideBounds1) {
    p2_.header.tsi                          = VRT_TSI_GPS;
    p1_.header.tsi                          = VRT_TSI_GPS;
    p2_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p1_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p2_.fields.fractional_seconds_timestamp = 1000;
    p1_.fields.fractional_seconds_timestamp = 750;
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 1000.0, &dur_), VRT_ERR_BOUNDS_SAMPLE_COUNT);
}

TEST_F(TimeDurationTest, GpsSampleCountOutsideBounds2) {
    p2_.header.tsi                          = VRT_TSI_GPS;
    p1_.header.tsi                          = VRT_TSI_GPS;
    p2_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p1_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p2_.fields.fractional_seconds_timestamp = 750;
    p1_.fields.fractional_seconds_timestamp = 1000;
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 1000.0, &dur_), VRT_ERR_BOUNDS_SAMPLE_COUNT);
}

TEST_F(TimeDurationTest, GpsRealTimePositive) {
    p2_.header.tsi                          = VRT_TSI_GPS;
    p1_.header.tsi                          = VRT_TSI_GPS;
    p2_.header.tsf                          = VRT_TSF_REAL_TIME;
    p1_.header.tsf                          = VRT_TSF_REAL_TIME;
    p2_.fields.integer_seconds_timestamp    = 12;
    p1_.fields.integer_seconds_timestamp    = 8;
    p2_.fields.fractional_seconds_timestamp = PS_IN_S * 3 / 4;
    p1_.fields.fractional_seconds_timestamp = PS_IN_S / 4;
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 0.0, &dur_), 0);
    ASSERT_EQ(dur_.s, 4);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDurationTest, GpsRealTimeNegative) {
    p2_.header.tsi                          = VRT_TSI_GPS;
    p1_.header.tsi                          = VRT_TSI_GPS;
    p2_.header.tsf                          = VRT_TSF_REAL_TIME;
    p1_.header.tsf                          = VRT_TSF_REAL_TIME;
    p2_.fields.integer_seconds_timestamp    = 8;
    p1_.fields.integer_seconds_timestamp    = 12;
    p2_.fields.fractional_seconds_timestamp = PS_IN_S / 4;
    p1_.fields.fractional_seconds_timestamp = PS_IN_S * 3 / 4;
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 0.0, &dur_), 0);
    ASSERT_EQ(dur_.s, -5);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDurationTest, GpsRealTimeBounds1) {
    p2_.header.tsi                          = VRT_TSI_GPS;
    p1_.header.tsi                          = VRT_TSI_GPS;
    p2_.header.tsf                          = VRT_TSF_REAL_TIME;
    p1_.header.tsf                          = VRT_TSF_REAL_TIME;
    p2_.fields.fractional_seconds_timestamp = PS_IN_S;
    p1_.fields.fractional_seconds_timestamp = PS_IN_S / 4;
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 0.0, &dur_), VRT_ERR_BOUNDS_REAL_TIME);
}

TEST_F(TimeDurationTest, GpsRealTimeBounds2) {
    p2_.header.tsi                          = VRT_TSI_GPS;
    p1_.header.tsi                          = VRT_TSI_GPS;
    p2_.header.tsf                          = VRT_TSF_REAL_TIME;
    p1_.header.tsf                          = VRT_TSF_REAL_TIME;
    p2_.fields.fractional_seconds_timestamp = PS_IN_S / 4;
    p1_.fields.fractional_seconds_timestamp = PS_IN_S;
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 0.0, &dur_), VRT_ERR_BOUNDS_REAL_TIME);
}

TEST_F(TimeDurationTest, GpsFreeRunningCountPositive) {
    p2_.header.tsi                          = VRT_TSI_GPS;
    p1_.header.tsi                          = VRT_TSI_GPS;
    p2_.header.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    p1_.header.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    p2_.fields.integer_seconds_timestamp    = 4;
    p1_.fields.integer_seconds_timestamp    = 2;
    p2_.fields.fractional_seconds_timestamp = 3000;
    p1_.fields.fractional_seconds_timestamp = 500;
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 0.0, &dur_), VRT_ERR_MISSING_SAMPLE_RATE);
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 1000.0, &dur_), 0);
    ASSERT_EQ(dur_.s, 2);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDurationTest, GpsFreeRunningCountNegative) {
    p2_.header.tsi                          = VRT_TSI_GPS;
    p1_.header.tsi                          = VRT_TSI_GPS;
    p2_.header.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    p1_.header.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    p2_.fields.integer_seconds_timestamp    = 2;
    p1_.fields.integer_seconds_timestamp    = 4;
    p2_.fields.fractional_seconds_timestamp = 500;
    p1_.fields.fractional_seconds_timestamp = 3000;
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 0.0, &dur_), VRT_ERR_MISSING_SAMPLE_RATE);
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 1000.0, &dur_), 0);
    ASSERT_EQ(dur_.s, -3);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDurationTest, GpsFreeRunningCountMismatch) {
    p2_.header.tsi                          = VRT_TSI_GPS;
    p1_.header.tsi                          = VRT_TSI_GPS;
    p2_.header.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    p1_.header.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    p2_.fields.integer_seconds_timestamp    = 2;
    p1_.fields.integer_seconds_timestamp    = 100;
    p2_.fields.fractional_seconds_timestamp = 500;
    p1_.fields.fractional_seconds_timestamp = 3000;
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 1000.0, &dur_), VRT_ERR_INTEGER_SECONDS_MISMATCH);
}

TEST_F(TimeDurationTest, OtherSampleCountPositive) {
    p2_.header.tsi                          = VRT_TSI_OTHER;
    p1_.header.tsi                          = VRT_TSI_OTHER;
    p2_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p1_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p2_.fields.integer_seconds_timestamp    = 5;
    p1_.fields.integer_seconds_timestamp    = 2;
    p2_.fields.fractional_seconds_timestamp = 750;
    p1_.fields.fractional_seconds_timestamp = 250;
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 0.0, &dur_), VRT_ERR_MISSING_SAMPLE_RATE);
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 1000.0, &dur_), 0);
    ASSERT_EQ(dur_.s, 3);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDurationTest, OtherSampleCountNegative) {
    p2_.header.tsi                          = VRT_TSI_OTHER;
    p1_.header.tsi                          = VRT_TSI_OTHER;
    p2_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p1_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p2_.fields.integer_seconds_timestamp    = 2;
    p1_.fields.integer_seconds_timestamp    = 5;
    p2_.fields.fractional_seconds_timestamp = 250;
    p1_.fields.fractional_seconds_timestamp = 750;
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 0.0, &dur_), VRT_ERR_MISSING_SAMPLE_RATE);
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 1000.0, &dur_), 0);
    ASSERT_EQ(dur_.s, -4);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDurationTest, OtherSampleCountOutsideBounds1) {
    p2_.header.tsi                          = VRT_TSI_OTHER;
    p1_.header.tsi                          = VRT_TSI_OTHER;
    p2_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p1_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p2_.fields.fractional_seconds_timestamp = 1000;
    p1_.fields.fractional_seconds_timestamp = 750;
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 1000.0, &dur_), VRT_ERR_BOUNDS_SAMPLE_COUNT);
}

TEST_F(TimeDurationTest, OtherSampleCountOutsideBounds2) {
    p2_.header.tsi                          = VRT_TSI_OTHER;
    p1_.header.tsi                          = VRT_TSI_OTHER;
    p2_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p1_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p2_.fields.fractional_seconds_timestamp = 750;
    p1_.fields.fractional_seconds_timestamp = 1000;
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 1000.0, &dur_), VRT_ERR_BOUNDS_SAMPLE_COUNT);
}

TEST_F(TimeDurationTest, OtherRealTimePositive) {
    p2_.header.tsi                          = VRT_TSI_OTHER;
    p1_.header.tsi                          = VRT_TSI_OTHER;
    p2_.header.tsf                          = VRT_TSF_REAL_TIME;
    p1_.header.tsf                          = VRT_TSF_REAL_TIME;
    p2_.fields.integer_seconds_timestamp    = 12;
    p1_.fields.integer_seconds_timestamp    = 8;
    p2_.fields.fractional_seconds_timestamp = PS_IN_S * 3 / 4;
    p1_.fields.fractional_seconds_timestamp = PS_IN_S / 4;
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 0.0, &dur_), 0);
    ASSERT_EQ(dur_.s, 4);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDurationTest, OtherRealTimeNegative) {
    p2_.header.tsi                          = VRT_TSI_OTHER;
    p1_.header.tsi                          = VRT_TSI_OTHER;
    p2_.header.tsf                          = VRT_TSF_REAL_TIME;
    p1_.header.tsf                          = VRT_TSF_REAL_TIME;
    p2_.fields.integer_seconds_timestamp    = 8;
    p1_.fields.integer_seconds_timestamp    = 12;
    p2_.fields.fractional_seconds_timestamp = PS_IN_S / 4;
    p1_.fields.fractional_seconds_timestamp = PS_IN_S * 3 / 4;
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 0.0, &dur_), 0);
    ASSERT_EQ(dur_.s, -5);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDurationTest, OtherRealTimeBounds1) {
    p2_.header.tsi                          = VRT_TSI_OTHER;
    p1_.header.tsi                          = VRT_TSI_OTHER;
    p2_.header.tsf                          = VRT_TSF_REAL_TIME;
    p1_.header.tsf                          = VRT_TSF_REAL_TIME;
    p2_.fields.fractional_seconds_timestamp = PS_IN_S;
    p1_.fields.fractional_seconds_timestamp = PS_IN_S / 4;
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 0.0, &dur_), VRT_ERR_BOUNDS_REAL_TIME);
}

TEST_F(TimeDurationTest, OtherRealTimeBounds2) {
    p2_.header.tsi                          = VRT_TSI_OTHER;
    p1_.header.tsi                          = VRT_TSI_OTHER;
    p2_.header.tsf                          = VRT_TSF_REAL_TIME;
    p1_.header.tsf                          = VRT_TSF_REAL_TIME;
    p2_.fields.fractional_seconds_timestamp = PS_IN_S / 4;
    p1_.fields.fractional_seconds_timestamp = PS_IN_S;
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 0.0, &dur_), VRT_ERR_BOUNDS_REAL_TIME);
}

TEST_F(TimeDurationTest, OtherFreeRunningCountPositive) {
    p2_.header.tsi                          = VRT_TSI_OTHER;
    p1_.header.tsi                          = VRT_TSI_OTHER;
    p2_.header.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    p1_.header.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    p2_.fields.integer_seconds_timestamp    = 4;
    p1_.fields.integer_seconds_timestamp    = 2;
    p2_.fields.fractional_seconds_timestamp = 3000;
    p1_.fields.fractional_seconds_timestamp = 500;
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 0.0, &dur_), VRT_ERR_MISSING_SAMPLE_RATE);
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 1000.0, &dur_), 0);
    ASSERT_EQ(dur_.s, 2);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDurationTest, OtherFreeRunningCountNegative) {
    p2_.header.tsi                          = VRT_TSI_OTHER;
    p1_.header.tsi                          = VRT_TSI_OTHER;
    p2_.header.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    p1_.header.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    p2_.fields.integer_seconds_timestamp    = 2;
    p1_.fields.integer_seconds_timestamp    = 4;
    p2_.fields.fractional_seconds_timestamp = 500;
    p1_.fields.fractional_seconds_timestamp = 3000;
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 0.0, &dur_), VRT_ERR_MISSING_SAMPLE_RATE);
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 1000.0, &dur_), 0);
    ASSERT_EQ(dur_.s, -3);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDurationTest, OtherFreeRunningCountMismatch) {
    p2_.header.tsi                          = VRT_TSI_OTHER;
    p1_.header.tsi                          = VRT_TSI_OTHER;
    p2_.header.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    p1_.header.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    p2_.fields.integer_seconds_timestamp    = 2;
    p1_.fields.integer_seconds_timestamp    = 100;
    p2_.fields.fractional_seconds_timestamp = 500;
    p1_.fields.fractional_seconds_timestamp = 3000;
    ASSERT_EQ(vrt_time_difference(&p2_, &p1_, 1000.0, &dur_), VRT_ERR_INTEGER_SECONDS_MISMATCH);
}
