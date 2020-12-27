#include <gtest/gtest.h>

#include <cstdint>

#include <vrt/vrt_error_code.h>
#include <vrt/vrt_init.h>
#include <vrt/vrt_time.h>
#include <vrt/vrt_types.h>

static const uint64_t PS_IN_S{1000000000000};

class TimeDifferenceFieldsTest : public ::testing::Test {
   protected:
    void SetUp() override {
        vrt_init_header(&h1_);
        vrt_init_fields(&f1_);
        vrt_init_header(&h2_);
        vrt_init_fields(&f2_);
    }

    vrt_header h1_{};
    vrt_fields f1_{};
    vrt_header h2_{};
    vrt_fields f2_{};
    vrt_time   dur_{};
};

TEST_F(TimeDifferenceFieldsTest, None) {
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 0.0, &dur_), 0);
}

TEST_F(TimeDifferenceFieldsTest, MismatchTsi) {
    h2_.tsi = VRT_TSI_UTC;
    h1_.tsi = VRT_TSI_GPS;
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 0.0, &dur_), VRT_ERR_MISMATCH_TIME_TYPES);
}

TEST_F(TimeDifferenceFieldsTest, MismatchTsf) {
    h2_.tsf = VRT_TSF_SAMPLE_COUNT;
    h1_.tsf = VRT_TSF_FREE_RUNNING_COUNT;
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 0.0, &dur_), VRT_ERR_MISMATCH_TIME_TYPES);
}

TEST_F(TimeDifferenceFieldsTest, UtcPositive) {
    h2_.tsi                          = VRT_TSI_UTC;
    h1_.tsi                          = VRT_TSI_UTC;
    f2_.integer_seconds_timestamp    = 5;
    f1_.integer_seconds_timestamp    = 2;
    f2_.fractional_seconds_timestamp = 5000;
    f1_.fractional_seconds_timestamp = 2000;
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 0.0, &dur_), 0);
    ASSERT_EQ(dur_.s, 3);
    ASSERT_EQ(dur_.ps, 0);
}

TEST_F(TimeDifferenceFieldsTest, UtcNegative) {
    h2_.tsi                          = VRT_TSI_UTC;
    h1_.tsi                          = VRT_TSI_UTC;
    f2_.integer_seconds_timestamp    = 2;
    f1_.integer_seconds_timestamp    = 5;
    f2_.fractional_seconds_timestamp = 5000;
    f1_.fractional_seconds_timestamp = 2000;
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 0.0, &dur_), 0);
    ASSERT_EQ(dur_.s, -3);
    ASSERT_EQ(dur_.ps, 0);
}

TEST_F(TimeDifferenceFieldsTest, GpsPositive) {
    h2_.tsi                          = VRT_TSI_GPS;
    h1_.tsi                          = VRT_TSI_GPS;
    f2_.integer_seconds_timestamp    = 5;
    f1_.integer_seconds_timestamp    = 2;
    f2_.fractional_seconds_timestamp = 5000;
    f1_.fractional_seconds_timestamp = 2000;
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 0.0, &dur_), 0);
    ASSERT_EQ(dur_.s, 3);
    ASSERT_EQ(dur_.ps, 0);
}

TEST_F(TimeDifferenceFieldsTest, GpsNegative) {
    h2_.tsi                          = VRT_TSI_GPS;
    h1_.tsi                          = VRT_TSI_GPS;
    f2_.integer_seconds_timestamp    = 2;
    f1_.integer_seconds_timestamp    = 5;
    f2_.fractional_seconds_timestamp = 5000;
    f1_.fractional_seconds_timestamp = 2000;
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 0.0, &dur_), 0);
    ASSERT_EQ(dur_.s, -3);
    ASSERT_EQ(dur_.ps, 0);
}

TEST_F(TimeDifferenceFieldsTest, OtherPositive) {
    h2_.tsi                          = VRT_TSI_OTHER;
    h1_.tsi                          = VRT_TSI_OTHER;
    f2_.integer_seconds_timestamp    = 5;
    f1_.integer_seconds_timestamp    = 2;
    f2_.fractional_seconds_timestamp = 5000;
    f1_.fractional_seconds_timestamp = 2000;
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 0.0, &dur_), 0);
    ASSERT_EQ(dur_.s, 3);
    ASSERT_EQ(dur_.ps, 0);
}

TEST_F(TimeDifferenceFieldsTest, OtherNegative) {
    h2_.tsi                          = VRT_TSI_OTHER;
    h1_.tsi                          = VRT_TSI_OTHER;
    f2_.integer_seconds_timestamp    = 2;
    f1_.integer_seconds_timestamp    = 5;
    f2_.fractional_seconds_timestamp = 5000;
    f1_.fractional_seconds_timestamp = 2000;
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 0.0, &dur_), 0);
    ASSERT_EQ(dur_.s, -3);
    ASSERT_EQ(dur_.ps, 0);
}

TEST_F(TimeDifferenceFieldsTest, SampleCountPositive) {
    h2_.tsf                          = VRT_TSF_SAMPLE_COUNT;
    h1_.tsf                          = VRT_TSF_SAMPLE_COUNT;
    f2_.integer_seconds_timestamp    = 2;
    f1_.integer_seconds_timestamp    = 5;
    f2_.fractional_seconds_timestamp = 750;
    f1_.fractional_seconds_timestamp = 250;
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 0.0, &dur_), VRT_ERR_MISSING_SAMPLE_RATE);
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 1000.0, &dur_), 0);
    ASSERT_EQ(dur_.s, 0);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDifferenceFieldsTest, SampleCountNegative) {
    h2_.tsf                          = VRT_TSF_SAMPLE_COUNT;
    h1_.tsf                          = VRT_TSF_SAMPLE_COUNT;
    f2_.integer_seconds_timestamp    = 2;
    f1_.integer_seconds_timestamp    = 5;
    f2_.fractional_seconds_timestamp = 250;
    f1_.fractional_seconds_timestamp = 750;
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 0.0, &dur_), VRT_ERR_MISSING_SAMPLE_RATE);
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 1000.0, &dur_), 0);
    ASSERT_EQ(dur_.s, -1);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDifferenceFieldsTest, SampleCountOutsideBounds1) {
    h2_.tsf                          = VRT_TSF_SAMPLE_COUNT;
    h1_.tsf                          = VRT_TSF_SAMPLE_COUNT;
    f2_.fractional_seconds_timestamp = 1000;
    f1_.fractional_seconds_timestamp = 750;
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 1000.0, &dur_), VRT_ERR_BOUNDS_SAMPLE_COUNT);
}

TEST_F(TimeDifferenceFieldsTest, SampleCountOutsideBounds2) {
    h2_.tsf                          = VRT_TSF_SAMPLE_COUNT;
    h1_.tsf                          = VRT_TSF_SAMPLE_COUNT;
    f2_.fractional_seconds_timestamp = 750;
    f1_.fractional_seconds_timestamp = 1000;
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 1000.0, &dur_), VRT_ERR_BOUNDS_SAMPLE_COUNT);
}

TEST_F(TimeDifferenceFieldsTest, RealTimePositive) {
    h2_.tsf                          = VRT_TSF_REAL_TIME;
    h1_.tsf                          = VRT_TSF_REAL_TIME;
    f2_.integer_seconds_timestamp    = 8;
    f1_.integer_seconds_timestamp    = 12;
    f2_.fractional_seconds_timestamp = PS_IN_S * 3 / 4;
    f1_.fractional_seconds_timestamp = PS_IN_S / 4;
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 0.0, &dur_), 0);
    ASSERT_EQ(dur_.s, 0);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDifferenceFieldsTest, RealTimeNegative) {
    h2_.tsf                          = VRT_TSF_REAL_TIME;
    h1_.tsf                          = VRT_TSF_REAL_TIME;
    f2_.integer_seconds_timestamp    = 8;
    f1_.integer_seconds_timestamp    = 12;
    f2_.fractional_seconds_timestamp = PS_IN_S / 4;
    f1_.fractional_seconds_timestamp = PS_IN_S * 3 / 4;
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 0.0, &dur_), 0);
    ASSERT_EQ(dur_.s, -1);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDifferenceFieldsTest, RealTimeBounds1) {
    h2_.tsf                          = VRT_TSF_REAL_TIME;
    h1_.tsf                          = VRT_TSF_REAL_TIME;
    f2_.fractional_seconds_timestamp = PS_IN_S;
    f1_.fractional_seconds_timestamp = PS_IN_S / 4;
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 0.0, &dur_), VRT_ERR_BOUNDS_REAL_TIME);
}

TEST_F(TimeDifferenceFieldsTest, RealTimeBounds2) {
    h2_.tsf                          = VRT_TSF_REAL_TIME;
    h1_.tsf                          = VRT_TSF_REAL_TIME;
    f2_.fractional_seconds_timestamp = PS_IN_S / 4;
    f1_.fractional_seconds_timestamp = PS_IN_S;
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 0.0, &dur_), VRT_ERR_BOUNDS_REAL_TIME);
}

TEST_F(TimeDifferenceFieldsTest, FreeRunningCountPositive) {
    h2_.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    h1_.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    f2_.integer_seconds_timestamp    = 2;
    f1_.integer_seconds_timestamp    = 5;
    f2_.fractional_seconds_timestamp = 3000;
    f1_.fractional_seconds_timestamp = 500;
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 0.0, &dur_), VRT_ERR_MISSING_SAMPLE_RATE);
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 1000.0, &dur_), 0);
    ASSERT_EQ(dur_.s, 2);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDifferenceFieldsTest, FreeRunningCountNegative) {
    h2_.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    h1_.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    f2_.integer_seconds_timestamp    = 2;
    f1_.integer_seconds_timestamp    = 5;
    f2_.fractional_seconds_timestamp = 500;
    f1_.fractional_seconds_timestamp = 3000;
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 0.0, &dur_), VRT_ERR_MISSING_SAMPLE_RATE);
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 1000.0, &dur_), 0);
    ASSERT_EQ(dur_.s, -3);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDifferenceFieldsTest, UtcSampleCountPositive) {
    h2_.tsi                          = VRT_TSI_UTC;
    h1_.tsi                          = VRT_TSI_UTC;
    h2_.tsf                          = VRT_TSF_SAMPLE_COUNT;
    h1_.tsf                          = VRT_TSF_SAMPLE_COUNT;
    f2_.integer_seconds_timestamp    = 5;
    f1_.integer_seconds_timestamp    = 2;
    f2_.fractional_seconds_timestamp = 750;
    f1_.fractional_seconds_timestamp = 250;
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 0.0, &dur_), VRT_ERR_MISSING_SAMPLE_RATE);
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 1000.0, &dur_), 0);
    ASSERT_EQ(dur_.s, 3);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDifferenceFieldsTest, UtcSampleCountNegative) {
    h2_.tsi                          = VRT_TSI_UTC;
    h1_.tsi                          = VRT_TSI_UTC;
    h2_.tsf                          = VRT_TSF_SAMPLE_COUNT;
    h1_.tsf                          = VRT_TSF_SAMPLE_COUNT;
    f2_.integer_seconds_timestamp    = 2;
    f1_.integer_seconds_timestamp    = 5;
    f2_.fractional_seconds_timestamp = 250;
    f1_.fractional_seconds_timestamp = 750;
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 0.0, &dur_), VRT_ERR_MISSING_SAMPLE_RATE);
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 1000.0, &dur_), 0);
    ASSERT_EQ(dur_.s, -4);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDifferenceFieldsTest, UtcSampleCountOutsideBounds1) {
    h2_.tsi                          = VRT_TSI_UTC;
    h1_.tsi                          = VRT_TSI_UTC;
    h2_.tsf                          = VRT_TSF_SAMPLE_COUNT;
    h1_.tsf                          = VRT_TSF_SAMPLE_COUNT;
    f2_.fractional_seconds_timestamp = 1000;
    f1_.fractional_seconds_timestamp = 750;
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 1000.0, &dur_), VRT_ERR_BOUNDS_SAMPLE_COUNT);
}

TEST_F(TimeDifferenceFieldsTest, UtcSampleCountOutsideBounds2) {
    h2_.tsi                          = VRT_TSI_UTC;
    h1_.tsi                          = VRT_TSI_UTC;
    h2_.tsf                          = VRT_TSF_SAMPLE_COUNT;
    h1_.tsf                          = VRT_TSF_SAMPLE_COUNT;
    f2_.fractional_seconds_timestamp = 750;
    f1_.fractional_seconds_timestamp = 1000;
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 1000.0, &dur_), VRT_ERR_BOUNDS_SAMPLE_COUNT);
}

TEST_F(TimeDifferenceFieldsTest, UtcRealTimePositive) {
    h2_.tsi                          = VRT_TSI_UTC;
    h1_.tsi                          = VRT_TSI_UTC;
    h2_.tsf                          = VRT_TSF_REAL_TIME;
    h1_.tsf                          = VRT_TSF_REAL_TIME;
    f2_.integer_seconds_timestamp    = 12;
    f1_.integer_seconds_timestamp    = 8;
    f2_.fractional_seconds_timestamp = PS_IN_S * 3 / 4;
    f1_.fractional_seconds_timestamp = PS_IN_S / 4;
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 0.0, &dur_), 0);
    ASSERT_EQ(dur_.s, 4);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDifferenceFieldsTest, UtcRealTimeNegative) {
    h2_.tsi                          = VRT_TSI_UTC;
    h1_.tsi                          = VRT_TSI_UTC;
    h2_.tsf                          = VRT_TSF_REAL_TIME;
    h1_.tsf                          = VRT_TSF_REAL_TIME;
    f2_.integer_seconds_timestamp    = 8;
    f1_.integer_seconds_timestamp    = 12;
    f2_.fractional_seconds_timestamp = PS_IN_S / 4;
    f1_.fractional_seconds_timestamp = PS_IN_S * 3 / 4;
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 0.0, &dur_), 0);
    ASSERT_EQ(dur_.s, -5);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDifferenceFieldsTest, UtcRealTimeBounds1) {
    h2_.tsi                          = VRT_TSI_UTC;
    h1_.tsi                          = VRT_TSI_UTC;
    h2_.tsf                          = VRT_TSF_REAL_TIME;
    h1_.tsf                          = VRT_TSF_REAL_TIME;
    f2_.fractional_seconds_timestamp = PS_IN_S;
    f1_.fractional_seconds_timestamp = PS_IN_S / 4;
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 0.0, &dur_), VRT_ERR_BOUNDS_REAL_TIME);
}

TEST_F(TimeDifferenceFieldsTest, UtcRealTimeBounds2) {
    h2_.tsi                          = VRT_TSI_UTC;
    h1_.tsi                          = VRT_TSI_UTC;
    h2_.tsf                          = VRT_TSF_REAL_TIME;
    h1_.tsf                          = VRT_TSF_REAL_TIME;
    f2_.fractional_seconds_timestamp = PS_IN_S / 4;
    f1_.fractional_seconds_timestamp = PS_IN_S;
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 0.0, &dur_), VRT_ERR_BOUNDS_REAL_TIME);
}

TEST_F(TimeDifferenceFieldsTest, UtcFreeRunningCountPositive) {
    h2_.tsi                          = VRT_TSI_UTC;
    h1_.tsi                          = VRT_TSI_UTC;
    h2_.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    h1_.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    f2_.integer_seconds_timestamp    = 4;
    f1_.integer_seconds_timestamp    = 2;
    f2_.fractional_seconds_timestamp = 3000;
    f1_.fractional_seconds_timestamp = 500;
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 0.0, &dur_), VRT_ERR_MISSING_SAMPLE_RATE);
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 1000.0, &dur_), 0);
    ASSERT_EQ(dur_.s, 2);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDifferenceFieldsTest, UtcFreeRunningCountNegative) {
    h2_.tsi                          = VRT_TSI_UTC;
    h1_.tsi                          = VRT_TSI_UTC;
    h2_.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    h1_.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    f2_.integer_seconds_timestamp    = 2;
    f1_.integer_seconds_timestamp    = 4;
    f2_.fractional_seconds_timestamp = 500;
    f1_.fractional_seconds_timestamp = 3000;
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 0.0, &dur_), VRT_ERR_MISSING_SAMPLE_RATE);
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 1000.0, &dur_), 0);
    ASSERT_EQ(dur_.s, -3);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDifferenceFieldsTest, UtcFreeRunningCountMismatch) {
    h2_.tsi                          = VRT_TSI_UTC;
    h1_.tsi                          = VRT_TSI_UTC;
    h2_.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    h1_.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    f2_.integer_seconds_timestamp    = 2;
    f1_.integer_seconds_timestamp    = 100;
    f2_.fractional_seconds_timestamp = 500;
    f1_.fractional_seconds_timestamp = 3000;
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 1000.0, &dur_), VRT_ERR_INTEGER_SECONDS_MISMATCH);
}

TEST_F(TimeDifferenceFieldsTest, GpsSampleCountPositive) {
    h2_.tsi                          = VRT_TSI_GPS;
    h1_.tsi                          = VRT_TSI_GPS;
    h2_.tsf                          = VRT_TSF_SAMPLE_COUNT;
    h1_.tsf                          = VRT_TSF_SAMPLE_COUNT;
    f2_.integer_seconds_timestamp    = 5;
    f1_.integer_seconds_timestamp    = 2;
    f2_.fractional_seconds_timestamp = 750;
    f1_.fractional_seconds_timestamp = 250;
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 0.0, &dur_), VRT_ERR_MISSING_SAMPLE_RATE);
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 1000.0, &dur_), 0);
    ASSERT_EQ(dur_.s, 3);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDifferenceFieldsTest, GpsSampleCountNegative) {
    h2_.tsi                          = VRT_TSI_GPS;
    h1_.tsi                          = VRT_TSI_GPS;
    h2_.tsf                          = VRT_TSF_SAMPLE_COUNT;
    h1_.tsf                          = VRT_TSF_SAMPLE_COUNT;
    f2_.integer_seconds_timestamp    = 2;
    f1_.integer_seconds_timestamp    = 5;
    f2_.fractional_seconds_timestamp = 250;
    f1_.fractional_seconds_timestamp = 750;
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 0.0, &dur_), VRT_ERR_MISSING_SAMPLE_RATE);
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 1000.0, &dur_), 0);
    ASSERT_EQ(dur_.s, -4);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDifferenceFieldsTest, GpsSampleCountOutsideBounds1) {
    h2_.tsi                          = VRT_TSI_GPS;
    h1_.tsi                          = VRT_TSI_GPS;
    h2_.tsf                          = VRT_TSF_SAMPLE_COUNT;
    h1_.tsf                          = VRT_TSF_SAMPLE_COUNT;
    f2_.fractional_seconds_timestamp = 1000;
    f1_.fractional_seconds_timestamp = 750;
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 1000.0, &dur_), VRT_ERR_BOUNDS_SAMPLE_COUNT);
}

TEST_F(TimeDifferenceFieldsTest, GpsSampleCountOutsideBounds2) {
    h2_.tsi                          = VRT_TSI_GPS;
    h1_.tsi                          = VRT_TSI_GPS;
    h2_.tsf                          = VRT_TSF_SAMPLE_COUNT;
    h1_.tsf                          = VRT_TSF_SAMPLE_COUNT;
    f2_.fractional_seconds_timestamp = 750;
    f1_.fractional_seconds_timestamp = 1000;
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 1000.0, &dur_), VRT_ERR_BOUNDS_SAMPLE_COUNT);
}

TEST_F(TimeDifferenceFieldsTest, GpsRealTimePositive) {
    h2_.tsi                          = VRT_TSI_GPS;
    h1_.tsi                          = VRT_TSI_GPS;
    h2_.tsf                          = VRT_TSF_REAL_TIME;
    h1_.tsf                          = VRT_TSF_REAL_TIME;
    f2_.integer_seconds_timestamp    = 12;
    f1_.integer_seconds_timestamp    = 8;
    f2_.fractional_seconds_timestamp = PS_IN_S * 3 / 4;
    f1_.fractional_seconds_timestamp = PS_IN_S / 4;
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 0.0, &dur_), 0);
    ASSERT_EQ(dur_.s, 4);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDifferenceFieldsTest, GpsRealTimeNegative) {
    h2_.tsi                          = VRT_TSI_GPS;
    h1_.tsi                          = VRT_TSI_GPS;
    h2_.tsf                          = VRT_TSF_REAL_TIME;
    h1_.tsf                          = VRT_TSF_REAL_TIME;
    f2_.integer_seconds_timestamp    = 8;
    f1_.integer_seconds_timestamp    = 12;
    f2_.fractional_seconds_timestamp = PS_IN_S / 4;
    f1_.fractional_seconds_timestamp = PS_IN_S * 3 / 4;
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 0.0, &dur_), 0);
    ASSERT_EQ(dur_.s, -5);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDifferenceFieldsTest, GpsRealTimeBounds1) {
    h2_.tsi                          = VRT_TSI_GPS;
    h1_.tsi                          = VRT_TSI_GPS;
    h2_.tsf                          = VRT_TSF_REAL_TIME;
    h1_.tsf                          = VRT_TSF_REAL_TIME;
    f2_.fractional_seconds_timestamp = PS_IN_S;
    f1_.fractional_seconds_timestamp = PS_IN_S / 4;
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 0.0, &dur_), VRT_ERR_BOUNDS_REAL_TIME);
}

TEST_F(TimeDifferenceFieldsTest, GpsRealTimeBounds2) {
    h2_.tsi                          = VRT_TSI_GPS;
    h1_.tsi                          = VRT_TSI_GPS;
    h2_.tsf                          = VRT_TSF_REAL_TIME;
    h1_.tsf                          = VRT_TSF_REAL_TIME;
    f2_.fractional_seconds_timestamp = PS_IN_S / 4;
    f1_.fractional_seconds_timestamp = PS_IN_S;
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 0.0, &dur_), VRT_ERR_BOUNDS_REAL_TIME);
}

TEST_F(TimeDifferenceFieldsTest, GpsFreeRunningCountPositive) {
    h2_.tsi                          = VRT_TSI_GPS;
    h1_.tsi                          = VRT_TSI_GPS;
    h2_.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    h1_.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    f2_.integer_seconds_timestamp    = 4;
    f1_.integer_seconds_timestamp    = 2;
    f2_.fractional_seconds_timestamp = 3000;
    f1_.fractional_seconds_timestamp = 500;
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 0.0, &dur_), VRT_ERR_MISSING_SAMPLE_RATE);
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 1000.0, &dur_), 0);
    ASSERT_EQ(dur_.s, 2);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDifferenceFieldsTest, GpsFreeRunningCountNegative) {
    h2_.tsi                          = VRT_TSI_GPS;
    h1_.tsi                          = VRT_TSI_GPS;
    h2_.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    h1_.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    f2_.integer_seconds_timestamp    = 2;
    f1_.integer_seconds_timestamp    = 4;
    f2_.fractional_seconds_timestamp = 500;
    f1_.fractional_seconds_timestamp = 3000;
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 0.0, &dur_), VRT_ERR_MISSING_SAMPLE_RATE);
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 1000.0, &dur_), 0);
    ASSERT_EQ(dur_.s, -3);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDifferenceFieldsTest, GpsFreeRunningCountMismatch) {
    h2_.tsi                          = VRT_TSI_GPS;
    h1_.tsi                          = VRT_TSI_GPS;
    h2_.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    h1_.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    f2_.integer_seconds_timestamp    = 2;
    f1_.integer_seconds_timestamp    = 100;
    f2_.fractional_seconds_timestamp = 500;
    f1_.fractional_seconds_timestamp = 3000;
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 1000.0, &dur_), VRT_ERR_INTEGER_SECONDS_MISMATCH);
}

TEST_F(TimeDifferenceFieldsTest, OtherSampleCountPositive) {
    h2_.tsi                          = VRT_TSI_OTHER;
    h1_.tsi                          = VRT_TSI_OTHER;
    h2_.tsf                          = VRT_TSF_SAMPLE_COUNT;
    h1_.tsf                          = VRT_TSF_SAMPLE_COUNT;
    f2_.integer_seconds_timestamp    = 5;
    f1_.integer_seconds_timestamp    = 2;
    f2_.fractional_seconds_timestamp = 750;
    f1_.fractional_seconds_timestamp = 250;
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 0.0, &dur_), VRT_ERR_MISSING_SAMPLE_RATE);
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 1000.0, &dur_), 0);
    ASSERT_EQ(dur_.s, 3);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDifferenceFieldsTest, OtherSampleCountNegative) {
    h2_.tsi                          = VRT_TSI_OTHER;
    h1_.tsi                          = VRT_TSI_OTHER;
    h2_.tsf                          = VRT_TSF_SAMPLE_COUNT;
    h1_.tsf                          = VRT_TSF_SAMPLE_COUNT;
    f2_.integer_seconds_timestamp    = 2;
    f1_.integer_seconds_timestamp    = 5;
    f2_.fractional_seconds_timestamp = 250;
    f1_.fractional_seconds_timestamp = 750;
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 0.0, &dur_), VRT_ERR_MISSING_SAMPLE_RATE);
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 1000.0, &dur_), 0);
    ASSERT_EQ(dur_.s, -4);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDifferenceFieldsTest, OtherSampleCountOutsideBounds1) {
    h2_.tsi                          = VRT_TSI_OTHER;
    h1_.tsi                          = VRT_TSI_OTHER;
    h2_.tsf                          = VRT_TSF_SAMPLE_COUNT;
    h1_.tsf                          = VRT_TSF_SAMPLE_COUNT;
    f2_.fractional_seconds_timestamp = 1000;
    f1_.fractional_seconds_timestamp = 750;
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 1000.0, &dur_), VRT_ERR_BOUNDS_SAMPLE_COUNT);
}

TEST_F(TimeDifferenceFieldsTest, OtherSampleCountOutsideBounds2) {
    h2_.tsi                          = VRT_TSI_OTHER;
    h1_.tsi                          = VRT_TSI_OTHER;
    h2_.tsf                          = VRT_TSF_SAMPLE_COUNT;
    h1_.tsf                          = VRT_TSF_SAMPLE_COUNT;
    f2_.fractional_seconds_timestamp = 750;
    f1_.fractional_seconds_timestamp = 1000;
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 1000.0, &dur_), VRT_ERR_BOUNDS_SAMPLE_COUNT);
}

TEST_F(TimeDifferenceFieldsTest, OtherRealTimePositive) {
    h2_.tsi                          = VRT_TSI_OTHER;
    h1_.tsi                          = VRT_TSI_OTHER;
    h2_.tsf                          = VRT_TSF_REAL_TIME;
    h1_.tsf                          = VRT_TSF_REAL_TIME;
    f2_.integer_seconds_timestamp    = 12;
    f1_.integer_seconds_timestamp    = 8;
    f2_.fractional_seconds_timestamp = PS_IN_S * 3 / 4;
    f1_.fractional_seconds_timestamp = PS_IN_S / 4;
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 0.0, &dur_), 0);
    ASSERT_EQ(dur_.s, 4);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDifferenceFieldsTest, OtherRealTimeNegative) {
    h2_.tsi                          = VRT_TSI_OTHER;
    h1_.tsi                          = VRT_TSI_OTHER;
    h2_.tsf                          = VRT_TSF_REAL_TIME;
    h1_.tsf                          = VRT_TSF_REAL_TIME;
    f2_.integer_seconds_timestamp    = 8;
    f1_.integer_seconds_timestamp    = 12;
    f2_.fractional_seconds_timestamp = PS_IN_S / 4;
    f1_.fractional_seconds_timestamp = PS_IN_S * 3 / 4;
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 0.0, &dur_), 0);
    ASSERT_EQ(dur_.s, -5);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDifferenceFieldsTest, OtherRealTimeBounds1) {
    h2_.tsi                          = VRT_TSI_OTHER;
    h1_.tsi                          = VRT_TSI_OTHER;
    h2_.tsf                          = VRT_TSF_REAL_TIME;
    h1_.tsf                          = VRT_TSF_REAL_TIME;
    f2_.fractional_seconds_timestamp = PS_IN_S;
    f1_.fractional_seconds_timestamp = PS_IN_S / 4;
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 0.0, &dur_), VRT_ERR_BOUNDS_REAL_TIME);
}

TEST_F(TimeDifferenceFieldsTest, OtherRealTimeBounds2) {
    h2_.tsi                          = VRT_TSI_OTHER;
    h1_.tsi                          = VRT_TSI_OTHER;
    h2_.tsf                          = VRT_TSF_REAL_TIME;
    h1_.tsf                          = VRT_TSF_REAL_TIME;
    f2_.fractional_seconds_timestamp = PS_IN_S / 4;
    f1_.fractional_seconds_timestamp = PS_IN_S;
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 0.0, &dur_), VRT_ERR_BOUNDS_REAL_TIME);
}

TEST_F(TimeDifferenceFieldsTest, OtherFreeRunningCountPositive) {
    h2_.tsi                          = VRT_TSI_OTHER;
    h1_.tsi                          = VRT_TSI_OTHER;
    h2_.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    h1_.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    f2_.integer_seconds_timestamp    = 4;
    f1_.integer_seconds_timestamp    = 2;
    f2_.fractional_seconds_timestamp = 3000;
    f1_.fractional_seconds_timestamp = 500;
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 0.0, &dur_), VRT_ERR_MISSING_SAMPLE_RATE);
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 1000.0, &dur_), 0);
    ASSERT_EQ(dur_.s, 2);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDifferenceFieldsTest, OtherFreeRunningCountNegative) {
    h2_.tsi                          = VRT_TSI_OTHER;
    h1_.tsi                          = VRT_TSI_OTHER;
    h2_.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    h1_.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    f2_.integer_seconds_timestamp    = 2;
    f1_.integer_seconds_timestamp    = 4;
    f2_.fractional_seconds_timestamp = 500;
    f1_.fractional_seconds_timestamp = 3000;
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 0.0, &dur_), VRT_ERR_MISSING_SAMPLE_RATE);
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 1000.0, &dur_), 0);
    ASSERT_EQ(dur_.s, -3);
    ASSERT_EQ(dur_.ps, PS_IN_S / 2);
}

TEST_F(TimeDifferenceFieldsTest, OtherFreeRunningCountMismatch) {
    h2_.tsi                          = VRT_TSI_OTHER;
    h1_.tsi                          = VRT_TSI_OTHER;
    h2_.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    h1_.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    f2_.integer_seconds_timestamp    = 2;
    f1_.integer_seconds_timestamp    = 100;
    f2_.fractional_seconds_timestamp = 500;
    f1_.fractional_seconds_timestamp = 3000;
    ASSERT_EQ(vrt_time_difference_fields(&h2_, &f2_, &h1_, &f1_, 1000.0, &dur_), VRT_ERR_INTEGER_SECONDS_MISMATCH);
}
