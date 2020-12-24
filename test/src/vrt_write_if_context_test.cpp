#include <gtest/gtest.h>

#include <array>
#include <cstdint>
#include <cstring>
#include <set>

#include <vrt/vrt_error_code.h>
#include <vrt/vrt_init.h>
#include <vrt/vrt_types.h>
#include <vrt/vrt_write.h>

#include "hex.h"

class WriteIfContextTest : public ::testing::Test {
   protected:
    void SetUp() override {
        vrt_init_if_context(&c_);
        buf_.fill(0xBAADF00D);
    }

    vrt_if_context             c_{};
    std::array<uint32_t, 1024> buf_{};
};

/**
 * Compare Formatted geolocation GPS/INS or ECEF/Relative ephemeris buffer elements with given values, but use 11 or 13
 * default values if none are given. Does not check header (Buffer index 0).
 *
 * \param buf    Buffer to compare with.
 * \param ignore List of indices to ignore.
 * \param use11  True if 11 values (GPS/INS geolocation) shall be used instead of 13 (ECEF/Relative ephemeris).
 */
static void buf_cmp_geolocation_ephemeris(const std::array<uint32_t, 1024>& buf,
                                          const std::set<size_t>&           ignore,
                                          bool                              use11) {
    if (ignore.count(1) == 0) {
        ASSERT_EQ(Hex(buf[1]), Hex(0x00000000));
    }
    if (ignore.count(2) == 0) {
        ASSERT_EQ(Hex(buf[2]), Hex(0xFFFFFFFF));
    }
    if (ignore.count(3) == 0) {
        ASSERT_EQ(Hex(buf[3]), Hex(0xFFFFFFFF));
    }
    if (ignore.count(4) == 0) {
        ASSERT_EQ(Hex(buf[4]), Hex(0xFFFFFFFF));
    }
    if (ignore.count(5) == 0) {
        ASSERT_EQ(Hex(buf[5]), Hex(0x7FFFFFFF));
    }
    if (ignore.count(6) == 0) {
        ASSERT_EQ(Hex(buf[6]), Hex(0x7FFFFFFF));
    }
    if (ignore.count(7) == 0) {
        ASSERT_EQ(Hex(buf[7]), Hex(0x7FFFFFFF));
    }
    if (ignore.count(8) == 0) {
        ASSERT_EQ(Hex(buf[8]), Hex(0x7FFFFFFF));
    }
    if (ignore.count(9) == 0) {
        ASSERT_EQ(Hex(buf[9]), Hex(0x7FFFFFFF));
    }
    if (ignore.count(10) == 0) {
        ASSERT_EQ(Hex(buf[10]), Hex(0x7FFFFFFF));
    }
    if (ignore.count(11) == 0) {
        ASSERT_EQ(Hex(buf[11]), Hex(0x7FFFFFFF));
    }
    if (!use11) {
        if (ignore.count(12) == 0) {
            ASSERT_EQ(Hex(buf[12]), Hex(0x7FFFFFFF));
        }
        if (ignore.count(13) == 0) {
            ASSERT_EQ(Hex(buf[13]), Hex(0x7FFFFFFF));
        }
    }

    ASSERT_EQ(Hex(buf[use11 ? 12 : 14]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, NegativeSizeBuffer) {
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), -1, true), VRT_ERR_BUFFER_SIZE);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), -1, false), VRT_ERR_BUFFER_SIZE);
}

TEST_F(WriteIfContextTest, ZeroSizeBuffer) {
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 0, true), VRT_ERR_BUFFER_SIZE);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 0, false), VRT_ERR_BUFFER_SIZE);
}

TEST_F(WriteIfContextTest, None) {
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 1, true), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, ContextFieldChange) {
    c_.context_field_change_indicator = true;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 1, true), 1);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x80000000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, ReferencePointIdentifier) {
    c_.has.reference_point_identifier = true;
    c_.reference_point_identifier     = 0xFEFEFEFE;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 2, true), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x40000000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xFEFEFEFE));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, Bandwidth) {
    c_.has.bandwidth = true;
    c_.bandwidth     = 4097.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, true), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x20000000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000001));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x00100000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, BandwidthSmall) {
    c_.has.bandwidth = true;
    c_.bandwidth     = 0.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, true), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x20000000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, BandwidthLarge) {
    c_.has.bandwidth = true;
    c_.bandwidth     = 8.79e12;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, true), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x20000000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x7FE94D3D));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xC0000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, BandwidthInvalidSmall) {
    c_.has.bandwidth = true;
    c_.bandwidth     = -1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, true), VRT_ERR_BOUNDS_BANDWIDTH);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, false), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x20000000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xFFFFFFFF));
    /* Due to low precision of double when converting to fixed point, don't do the comparison below */
    /*ASSERT_EQ(Hex(buf_[2]), Hex(0xFFFFFFFF));*/
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, BandwidthInvalidLarge) {
    c_.has.bandwidth = true;
    c_.bandwidth     = 8.80e12;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, true), VRT_ERR_BOUNDS_BANDWIDTH);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, false), 3);
}

TEST_F(WriteIfContextTest, IfReferenceFrequency) {
    c_.has.if_reference_frequency = true;
    c_.if_reference_frequency     = 4097.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, true), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x10000000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000001));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x00100000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, IfReferenceFrequencySmall) {
    c_.has.if_reference_frequency = true;
    c_.if_reference_frequency     = -8.79e12;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, true), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x10000000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x8016B2C2));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x40000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, IfReferenceFrequencyLarge) {
    c_.has.if_reference_frequency = true;
    c_.if_reference_frequency     = 8.79e12;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, true), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x10000000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x7FE94D3D));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xC0000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, IfReferenceFrequencyInvalidSmall) {
    c_.has.if_reference_frequency = true;
    c_.if_reference_frequency     = -8.80e12;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, true), VRT_ERR_BOUNDS_IF_REFERENCE_FREQUENCY);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, false), 3);
}

TEST_F(WriteIfContextTest, IfReferenceFrequencyInvalidLarge) {
    c_.has.if_reference_frequency = true;
    c_.if_reference_frequency     = 8.80e12;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, true), VRT_ERR_BOUNDS_IF_REFERENCE_FREQUENCY);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, false), 3);
}

TEST_F(WriteIfContextTest, RfReferenceFrequency) {
    c_.has.rf_reference_frequency = true;
    c_.rf_reference_frequency     = 4097.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, true), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x08000000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000001));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x00100000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, RfReferenceFrequencySmall) {
    c_.has.rf_reference_frequency = true;
    c_.rf_reference_frequency     = -8.79e12;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, true), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x08000000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x8016B2C2));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x40000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, RfReferenceFrequencyLarge) {
    c_.has.rf_reference_frequency = true;
    c_.rf_reference_frequency     = 8.79e12;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, true), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x08000000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x7FE94D3D));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xC0000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, RfReferenceFrequencyInvalidSmall) {
    c_.has.rf_reference_frequency = true;
    c_.rf_reference_frequency     = -8.80e12;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, true), VRT_ERR_BOUNDS_RF_REFERENCE_FREQUENCY);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, false), 3);
}

TEST_F(WriteIfContextTest, RfReferenceFrequencyInvalidLarge) {
    c_.has.rf_reference_frequency = true;
    c_.rf_reference_frequency     = 8.80e12;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, true), VRT_ERR_BOUNDS_RF_REFERENCE_FREQUENCY);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, false), 3);
}

TEST_F(WriteIfContextTest, RfReferenceFrequencyOffset) {
    c_.has.rf_reference_frequency_offset = true;
    c_.rf_reference_frequency_offset     = 4097.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, true), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x04000000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000001));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x00100000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, RfReferenceFrequencyOffsetSmall) {
    c_.has.rf_reference_frequency_offset = true;
    c_.rf_reference_frequency_offset     = -8.79e12;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, true), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x04000000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x8016B2C2));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x40000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, RfReferenceFrequencyOffsetLarge) {
    c_.has.rf_reference_frequency_offset = true;
    c_.rf_reference_frequency_offset     = 8.79e12;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, true), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x04000000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x7FE94D3D));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xC0000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, RfReferenceFrequencyOffsetInvalidSmall) {
    c_.has.rf_reference_frequency_offset = true;
    c_.rf_reference_frequency_offset     = -8.80e12;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, true), VRT_ERR_BOUNDS_RF_REFERENCE_FREQUENCY_OFFSET);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, false), 3);
}

TEST_F(WriteIfContextTest, RfReferenceFrequencyOffsetInvalidLarge) {
    c_.has.rf_reference_frequency_offset = true;
    c_.rf_reference_frequency_offset     = 8.80e12;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, true), VRT_ERR_BOUNDS_RF_REFERENCE_FREQUENCY_OFFSET);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, false), 3);
}

TEST_F(WriteIfContextTest, IfBandOffset) {
    c_.has.if_band_offset = true;
    c_.if_band_offset     = 4097.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, true), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x02000000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000001));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x00100000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, IfBandOffsetSmall) {
    c_.has.if_band_offset = true;
    c_.if_band_offset     = -8.79e12;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, true), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x02000000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x8016B2C2));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x40000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, IfBandOffsetLarge) {
    c_.has.if_band_offset = true;
    c_.if_band_offset     = 8.79e12;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, true), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x02000000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x7FE94D3D));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xC0000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, IfBandOffsetInvalidSmall) {
    c_.has.if_band_offset = true;
    c_.if_band_offset     = -8.80e12;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, true), VRT_ERR_BOUNDS_IF_BAND_OFFSET);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, false), 3);
}

TEST_F(WriteIfContextTest, IfBandOffsetInvalidLarge) {
    c_.has.if_band_offset = true;
    c_.if_band_offset     = 8.80e12;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, true), VRT_ERR_BOUNDS_IF_BAND_OFFSET);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, false), 3);
}

TEST_F(WriteIfContextTest, ReferenceLevel) {
    c_.has.reference_level = true;
    c_.reference_level     = -1.0F;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 2, true), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x01000000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x0000FF80));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, ReferenceLevelSmall) {
    c_.has.reference_level = true;
    c_.reference_level     = -256.0F;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 2, true), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x01000000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00008000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, ReferenceLevelLarge) {
    c_.has.reference_level = true;
    c_.reference_level     = 255.0F;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 2, true), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x01000000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00007F80));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, ReferenceLevelInvalidSmall) {
    c_.has.reference_level = true;
    c_.reference_level     = -257.0F;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 2, true), VRT_ERR_BOUNDS_REFERENCE_LEVEL);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 2, false), 2);
}

TEST_F(WriteIfContextTest, ReferenceLevelInvalidLarge) {
    c_.has.reference_level = true;
    c_.reference_level     = 256.0F;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 2, true), VRT_ERR_BOUNDS_REFERENCE_LEVEL);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 2, false), 2);
}

TEST_F(WriteIfContextTest, Gain1) {
    c_.has.gain    = true;
    c_.gain.stage1 = -1.0F;
    c_.gain.stage2 = 1.0F;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 2, true), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00800000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x0080FF80));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, Gain2) {
    c_.has.gain    = true;
    c_.gain.stage1 = -1.0F;
    c_.gain.stage2 = 0.0F;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 2, true), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00800000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x0000FF80));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, GainSmall) {
    c_.has.gain    = true;
    c_.gain.stage1 = -256.0F;
    c_.gain.stage2 = -256.0F;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 2, true), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00800000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x80008000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, GainLarge) {
    c_.has.gain    = true;
    c_.gain.stage1 = 255.0F;
    c_.gain.stage2 = 255.0F;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 2, true), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00800000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x7F807F80));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, GainStage1InvalidSmall) {
    c_.has.gain    = true;
    c_.gain.stage1 = -257.0F;
    c_.gain.stage2 = 1.0F;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 2, true), VRT_ERR_BOUNDS_GAIN);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 2, false), 2);
}

TEST_F(WriteIfContextTest, GainStage2InvalidSmall) {
    c_.has.gain    = true;
    c_.gain.stage1 = 1.0F;
    c_.gain.stage2 = -257.0F;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 2, true), VRT_ERR_BOUNDS_GAIN);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 2, false), 2);
}

TEST_F(WriteIfContextTest, GainStage1InvalidLarge) {
    c_.has.gain    = true;
    c_.gain.stage1 = 256.0F;
    c_.gain.stage2 = 1.0F;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 2, true), VRT_ERR_BOUNDS_GAIN);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 2, false), 2);
}

TEST_F(WriteIfContextTest, GainStage2InvalidLarge) {
    c_.has.gain    = true;
    c_.gain.stage1 = 1.0F;
    c_.gain.stage2 = 256.0F;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 2, true), VRT_ERR_BOUNDS_GAIN);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 2, false), 2);
}

TEST_F(WriteIfContextTest, GainInvalidStage2Set) {
    c_.has.gain    = true;
    c_.gain.stage1 = 0.0F;
    c_.gain.stage2 = 1.0F;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 2, true), VRT_ERR_GAIN_STAGE2_SET);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 2, false), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00800000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00800000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, OverRangeCount) {
    c_.has.over_range_count = true;
    c_.over_range_count     = 0xFEFEFEFE;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 2, true), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00400000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xFEFEFEFE));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, SampleRate) {
    c_.has.sample_rate = true;
    c_.sample_rate     = 4097.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, true), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00200000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000001));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x00100000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, SampleRateSmall) {
    c_.has.sample_rate = true;
    c_.sample_rate     = 0.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, true), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00200000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, SampleRateLarge) {
    c_.has.sample_rate = true;
    c_.sample_rate     = 8.79e12;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, true), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00200000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x7FE94D3D));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xC0000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, SampleRateInvalidSmall) {
    c_.has.sample_rate = true;
    c_.sample_rate     = -1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, true), VRT_ERR_BOUNDS_SAMPLE_RATE);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, false), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00200000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xFFFFFFFF));
    /* Due to low precision of double when converting to fixed point, don't do the comparison below */
    /*ASSERT_EQ(Hex(buf_[2]), Hex(0xFFFFFFFF));*/
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, SampleRateInvalidLarge) {
    c_.has.sample_rate = true;
    c_.sample_rate     = 8.80e12;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, true), VRT_ERR_BOUNDS_SAMPLE_RATE);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, false), 3);
}

TEST_F(WriteIfContextTest, TimestampAdjustment) {
    c_.has.timestamp_adjustment = true;
    c_.timestamp_adjustment     = 0xABABABABBEBEBEBE;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, true), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00100000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xABABABAB));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBEBEBEBE));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, TimestampCalibrationTime) {
    c_.has.timestamp_calibration_time = true;
    c_.timestamp_calibration_time     = 0xABABABAB;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 2, true), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00080000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xABABABAB));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, Temperature) {
    c_.has.temperature = true;
    c_.temperature     = -1.0F;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 2, true), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00040000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x0000FFC0));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, TemperatureSmall) {
    c_.has.temperature = true;
    c_.temperature     = -273.0F;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 2, true), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00040000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x0000BBC0));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, TemperatureLarge) {
    c_.has.temperature = true;
    c_.temperature     = 511.0F;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 2, true), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00040000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00007FC0));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, TemperatureInvalidSmall) {
    c_.has.temperature = true;
    c_.temperature     = -274.0F;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 2, true), VRT_ERR_BOUNDS_TEMPERATURE);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 2, false), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00040000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x0000BB80));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, TemperatureInvalidLarge) {
    c_.has.temperature = true;
    c_.temperature     = 512.0F;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 2, true), VRT_ERR_BOUNDS_TEMPERATURE);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 2, false), 2);
}

TEST_F(WriteIfContextTest, DeviceIdentifierOui) {
    c_.has.device_identifier = true;
    c_.device_identifier.oui = 0x00FFFFFF;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, true), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00020000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00FFFFFF));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, DeviceIdentifierOuiInvalid) {
    c_.has.device_identifier         = true;
    c_.device_identifier.oui         = 0x01FFFFFF;
    c_.device_identifier.device_code = 0xABAF;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, true), VRT_ERR_BOUNDS_OUI);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, false), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00020000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00FFFFFF));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x0000ABAF));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, DeviceIdentifierDeviceCode) {
    c_.has.device_identifier         = true;
    c_.device_identifier.device_code = 0xABAF;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, true), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00020000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x0000ABAF));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, StateAndEventIndicatorsHasCalibratedTime) {
    c_.has.state_and_event_indicators                 = true;
    c_.state_and_event_indicators.has.calibrated_time = true;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 2, true), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x80000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, StateAndEventIndicatorsHasValidData) {
    c_.has.state_and_event_indicators            = true;
    c_.state_and_event_indicators.has.valid_data = true;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 2, true), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x40000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, StateAndEventIndicatorsHasReferenceLock) {
    c_.has.state_and_event_indicators                = true;
    c_.state_and_event_indicators.has.reference_lock = true;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 2, true), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x20000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, StateAndEventIndicatorsHasAgcOrMgc) {
    c_.has.state_and_event_indicators            = true;
    c_.state_and_event_indicators.has.agc_or_mgc = true;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 2, true), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x10000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, StateAndEventIndicatorsHasDetectedSignal) {
    c_.has.state_and_event_indicators                 = true;
    c_.state_and_event_indicators.has.detected_signal = true;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 2, true), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x08000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, StateAndEventIndicatorsHasSpectralInversion) {
    c_.has.state_and_event_indicators                    = true;
    c_.state_and_event_indicators.has.spectral_inversion = true;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 2, true), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x04000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, StateAndEventIndicatorsHasOverRange) {
    c_.has.state_and_event_indicators            = true;
    c_.state_and_event_indicators.has.over_range = true;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 2, true), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x02000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, StateAndEventIndicatorsHasSampleLoss) {
    c_.has.state_and_event_indicators             = true;
    c_.state_and_event_indicators.has.sample_loss = true;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 2, true), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x01000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, StateAndEventIndicatorsCalibratedTime) {
    c_.has.state_and_event_indicators             = true;
    c_.state_and_event_indicators.calibrated_time = true;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 2, true), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, StateAndEventIndicatorsValidData) {
    c_.has.state_and_event_indicators        = true;
    c_.state_and_event_indicators.valid_data = true;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 2, true), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, StateAndEventIndicatorsReferenceLock) {
    c_.has.state_and_event_indicators            = true;
    c_.state_and_event_indicators.reference_lock = true;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 2, true), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, StateAndEventIndicatorsAgcOrMgc) {
    c_.has.state_and_event_indicators        = true;
    c_.state_and_event_indicators.agc_or_mgc = VRT_AOM_AGC;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 2, true), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, StateAndEventIndicatorsDetectedSignal) {
    c_.has.state_and_event_indicators             = true;
    c_.state_and_event_indicators.detected_signal = true;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 2, true), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, StateAndEventIndicatorsSpectralInversion) {
    c_.has.state_and_event_indicators                = true;
    c_.state_and_event_indicators.spectral_inversion = true;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 2, true), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, StateAndEventIndicatorsOverRange) {
    c_.has.state_and_event_indicators        = true;
    c_.state_and_event_indicators.over_range = true;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 2, true), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, StateAndEventIndicatorsSampleLoss) {
    c_.has.state_and_event_indicators         = true;
    c_.state_and_event_indicators.sample_loss = true;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 2, true), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, StateAndEventIndicatorsBothCalibratedTime) {
    c_.has.state_and_event_indicators                 = true;
    c_.state_and_event_indicators.has.calibrated_time = true;
    c_.state_and_event_indicators.calibrated_time     = true;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 2, true), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x80080000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, StateAndEventIndicatorsBothValidData) {
    c_.has.state_and_event_indicators            = true;
    c_.state_and_event_indicators.has.valid_data = true;
    c_.state_and_event_indicators.valid_data     = true;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 2, true), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x40040000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, StateAndEventIndicatorsBothReferenceLock) {
    c_.has.state_and_event_indicators                = true;
    c_.state_and_event_indicators.has.reference_lock = true;
    c_.state_and_event_indicators.reference_lock     = true;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 2, true), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x20020000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, StateAndEventIndicatorsBothAgcOrMgc) {
    c_.has.state_and_event_indicators            = true;
    c_.state_and_event_indicators.has.agc_or_mgc = true;
    c_.state_and_event_indicators.agc_or_mgc     = VRT_AOM_AGC;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 2, true), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x10010000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, StateAndEventIndicatorsBothDetectedSignal) {
    c_.has.state_and_event_indicators                 = true;
    c_.state_and_event_indicators.has.detected_signal = true;
    c_.state_and_event_indicators.detected_signal     = true;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 2, true), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x08008000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, StateAndEventIndicatorsBothSpectralInversion) {
    c_.has.state_and_event_indicators                    = true;
    c_.state_and_event_indicators.has.spectral_inversion = true;
    c_.state_and_event_indicators.spectral_inversion     = true;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 2, true), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x04004000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, StateAndEventIndicatorsBothOverRange) {
    c_.has.state_and_event_indicators            = true;
    c_.state_and_event_indicators.has.over_range = true;
    c_.state_and_event_indicators.over_range     = true;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 2, true), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x02002000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, StateAndEventIndicatorsBothSampleLoss) {
    c_.has.state_and_event_indicators             = true;
    c_.state_and_event_indicators.has.sample_loss = true;
    c_.state_and_event_indicators.sample_loss     = true;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 2, true), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x01001000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, StateAndEventIndicatorsBothUserDefined) {
    c_.has.state_and_event_indicators          = true;
    c_.state_and_event_indicators.user_defined = 0xFF;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 2, true), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00010000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x000000FF));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, DataPacketPayloadFormatPackingMethod) {
    c_.has.data_packet_payload_format            = true;
    c_.data_packet_payload_format.packing_method = VRT_PM_LINK_EFFICIENT;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, true), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00008000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x80000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, DataPacketPayloadFormatPackingMethodInvalidSmall) {
    c_.has.data_packet_payload_format            = true;
    c_.data_packet_payload_format.packing_method = static_cast<vrt_packing_method>(-1);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, true), VRT_ERR_INVALID_PACKING_METHOD);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, false), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00008000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x80000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, DataPacketPayloadFormatPackingMethodInvalidLarge) {
    c_.has.data_packet_payload_format            = true;
    c_.data_packet_payload_format.packing_method = static_cast<vrt_packing_method>(2);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, true), VRT_ERR_INVALID_PACKING_METHOD);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, false), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00008000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, DataPacketPayloadFormatRealOrComplex) {
    c_.has.data_packet_payload_format             = true;
    c_.data_packet_payload_format.real_or_complex = VRT_ROC_COMPLEX_POLAR;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, true), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00008000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x40000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, DataPacketPayloadFormatRealOrComplexInvalidSmall) {
    c_.has.data_packet_payload_format             = true;
    c_.data_packet_payload_format.real_or_complex = static_cast<vrt_real_complex>(-1);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, true), VRT_ERR_INVALID_REAL_OR_COMPLEX);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, false), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00008000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x60000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, DataPacketPayloadFormatRealOrComplexInvalidLarge) {
    c_.has.data_packet_payload_format             = true;
    c_.data_packet_payload_format.real_or_complex = static_cast<vrt_real_complex>(3);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, true), VRT_ERR_INVALID_REAL_OR_COMPLEX);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, false), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00008000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x60000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, DataPacketPayloadFormatDataItemFormat) {
    c_.has.data_packet_payload_format              = true;
    c_.data_packet_payload_format.data_item_format = VRT_DIF_UNSIGNED_VRT_6_BIT_EXPONENT;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, true), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00008000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x16000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, DataPacketPayloadFormatDataItemFormatInvalid1) {
    c_.has.data_packet_payload_format              = true;
    c_.data_packet_payload_format.data_item_format = static_cast<vrt_data_item_format>(-1);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, true), VRT_ERR_INVALID_DATA_ITEM_FORMAT);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, false), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00008000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x1F000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, DataPacketPayloadFormatDataItemFormatInvalid2) {
    c_.has.data_packet_payload_format              = true;
    c_.data_packet_payload_format.data_item_format = static_cast<vrt_data_item_format>(0x07);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, true), VRT_ERR_INVALID_DATA_ITEM_FORMAT);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, false), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00008000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x07000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, DataPacketPayloadFormatDataItemFormatInvalid3) {
    c_.has.data_packet_payload_format              = true;
    c_.data_packet_payload_format.data_item_format = static_cast<vrt_data_item_format>(0x0D);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, true), VRT_ERR_INVALID_DATA_ITEM_FORMAT);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, false), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00008000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x0D000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, DataPacketPayloadFormatDataItemFormatInvalid4) {
    c_.has.data_packet_payload_format              = true;
    c_.data_packet_payload_format.data_item_format = static_cast<vrt_data_item_format>(0x17);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, true), VRT_ERR_INVALID_DATA_ITEM_FORMAT);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, false), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00008000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x17000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, DataPacketPayloadFormatSampleComponentRepeat) {
    c_.has.data_packet_payload_format                     = true;
    c_.data_packet_payload_format.sample_component_repeat = true;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, true), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00008000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00800000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, DataPacketPayloadFormatEventTagSize) {
    c_.has.data_packet_payload_format            = true;
    c_.data_packet_payload_format.event_tag_size = 0x07;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, true), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00008000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00700000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, DataPacketPayloadFormatEventTagSizeInvalid) {
    c_.has.data_packet_payload_format            = true;
    c_.data_packet_payload_format.event_tag_size = 0x08;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, true), VRT_ERR_BOUNDS_EVENT_TAG_SIZE);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, false), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00008000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, DataPacketPayloadFormatChannelTagSize) {
    c_.has.data_packet_payload_format              = true;
    c_.data_packet_payload_format.channel_tag_size = 0x0F;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, true), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00008000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x000F0000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, DataPacketPayloadFormatChannelTagSizeInvalid) {
    c_.has.data_packet_payload_format              = true;
    c_.data_packet_payload_format.channel_tag_size = 0x10;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, true), VRT_ERR_BOUNDS_CHANNEL_TAG_SIZE);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, false), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00008000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, DataPacketPayloadFormatItemPackingFieldSize) {
    c_.has.data_packet_payload_format                     = true;
    c_.data_packet_payload_format.item_packing_field_size = 0x3F;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, true), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00008000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000FC0));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, DataPacketPayloadFormatItemPackingFieldSizeInvalid) {
    c_.has.data_packet_payload_format                     = true;
    c_.data_packet_payload_format.item_packing_field_size = 0x40;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, true), VRT_ERR_BOUNDS_ITEM_PACKING_FIELD_SIZE);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, false), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00008000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, DataPacketPayloadFormatDataItemSize) {
    c_.has.data_packet_payload_format            = true;
    c_.data_packet_payload_format.data_item_size = 0x3F;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, true), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00008000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x0000003F));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, DataPacketPayloadFormatDataItemSizeInvalid) {
    c_.has.data_packet_payload_format            = true;
    c_.data_packet_payload_format.data_item_size = 0x40;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, true), VRT_ERR_BOUNDS_DATA_ITEM_SIZE);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, false), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00008000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, DataPacketPayloadFormatRepeatCount) {
    c_.has.data_packet_payload_format          = true;
    c_.data_packet_payload_format.repeat_count = 0xFFFF;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, true), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00008000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xFFFF0000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, DataPacketPayloadFormatVectorSize) {
    c_.has.data_packet_payload_format         = true;
    c_.data_packet_payload_format.vector_size = 0xFFFF;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, true), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00008000));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x0000FFFF));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, FormattedGpsGeolocationTsi) {
    c_.has.formatted_gps_geolocation = true;
    c_.formatted_gps_geolocation.tsi = VRT_TSI_OTHER;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {1}, true);
    ASSERT_EQ(Hex(buf_[1]), Hex(0x0C000000));
}

TEST_F(WriteIfContextTest, FormattedGpsGeolocationTsiInvalidSmall) {
    c_.has.formatted_gps_geolocation = true;
    c_.formatted_gps_geolocation.tsi = static_cast<vrt_tsi>(-1);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), VRT_ERR_INVALID_TSI);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, false), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {1}, true);
    ASSERT_EQ(Hex(buf_[1]), Hex(0x0C000000));
}

TEST_F(WriteIfContextTest, FormattedGpsGeolocationTsiInvalidLarge) {
    c_.has.formatted_gps_geolocation = true;
    c_.formatted_gps_geolocation.tsi = static_cast<vrt_tsi>(4);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), VRT_ERR_INVALID_TSI);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, false), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {}, true);
}

TEST_F(WriteIfContextTest, FormattedGpsGeolocationTsf) {
    c_.has.formatted_gps_geolocation = true;
    c_.formatted_gps_geolocation.tsf = VRT_TSF_FREE_RUNNING_COUNT;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, false), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {1}, true);
    ASSERT_EQ(Hex(buf_[1]), Hex(0x03000000));
}

TEST_F(WriteIfContextTest, FormattedGpsGeolocationTsfInvalidSmall) {
    c_.has.formatted_gps_geolocation = true;
    c_.formatted_gps_geolocation.tsf = static_cast<vrt_tsf>(-1);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), VRT_ERR_INVALID_TSF);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, false), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {1}, true);
    ASSERT_EQ(Hex(buf_[1]), Hex(0x03000000));
}

TEST_F(WriteIfContextTest, FormattedGpsGeolocationTsfInvalidLarge) {
    c_.has.formatted_gps_geolocation = true;
    c_.formatted_gps_geolocation.tsf = static_cast<vrt_tsf>(4);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), VRT_ERR_INVALID_TSF);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, false), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {}, true);
}

TEST_F(WriteIfContextTest, FormattedGpsGeolocationOui) {
    c_.has.formatted_gps_geolocation = true;
    c_.formatted_gps_geolocation.oui = 0x00FFFFFF;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {1}, true);
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00FFFFFF));
}

TEST_F(WriteIfContextTest, FormattedGpsGeolocationOuiInvalid) {
    c_.has.formatted_gps_geolocation = true;
    c_.formatted_gps_geolocation.oui = 0x01FFFFFF;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), VRT_ERR_BOUNDS_OUI);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, false), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {1}, true);
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00FFFFFF));
}

TEST_F(WriteIfContextTest, FormattedGpsGeolocationIntegerSecondTimestamp) {
    c_.has.formatted_gps_geolocation                      = true;
    c_.formatted_gps_geolocation.tsi                      = VRT_TSI_UTC;
    c_.formatted_gps_geolocation.integer_second_timestamp = 0xABABABAB;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {{1, 2}}, true);
    ASSERT_EQ(Hex(buf_[1]), Hex(0x04000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xABABABAB));
}

TEST_F(WriteIfContextTest, FormattedGpsGeolocationIntegerSecondTimestampInvalid) {
    c_.has.formatted_gps_geolocation                      = true;
    c_.formatted_gps_geolocation.tsi                      = VRT_TSI_UNDEFINED;
    c_.formatted_gps_geolocation.integer_second_timestamp = 0xABABABAB;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), VRT_ERR_SET_INTEGER_SECOND_TIMESTAMP);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, false), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {2}, true);
    ASSERT_EQ(Hex(buf_[2]), Hex(0xABABABAB));
}

TEST_F(WriteIfContextTest, FormattedGpsGeolocationFractionalSecondTimestamp) {
    c_.has.formatted_gps_geolocation                         = true;
    c_.formatted_gps_geolocation.tsf                         = VRT_TSF_REAL_TIME;
    c_.formatted_gps_geolocation.fractional_second_timestamp = 0x000000E8D4A50FFF;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {{1, 3, 4}}, true);
    ASSERT_EQ(Hex(buf_[1]), Hex(0x02000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0x000000E8));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xD4A50FFF));
}

TEST_F(WriteIfContextTest, FormattedGpsGeolocationFractionalSecondTimestampInvalid1) {
    c_.has.formatted_gps_geolocation                         = true;
    c_.formatted_gps_geolocation.tsf                         = VRT_TSF_UNDEFINED;
    c_.formatted_gps_geolocation.fractional_second_timestamp = 0x000000E8D4A50FFF;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), VRT_ERR_SET_FRACTIONAL_SECOND_TIMESTAMP);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, false), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {{3, 4}}, true);
    ASSERT_EQ(Hex(buf_[3]), Hex(0x000000E8));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xD4A50FFF));
}

TEST_F(WriteIfContextTest, FormattedGpsGeolocationFractionalSecondTimestampInvalid2) {
    c_.has.formatted_gps_geolocation                         = true;
    c_.formatted_gps_geolocation.tsf                         = VRT_TSF_REAL_TIME;
    c_.formatted_gps_geolocation.fractional_second_timestamp = 0x000000E8D4A51000;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), VRT_ERR_BOUNDS_REAL_TIME);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, false), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {{1, 3, 4}}, true);
    ASSERT_EQ(Hex(buf_[1]), Hex(0x02000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0x000000E8));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xD4A51000));
}

TEST_F(WriteIfContextTest, FormattedGpsGeolocationLatitude) {
    c_.has.formatted_gps_geolocation      = true;
    c_.formatted_gps_geolocation.latitude = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {}, true);
}

TEST_F(WriteIfContextTest, FormattedGpsGeolocationLongitude) {
    c_.has.formatted_gps_geolocation       = true;
    c_.formatted_gps_geolocation.longitude = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {}, true);
}

TEST_F(WriteIfContextTest, FormattedGpsGeolocationAltitude) {
    c_.has.formatted_gps_geolocation      = true;
    c_.formatted_gps_geolocation.altitude = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {}, true);
}

TEST_F(WriteIfContextTest, FormattedGpsGeolocationSpeedOverGround) {
    c_.has.formatted_gps_geolocation               = true;
    c_.formatted_gps_geolocation.speed_over_ground = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {}, true);
}

TEST_F(WriteIfContextTest, FormattedGpsGeolocationHeadingAngle) {
    c_.has.formatted_gps_geolocation           = true;
    c_.formatted_gps_geolocation.heading_angle = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {}, true);
}

TEST_F(WriteIfContextTest, FormattedGpsGeolocationTrackAngle) {
    c_.has.formatted_gps_geolocation         = true;
    c_.formatted_gps_geolocation.track_angle = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {}, true);
}

TEST_F(WriteIfContextTest, FormattedGpsGeolocationMagneticVariation) {
    c_.has.formatted_gps_geolocation                = true;
    c_.formatted_gps_geolocation.magnetic_variation = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {}, true);
}

TEST_F(WriteIfContextTest, FormattedGpsGeolocationBothLatitude) {
    c_.has.formatted_gps_geolocation          = true;
    c_.formatted_gps_geolocation.has.latitude = true;
    c_.formatted_gps_geolocation.latitude     = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {5}, true);
    ASSERT_EQ(Hex(buf_[5]), Hex(0x00400000));
}

TEST_F(WriteIfContextTest, FormattedGpsGeolocationBothLatitudeSmall) {
    c_.has.formatted_gps_geolocation          = true;
    c_.formatted_gps_geolocation.has.latitude = true;
    c_.formatted_gps_geolocation.latitude     = -90.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {5}, true);
    ASSERT_EQ(Hex(buf_[5]), Hex(0xE9800000));
}

TEST_F(WriteIfContextTest, FormattedGpsGeolocationBothLatitudeLarge) {
    c_.has.formatted_gps_geolocation          = true;
    c_.formatted_gps_geolocation.has.latitude = true;
    c_.formatted_gps_geolocation.latitude     = 90.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {5}, true);
    ASSERT_EQ(Hex(buf_[5]), Hex(0x16800000));
}

TEST_F(WriteIfContextTest, FormattedGpsGeolocationBothLatitudeInvalidSmall) {
    c_.has.formatted_gps_geolocation          = true;
    c_.formatted_gps_geolocation.has.latitude = true;
    c_.formatted_gps_geolocation.latitude     = -91.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), VRT_ERR_BOUNDS_LATITUDE);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, false), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {5}, true);
    ASSERT_EQ(Hex(buf_[5]), Hex(0xE9400000));
}

TEST_F(WriteIfContextTest, FormattedGpsGeolocationBothLatitudeInvalidLarge) {
    c_.has.formatted_gps_geolocation          = true;
    c_.formatted_gps_geolocation.has.latitude = true;
    c_.formatted_gps_geolocation.latitude     = 91.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), VRT_ERR_BOUNDS_LATITUDE);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, false), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {5}, true);
    ASSERT_EQ(Hex(buf_[5]), Hex(0x16C00000));
}

TEST_F(WriteIfContextTest, FormattedGpsGeolocationBothLongitude) {
    c_.has.formatted_gps_geolocation           = true;
    c_.formatted_gps_geolocation.has.longitude = true;
    c_.formatted_gps_geolocation.longitude     = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {6}, true);
    ASSERT_EQ(Hex(buf_[6]), Hex(0x00400000));
}

TEST_F(WriteIfContextTest, FormattedGpsGeolocationBothLongitudeSmall) {
    c_.has.formatted_gps_geolocation           = true;
    c_.formatted_gps_geolocation.has.longitude = true;
    c_.formatted_gps_geolocation.longitude     = -180.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {6}, true);
    ASSERT_EQ(Hex(buf_[6]), Hex(0xD3000000));
}

TEST_F(WriteIfContextTest, FormattedGpsGeolocationBothLongitudeLarge) {
    c_.has.formatted_gps_geolocation           = true;
    c_.formatted_gps_geolocation.has.longitude = true;
    c_.formatted_gps_geolocation.longitude     = 180.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {6}, true);
    ASSERT_EQ(Hex(buf_[6]), Hex(0x2D000000));
}

TEST_F(WriteIfContextTest, FormattedGpsGeolocationBothLongitudeInvalidSmall) {
    c_.has.formatted_gps_geolocation           = true;
    c_.formatted_gps_geolocation.has.longitude = true;
    c_.formatted_gps_geolocation.longitude     = -181.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), VRT_ERR_BOUNDS_LONGITUDE);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, false), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {6}, true);
    ASSERT_EQ(Hex(buf_[6]), Hex(0xD2C00000));
}

TEST_F(WriteIfContextTest, FormattedGpsGeolocationBothLongitudeInvalidLarge) {
    c_.has.formatted_gps_geolocation           = true;
    c_.formatted_gps_geolocation.has.longitude = true;
    c_.formatted_gps_geolocation.longitude     = 181.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), VRT_ERR_BOUNDS_LONGITUDE);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, false), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {6}, true);
    ASSERT_EQ(Hex(buf_[6]), Hex(0x2D400000));
}

TEST_F(WriteIfContextTest, FormattedGpsGeolocationBothAltitude) {
    c_.has.formatted_gps_geolocation          = true;
    c_.formatted_gps_geolocation.has.altitude = true;
    c_.formatted_gps_geolocation.altitude     = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {7}, true);
    ASSERT_EQ(Hex(buf_[7]), Hex(0x00000020));
}

TEST_F(WriteIfContextTest, FormattedGpsGeolocationBothAltitudeSmall) {
    c_.has.formatted_gps_geolocation          = true;
    c_.formatted_gps_geolocation.has.altitude = true;
    c_.formatted_gps_geolocation.altitude     = -67108e3;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {7}, true);
    ASSERT_EQ(Hex(buf_[7]), Hex(0x80006C00));
}

TEST_F(WriteIfContextTest, FormattedGpsGeolocationBothAltitudeLarge) {
    c_.has.formatted_gps_geolocation          = true;
    c_.formatted_gps_geolocation.has.altitude = true;
    c_.formatted_gps_geolocation.altitude     = 67108e3;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {7}, true);
    ASSERT_EQ(Hex(buf_[7]), Hex(0x7FFF9400));
}

TEST_F(WriteIfContextTest, FormattedGpsGeolocationBothAltitudeInvalidSmall) {
    c_.has.formatted_gps_geolocation          = true;
    c_.formatted_gps_geolocation.has.altitude = true;
    c_.formatted_gps_geolocation.altitude     = -67109e3;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), VRT_ERR_BOUNDS_ALTITUDE);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, false), 12);
}

TEST_F(WriteIfContextTest, FormattedGpsGeolocationBothAltitudeInvalidLarge) {
    c_.has.formatted_gps_geolocation          = true;
    c_.formatted_gps_geolocation.has.altitude = true;
    c_.formatted_gps_geolocation.altitude     = 67109e3;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), VRT_ERR_BOUNDS_ALTITUDE);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, false), 12);
}

TEST_F(WriteIfContextTest, FormattedGpsGeolocationBothSpeedOverGround) {
    c_.has.formatted_gps_geolocation                   = true;
    c_.formatted_gps_geolocation.has.speed_over_ground = true;
    c_.formatted_gps_geolocation.speed_over_ground     = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {8}, true);
    ASSERT_EQ(Hex(buf_[8]), Hex(0x00010000));
}

TEST_F(WriteIfContextTest, FormattedGpsGeolocationBothSpeedOverGroundSmall) {
    c_.has.formatted_gps_geolocation                   = true;
    c_.formatted_gps_geolocation.has.speed_over_ground = true;
    c_.formatted_gps_geolocation.speed_over_ground     = 0.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {8}, true);
    ASSERT_EQ(Hex(buf_[8]), Hex(0x00000000));
}

TEST_F(WriteIfContextTest, FormattedGpsGeolocationBothSpeedOverGroundLarge) {
    c_.has.formatted_gps_geolocation                   = true;
    c_.formatted_gps_geolocation.has.speed_over_ground = true;
    c_.formatted_gps_geolocation.speed_over_ground     = 65535.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {8}, true);
    ASSERT_EQ(Hex(buf_[8]), Hex(0xFFFF0000));
}

TEST_F(WriteIfContextTest, FormattedGpsGeolocationBothSpeedOverGroundInvalidSmall) {
    c_.has.formatted_gps_geolocation                   = true;
    c_.formatted_gps_geolocation.has.speed_over_ground = true;
    c_.formatted_gps_geolocation.speed_over_ground     = -1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), VRT_ERR_BOUNDS_SPEED_OVER_GROUND);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, false), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {8}, true);
    ASSERT_EQ(Hex(buf_[8]), Hex(0xFFFF0000));
}

TEST_F(WriteIfContextTest, FormattedGpsGeolocationBothSpeedOverGroundInvalidLarge) {
    c_.has.formatted_gps_geolocation                   = true;
    c_.formatted_gps_geolocation.has.speed_over_ground = true;
    c_.formatted_gps_geolocation.speed_over_ground     = 65536.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), VRT_ERR_BOUNDS_SPEED_OVER_GROUND);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, false), 12);
}

TEST_F(WriteIfContextTest, FormattedGpsGeolocationBothHeadingAngle) {
    c_.has.formatted_gps_geolocation               = true;
    c_.formatted_gps_geolocation.has.heading_angle = true;
    c_.formatted_gps_geolocation.heading_angle     = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {9}, true);
    ASSERT_EQ(Hex(buf_[9]), Hex(0x00400000));
}

TEST_F(WriteIfContextTest, FormattedGpsGeolocationBothHeadingAngleSmall) {
    c_.has.formatted_gps_geolocation               = true;
    c_.formatted_gps_geolocation.has.heading_angle = true;
    c_.formatted_gps_geolocation.heading_angle     = 0.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {9}, true);
    ASSERT_EQ(Hex(buf_[9]), Hex(0x00000000));
}

TEST_F(WriteIfContextTest, FormattedGpsGeolocationBothHeadingAngleLarge) {
    c_.has.formatted_gps_geolocation               = true;
    c_.formatted_gps_geolocation.has.heading_angle = true;
    c_.formatted_gps_geolocation.heading_angle     = 359.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {9}, true);
    ASSERT_EQ(Hex(buf_[9]), Hex(0x59C00000));
}

TEST_F(WriteIfContextTest, FormattedGpsGeolocationBothHeadingAngleInvalidSmall) {
    c_.has.formatted_gps_geolocation               = true;
    c_.formatted_gps_geolocation.has.heading_angle = true;
    c_.formatted_gps_geolocation.heading_angle     = -1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), VRT_ERR_BOUNDS_HEADING_ANGLE);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, false), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {9}, true);
    ASSERT_EQ(Hex(buf_[9]), Hex(0xFFC00000));
}

TEST_F(WriteIfContextTest, FormattedGpsGeolocationBothHeadingAngleInvalidLarge) {
    c_.has.formatted_gps_geolocation               = true;
    c_.formatted_gps_geolocation.has.heading_angle = true;
    c_.formatted_gps_geolocation.heading_angle     = 360.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), VRT_ERR_BOUNDS_HEADING_ANGLE);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, false), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {9}, true);
    ASSERT_EQ(Hex(buf_[9]), Hex(0x5A000000));
}

TEST_F(WriteIfContextTest, FormattedGpsGeolocationBothTrackAngle) {
    c_.has.formatted_gps_geolocation             = true;
    c_.formatted_gps_geolocation.has.track_angle = true;
    c_.formatted_gps_geolocation.track_angle     = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {10}, true);
    ASSERT_EQ(Hex(buf_[10]), Hex(0x00400000));
}

TEST_F(WriteIfContextTest, FormattedGpsGeolocationBothTrackAngleSmall) {
    c_.has.formatted_gps_geolocation             = true;
    c_.formatted_gps_geolocation.has.track_angle = true;
    c_.formatted_gps_geolocation.track_angle     = 0.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {10}, true);
    ASSERT_EQ(Hex(buf_[10]), Hex(0x00000000));
}

TEST_F(WriteIfContextTest, FormattedGpsGeolocationBothTrackAngleLarge) {
    c_.has.formatted_gps_geolocation             = true;
    c_.formatted_gps_geolocation.has.track_angle = true;
    c_.formatted_gps_geolocation.track_angle     = 359.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {10}, true);
    ASSERT_EQ(Hex(buf_[10]), Hex(0x59C00000));
}

TEST_F(WriteIfContextTest, FormattedGpsGeolocationBothTrackAngleInvalidSmall) {
    c_.has.formatted_gps_geolocation             = true;
    c_.formatted_gps_geolocation.has.track_angle = true;
    c_.formatted_gps_geolocation.track_angle     = -1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), VRT_ERR_BOUNDS_TRACK_ANGLE);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, false), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {10}, true);
    ASSERT_EQ(Hex(buf_[10]), Hex(0xFFC00000));
}

TEST_F(WriteIfContextTest, FormattedGpsGeolocationBothTrackAngleInvalidLarge) {
    c_.has.formatted_gps_geolocation             = true;
    c_.formatted_gps_geolocation.has.track_angle = true;
    c_.formatted_gps_geolocation.track_angle     = 360.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), VRT_ERR_BOUNDS_TRACK_ANGLE);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, false), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {10}, true);
    ASSERT_EQ(Hex(buf_[10]), Hex(0x5A000000));
}

TEST_F(WriteIfContextTest, FormattedGpsGeolocationBothMagneticVariation) {
    c_.has.formatted_gps_geolocation                    = true;
    c_.formatted_gps_geolocation.has.magnetic_variation = true;
    c_.formatted_gps_geolocation.magnetic_variation     = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {11}, true);
    ASSERT_EQ(Hex(buf_[11]), Hex(0x00400000));
}

TEST_F(WriteIfContextTest, FormattedGpsGeolocationBothMagneticVariationSmall) {
    c_.has.formatted_gps_geolocation                    = true;
    c_.formatted_gps_geolocation.has.magnetic_variation = true;
    c_.formatted_gps_geolocation.magnetic_variation     = -180.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {11}, true);
    ASSERT_EQ(Hex(buf_[11]), Hex(0xD3000000));
}

TEST_F(WriteIfContextTest, FormattedGpsGeolocationBothMagneticVariationLarge) {
    c_.has.formatted_gps_geolocation                    = true;
    c_.formatted_gps_geolocation.has.magnetic_variation = true;
    c_.formatted_gps_geolocation.magnetic_variation     = 180.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {11}, true);
    ASSERT_EQ(Hex(buf_[11]), Hex(0x2D000000));
}

TEST_F(WriteIfContextTest, FormattedGpsGeolocationBothMagneticVariationInvalidSmall) {
    c_.has.formatted_gps_geolocation                    = true;
    c_.formatted_gps_geolocation.has.magnetic_variation = true;
    c_.formatted_gps_geolocation.magnetic_variation     = -181.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), VRT_ERR_BOUNDS_MAGNETIC_VARIATION);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, false), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {11}, true);
    ASSERT_EQ(Hex(buf_[11]), Hex(0xD2C00000));
}

TEST_F(WriteIfContextTest, FormattedGpsGeolocationBothMagneticVariationInvalidLarge) {
    c_.has.formatted_gps_geolocation                    = true;
    c_.formatted_gps_geolocation.has.magnetic_variation = true;
    c_.formatted_gps_geolocation.magnetic_variation     = 181.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), VRT_ERR_BOUNDS_MAGNETIC_VARIATION);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, false), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00004000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {11}, true);
    ASSERT_EQ(Hex(buf_[11]), Hex(0x2D400000));
}

TEST_F(WriteIfContextTest, FormattedInsGeolocationTsi) {
    c_.has.formatted_ins_geolocation = true;
    c_.formatted_ins_geolocation.tsi = VRT_TSI_OTHER;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {1}, true);
    ASSERT_EQ(Hex(buf_[1]), Hex(0x0C000000));
}

TEST_F(WriteIfContextTest, FormattedInsGeolocationTsiInvalidSmall) {
    c_.has.formatted_ins_geolocation = true;
    c_.formatted_ins_geolocation.tsi = static_cast<vrt_tsi>(-1);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), VRT_ERR_INVALID_TSI);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, false), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {1}, true);
    ASSERT_EQ(Hex(buf_[1]), Hex(0x0C000000));
}

TEST_F(WriteIfContextTest, FormattedInsGeolocationTsiInvalidLarge) {
    c_.has.formatted_ins_geolocation = true;
    c_.formatted_ins_geolocation.tsi = static_cast<vrt_tsi>(4);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), VRT_ERR_INVALID_TSI);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, false), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {}, true);
}

TEST_F(WriteIfContextTest, FormattedInsGeolocationTsf) {
    c_.has.formatted_ins_geolocation = true;
    c_.formatted_ins_geolocation.tsf = VRT_TSF_FREE_RUNNING_COUNT;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, false), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {1}, true);
    ASSERT_EQ(Hex(buf_[1]), Hex(0x03000000));
}

TEST_F(WriteIfContextTest, FormattedInsGeolocationTsfInvalidSmall) {
    c_.has.formatted_ins_geolocation = true;
    c_.formatted_ins_geolocation.tsf = static_cast<vrt_tsf>(-1);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), VRT_ERR_INVALID_TSF);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, false), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {1}, true);
    ASSERT_EQ(Hex(buf_[1]), Hex(0x03000000));
}

TEST_F(WriteIfContextTest, FormattedInsGeolocationTsfInvalidLarge) {
    c_.has.formatted_ins_geolocation = true;
    c_.formatted_ins_geolocation.tsf = static_cast<vrt_tsf>(4);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), VRT_ERR_INVALID_TSF);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, false), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {}, true);
}

TEST_F(WriteIfContextTest, FormattedInsGeolocationOui) {
    c_.has.formatted_ins_geolocation = true;
    c_.formatted_ins_geolocation.oui = 0x00FFFFFF;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {1}, true);
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00FFFFFF));
}

TEST_F(WriteIfContextTest, FormattedInsGeolocationOuiInvalid) {
    c_.has.formatted_ins_geolocation = true;
    c_.formatted_ins_geolocation.oui = 0x01FFFFFF;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), VRT_ERR_BOUNDS_OUI);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, false), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {1}, true);
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00FFFFFF));
}

TEST_F(WriteIfContextTest, FormattedInsGeolocationIntegerSecondTimestamp) {
    c_.has.formatted_ins_geolocation                      = true;
    c_.formatted_ins_geolocation.tsi                      = VRT_TSI_UTC;
    c_.formatted_ins_geolocation.integer_second_timestamp = 0xABABABAB;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {{1, 2}}, true);
    ASSERT_EQ(Hex(buf_[1]), Hex(0x04000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xABABABAB));
}

TEST_F(WriteIfContextTest, FormattedInsGeolocationIntegerSecondTimestampInvalid) {
    c_.has.formatted_ins_geolocation                      = true;
    c_.formatted_ins_geolocation.tsi                      = VRT_TSI_UNDEFINED;
    c_.formatted_ins_geolocation.integer_second_timestamp = 0xABABABAB;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), VRT_ERR_SET_INTEGER_SECOND_TIMESTAMP);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, false), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {2}, true);
    ASSERT_EQ(Hex(buf_[2]), Hex(0xABABABAB));
}

TEST_F(WriteIfContextTest, FormattedInsGeolocationFractionalSecondTimestamp) {
    c_.has.formatted_ins_geolocation                         = true;
    c_.formatted_ins_geolocation.tsf                         = VRT_TSF_REAL_TIME;
    c_.formatted_ins_geolocation.fractional_second_timestamp = 0x000000E8D4A50FFF;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {{1, 3, 4}}, true);
    ASSERT_EQ(Hex(buf_[1]), Hex(0x02000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0x000000E8));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xD4A50FFF));
}

TEST_F(WriteIfContextTest, FormattedInsGeolocationFractionalSecondTimestampInvalid1) {
    c_.has.formatted_ins_geolocation                         = true;
    c_.formatted_ins_geolocation.tsf                         = VRT_TSF_UNDEFINED;
    c_.formatted_ins_geolocation.fractional_second_timestamp = 0x000000E8D4A50FFF;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), VRT_ERR_SET_FRACTIONAL_SECOND_TIMESTAMP);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, false), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {{3, 4}}, true);
    ASSERT_EQ(Hex(buf_[3]), Hex(0x000000E8));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xD4A50FFF));
}

TEST_F(WriteIfContextTest, FormattedInsGeolocationFractionalSecondTimestampInvalid2) {
    c_.has.formatted_ins_geolocation                         = true;
    c_.formatted_ins_geolocation.tsf                         = VRT_TSF_REAL_TIME;
    c_.formatted_ins_geolocation.fractional_second_timestamp = 0x000000E8D4A51000;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), VRT_ERR_BOUNDS_REAL_TIME);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, false), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {{1, 3, 4}}, true);
    ASSERT_EQ(Hex(buf_[1]), Hex(0x02000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0x000000E8));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xD4A51000));
}

TEST_F(WriteIfContextTest, FormattedInsGeolocationLatitude) {
    c_.has.formatted_ins_geolocation      = true;
    c_.formatted_ins_geolocation.latitude = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {}, true);
}

TEST_F(WriteIfContextTest, FormattedInsGeolocationLongitude) {
    c_.has.formatted_ins_geolocation       = true;
    c_.formatted_ins_geolocation.longitude = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {}, true);
}

TEST_F(WriteIfContextTest, FormattedInsGeolocationAltitude) {
    c_.has.formatted_ins_geolocation      = true;
    c_.formatted_ins_geolocation.altitude = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {}, true);
}

TEST_F(WriteIfContextTest, FormattedInsGeolocationSpeedOverGround) {
    c_.has.formatted_ins_geolocation               = true;
    c_.formatted_ins_geolocation.speed_over_ground = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {}, true);
}

TEST_F(WriteIfContextTest, FormattedInsGeolocationHeadingAngle) {
    c_.has.formatted_ins_geolocation           = true;
    c_.formatted_ins_geolocation.heading_angle = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {}, true);
}

TEST_F(WriteIfContextTest, FormattedInsGeolocationTrackAngle) {
    c_.has.formatted_ins_geolocation         = true;
    c_.formatted_ins_geolocation.track_angle = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {}, true);
}

TEST_F(WriteIfContextTest, FormattedInsGeolocationMagneticVariation) {
    c_.has.formatted_ins_geolocation                = true;
    c_.formatted_ins_geolocation.magnetic_variation = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {}, true);
}

TEST_F(WriteIfContextTest, FormattedInsGeolocationBothLatitude) {
    c_.has.formatted_ins_geolocation          = true;
    c_.formatted_ins_geolocation.has.latitude = true;
    c_.formatted_ins_geolocation.latitude     = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {5}, true);
    ASSERT_EQ(Hex(buf_[5]), Hex(0x00400000));
}

TEST_F(WriteIfContextTest, FormattedInsGeolocationBothLatitudeSmall) {
    c_.has.formatted_ins_geolocation          = true;
    c_.formatted_ins_geolocation.has.latitude = true;
    c_.formatted_ins_geolocation.latitude     = -90.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {5}, true);
    ASSERT_EQ(Hex(buf_[5]), Hex(0xE9800000));
}

TEST_F(WriteIfContextTest, FormattedInsGeolocationBothLatitudeLarge) {
    c_.has.formatted_ins_geolocation          = true;
    c_.formatted_ins_geolocation.has.latitude = true;
    c_.formatted_ins_geolocation.latitude     = 90.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {5}, true);
    ASSERT_EQ(Hex(buf_[5]), Hex(0x16800000));
}

TEST_F(WriteIfContextTest, FormattedInsGeolocationBothLatitudeInvalidSmall) {
    c_.has.formatted_ins_geolocation          = true;
    c_.formatted_ins_geolocation.has.latitude = true;
    c_.formatted_ins_geolocation.latitude     = -91.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), VRT_ERR_BOUNDS_LATITUDE);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, false), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {5}, true);
    ASSERT_EQ(Hex(buf_[5]), Hex(0xE9400000));
}

TEST_F(WriteIfContextTest, FormattedInsGeolocationBothLatitudeInvalidLarge) {
    c_.has.formatted_ins_geolocation          = true;
    c_.formatted_ins_geolocation.has.latitude = true;
    c_.formatted_ins_geolocation.latitude     = 91.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), VRT_ERR_BOUNDS_LATITUDE);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, false), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {5}, true);
    ASSERT_EQ(Hex(buf_[5]), Hex(0x16C00000));
}

TEST_F(WriteIfContextTest, FormattedInsGeolocationBothLongitude) {
    c_.has.formatted_ins_geolocation           = true;
    c_.formatted_ins_geolocation.has.longitude = true;
    c_.formatted_ins_geolocation.longitude     = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {6}, true);
    ASSERT_EQ(Hex(buf_[6]), Hex(0x00400000));
}

TEST_F(WriteIfContextTest, FormattedInsGeolocationBothLongitudeSmall) {
    c_.has.formatted_ins_geolocation           = true;
    c_.formatted_ins_geolocation.has.longitude = true;
    c_.formatted_ins_geolocation.longitude     = -180.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {6}, true);
    ASSERT_EQ(Hex(buf_[6]), Hex(0xD3000000));
}

TEST_F(WriteIfContextTest, FormattedInsGeolocationBothLongitudeLarge) {
    c_.has.formatted_ins_geolocation           = true;
    c_.formatted_ins_geolocation.has.longitude = true;
    c_.formatted_ins_geolocation.longitude     = 180.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {6}, true);
    ASSERT_EQ(Hex(buf_[6]), Hex(0x2D000000));
}

TEST_F(WriteIfContextTest, FormattedInsGeolocationBothLongitudeInvalidSmall) {
    c_.has.formatted_ins_geolocation           = true;
    c_.formatted_ins_geolocation.has.longitude = true;
    c_.formatted_ins_geolocation.longitude     = -181.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), VRT_ERR_BOUNDS_LONGITUDE);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, false), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {6}, true);
    ASSERT_EQ(Hex(buf_[6]), Hex(0xD2C00000));
}

TEST_F(WriteIfContextTest, FormattedInsGeolocationBothLongitudeInvalidLarge) {
    c_.has.formatted_ins_geolocation           = true;
    c_.formatted_ins_geolocation.has.longitude = true;
    c_.formatted_ins_geolocation.longitude     = 181.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), VRT_ERR_BOUNDS_LONGITUDE);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, false), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {6}, true);
    ASSERT_EQ(Hex(buf_[6]), Hex(0x2D400000));
}

TEST_F(WriteIfContextTest, FormattedInsGeolocationBothAltitude) {
    c_.has.formatted_ins_geolocation          = true;
    c_.formatted_ins_geolocation.has.altitude = true;
    c_.formatted_ins_geolocation.altitude     = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {7}, true);
    ASSERT_EQ(Hex(buf_[7]), Hex(0x00000020));
}

TEST_F(WriteIfContextTest, FormattedInsGeolocationBothAltitudeSmall) {
    c_.has.formatted_ins_geolocation          = true;
    c_.formatted_ins_geolocation.has.altitude = true;
    c_.formatted_ins_geolocation.altitude     = -67108e3;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {7}, true);
    ASSERT_EQ(Hex(buf_[7]), Hex(0x80006C00));
}

TEST_F(WriteIfContextTest, FormattedInsGeolocationBothAltitudeLarge) {
    c_.has.formatted_ins_geolocation          = true;
    c_.formatted_ins_geolocation.has.altitude = true;
    c_.formatted_ins_geolocation.altitude     = 67108e3;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {7}, true);
    ASSERT_EQ(Hex(buf_[7]), Hex(0x7FFF9400));
}

TEST_F(WriteIfContextTest, FormattedInsGeolocationBothAltitudeInvalidSmall) {
    c_.has.formatted_ins_geolocation          = true;
    c_.formatted_ins_geolocation.has.altitude = true;
    c_.formatted_ins_geolocation.altitude     = -67109e3;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), VRT_ERR_BOUNDS_ALTITUDE);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, false), 12);
}

TEST_F(WriteIfContextTest, FormattedInsGeolocationBothAltitudeInvalidLarge) {
    c_.has.formatted_ins_geolocation          = true;
    c_.formatted_ins_geolocation.has.altitude = true;
    c_.formatted_ins_geolocation.altitude     = 67109e3;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), VRT_ERR_BOUNDS_ALTITUDE);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, false), 12);
}

TEST_F(WriteIfContextTest, FormattedInsGeolocationBothSpeedOverGround) {
    c_.has.formatted_ins_geolocation                   = true;
    c_.formatted_ins_geolocation.has.speed_over_ground = true;
    c_.formatted_ins_geolocation.speed_over_ground     = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {8}, true);
    ASSERT_EQ(Hex(buf_[8]), Hex(0x00010000));
}

TEST_F(WriteIfContextTest, FormattedInsGeolocationBothSpeedOverGroundSmall) {
    c_.has.formatted_ins_geolocation                   = true;
    c_.formatted_ins_geolocation.has.speed_over_ground = true;
    c_.formatted_ins_geolocation.speed_over_ground     = 0.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {8}, true);
    ASSERT_EQ(Hex(buf_[8]), Hex(0x00000000));
}

TEST_F(WriteIfContextTest, FormattedInsGeolocationBothSpeedOverGroundLarge) {
    c_.has.formatted_ins_geolocation                   = true;
    c_.formatted_ins_geolocation.has.speed_over_ground = true;
    c_.formatted_ins_geolocation.speed_over_ground     = 65535.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {8}, true);
    ASSERT_EQ(Hex(buf_[8]), Hex(0xFFFF0000));
}

TEST_F(WriteIfContextTest, FormattedInsGeolocationBothSpeedOverGroundInvalidSmall) {
    c_.has.formatted_ins_geolocation                   = true;
    c_.formatted_ins_geolocation.has.speed_over_ground = true;
    c_.formatted_ins_geolocation.speed_over_ground     = -1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), VRT_ERR_BOUNDS_SPEED_OVER_GROUND);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, false), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {8}, true);
    ASSERT_EQ(Hex(buf_[8]), Hex(0xFFFF0000));
}

TEST_F(WriteIfContextTest, FormattedInsGeolocationBothSpeedOverGroundInvalidLarge) {
    c_.has.formatted_ins_geolocation                   = true;
    c_.formatted_ins_geolocation.has.speed_over_ground = true;
    c_.formatted_ins_geolocation.speed_over_ground     = 65536.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), VRT_ERR_BOUNDS_SPEED_OVER_GROUND);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, false), 12);
}

TEST_F(WriteIfContextTest, FormattedInsGeolocationBothHeadingAngle) {
    c_.has.formatted_ins_geolocation               = true;
    c_.formatted_ins_geolocation.has.heading_angle = true;
    c_.formatted_ins_geolocation.heading_angle     = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {9}, true);
    ASSERT_EQ(Hex(buf_[9]), Hex(0x00400000));
}

TEST_F(WriteIfContextTest, FormattedInsGeolocationBothHeadingAngleSmall) {
    c_.has.formatted_ins_geolocation               = true;
    c_.formatted_ins_geolocation.has.heading_angle = true;
    c_.formatted_ins_geolocation.heading_angle     = 0.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {9}, true);
    ASSERT_EQ(Hex(buf_[9]), Hex(0x00000000));
}

TEST_F(WriteIfContextTest, FormattedInsGeolocationBothHeadingAngleLarge) {
    c_.has.formatted_ins_geolocation               = true;
    c_.formatted_ins_geolocation.has.heading_angle = true;
    c_.formatted_ins_geolocation.heading_angle     = 359.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {9}, true);
    ASSERT_EQ(Hex(buf_[9]), Hex(0x59C00000));
}

TEST_F(WriteIfContextTest, FormattedInsGeolocationBothHeadingAngleInvalidSmall) {
    c_.has.formatted_ins_geolocation               = true;
    c_.formatted_ins_geolocation.has.heading_angle = true;
    c_.formatted_ins_geolocation.heading_angle     = -1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), VRT_ERR_BOUNDS_HEADING_ANGLE);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, false), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {9}, true);
    ASSERT_EQ(Hex(buf_[9]), Hex(0xFFC00000));
}

TEST_F(WriteIfContextTest, FormattedInsGeolocationBothHeadingAngleInvalidLarge) {
    c_.has.formatted_ins_geolocation               = true;
    c_.formatted_ins_geolocation.has.heading_angle = true;
    c_.formatted_ins_geolocation.heading_angle     = 360.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), VRT_ERR_BOUNDS_HEADING_ANGLE);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, false), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {9}, true);
    ASSERT_EQ(Hex(buf_[9]), Hex(0x5A000000));
}

TEST_F(WriteIfContextTest, FormattedInsGeolocationBothTrackAngle) {
    c_.has.formatted_ins_geolocation             = true;
    c_.formatted_ins_geolocation.has.track_angle = true;
    c_.formatted_ins_geolocation.track_angle     = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {10}, true);
    ASSERT_EQ(Hex(buf_[10]), Hex(0x00400000));
}

TEST_F(WriteIfContextTest, FormattedInsGeolocationBothTrackAngleSmall) {
    c_.has.formatted_ins_geolocation             = true;
    c_.formatted_ins_geolocation.has.track_angle = true;
    c_.formatted_ins_geolocation.track_angle     = 0.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {10}, true);
    ASSERT_EQ(Hex(buf_[10]), Hex(0x00000000));
}

TEST_F(WriteIfContextTest, FormattedInsGeolocationBothTrackAngleLarge) {
    c_.has.formatted_ins_geolocation             = true;
    c_.formatted_ins_geolocation.has.track_angle = true;
    c_.formatted_ins_geolocation.track_angle     = 359.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {10}, true);
    ASSERT_EQ(Hex(buf_[10]), Hex(0x59C00000));
}

TEST_F(WriteIfContextTest, FormattedInsGeolocationBothTrackAngleInvalidSmall) {
    c_.has.formatted_ins_geolocation             = true;
    c_.formatted_ins_geolocation.has.track_angle = true;
    c_.formatted_ins_geolocation.track_angle     = -1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), VRT_ERR_BOUNDS_TRACK_ANGLE);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, false), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {10}, true);
    ASSERT_EQ(Hex(buf_[10]), Hex(0xFFC00000));
}

TEST_F(WriteIfContextTest, FormattedInsGeolocationBothTrackAngleInvalidLarge) {
    c_.has.formatted_ins_geolocation             = true;
    c_.formatted_ins_geolocation.has.track_angle = true;
    c_.formatted_ins_geolocation.track_angle     = 360.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), VRT_ERR_BOUNDS_TRACK_ANGLE);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, false), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {10}, true);
    ASSERT_EQ(Hex(buf_[10]), Hex(0x5A000000));
}

TEST_F(WriteIfContextTest, FormattedInsGeolocationBothMagneticVariation) {
    c_.has.formatted_ins_geolocation                    = true;
    c_.formatted_ins_geolocation.has.magnetic_variation = true;
    c_.formatted_ins_geolocation.magnetic_variation     = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {11}, true);
    ASSERT_EQ(Hex(buf_[11]), Hex(0x00400000));
}

TEST_F(WriteIfContextTest, FormattedInsGeolocationBothMagneticVariationSmall) {
    c_.has.formatted_ins_geolocation                    = true;
    c_.formatted_ins_geolocation.has.magnetic_variation = true;
    c_.formatted_ins_geolocation.magnetic_variation     = -180.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {11}, true);
    ASSERT_EQ(Hex(buf_[11]), Hex(0xD3000000));
}

TEST_F(WriteIfContextTest, FormattedInsGeolocationBothMagneticVariationLarge) {
    c_.has.formatted_ins_geolocation                    = true;
    c_.formatted_ins_geolocation.has.magnetic_variation = true;
    c_.formatted_ins_geolocation.magnetic_variation     = 180.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {11}, true);
    ASSERT_EQ(Hex(buf_[11]), Hex(0x2D000000));
}

TEST_F(WriteIfContextTest, FormattedInsGeolocationBothMagneticVariationInvalidSmall) {
    c_.has.formatted_ins_geolocation                    = true;
    c_.formatted_ins_geolocation.has.magnetic_variation = true;
    c_.formatted_ins_geolocation.magnetic_variation     = -181.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), VRT_ERR_BOUNDS_MAGNETIC_VARIATION);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, false), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {11}, true);
    ASSERT_EQ(Hex(buf_[11]), Hex(0xD2C00000));
}

TEST_F(WriteIfContextTest, FormattedInsGeolocationBothMagneticVariationInvalidLarge) {
    c_.has.formatted_ins_geolocation                    = true;
    c_.formatted_ins_geolocation.has.magnetic_variation = true;
    c_.formatted_ins_geolocation.magnetic_variation     = 181.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, true), VRT_ERR_BOUNDS_MAGNETIC_VARIATION);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 12, false), 12);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00002000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {11}, true);
    ASSERT_EQ(Hex(buf_[11]), Hex(0x2D400000));
}

TEST_F(WriteIfContextTest, EcefEphemerisTsi) {
    c_.has.ecef_ephemeris = true;
    c_.ecef_ephemeris.tsi = VRT_TSI_OTHER;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {1}, false);
    ASSERT_EQ(Hex(buf_[1]), Hex(0x0C000000));
}

TEST_F(WriteIfContextTest, EcefEphemerisTsiInvalidSmall) {
    c_.has.ecef_ephemeris = true;
    c_.ecef_ephemeris.tsi = static_cast<vrt_tsi>(-1);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), VRT_ERR_INVALID_TSI);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, false), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {1}, false);
    ASSERT_EQ(Hex(buf_[1]), Hex(0x0C000000));
}

TEST_F(WriteIfContextTest, EcefEphemerisTsiInvalidLarge) {
    c_.has.ecef_ephemeris = true;
    c_.ecef_ephemeris.tsi = static_cast<vrt_tsi>(4);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), VRT_ERR_INVALID_TSI);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, false), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {}, false);
}

TEST_F(WriteIfContextTest, EcefEphemerisTsf) {
    c_.has.ecef_ephemeris = true;
    c_.ecef_ephemeris.tsf = VRT_TSF_FREE_RUNNING_COUNT;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {1}, false);
    ASSERT_EQ(Hex(buf_[1]), Hex(0x03000000));
}

TEST_F(WriteIfContextTest, EcefEphemerisTsfInvalidSmall) {
    c_.has.ecef_ephemeris = true;
    c_.ecef_ephemeris.tsf = static_cast<vrt_tsf>(-1);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), VRT_ERR_INVALID_TSF);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, false), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {1}, false);
    ASSERT_EQ(Hex(buf_[1]), Hex(0x03000000));
}

TEST_F(WriteIfContextTest, EcefEphemerisTsfInvalidLarge) {
    c_.has.ecef_ephemeris = true;
    c_.ecef_ephemeris.tsf = static_cast<vrt_tsf>(4);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), VRT_ERR_INVALID_TSF);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, false), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {}, false);
}

TEST_F(WriteIfContextTest, EcefEphemerisOui) {
    c_.has.ecef_ephemeris = true;
    c_.ecef_ephemeris.oui = 0x00FFFFFF;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {1}, false);
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00FFFFFF));
}

TEST_F(WriteIfContextTest, EcefEphemerisOuiInvalid) {
    c_.has.ecef_ephemeris = true;
    c_.ecef_ephemeris.oui = 0x01FFFFFF;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), VRT_ERR_BOUNDS_OUI);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, false), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {1}, false);
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00FFFFFF));
}

TEST_F(WriteIfContextTest, EcefEphemerisIntegerSecondTimestamp) {
    c_.has.ecef_ephemeris                      = true;
    c_.ecef_ephemeris.tsi                      = VRT_TSI_UTC;
    c_.ecef_ephemeris.integer_second_timestamp = 0xABABABAB;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {1, 2}, false);
    ASSERT_EQ(Hex(buf_[1]), Hex(0x04000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xABABABAB));
}

TEST_F(WriteIfContextTest, EcefEphemerisIntegerSecondTimestampInvalid) {
    c_.has.ecef_ephemeris                      = true;
    c_.ecef_ephemeris.tsi                      = VRT_TSI_UNDEFINED;
    c_.ecef_ephemeris.integer_second_timestamp = 0xABABABAB;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), VRT_ERR_SET_INTEGER_SECOND_TIMESTAMP);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, false), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {2}, false);
    ASSERT_EQ(Hex(buf_[2]), Hex(0xABABABAB));
}

TEST_F(WriteIfContextTest, EcefEphemerisFractionalSecondTimestamp) {
    c_.has.ecef_ephemeris                         = true;
    c_.ecef_ephemeris.tsf                         = VRT_TSF_REAL_TIME;
    c_.ecef_ephemeris.fractional_second_timestamp = 0x000000E8D4A50FFF;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {1, 3, 4}, false);
    ASSERT_EQ(Hex(buf_[1]), Hex(0x02000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0x000000E8));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xD4A50FFF));
}

TEST_F(WriteIfContextTest, EcefEphemerisFractionalSecondTimestampInvalid1) {
    c_.has.ecef_ephemeris                         = true;
    c_.ecef_ephemeris.tsf                         = VRT_TSF_UNDEFINED;
    c_.ecef_ephemeris.fractional_second_timestamp = 0x000000E8D4A50FFF;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), VRT_ERR_SET_FRACTIONAL_SECOND_TIMESTAMP);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, false), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {3, 4}, false);
    ASSERT_EQ(Hex(buf_[3]), Hex(0x000000E8));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xD4A50FFF));
}

TEST_F(WriteIfContextTest, EcefEphemerisFractionalSecondTimestampInvalid2) {
    c_.has.ecef_ephemeris                         = true;
    c_.ecef_ephemeris.tsf                         = VRT_TSF_REAL_TIME;
    c_.ecef_ephemeris.fractional_second_timestamp = 0x000000E8D4A51000;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), VRT_ERR_BOUNDS_REAL_TIME);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, false), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {1, 3, 4}, false);
    ASSERT_EQ(Hex(buf_[1]), Hex(0x02000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0x000000E8));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xD4A51000));
}

TEST_F(WriteIfContextTest, EcefEphemerisPositionX) {
    c_.has.ecef_ephemeris        = true;
    c_.ecef_ephemeris.position_x = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {}, false);
}

TEST_F(WriteIfContextTest, EcefEphemerisPositionY) {
    c_.has.ecef_ephemeris        = true;
    c_.ecef_ephemeris.position_y = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {}, false);
}

TEST_F(WriteIfContextTest, EcefEphemerisPositionZ) {
    c_.has.ecef_ephemeris        = true;
    c_.ecef_ephemeris.position_z = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {}, false);
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
}

TEST_F(WriteIfContextTest, EcefEphemerisAttitudeAlpha) {
    c_.has.ecef_ephemeris            = true;
    c_.ecef_ephemeris.attitude_alpha = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {}, false);
}

TEST_F(WriteIfContextTest, EcefEphemerisAttitudeBeta) {
    c_.has.ecef_ephemeris           = true;
    c_.ecef_ephemeris.attitude_beta = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {}, false);
}

TEST_F(WriteIfContextTest, EcefEphemerisAttitudePhi) {
    c_.has.ecef_ephemeris          = true;
    c_.ecef_ephemeris.attitude_phi = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {}, false);
}

TEST_F(WriteIfContextTest, EcefEphemerisVelocityDx) {
    c_.has.ecef_ephemeris         = true;
    c_.ecef_ephemeris.velocity_dx = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {}, false);
}

TEST_F(WriteIfContextTest, EcefEphemerisVelocityDy) {
    c_.has.ecef_ephemeris         = true;
    c_.ecef_ephemeris.velocity_dy = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {}, false);
}

TEST_F(WriteIfContextTest, EcefEphemerisVelocityDz) {
    c_.has.ecef_ephemeris         = true;
    c_.ecef_ephemeris.velocity_dz = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {}, false);
}

TEST_F(WriteIfContextTest, EcefEphemerisBothPositionX) {
    c_.has.ecef_ephemeris            = true;
    c_.ecef_ephemeris.has.position_x = true;
    c_.ecef_ephemeris.position_x     = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {5}, false);
    ASSERT_EQ(Hex(buf_[5]), Hex(0x00000020));
}

TEST_F(WriteIfContextTest, EcefEphemerisBothPositionXSmall) {
    c_.has.ecef_ephemeris            = true;
    c_.ecef_ephemeris.has.position_x = true;
    c_.ecef_ephemeris.position_x     = -67108864;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {5}, false);
    ASSERT_EQ(Hex(buf_[5]), Hex(0x80000000));
}

TEST_F(WriteIfContextTest, EcefEphemerisBothPositionXLarge) {
    c_.has.ecef_ephemeris            = true;
    c_.ecef_ephemeris.has.position_x = true;
    c_.ecef_ephemeris.position_x     = 67108863;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {5}, false);
    ASSERT_EQ(Hex(buf_[5]), Hex(0x7FFFFFE0));
}

TEST_F(WriteIfContextTest, EcefEphemerisBothPositionXInvalidSmall) {
    c_.has.ecef_ephemeris            = true;
    c_.ecef_ephemeris.has.position_x = true;
    c_.ecef_ephemeris.position_x     = -67108865;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), VRT_ERR_BOUNDS_POSITION);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, false), 14);
}

TEST_F(WriteIfContextTest, EcefEphemerisBothPositionXInvalidLarge) {
    c_.has.ecef_ephemeris            = true;
    c_.ecef_ephemeris.has.position_x = true;
    c_.ecef_ephemeris.position_x     = 67108864;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), VRT_ERR_BOUNDS_POSITION);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, false), 14);
}

TEST_F(WriteIfContextTest, EcefEphemerisBothPositionY) {
    c_.has.ecef_ephemeris            = true;
    c_.ecef_ephemeris.has.position_y = true;
    c_.ecef_ephemeris.position_y     = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {6}, false);
    ASSERT_EQ(Hex(buf_[6]), Hex(0x00000020));
}

TEST_F(WriteIfContextTest, EcefEphemerisBothPositionYSmall) {
    c_.has.ecef_ephemeris            = true;
    c_.ecef_ephemeris.has.position_y = true;
    c_.ecef_ephemeris.position_y     = -67108864;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {6}, false);
    ASSERT_EQ(Hex(buf_[6]), Hex(0x80000000));
}

TEST_F(WriteIfContextTest, EcefEphemerisBothPositionYLarge) {
    c_.has.ecef_ephemeris            = true;
    c_.ecef_ephemeris.has.position_y = true;
    c_.ecef_ephemeris.position_y     = 67108863;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {6}, false);
    ASSERT_EQ(Hex(buf_[6]), Hex(0x7FFFFFE0));
}

TEST_F(WriteIfContextTest, EcefEphemerisBothPositionYInvalidSmall) {
    c_.has.ecef_ephemeris            = true;
    c_.ecef_ephemeris.has.position_y = true;
    c_.ecef_ephemeris.position_y     = -67108865;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), VRT_ERR_BOUNDS_POSITION);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, false), 14);
}

TEST_F(WriteIfContextTest, EcefEphemerisBothPositionYInvalidLarge) {
    c_.has.ecef_ephemeris            = true;
    c_.ecef_ephemeris.has.position_y = true;
    c_.ecef_ephemeris.position_y     = 67108864;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), VRT_ERR_BOUNDS_POSITION);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, false), 14);
}

TEST_F(WriteIfContextTest, EcefEphemerisBothPositionZ) {
    c_.has.ecef_ephemeris            = true;
    c_.ecef_ephemeris.has.position_z = true;
    c_.ecef_ephemeris.position_z     = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {7}, false);
    ASSERT_EQ(Hex(buf_[7]), Hex(0x00000020));
}

TEST_F(WriteIfContextTest, EcefEphemerisBothPositionZSmall) {
    c_.has.ecef_ephemeris            = true;
    c_.ecef_ephemeris.has.position_z = true;
    c_.ecef_ephemeris.position_z     = -67108864;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {7}, false);
    ASSERT_EQ(Hex(buf_[7]), Hex(0x80000000));
}

TEST_F(WriteIfContextTest, EcefEphemerisBothPositionZLarge) {
    c_.has.ecef_ephemeris            = true;
    c_.ecef_ephemeris.has.position_z = true;
    c_.ecef_ephemeris.position_z     = 67108863;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {7}, false);
    ASSERT_EQ(Hex(buf_[7]), Hex(0x7FFFFFE0));
}

TEST_F(WriteIfContextTest, EcefEphemerisBothPositionZInvalidSmall) {
    c_.has.ecef_ephemeris            = true;
    c_.ecef_ephemeris.has.position_z = true;
    c_.ecef_ephemeris.position_z     = -67108865;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), VRT_ERR_BOUNDS_POSITION);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, false), 14);
}

TEST_F(WriteIfContextTest, EcefEphemerisBothPositionZInvalidLarge) {
    c_.has.ecef_ephemeris            = true;
    c_.ecef_ephemeris.has.position_z = true;
    c_.ecef_ephemeris.position_z     = 67108864;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), VRT_ERR_BOUNDS_POSITION);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, false), 14);
}

TEST_F(WriteIfContextTest, EcefEphemerisBothAttitudeAlpha) {
    c_.has.ecef_ephemeris                = true;
    c_.ecef_ephemeris.has.attitude_alpha = true;
    c_.ecef_ephemeris.attitude_alpha     = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {8}, false);
    ASSERT_EQ(Hex(buf_[8]), Hex(0x00400000));
}

TEST_F(WriteIfContextTest, EcefEphemerisBothAttitudeAlphaSmall) {
    c_.has.ecef_ephemeris                = true;
    c_.ecef_ephemeris.has.attitude_alpha = true;
    c_.ecef_ephemeris.attitude_alpha     = -512;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {8}, false);
    ASSERT_EQ(Hex(buf_[8]), Hex(0x80000000));
}

TEST_F(WriteIfContextTest, EcefEphemerisBothAttitudeAlphaLarge) {
    c_.has.ecef_ephemeris                = true;
    c_.ecef_ephemeris.has.attitude_alpha = true;
    c_.ecef_ephemeris.attitude_alpha     = 511;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {8}, false);
    ASSERT_EQ(Hex(buf_[8]), Hex(0x7FC00000));
}

TEST_F(WriteIfContextTest, EcefEphemerisBothAttitudeAlphaInvalidSmall) {
    c_.has.ecef_ephemeris                = true;
    c_.ecef_ephemeris.has.attitude_alpha = true;
    c_.ecef_ephemeris.attitude_alpha     = -513;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), VRT_ERR_BOUNDS_ATTITUDE);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, false), 14);
}

TEST_F(WriteIfContextTest, EcefEphemerisBothAttitudeAlphaInvalidLarge) {
    c_.has.ecef_ephemeris                = true;
    c_.ecef_ephemeris.has.attitude_alpha = true;
    c_.ecef_ephemeris.attitude_alpha     = 512;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), VRT_ERR_BOUNDS_ATTITUDE);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, false), 14);
}

TEST_F(WriteIfContextTest, EcefEphemerisBothAttitudeBeta) {
    c_.has.ecef_ephemeris               = true;
    c_.ecef_ephemeris.has.attitude_beta = true;
    c_.ecef_ephemeris.attitude_beta     = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {9}, false);
    ASSERT_EQ(Hex(buf_[9]), Hex(0x00400000));
}

TEST_F(WriteIfContextTest, EcefEphemerisBothAttitudeBetaSmall) {
    c_.has.ecef_ephemeris               = true;
    c_.ecef_ephemeris.has.attitude_beta = true;
    c_.ecef_ephemeris.attitude_beta     = -512;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {9}, false);
    ASSERT_EQ(Hex(buf_[9]), Hex(0x80000000));
}

TEST_F(WriteIfContextTest, EcefEphemerisBothAttitudeBetaLarge) {
    c_.has.ecef_ephemeris               = true;
    c_.ecef_ephemeris.has.attitude_beta = true;
    c_.ecef_ephemeris.attitude_beta     = 511;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {9}, false);
    ASSERT_EQ(Hex(buf_[9]), Hex(0x7FC00000));
}

TEST_F(WriteIfContextTest, EcefEphemerisBothAttitudeBetaInvalidSmall) {
    c_.has.ecef_ephemeris               = true;
    c_.ecef_ephemeris.has.attitude_beta = true;
    c_.ecef_ephemeris.attitude_beta     = -513;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), VRT_ERR_BOUNDS_ATTITUDE);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, false), 14);
}

TEST_F(WriteIfContextTest, EcefEphemerisBothAttitudeBetaInvalidLarge) {
    c_.has.ecef_ephemeris               = true;
    c_.ecef_ephemeris.has.attitude_beta = true;
    c_.ecef_ephemeris.attitude_beta     = 512;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), VRT_ERR_BOUNDS_ATTITUDE);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, false), 14);
}

TEST_F(WriteIfContextTest, EcefEphemerisBothAttitudePhi) {
    c_.has.ecef_ephemeris              = true;
    c_.ecef_ephemeris.has.attitude_phi = true;
    c_.ecef_ephemeris.attitude_phi     = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {10}, false);
    ASSERT_EQ(Hex(buf_[10]), Hex(0x00400000));
}

TEST_F(WriteIfContextTest, EcefEphemerisBothAttitudePhiSmall) {
    c_.has.ecef_ephemeris              = true;
    c_.ecef_ephemeris.has.attitude_phi = true;
    c_.ecef_ephemeris.attitude_phi     = -512;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {10}, false);
    ASSERT_EQ(Hex(buf_[10]), Hex(0x80000000));
}

TEST_F(WriteIfContextTest, EcefEphemerisBothAttitudePhiLarge) {
    c_.has.ecef_ephemeris              = true;
    c_.ecef_ephemeris.has.attitude_phi = true;
    c_.ecef_ephemeris.attitude_phi     = 511;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {10}, false);
    ASSERT_EQ(Hex(buf_[10]), Hex(0x7FC00000));
}

TEST_F(WriteIfContextTest, EcefEphemerisBothAttitudePhiInvalidSmall) {
    c_.has.ecef_ephemeris              = true;
    c_.ecef_ephemeris.has.attitude_phi = true;
    c_.ecef_ephemeris.attitude_phi     = -513;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), VRT_ERR_BOUNDS_ATTITUDE);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, false), 14);
}

TEST_F(WriteIfContextTest, EcefEphemerisBothAttitudePhiInvalidLarge) {
    c_.has.ecef_ephemeris              = true;
    c_.ecef_ephemeris.has.attitude_phi = true;
    c_.ecef_ephemeris.attitude_phi     = 512;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), VRT_ERR_BOUNDS_ATTITUDE);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, false), 14);
}

TEST_F(WriteIfContextTest, EcefEphemerisBothVelocityDx) {
    c_.has.ecef_ephemeris             = true;
    c_.ecef_ephemeris.has.velocity_dx = true;
    c_.ecef_ephemeris.velocity_dx     = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {11}, false);
    ASSERT_EQ(Hex(buf_[11]), Hex(0x00010000));
}

TEST_F(WriteIfContextTest, EcefEphemerisBothVelocityDxSmall) {
    c_.has.ecef_ephemeris             = true;
    c_.ecef_ephemeris.has.velocity_dx = true;
    c_.ecef_ephemeris.velocity_dx     = -32768;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {11}, false);
    ASSERT_EQ(Hex(buf_[11]), Hex(0x80000000));
}

TEST_F(WriteIfContextTest, EcefEphemerisBothVelocityDxLarge) {
    c_.has.ecef_ephemeris             = true;
    c_.ecef_ephemeris.has.velocity_dx = true;
    c_.ecef_ephemeris.velocity_dx     = 32767;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {11}, false);
    ASSERT_EQ(Hex(buf_[11]), Hex(0x7FFF0000));
}

TEST_F(WriteIfContextTest, EcefEphemerisBothVelocityDxInvalidSmall) {
    c_.has.ecef_ephemeris             = true;
    c_.ecef_ephemeris.has.velocity_dx = true;
    c_.ecef_ephemeris.velocity_dx     = -32769;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), VRT_ERR_BOUNDS_VELOCITY);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, false), 14);
}

TEST_F(WriteIfContextTest, EcefEphemerisBothVelocityDxInvalidLarge) {
    c_.has.ecef_ephemeris             = true;
    c_.ecef_ephemeris.has.velocity_dx = true;
    c_.ecef_ephemeris.velocity_dx     = 32768;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), VRT_ERR_BOUNDS_VELOCITY);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, false), 14);
}

TEST_F(WriteIfContextTest, EcefEphemerisBothVelocityDy) {
    c_.has.ecef_ephemeris             = true;
    c_.ecef_ephemeris.has.velocity_dy = true;
    c_.ecef_ephemeris.velocity_dy     = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {12}, false);
    ASSERT_EQ(Hex(buf_[12]), Hex(0x00010000));
}

TEST_F(WriteIfContextTest, EcefEphemerisBothVelocityDySmall) {
    c_.has.ecef_ephemeris             = true;
    c_.ecef_ephemeris.has.velocity_dy = true;
    c_.ecef_ephemeris.velocity_dy     = -32768;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {12}, false);
    ASSERT_EQ(Hex(buf_[12]), Hex(0x80000000));
}

TEST_F(WriteIfContextTest, EcefEphemerisBothVelocityDyLarge) {
    c_.has.ecef_ephemeris             = true;
    c_.ecef_ephemeris.has.velocity_dy = true;
    c_.ecef_ephemeris.velocity_dy     = 32767;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {12}, false);
    ASSERT_EQ(Hex(buf_[12]), Hex(0x7FFF0000));
}

TEST_F(WriteIfContextTest, EcefEphemerisBothVelocityDyInvalidSmall) {
    c_.has.ecef_ephemeris             = true;
    c_.ecef_ephemeris.has.velocity_dy = true;
    c_.ecef_ephemeris.velocity_dy     = -32769;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), VRT_ERR_BOUNDS_VELOCITY);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, false), 14);
}

TEST_F(WriteIfContextTest, EcefEphemerisBothVelocityDyInvalidLarge) {
    c_.has.ecef_ephemeris             = true;
    c_.ecef_ephemeris.has.velocity_dy = true;
    c_.ecef_ephemeris.velocity_dy     = 32768;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), VRT_ERR_BOUNDS_VELOCITY);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, false), 14);
}

TEST_F(WriteIfContextTest, EcefEphemerisBothVelocityDz) {
    c_.has.ecef_ephemeris             = true;
    c_.ecef_ephemeris.has.velocity_dz = true;
    c_.ecef_ephemeris.velocity_dz     = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {13}, false);
    ASSERT_EQ(Hex(buf_[13]), Hex(0x00010000));
}

TEST_F(WriteIfContextTest, EcefEphemerisBothVelocityDzSmall) {
    c_.has.ecef_ephemeris             = true;
    c_.ecef_ephemeris.has.velocity_dz = true;
    c_.ecef_ephemeris.velocity_dz     = -32768;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {13}, false);
    ASSERT_EQ(Hex(buf_[13]), Hex(0x80000000));
}

TEST_F(WriteIfContextTest, EcefEphemerisBothVelocityDzLarge) {
    c_.has.ecef_ephemeris             = true;
    c_.ecef_ephemeris.has.velocity_dz = true;
    c_.ecef_ephemeris.velocity_dz     = 32767;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00001000));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {13}, false);
    ASSERT_EQ(Hex(buf_[13]), Hex(0x7FFF0000));
}

TEST_F(WriteIfContextTest, EcefEphemerisBothVelocityDzInvalidSmall) {
    c_.has.ecef_ephemeris             = true;
    c_.ecef_ephemeris.has.velocity_dz = true;
    c_.ecef_ephemeris.velocity_dz     = -32769;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), VRT_ERR_BOUNDS_VELOCITY);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, false), 14);
}

TEST_F(WriteIfContextTest, EcefEphemerisBothVelocityDzInvalidLarge) {
    c_.has.ecef_ephemeris             = true;
    c_.ecef_ephemeris.has.velocity_dz = true;
    c_.ecef_ephemeris.velocity_dz     = 32768;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), VRT_ERR_BOUNDS_VELOCITY);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, false), 14);
}

TEST_F(WriteIfContextTest, RelativeEphemerisTsi) {
    c_.has.relative_ephemeris = true;
    c_.relative_ephemeris.tsi = VRT_TSI_OTHER;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {1}, false);
    ASSERT_EQ(Hex(buf_[1]), Hex(0x0C000000));
}

TEST_F(WriteIfContextTest, RelativeEphemerisTsiInvalidSmall) {
    c_.has.relative_ephemeris = true;
    c_.relative_ephemeris.tsi = static_cast<vrt_tsi>(-1);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), VRT_ERR_INVALID_TSI);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, false), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {1}, false);
    ASSERT_EQ(Hex(buf_[1]), Hex(0x0C000000));
}

TEST_F(WriteIfContextTest, RelativeEphemerisTsiInvalidLarge) {
    c_.has.relative_ephemeris = true;
    c_.relative_ephemeris.tsi = static_cast<vrt_tsi>(4);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), VRT_ERR_INVALID_TSI);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, false), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {}, false);
}

TEST_F(WriteIfContextTest, RelativeEphemerisTsf) {
    c_.has.relative_ephemeris = true;
    c_.relative_ephemeris.tsf = VRT_TSF_FREE_RUNNING_COUNT;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {1}, false);
    ASSERT_EQ(Hex(buf_[1]), Hex(0x03000000));
}

TEST_F(WriteIfContextTest, RelativeEphemerisTsfInvalidSmall) {
    c_.has.relative_ephemeris = true;
    c_.relative_ephemeris.tsf = static_cast<vrt_tsf>(-1);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), VRT_ERR_INVALID_TSF);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, false), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {1}, false);
    ASSERT_EQ(Hex(buf_[1]), Hex(0x03000000));
}

TEST_F(WriteIfContextTest, RelativeEphemerisTsfInvalidLarge) {
    c_.has.relative_ephemeris = true;
    c_.relative_ephemeris.tsf = static_cast<vrt_tsf>(4);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), VRT_ERR_INVALID_TSF);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, false), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {}, false);
}

TEST_F(WriteIfContextTest, RelativeEphemerisOui) {
    c_.has.relative_ephemeris = true;
    c_.relative_ephemeris.oui = 0x00FFFFFF;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {1}, false);
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00FFFFFF));
}

TEST_F(WriteIfContextTest, RelativeEphemerisOuiInvalid) {
    c_.has.relative_ephemeris = true;
    c_.relative_ephemeris.oui = 0x01FFFFFF;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), VRT_ERR_BOUNDS_OUI);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, false), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {1}, false);
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00FFFFFF));
}

TEST_F(WriteIfContextTest, RelativeEphemerisIntegerSecondTimestamp) {
    c_.has.relative_ephemeris                      = true;
    c_.relative_ephemeris.tsi                      = VRT_TSI_UTC;
    c_.relative_ephemeris.integer_second_timestamp = 0xABABABAB;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {1, 2}, false);
    ASSERT_EQ(Hex(buf_[1]), Hex(0x04000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xABABABAB));
}

TEST_F(WriteIfContextTest, RelativeEphemerisIntegerSecondTimestampInvalid) {
    c_.has.relative_ephemeris                      = true;
    c_.relative_ephemeris.tsi                      = VRT_TSI_UNDEFINED;
    c_.relative_ephemeris.integer_second_timestamp = 0xABABABAB;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), VRT_ERR_SET_INTEGER_SECOND_TIMESTAMP);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, false), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {2}, false);
    ASSERT_EQ(Hex(buf_[2]), Hex(0xABABABAB));
}

TEST_F(WriteIfContextTest, RelativeEphemerisFractionalSecondTimestamp) {
    c_.has.relative_ephemeris                         = true;
    c_.relative_ephemeris.tsf                         = VRT_TSF_REAL_TIME;
    c_.relative_ephemeris.fractional_second_timestamp = 0x000000E8D4A50FFF;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {1, 3, 4}, false);
    ASSERT_EQ(Hex(buf_[1]), Hex(0x02000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0x000000E8));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xD4A50FFF));
}

TEST_F(WriteIfContextTest, RelativeEphemerisFractionalSecondTimestampInvalid1) {
    c_.has.relative_ephemeris                         = true;
    c_.relative_ephemeris.tsf                         = VRT_TSF_UNDEFINED;
    c_.relative_ephemeris.fractional_second_timestamp = 0x000000E8D4A50FFF;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), VRT_ERR_SET_FRACTIONAL_SECOND_TIMESTAMP);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, false), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {3, 4}, false);
    ASSERT_EQ(Hex(buf_[3]), Hex(0x000000E8));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xD4A50FFF));
}

TEST_F(WriteIfContextTest, RelativeEphemerisFractionalSecondTimestampInvalid2) {
    c_.has.relative_ephemeris                         = true;
    c_.relative_ephemeris.tsf                         = VRT_TSF_REAL_TIME;
    c_.relative_ephemeris.fractional_second_timestamp = 0x000000E8D4A51000;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), VRT_ERR_BOUNDS_REAL_TIME);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, false), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {1, 3, 4}, false);
    ASSERT_EQ(Hex(buf_[1]), Hex(0x02000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0x000000E8));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xD4A51000));
}

TEST_F(WriteIfContextTest, RelativeEphemerisPositionX) {
    c_.has.relative_ephemeris        = true;
    c_.relative_ephemeris.position_x = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {}, false);
}

TEST_F(WriteIfContextTest, RelativeEphemerisPositionXSmall) {
    c_.has.relative_ephemeris        = true;
    c_.relative_ephemeris.position_x = -67108864;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {}, false);
}

TEST_F(WriteIfContextTest, RelativeEphemerisPositionY) {
    c_.has.relative_ephemeris        = true;
    c_.relative_ephemeris.position_y = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {}, false);
}

TEST_F(WriteIfContextTest, RelativeEphemerisPositionZ) {
    c_.has.relative_ephemeris        = true;
    c_.relative_ephemeris.position_z = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {}, false);
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
}

TEST_F(WriteIfContextTest, RelativeEphemerisAttitudeAlpha) {
    c_.has.relative_ephemeris            = true;
    c_.relative_ephemeris.attitude_alpha = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {}, false);
}

TEST_F(WriteIfContextTest, RelativeEphemerisAttitudeBeta) {
    c_.has.relative_ephemeris           = true;
    c_.relative_ephemeris.attitude_beta = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {}, false);
}

TEST_F(WriteIfContextTest, RelativeEphemerisAttitudePhi) {
    c_.has.relative_ephemeris          = true;
    c_.relative_ephemeris.attitude_phi = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {}, false);
}

TEST_F(WriteIfContextTest, RelativeEphemerisVelocityDx) {
    c_.has.relative_ephemeris         = true;
    c_.relative_ephemeris.velocity_dx = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {}, false);
}

TEST_F(WriteIfContextTest, RelativeEphemerisVelocityDy) {
    c_.has.relative_ephemeris         = true;
    c_.relative_ephemeris.velocity_dy = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {}, false);
}

TEST_F(WriteIfContextTest, RelativeEphemerisVelocityDz) {
    c_.has.relative_ephemeris         = true;
    c_.relative_ephemeris.velocity_dz = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {}, false);
}

TEST_F(WriteIfContextTest, RelativeEphemerisBothPositionX) {
    c_.has.relative_ephemeris            = true;
    c_.relative_ephemeris.has.position_x = true;
    c_.relative_ephemeris.position_x     = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {5}, false);
    ASSERT_EQ(Hex(buf_[5]), Hex(0x00000020));
}

TEST_F(WriteIfContextTest, RelativeEphemerisBothPositionXSmall) {
    c_.has.relative_ephemeris            = true;
    c_.relative_ephemeris.has.position_x = true;
    c_.relative_ephemeris.position_x     = -67108864;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {5}, false);
    ASSERT_EQ(Hex(buf_[5]), Hex(0x80000000));
}

TEST_F(WriteIfContextTest, RelativeEphemerisBothPositionXLarge) {
    c_.has.relative_ephemeris            = true;
    c_.relative_ephemeris.has.position_x = true;
    c_.relative_ephemeris.position_x     = 67108863;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {5}, false);
    ASSERT_EQ(Hex(buf_[5]), Hex(0x7FFFFFE0));
}

TEST_F(WriteIfContextTest, RelativeEphemerisBothPositionXInvalidSmall) {
    c_.has.relative_ephemeris            = true;
    c_.relative_ephemeris.has.position_x = true;
    c_.relative_ephemeris.position_x     = -67108865;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), VRT_ERR_BOUNDS_POSITION);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, false), 14);
}

TEST_F(WriteIfContextTest, RelativeEphemerisBothPositionXInvalidLarge) {
    c_.has.relative_ephemeris            = true;
    c_.relative_ephemeris.has.position_x = true;
    c_.relative_ephemeris.position_x     = 67108864;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), VRT_ERR_BOUNDS_POSITION);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, false), 14);
}

TEST_F(WriteIfContextTest, RelativeEphemerisBothPositionY) {
    c_.has.relative_ephemeris            = true;
    c_.relative_ephemeris.has.position_y = true;
    c_.relative_ephemeris.position_y     = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {6}, false);
    ASSERT_EQ(Hex(buf_[6]), Hex(0x00000020));
}

TEST_F(WriteIfContextTest, RelativeEphemerisBothPositionYSmall) {
    c_.has.relative_ephemeris            = true;
    c_.relative_ephemeris.has.position_y = true;
    c_.relative_ephemeris.position_y     = -67108864;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {6}, false);
    ASSERT_EQ(Hex(buf_[6]), Hex(0x80000000));
}

TEST_F(WriteIfContextTest, RelativeEphemerisBothPositionYLarge) {
    c_.has.relative_ephemeris            = true;
    c_.relative_ephemeris.has.position_y = true;
    c_.relative_ephemeris.position_y     = 67108863;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {6}, false);
    ASSERT_EQ(Hex(buf_[6]), Hex(0x7FFFFFE0));
}

TEST_F(WriteIfContextTest, RelativeEphemerisBothPositionYInvalidSmall) {
    c_.has.relative_ephemeris            = true;
    c_.relative_ephemeris.has.position_y = true;
    c_.relative_ephemeris.position_y     = -67108865;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), VRT_ERR_BOUNDS_POSITION);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, false), 14);
}

TEST_F(WriteIfContextTest, RelativeEphemerisBothPositionYInvalidLarge) {
    c_.has.relative_ephemeris            = true;
    c_.relative_ephemeris.has.position_y = true;
    c_.relative_ephemeris.position_y     = 67108864;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), VRT_ERR_BOUNDS_POSITION);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, false), 14);
}

TEST_F(WriteIfContextTest, RelativeEphemerisBothPositionZ) {
    c_.has.relative_ephemeris            = true;
    c_.relative_ephemeris.has.position_z = true;
    c_.relative_ephemeris.position_z     = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {7}, false);
    ASSERT_EQ(Hex(buf_[7]), Hex(0x00000020));
}

TEST_F(WriteIfContextTest, RelativeEphemerisBothPositionZSmall) {
    c_.has.relative_ephemeris            = true;
    c_.relative_ephemeris.has.position_z = true;
    c_.relative_ephemeris.position_z     = -67108864;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {7}, false);
    ASSERT_EQ(Hex(buf_[7]), Hex(0x80000000));
}

TEST_F(WriteIfContextTest, RelativeEphemerisBothPositionZLarge) {
    c_.has.relative_ephemeris            = true;
    c_.relative_ephemeris.has.position_z = true;
    c_.relative_ephemeris.position_z     = 67108863;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {7}, false);
    ASSERT_EQ(Hex(buf_[7]), Hex(0x7FFFFFE0));
}

TEST_F(WriteIfContextTest, RelativeEphemerisBothPositionZInvalidSmall) {
    c_.has.relative_ephemeris            = true;
    c_.relative_ephemeris.has.position_z = true;
    c_.relative_ephemeris.position_z     = -67108865;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), VRT_ERR_BOUNDS_POSITION);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, false), 14);
}

TEST_F(WriteIfContextTest, RelativeEphemerisBothPositionZInvalidLarge) {
    c_.has.relative_ephemeris            = true;
    c_.relative_ephemeris.has.position_z = true;
    c_.relative_ephemeris.position_z     = 67108864;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), VRT_ERR_BOUNDS_POSITION);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, false), 14);
}

TEST_F(WriteIfContextTest, RelativeEphemerisBothAttitudeAlpha) {
    c_.has.relative_ephemeris                = true;
    c_.relative_ephemeris.has.attitude_alpha = true;
    c_.relative_ephemeris.attitude_alpha     = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {8}, false);
    ASSERT_EQ(Hex(buf_[8]), Hex(0x00400000));
}

TEST_F(WriteIfContextTest, RelativeEphemerisBothAttitudeAlphaSmall) {
    c_.has.relative_ephemeris                = true;
    c_.relative_ephemeris.has.attitude_alpha = true;
    c_.relative_ephemeris.attitude_alpha     = -512;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {8}, false);
    ASSERT_EQ(Hex(buf_[8]), Hex(0x80000000));
}

TEST_F(WriteIfContextTest, RelativeEphemerisBothAttitudeAlphaLarge) {
    c_.has.relative_ephemeris                = true;
    c_.relative_ephemeris.has.attitude_alpha = true;
    c_.relative_ephemeris.attitude_alpha     = 511;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {8}, false);
    ASSERT_EQ(Hex(buf_[8]), Hex(0x7FC00000));
}

TEST_F(WriteIfContextTest, RelativeEphemerisBothAttitudeAlphaInvalidSmall) {
    c_.has.relative_ephemeris                = true;
    c_.relative_ephemeris.has.attitude_alpha = true;
    c_.relative_ephemeris.attitude_alpha     = -513;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), VRT_ERR_BOUNDS_ATTITUDE);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, false), 14);
}

TEST_F(WriteIfContextTest, RelativeEphemerisBothAttitudeAlphaInvalidLarge) {
    c_.has.relative_ephemeris                = true;
    c_.relative_ephemeris.has.attitude_alpha = true;
    c_.relative_ephemeris.attitude_alpha     = 512;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), VRT_ERR_BOUNDS_ATTITUDE);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, false), 14);
}

TEST_F(WriteIfContextTest, RelativeEphemerisBothAttitudeBeta) {
    c_.has.relative_ephemeris               = true;
    c_.relative_ephemeris.has.attitude_beta = true;
    c_.relative_ephemeris.attitude_beta     = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {9}, false);
    ASSERT_EQ(Hex(buf_[9]), Hex(0x00400000));
}

TEST_F(WriteIfContextTest, RelativeEphemerisBothAttitudeBetaSmall) {
    c_.has.relative_ephemeris               = true;
    c_.relative_ephemeris.has.attitude_beta = true;
    c_.relative_ephemeris.attitude_beta     = -512;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {9}, false);
    ASSERT_EQ(Hex(buf_[9]), Hex(0x80000000));
}

TEST_F(WriteIfContextTest, RelativeEphemerisBothAttitudeBetaLarge) {
    c_.has.relative_ephemeris               = true;
    c_.relative_ephemeris.has.attitude_beta = true;
    c_.relative_ephemeris.attitude_beta     = 511;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {9}, false);
    ASSERT_EQ(Hex(buf_[9]), Hex(0x7FC00000));
}

TEST_F(WriteIfContextTest, RelativeEphemerisBothAttitudeBetaInvalidSmall) {
    c_.has.relative_ephemeris               = true;
    c_.relative_ephemeris.has.attitude_beta = true;
    c_.relative_ephemeris.attitude_beta     = -513;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), VRT_ERR_BOUNDS_ATTITUDE);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, false), 14);
}

TEST_F(WriteIfContextTest, RelativeEphemerisBothAttitudeBetaInvalidLarge) {
    c_.has.relative_ephemeris               = true;
    c_.relative_ephemeris.has.attitude_beta = true;
    c_.relative_ephemeris.attitude_beta     = 512;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), VRT_ERR_BOUNDS_ATTITUDE);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, false), 14);
}

TEST_F(WriteIfContextTest, RelativeEphemerisBothAttitudePhi) {
    c_.has.relative_ephemeris              = true;
    c_.relative_ephemeris.has.attitude_phi = true;
    c_.relative_ephemeris.attitude_phi     = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {10}, false);
    ASSERT_EQ(Hex(buf_[10]), Hex(0x00400000));
}

TEST_F(WriteIfContextTest, RelativeEphemerisBothAttitudePhiSmall) {
    c_.has.relative_ephemeris              = true;
    c_.relative_ephemeris.has.attitude_phi = true;
    c_.relative_ephemeris.attitude_phi     = -512;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {10}, false);
    ASSERT_EQ(Hex(buf_[10]), Hex(0x80000000));
}

TEST_F(WriteIfContextTest, RelativeEphemerisBothAttitudePhiLarge) {
    c_.has.relative_ephemeris              = true;
    c_.relative_ephemeris.has.attitude_phi = true;
    c_.relative_ephemeris.attitude_phi     = 511;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {10}, false);
    ASSERT_EQ(Hex(buf_[10]), Hex(0x7FC00000));
}

TEST_F(WriteIfContextTest, RelativeEphemerisBothAttitudePhiInvalidSmall) {
    c_.has.relative_ephemeris              = true;
    c_.relative_ephemeris.has.attitude_phi = true;
    c_.relative_ephemeris.attitude_phi     = -513;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), VRT_ERR_BOUNDS_ATTITUDE);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, false), 14);
}

TEST_F(WriteIfContextTest, RelativeEphemerisBothAttitudePhiInvalidLarge) {
    c_.has.relative_ephemeris              = true;
    c_.relative_ephemeris.has.attitude_phi = true;
    c_.relative_ephemeris.attitude_phi     = 512;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), VRT_ERR_BOUNDS_ATTITUDE);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, false), 14);
}

TEST_F(WriteIfContextTest, RelativeEphemerisBothVelocityDx) {
    c_.has.relative_ephemeris             = true;
    c_.relative_ephemeris.has.velocity_dx = true;
    c_.relative_ephemeris.velocity_dx     = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {11}, false);
    ASSERT_EQ(Hex(buf_[11]), Hex(0x00010000));
}

TEST_F(WriteIfContextTest, RelativeEphemerisBothVelocityDxSmall) {
    c_.has.relative_ephemeris             = true;
    c_.relative_ephemeris.has.velocity_dx = true;
    c_.relative_ephemeris.velocity_dx     = -32768;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {11}, false);
    ASSERT_EQ(Hex(buf_[11]), Hex(0x80000000));
}

TEST_F(WriteIfContextTest, RelativeEphemerisBothVelocityDxLarge) {
    c_.has.relative_ephemeris             = true;
    c_.relative_ephemeris.has.velocity_dx = true;
    c_.relative_ephemeris.velocity_dx     = 32767;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {11}, false);
    ASSERT_EQ(Hex(buf_[11]), Hex(0x7FFF0000));
}

TEST_F(WriteIfContextTest, RelativeEphemerisBothVelocityDxInvalidSmall) {
    c_.has.relative_ephemeris             = true;
    c_.relative_ephemeris.has.velocity_dx = true;
    c_.relative_ephemeris.velocity_dx     = -32769;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), VRT_ERR_BOUNDS_VELOCITY);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, false), 14);
}

TEST_F(WriteIfContextTest, RelativeEphemerisBothVelocityDxInvalidLarge) {
    c_.has.relative_ephemeris             = true;
    c_.relative_ephemeris.has.velocity_dx = true;
    c_.relative_ephemeris.velocity_dx     = 32768;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), VRT_ERR_BOUNDS_VELOCITY);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, false), 14);
}

TEST_F(WriteIfContextTest, RelativeEphemerisBothVelocityDy) {
    c_.has.relative_ephemeris             = true;
    c_.relative_ephemeris.has.velocity_dy = true;
    c_.relative_ephemeris.velocity_dy     = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {12}, false);
    ASSERT_EQ(Hex(buf_[12]), Hex(0x00010000));
}

TEST_F(WriteIfContextTest, RelativeEphemerisBothVelocityDySmall) {
    c_.has.relative_ephemeris             = true;
    c_.relative_ephemeris.has.velocity_dy = true;
    c_.relative_ephemeris.velocity_dy     = -32768;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {12}, false);
    ASSERT_EQ(Hex(buf_[12]), Hex(0x80000000));
}

TEST_F(WriteIfContextTest, RelativeEphemerisBothVelocityDyLarge) {
    c_.has.relative_ephemeris             = true;
    c_.relative_ephemeris.has.velocity_dy = true;
    c_.relative_ephemeris.velocity_dy     = 32767;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {12}, false);
    ASSERT_EQ(Hex(buf_[12]), Hex(0x7FFF0000));
}

TEST_F(WriteIfContextTest, RelativeEphemerisBothVelocityDyInvalidSmall) {
    c_.has.relative_ephemeris             = true;
    c_.relative_ephemeris.has.velocity_dy = true;
    c_.relative_ephemeris.velocity_dy     = -32769;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), VRT_ERR_BOUNDS_VELOCITY);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, false), 14);
}

TEST_F(WriteIfContextTest, RelativeEphemerisBothVelocityDyInvalidLarge) {
    c_.has.relative_ephemeris             = true;
    c_.relative_ephemeris.has.velocity_dy = true;
    c_.relative_ephemeris.velocity_dy     = 32768;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), VRT_ERR_BOUNDS_VELOCITY);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, false), 14);
}

TEST_F(WriteIfContextTest, RelativeEphemerisBothVelocityDz) {
    c_.has.relative_ephemeris             = true;
    c_.relative_ephemeris.has.velocity_dz = true;
    c_.relative_ephemeris.velocity_dz     = 1.0;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {13}, false);
    ASSERT_EQ(Hex(buf_[13]), Hex(0x00010000));
}

TEST_F(WriteIfContextTest, RelativeEphemerisBothVelocityDzSmall) {
    c_.has.relative_ephemeris             = true;
    c_.relative_ephemeris.has.velocity_dz = true;
    c_.relative_ephemeris.velocity_dz     = -32768;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {13}, false);
    ASSERT_EQ(Hex(buf_[13]), Hex(0x80000000));
}

TEST_F(WriteIfContextTest, RelativeEphemerisBothVelocityDzLarge) {
    c_.has.relative_ephemeris             = true;
    c_.relative_ephemeris.has.velocity_dz = true;
    c_.relative_ephemeris.velocity_dz     = 32767;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), 14);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000800));
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    buf_cmp_geolocation_ephemeris(buf_, {13}, false);
    ASSERT_EQ(Hex(buf_[13]), Hex(0x7FFF0000));
}

TEST_F(WriteIfContextTest, RelativeEphemerisBothVelocityDzInvalidSmall) {
    c_.has.relative_ephemeris             = true;
    c_.relative_ephemeris.has.velocity_dz = true;
    c_.relative_ephemeris.velocity_dz     = -32769;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), VRT_ERR_BOUNDS_VELOCITY);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, false), 14);
}

TEST_F(WriteIfContextTest, RelativeEphemerisBothVelocityDzInvalidLarge) {
    c_.has.relative_ephemeris             = true;
    c_.relative_ephemeris.has.velocity_dz = true;
    c_.relative_ephemeris.velocity_dz     = 32768;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, true), VRT_ERR_BOUNDS_VELOCITY);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 14, false), 14);
}

TEST_F(WriteIfContextTest, EphemerisReferenceIdentifier) {
    c_.has.ephemeris_reference_identifier = true;
    c_.ephemeris_reference_identifier     = 0xFEFEFEFE;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 2, true), 2);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000400));
    ASSERT_EQ(Hex(buf_[1]), Hex(0xFEFEFEFE));
    ASSERT_EQ(Hex(buf_[2]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, GpsAsciiOui) {
    c_.has.gps_ascii = true;
    c_.gps_ascii.oui = 0x00FFFFFF;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, true), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000200));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00FFFFFF));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, GpsAsciiOuiInvalid) {
    c_.has.gps_ascii = true;
    c_.gps_ascii.oui = 0x01FFFFFF;
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, true), VRT_ERR_BOUNDS_OUI);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 3, false), 3);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000200));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00FFFFFF));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, GpsAsciiAscii) {
    c_.has.gps_ascii             = true;
    c_.gps_ascii.number_of_words = 5;
    c_.gps_ascii.ascii           = "What's your name?";
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 8, true), 8);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000200));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x00000005));
    ASSERT_EQ(std::memcmp(c_.gps_ascii.ascii, buf_.data() + 3, sizeof(uint32_t) * c_.gps_ascii.number_of_words), 0);
    ASSERT_EQ(Hex(buf_[8]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, ContextAssociationListsSource) {
    c_.has.context_association_lists                             = true;
    c_.context_association_lists.source_list_size                = 0x01FF;
    std::array<uint32_t, 0x1FF> l                                = {};
    l[0]                                                         = 0xABABABAB;
    l[0x1FE]                                                     = 0xCBCBCBCB;
    c_.context_association_lists.source_context_association_list = l.data();
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 0x202, true), 0x202);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000100));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x01FF0000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xABABABAB));
    ASSERT_EQ(Hex(buf_[0x201]), Hex(0xCBCBCBCB));
    ASSERT_EQ(Hex(buf_[0x202]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, ContextAssociationListsSourceInvalid) {
    c_.has.context_association_lists                             = true;
    c_.context_association_lists.source_list_size                = 0x0200;
    std::array<uint32_t, 0x1FF> l                                = {};
    l[0]                                                         = 0xABABABAB;
    l[0x1FE]                                                     = 0xCBCBCBCB;
    c_.context_association_lists.source_context_association_list = l.data();
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 0x202, true), VRT_ERR_BOUNDS_SOURCE_LIST_SIZE);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 0x202, false), 0x003);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000100));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, ContextAssociationListsSystem) {
    c_.has.context_association_lists                             = true;
    c_.context_association_lists.system_list_size                = 0x01FF;
    std::array<uint32_t, 0x1FF> l                                = {};
    l[0]                                                         = 0xABABABAB;
    l[0x1FE]                                                     = 0xCBCBCBCB;
    c_.context_association_lists.system_context_association_list = l.data();
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 0x202, true), 0x202);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000100));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x000001FF));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xABABABAB));
    ASSERT_EQ(Hex(buf_[0x201]), Hex(0xCBCBCBCB));
    ASSERT_EQ(Hex(buf_[0x202]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, ContextAssociationListsSystemInvalid) {
    c_.has.context_association_lists                             = true;
    c_.context_association_lists.system_list_size                = 0x0200;
    std::array<uint32_t, 0x1FF> l                                = {};
    l[0]                                                         = 0xABABABAB;
    l[0x1FE]                                                     = 0xCBCBCBCB;
    c_.context_association_lists.system_context_association_list = l.data();
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 0x202, true), VRT_ERR_BOUNDS_SYSTEM_LIST_SIZE);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 0x202, false), 0x003);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000100));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, ContextAssociationListsVectorComponent) {
    c_.has.context_association_lists                                       = true;
    c_.context_association_lists.vector_component_list_size                = 0x1FF;
    std::array<uint32_t, 0x1FF> l                                          = {};
    l[0]                                                                   = 0xABABABAB;
    l[0x1FE]                                                               = 0xCBCBCBCB;
    c_.context_association_lists.vector_component_context_association_list = l.data();
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 0x202, true), 0x202);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000100));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x01FF0000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xABABABAB));
    ASSERT_EQ(Hex(buf_[0x201]), Hex(0xCBCBCBCB));
    ASSERT_EQ(Hex(buf_[0x202]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, ContextAssociationListsAsynchronous) {
    c_.has.context_association_lists                                           = true;
    c_.context_association_lists.asynchronous_channel_list_size                = 0x1FF;
    std::array<uint32_t, 0x1FF> l                                              = {};
    l[0]                                                                       = 0xABABABAB;
    l[0x1FE]                                                                   = 0xCBCBCBCB;
    c_.context_association_lists.asynchronous_channel_context_association_list = l.data();
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 0x202, true), 0x202);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000100));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x000001FF));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xABABABAB));
    ASSERT_EQ(Hex(buf_[0x201]), Hex(0xCBCBCBCB));
    ASSERT_EQ(Hex(buf_[0x202]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, ContextAssociationListsAsynchronousInvalid) {
    c_.has.context_association_lists                                           = true;
    c_.context_association_lists.asynchronous_channel_list_size                = 0x8000;
    std::array<uint32_t, 0x1FF> l                                              = {};
    c_.context_association_lists.asynchronous_channel_context_association_list = l.data();
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 0x8000, true), VRT_ERR_BOUNDS_CHANNEL_LIST_SIZE);
}

TEST_F(WriteIfContextTest, ContextAssociationListsAsynchronousTag) {
    c_.has.context_association_lists                                           = true;
    c_.context_association_lists.asynchronous_channel_list_size                = 3;
    c_.context_association_lists.has.asynchronous_channel_tag_list             = true;
    std::array<uint32_t, 3> l1                                                 = {};
    l1[0]                                                                      = 0xABABABAB;
    l1[1]                                                                      = 0xBCBCBCBC;
    l1[2]                                                                      = 0xCDCDCDCD;
    std::array<uint32_t, 3> l2                                                 = {};
    l2[0]                                                                      = 0xABABABAB;
    l2[1]                                                                      = 0xBCBCBCBC;
    l2[2]                                                                      = 0xCDCDCDCD;
    c_.context_association_lists.asynchronous_channel_context_association_list = l1.data();
    c_.context_association_lists.asynchronous_channel_tag_list                 = l2.data();
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 9, true), 9);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x00000100));
    ASSERT_EQ(Hex(buf_[1]), Hex(0x00000000));
    ASSERT_EQ(Hex(buf_[2]), Hex(0x00008003));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xABABABAB));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xBCBCBCBC));
    ASSERT_EQ(Hex(buf_[5]), Hex(0xCDCDCDCD));
    ASSERT_EQ(Hex(buf_[6]), Hex(0xABABABAB));
    ASSERT_EQ(Hex(buf_[7]), Hex(0xBCBCBCBC));
    ASSERT_EQ(Hex(buf_[8]), Hex(0xCDCDCDCD));
    ASSERT_EQ(Hex(buf_[9]), Hex(0xBAADF00D));
}

TEST_F(WriteIfContextTest, EveryOther1) {
    c_.context_field_change_indicator = true;
    c_.has.bandwidth                  = true;
    c_.has.rf_reference_frequency     = true;
    c_.has.if_band_offset             = true;
    c_.has.gain                       = true;
    c_.has.sample_rate                = true;
    c_.has.timestamp_calibration_time = true;
    c_.has.device_identifier          = true;
    c_.has.data_packet_payload_format = true;
    c_.has.formatted_ins_geolocation  = true;
    c_.has.relative_ephemeris         = true;
    c_.has.gps_ascii                  = true;

    c_.bandwidth                     = -1.0;
    c_.rf_reference_frequency        = -1.0;
    c_.if_band_offset                = -1.0;
    c_.gain.stage1                   = -1.0F;
    c_.gain.stage2                   = -1.0F;
    c_.sample_rate                   = -1.0;
    c_.timestamp_calibration_time    = 0xBABABABA;
    c_.device_identifier.oui         = 0xFEBEFEBE;
    c_.device_identifier.device_code = 0xBEBA;

    c_.data_packet_payload_format.packing_method          = VRT_PM_LINK_EFFICIENT;
    c_.data_packet_payload_format.real_or_complex         = VRT_ROC_COMPLEX_POLAR;
    c_.data_packet_payload_format.data_item_format        = VRT_DIF_UNSIGNED_VRT_6_BIT_EXPONENT;
    c_.data_packet_payload_format.sample_component_repeat = true;
    c_.data_packet_payload_format.event_tag_size          = 0xFF;
    c_.data_packet_payload_format.channel_tag_size        = 0xFF;
    c_.data_packet_payload_format.item_packing_field_size = 0xFF;
    c_.data_packet_payload_format.data_item_size          = 0xFF;
    c_.data_packet_payload_format.repeat_count            = 0xFFFF;
    c_.data_packet_payload_format.vector_size             = 0xFFFF;

    c_.formatted_ins_geolocation.tsi                         = VRT_TSI_OTHER;
    c_.formatted_ins_geolocation.tsf                         = VRT_TSF_FREE_RUNNING_COUNT;
    c_.formatted_ins_geolocation.oui                         = 0xFFFFFFFF;
    c_.formatted_ins_geolocation.integer_second_timestamp    = 0xABABBABA;
    c_.formatted_ins_geolocation.fractional_second_timestamp = 0xBEBEBABACECECBCB;
    c_.formatted_ins_geolocation.has.latitude                = true;
    c_.formatted_ins_geolocation.has.longitude               = true;
    c_.formatted_ins_geolocation.has.altitude                = true;
    c_.formatted_ins_geolocation.has.speed_over_ground       = true;
    c_.formatted_ins_geolocation.has.heading_angle           = true;
    c_.formatted_ins_geolocation.has.track_angle             = true;
    c_.formatted_ins_geolocation.has.magnetic_variation      = true;
    c_.formatted_ins_geolocation.latitude                    = -1.0;
    c_.formatted_ins_geolocation.longitude                   = -1.0;
    c_.formatted_ins_geolocation.altitude                    = -1.0;
    c_.formatted_ins_geolocation.speed_over_ground           = -1.0;
    c_.formatted_ins_geolocation.heading_angle               = -1.0;
    c_.formatted_ins_geolocation.track_angle                 = -1.0;
    c_.formatted_ins_geolocation.magnetic_variation          = -1.0;

    c_.relative_ephemeris.tsi                         = VRT_TSI_OTHER;
    c_.relative_ephemeris.tsf                         = VRT_TSF_FREE_RUNNING_COUNT;
    c_.relative_ephemeris.oui                         = 0xFFFFFFFF;
    c_.relative_ephemeris.integer_second_timestamp    = 0xABABBABA;
    c_.relative_ephemeris.fractional_second_timestamp = 0xBEBEBABACECECBCB;
    c_.relative_ephemeris.has.position_x              = true;
    c_.relative_ephemeris.has.position_y              = true;
    c_.relative_ephemeris.has.position_z              = true;
    c_.relative_ephemeris.has.attitude_alpha          = true;
    c_.relative_ephemeris.has.attitude_beta           = true;
    c_.relative_ephemeris.has.attitude_phi            = true;
    c_.relative_ephemeris.has.velocity_dx             = true;
    c_.relative_ephemeris.has.velocity_dy             = true;
    c_.relative_ephemeris.has.velocity_dz             = true;
    c_.relative_ephemeris.position_x                  = -1.0;
    c_.relative_ephemeris.position_y                  = -1.0;
    c_.relative_ephemeris.position_z                  = -1.0;
    c_.relative_ephemeris.attitude_alpha              = -1.0;
    c_.relative_ephemeris.attitude_beta               = -1.0;
    c_.relative_ephemeris.attitude_phi                = -1.0;
    c_.relative_ephemeris.velocity_dx                 = -1.0;
    c_.relative_ephemeris.velocity_dy                 = -1.0;
    c_.relative_ephemeris.velocity_dz                 = -1.0;

    c_.gps_ascii.oui             = 0xFFFFFFFF;
    c_.gps_ascii.number_of_words = 3;
    c_.gps_ascii.ascii           = "Lorem ipsum!";

    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 43, true), VRT_ERR_BUFFER_SIZE);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 44, false), 44);
    ASSERT_EQ(Hex(buf_[0]), Hex(0xAAAAAA00)); /* Context indicators */
    ASSERT_EQ(Hex(buf_[1]), Hex(0xFFFFFFFF)); /* Bandwidth */
    ASSERT_EQ(Hex(buf_[2]), Hex(0xFFF00000));
    ASSERT_EQ(Hex(buf_[3]), Hex(0xFFFFFFFF)); /* RF reference frequency */
    ASSERT_EQ(Hex(buf_[4]), Hex(0xFFF00000));
    ASSERT_EQ(Hex(buf_[5]), Hex(0xFFFFFFFF)); /* IF band offset */
    ASSERT_EQ(Hex(buf_[6]), Hex(0xFFF00000));
    ASSERT_EQ(Hex(buf_[7]), Hex(0xFF80FF80)); /* Gain stage 1 & 2 */
    ASSERT_EQ(Hex(buf_[8]), Hex(0xFFFFFFFF)); /* Sample rate */
    ASSERT_EQ(Hex(buf_[9]), Hex(0xFFF00000));
    ASSERT_EQ(Hex(buf_[10]), Hex(0xBABABABA)); /* Timestamp calibration time */
    ASSERT_EQ(Hex(buf_[11]), Hex(0x00BEFEBE)); /* Device identifier.oui */
    ASSERT_EQ(Hex(buf_[12]), Hex(0x0000BEBA)); /* Device identifier.device_code */
    ASSERT_EQ(Hex(buf_[13]), Hex(0xD6FF0FFF)); /* Data packet payload format */
    ASSERT_EQ(Hex(buf_[14]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[15]), Hex(0x0FFFFFFF)); /* Formatted INS geolocation */
    ASSERT_EQ(Hex(buf_[16]), Hex(0xABABBABA));
    ASSERT_EQ(Hex(buf_[17]), Hex(0xBEBEBABA));
    ASSERT_EQ(Hex(buf_[18]), Hex(0xCECECBCB));
    ASSERT_EQ(Hex(buf_[19]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[20]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[21]), Hex(0xFFFFFFE0));
    ASSERT_EQ(Hex(buf_[22]), Hex(0xFFFF0000));
    ASSERT_EQ(Hex(buf_[23]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[24]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[25]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[26]), Hex(0x0FFFFFFF)); /* Relative ephemeris */
    ASSERT_EQ(Hex(buf_[27]), Hex(0xABABBABA));
    ASSERT_EQ(Hex(buf_[28]), Hex(0xBEBEBABA));
    ASSERT_EQ(Hex(buf_[29]), Hex(0xCECECBCB));
    ASSERT_EQ(Hex(buf_[30]), Hex(0xFFFFFFE0));
    ASSERT_EQ(Hex(buf_[31]), Hex(0xFFFFFFE0));
    ASSERT_EQ(Hex(buf_[32]), Hex(0xFFFFFFE0));
    ASSERT_EQ(Hex(buf_[33]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[34]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[35]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[36]), Hex(0xFFFF0000));
    ASSERT_EQ(Hex(buf_[37]), Hex(0xFFFF0000));
    ASSERT_EQ(Hex(buf_[38]), Hex(0xFFFF0000));
    ASSERT_EQ(Hex(buf_[39]), Hex(0x00FFFFFF)); /* GPS ASCII */
    ASSERT_EQ(Hex(buf_[40]), Hex(0x00000003));
    ASSERT_EQ(Hex(buf_[41]), Hex(0x65726F4C));
    ASSERT_EQ(Hex(buf_[42]), Hex(0x7069206D));
    ASSERT_EQ(Hex(buf_[43]), Hex(0x216D7573));
}

TEST_F(WriteIfContextTest, EveryOther2) {
    c_.has.reference_point_identifier     = true;
    c_.has.if_reference_frequency         = true;
    c_.has.rf_reference_frequency_offset  = true;
    c_.has.reference_level                = true;
    c_.has.over_range_count               = true;
    c_.has.timestamp_adjustment           = true;
    c_.has.temperature                    = true;
    c_.has.state_and_event_indicators     = true;
    c_.has.formatted_gps_geolocation      = true;
    c_.has.ecef_ephemeris                 = true;
    c_.has.ephemeris_reference_identifier = true;
    c_.has.context_association_lists      = true;

    c_.reference_point_identifier    = 0xACACACAC;
    c_.if_reference_frequency        = -1.0;
    c_.rf_reference_frequency_offset = -1.0;
    c_.reference_level               = -1.0F;
    c_.over_range_count              = 0xBDBDBDBD;
    c_.timestamp_adjustment          = 0xABCABCABCABCABCA;
    c_.temperature                   = -1.0F;

    c_.state_and_event_indicators.has.calibrated_time    = true;
    c_.state_and_event_indicators.has.valid_data         = true;
    c_.state_and_event_indicators.has.reference_lock     = true;
    c_.state_and_event_indicators.has.agc_or_mgc         = true;
    c_.state_and_event_indicators.has.detected_signal    = true;
    c_.state_and_event_indicators.has.spectral_inversion = true;
    c_.state_and_event_indicators.has.over_range         = true;
    c_.state_and_event_indicators.has.sample_loss        = true;
    c_.state_and_event_indicators.calibrated_time        = true;
    c_.state_and_event_indicators.valid_data             = true;
    c_.state_and_event_indicators.reference_lock         = true;
    c_.state_and_event_indicators.agc_or_mgc             = VRT_AOM_AGC;
    c_.state_and_event_indicators.detected_signal        = true;
    c_.state_and_event_indicators.spectral_inversion     = true;
    c_.state_and_event_indicators.over_range             = true;
    c_.state_and_event_indicators.sample_loss            = true;
    c_.state_and_event_indicators.user_defined           = 0xFF;

    c_.formatted_gps_geolocation.tsi                         = VRT_TSI_OTHER;
    c_.formatted_gps_geolocation.tsf                         = VRT_TSF_FREE_RUNNING_COUNT;
    c_.formatted_gps_geolocation.oui                         = 0xFFFFFFFF;
    c_.formatted_gps_geolocation.integer_second_timestamp    = 0xABABBABA;
    c_.formatted_gps_geolocation.fractional_second_timestamp = 0xBEBEBABACECECBCB;
    c_.formatted_gps_geolocation.has.latitude                = true;
    c_.formatted_gps_geolocation.has.longitude               = true;
    c_.formatted_gps_geolocation.has.altitude                = true;
    c_.formatted_gps_geolocation.has.speed_over_ground       = true;
    c_.formatted_gps_geolocation.has.heading_angle           = true;
    c_.formatted_gps_geolocation.has.track_angle             = true;
    c_.formatted_gps_geolocation.has.magnetic_variation      = true;
    c_.formatted_gps_geolocation.latitude                    = -1.0;
    c_.formatted_gps_geolocation.longitude                   = -1.0;
    c_.formatted_gps_geolocation.altitude                    = -1.0;
    c_.formatted_gps_geolocation.speed_over_ground           = -1.0;
    c_.formatted_gps_geolocation.heading_angle               = -1.0;
    c_.formatted_gps_geolocation.track_angle                 = -1.0;
    c_.formatted_gps_geolocation.magnetic_variation          = -1.0;

    c_.ecef_ephemeris.tsi                         = VRT_TSI_OTHER;
    c_.ecef_ephemeris.tsf                         = VRT_TSF_FREE_RUNNING_COUNT;
    c_.ecef_ephemeris.oui                         = 0xFFFFFFFF;
    c_.ecef_ephemeris.integer_second_timestamp    = 0xABABBABA;
    c_.ecef_ephemeris.fractional_second_timestamp = 0xBEBEBABACECECBCB;
    c_.ecef_ephemeris.has.position_x              = true;
    c_.ecef_ephemeris.has.position_y              = true;
    c_.ecef_ephemeris.has.position_z              = true;
    c_.ecef_ephemeris.has.attitude_alpha          = true;
    c_.ecef_ephemeris.has.attitude_beta           = true;
    c_.ecef_ephemeris.has.attitude_phi            = true;
    c_.ecef_ephemeris.has.velocity_dx             = true;
    c_.ecef_ephemeris.has.velocity_dy             = true;
    c_.ecef_ephemeris.has.velocity_dz             = true;
    c_.ecef_ephemeris.position_x                  = -1.0;
    c_.ecef_ephemeris.position_y                  = -1.0;
    c_.ecef_ephemeris.position_z                  = -1.0;
    c_.ecef_ephemeris.attitude_alpha              = -1.0;
    c_.ecef_ephemeris.attitude_beta               = -1.0;
    c_.ecef_ephemeris.attitude_phi                = -1.0;
    c_.ecef_ephemeris.velocity_dx                 = -1.0;
    c_.ecef_ephemeris.velocity_dy                 = -1.0;
    c_.ecef_ephemeris.velocity_dz                 = -1.0;

    c_.ephemeris_reference_identifier = 0xDEDEDEDE;

    std::array<uint32_t, 2> l1                                                 = {0x12345678, 0x23456789};
    std::array<uint32_t, 2> l2                                                 = {0x3456789A, 0x456789AB};
    std::array<uint32_t, 2> l3                                                 = {0x56789ABC, 0x6789ABCD};
    std::array<uint32_t, 2> l4                                                 = {0x789ABCDE, 0x89ABCDEF};
    std::array<uint32_t, 2> l5                                                 = {0x9ABCDEF0, 0xABCDEF01};
    c_.context_association_lists.has.asynchronous_channel_tag_list             = true;
    c_.context_association_lists.source_list_size                              = l1.size();
    c_.context_association_lists.system_list_size                              = l2.size();
    c_.context_association_lists.vector_component_list_size                    = l3.size();
    c_.context_association_lists.asynchronous_channel_list_size                = l4.size();
    c_.context_association_lists.source_context_association_list               = l1.data();
    c_.context_association_lists.system_context_association_list               = l2.data();
    c_.context_association_lists.vector_component_context_association_list     = l3.data();
    c_.context_association_lists.asynchronous_channel_context_association_list = l4.data();
    c_.context_association_lists.asynchronous_channel_tag_list                 = l5.data();

    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 48, true), VRT_ERR_BUFFER_SIZE);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 49, false), 49);
    ASSERT_EQ(Hex(buf_[0]), Hex(0x55555500)); /* Context indicators */
    ASSERT_EQ(Hex(buf_[1]), Hex(0xACACACAC)); /* Reference point identifier */
    ASSERT_EQ(Hex(buf_[2]), Hex(0xFFFFFFFF)); /* IF reference frequency */
    ASSERT_EQ(Hex(buf_[3]), Hex(0xFFF00000));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xFFFFFFFF)); /* RF reference frequency offset */
    ASSERT_EQ(Hex(buf_[5]), Hex(0xFFF00000));
    ASSERT_EQ(Hex(buf_[6]), Hex(0x0000FF80)); /* Reference level */
    ASSERT_EQ(Hex(buf_[7]), Hex(0xBDBDBDBD)); /* Over-range count */
    ASSERT_EQ(Hex(buf_[8]), Hex(0xABCABCAB)); /* Timestamp adjustment */
    ASSERT_EQ(Hex(buf_[9]), Hex(0xCABCABCA));
    ASSERT_EQ(Hex(buf_[10]), Hex(0x0000FFC0)); /* Temperature */
    ASSERT_EQ(Hex(buf_[11]), Hex(0xFF0FF0FF)); /* State and event indicators */
    ASSERT_EQ(Hex(buf_[12]), Hex(0x0FFFFFFF)); /* Formatted GPS geolocation */
    ASSERT_EQ(Hex(buf_[13]), Hex(0xABABBABA));
    ASSERT_EQ(Hex(buf_[14]), Hex(0xBEBEBABA));
    ASSERT_EQ(Hex(buf_[15]), Hex(0xCECECBCB));
    ASSERT_EQ(Hex(buf_[16]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[17]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[18]), Hex(0xFFFFFFE0));
    ASSERT_EQ(Hex(buf_[19]), Hex(0xFFFF0000));
    ASSERT_EQ(Hex(buf_[20]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[21]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[22]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[23]), Hex(0x0FFFFFFF)); /* ECEF ephemeris */
    ASSERT_EQ(Hex(buf_[24]), Hex(0xABABBABA));
    ASSERT_EQ(Hex(buf_[25]), Hex(0xBEBEBABA));
    ASSERT_EQ(Hex(buf_[26]), Hex(0xCECECBCB));
    ASSERT_EQ(Hex(buf_[27]), Hex(0xFFFFFFE0));
    ASSERT_EQ(Hex(buf_[28]), Hex(0xFFFFFFE0));
    ASSERT_EQ(Hex(buf_[29]), Hex(0xFFFFFFE0));
    ASSERT_EQ(Hex(buf_[30]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[31]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[32]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[33]), Hex(0xFFFF0000));
    ASSERT_EQ(Hex(buf_[34]), Hex(0xFFFF0000));
    ASSERT_EQ(Hex(buf_[35]), Hex(0xFFFF0000));
    ASSERT_EQ(Hex(buf_[36]), Hex(0xDEDEDEDE)); /* Ephemeris reference identifier */
    ASSERT_EQ(Hex(buf_[37]), Hex(0x00020002)); /* Context association lists */
    ASSERT_EQ(Hex(buf_[38]), Hex(0x00028002));
    ASSERT_EQ(Hex(buf_[39]), Hex(0x12345678));
    ASSERT_EQ(Hex(buf_[40]), Hex(0x23456789));
    ASSERT_EQ(Hex(buf_[41]), Hex(0x3456789A));
    ASSERT_EQ(Hex(buf_[42]), Hex(0x456789AB));
    ASSERT_EQ(Hex(buf_[43]), Hex(0x56789ABC));
    ASSERT_EQ(Hex(buf_[44]), Hex(0x6789ABCD));
    ASSERT_EQ(Hex(buf_[45]), Hex(0x789ABCDE));
    ASSERT_EQ(Hex(buf_[46]), Hex(0x89ABCDEF));
    ASSERT_EQ(Hex(buf_[47]), Hex(0x9ABCDEF0));
    ASSERT_EQ(Hex(buf_[48]), Hex(0xABCDEF01));
}

TEST_F(WriteIfContextTest, All) {
    c_.context_field_change_indicator     = true;
    c_.has.reference_point_identifier     = true;
    c_.has.bandwidth                      = true;
    c_.has.if_reference_frequency         = true;
    c_.has.rf_reference_frequency         = true;
    c_.has.rf_reference_frequency_offset  = true;
    c_.has.if_band_offset                 = true;
    c_.has.reference_level                = true;
    c_.has.gain                           = true;
    c_.has.over_range_count               = true;
    c_.has.sample_rate                    = true;
    c_.has.timestamp_adjustment           = true;
    c_.has.timestamp_calibration_time     = true;
    c_.has.temperature                    = true;
    c_.has.device_identifier              = true;
    c_.has.state_and_event_indicators     = true;
    c_.has.data_packet_payload_format     = true;
    c_.has.formatted_gps_geolocation      = true;
    c_.has.formatted_ins_geolocation      = true;
    c_.has.ecef_ephemeris                 = true;
    c_.has.relative_ephemeris             = true;
    c_.has.ephemeris_reference_identifier = true;
    c_.has.gps_ascii                      = true;
    c_.has.context_association_lists      = true;

    c_.reference_point_identifier    = 0xACACACAC;
    c_.bandwidth                     = -1.0;
    c_.if_reference_frequency        = -1.0;
    c_.rf_reference_frequency        = -1.0;
    c_.rf_reference_frequency_offset = -1.0;
    c_.if_band_offset                = -1.0;
    c_.reference_level               = -1.0F;
    c_.gain.stage1                   = -1.0F;
    c_.gain.stage2                   = -1.0F;
    c_.over_range_count              = 0xBDBDBDBD;
    c_.sample_rate                   = -1.0;
    c_.timestamp_adjustment          = 0xABCABCABCABCABCA;
    c_.timestamp_calibration_time    = 0xBABABABA;
    c_.temperature                   = -1.0F;
    c_.device_identifier.oui         = 0xFEBEFEBE;
    c_.device_identifier.device_code = 0xBEBA;

    c_.state_and_event_indicators.has.calibrated_time    = true;
    c_.state_and_event_indicators.has.valid_data         = true;
    c_.state_and_event_indicators.has.reference_lock     = true;
    c_.state_and_event_indicators.has.agc_or_mgc         = true;
    c_.state_and_event_indicators.has.detected_signal    = true;
    c_.state_and_event_indicators.has.spectral_inversion = true;
    c_.state_and_event_indicators.has.over_range         = true;
    c_.state_and_event_indicators.has.sample_loss        = true;
    c_.state_and_event_indicators.calibrated_time        = true;
    c_.state_and_event_indicators.valid_data             = true;
    c_.state_and_event_indicators.reference_lock         = true;
    c_.state_and_event_indicators.agc_or_mgc             = VRT_AOM_AGC;
    c_.state_and_event_indicators.detected_signal        = true;
    c_.state_and_event_indicators.spectral_inversion     = true;
    c_.state_and_event_indicators.over_range             = true;
    c_.state_and_event_indicators.sample_loss            = true;
    c_.state_and_event_indicators.user_defined           = 0xFF;

    c_.data_packet_payload_format.packing_method          = VRT_PM_LINK_EFFICIENT;
    c_.data_packet_payload_format.real_or_complex         = VRT_ROC_COMPLEX_POLAR;
    c_.data_packet_payload_format.data_item_format        = VRT_DIF_UNSIGNED_VRT_6_BIT_EXPONENT;
    c_.data_packet_payload_format.sample_component_repeat = true;
    c_.data_packet_payload_format.event_tag_size          = 0xFF;
    c_.data_packet_payload_format.channel_tag_size        = 0xFF;
    c_.data_packet_payload_format.item_packing_field_size = 0xFF;
    c_.data_packet_payload_format.data_item_size          = 0xFF;
    c_.data_packet_payload_format.repeat_count            = 0xFFFF;
    c_.data_packet_payload_format.vector_size             = 0xFFFF;

    c_.formatted_gps_geolocation.tsi                         = VRT_TSI_OTHER;
    c_.formatted_gps_geolocation.tsf                         = VRT_TSF_FREE_RUNNING_COUNT;
    c_.formatted_gps_geolocation.oui                         = 0xFFFFFFFF;
    c_.formatted_gps_geolocation.integer_second_timestamp    = 0xABABBABA;
    c_.formatted_gps_geolocation.fractional_second_timestamp = 0xBEBEBABACECECBCB;
    c_.formatted_gps_geolocation.has.latitude                = true;
    c_.formatted_gps_geolocation.has.longitude               = true;
    c_.formatted_gps_geolocation.has.altitude                = true;
    c_.formatted_gps_geolocation.has.speed_over_ground       = true;
    c_.formatted_gps_geolocation.has.heading_angle           = true;
    c_.formatted_gps_geolocation.has.track_angle             = true;
    c_.formatted_gps_geolocation.has.magnetic_variation      = true;
    c_.formatted_gps_geolocation.latitude                    = -1.0;
    c_.formatted_gps_geolocation.longitude                   = -1.0;
    c_.formatted_gps_geolocation.altitude                    = -1.0;
    c_.formatted_gps_geolocation.speed_over_ground           = -1.0;
    c_.formatted_gps_geolocation.heading_angle               = -1.0;
    c_.formatted_gps_geolocation.track_angle                 = -1.0;
    c_.formatted_gps_geolocation.magnetic_variation          = -1.0;

    c_.formatted_ins_geolocation.tsi                         = VRT_TSI_OTHER;
    c_.formatted_ins_geolocation.tsf                         = VRT_TSF_FREE_RUNNING_COUNT;
    c_.formatted_ins_geolocation.oui                         = 0xFFFFFFFF;
    c_.formatted_ins_geolocation.integer_second_timestamp    = 0xABABBABA;
    c_.formatted_ins_geolocation.fractional_second_timestamp = 0xBEBEBABACECECBCB;
    c_.formatted_ins_geolocation.has.latitude                = true;
    c_.formatted_ins_geolocation.has.longitude               = true;
    c_.formatted_ins_geolocation.has.altitude                = true;
    c_.formatted_ins_geolocation.has.speed_over_ground       = true;
    c_.formatted_ins_geolocation.has.heading_angle           = true;
    c_.formatted_ins_geolocation.has.track_angle             = true;
    c_.formatted_ins_geolocation.has.magnetic_variation      = true;
    c_.formatted_ins_geolocation.latitude                    = -1.0;
    c_.formatted_ins_geolocation.longitude                   = -1.0;
    c_.formatted_ins_geolocation.altitude                    = -1.0;
    c_.formatted_ins_geolocation.speed_over_ground           = -1.0;
    c_.formatted_ins_geolocation.heading_angle               = -1.0;
    c_.formatted_ins_geolocation.track_angle                 = -1.0;
    c_.formatted_ins_geolocation.magnetic_variation          = -1.0;

    c_.ecef_ephemeris.tsi                         = VRT_TSI_OTHER;
    c_.ecef_ephemeris.tsf                         = VRT_TSF_FREE_RUNNING_COUNT;
    c_.ecef_ephemeris.oui                         = 0xFFFFFFFF;
    c_.ecef_ephemeris.integer_second_timestamp    = 0xABABBABA;
    c_.ecef_ephemeris.fractional_second_timestamp = 0xBEBEBABACECECBCB;
    c_.ecef_ephemeris.has.position_x              = true;
    c_.ecef_ephemeris.has.position_y              = true;
    c_.ecef_ephemeris.has.position_z              = true;
    c_.ecef_ephemeris.has.attitude_alpha          = true;
    c_.ecef_ephemeris.has.attitude_beta           = true;
    c_.ecef_ephemeris.has.attitude_phi            = true;
    c_.ecef_ephemeris.has.velocity_dx             = true;
    c_.ecef_ephemeris.has.velocity_dy             = true;
    c_.ecef_ephemeris.has.velocity_dz             = true;
    c_.ecef_ephemeris.position_x                  = -1.0;
    c_.ecef_ephemeris.position_y                  = -1.0;
    c_.ecef_ephemeris.position_z                  = -1.0;
    c_.ecef_ephemeris.attitude_alpha              = -1.0;
    c_.ecef_ephemeris.attitude_beta               = -1.0;
    c_.ecef_ephemeris.attitude_phi                = -1.0;
    c_.ecef_ephemeris.velocity_dx                 = -1.0;
    c_.ecef_ephemeris.velocity_dy                 = -1.0;
    c_.ecef_ephemeris.velocity_dz                 = -1.0;

    c_.relative_ephemeris.tsi                         = VRT_TSI_OTHER;
    c_.relative_ephemeris.tsf                         = VRT_TSF_FREE_RUNNING_COUNT;
    c_.relative_ephemeris.oui                         = 0xFFFFFFFF;
    c_.relative_ephemeris.integer_second_timestamp    = 0xABABBABA;
    c_.relative_ephemeris.fractional_second_timestamp = 0xBEBEBABACECECBCB;
    c_.relative_ephemeris.has.position_x              = true;
    c_.relative_ephemeris.has.position_y              = true;
    c_.relative_ephemeris.has.position_z              = true;
    c_.relative_ephemeris.has.attitude_alpha          = true;
    c_.relative_ephemeris.has.attitude_beta           = true;
    c_.relative_ephemeris.has.attitude_phi            = true;
    c_.relative_ephemeris.has.velocity_dx             = true;
    c_.relative_ephemeris.has.velocity_dy             = true;
    c_.relative_ephemeris.has.velocity_dz             = true;
    c_.relative_ephemeris.position_x                  = -1.0;
    c_.relative_ephemeris.position_y                  = -1.0;
    c_.relative_ephemeris.position_z                  = -1.0;
    c_.relative_ephemeris.attitude_alpha              = -1.0;
    c_.relative_ephemeris.attitude_beta               = -1.0;
    c_.relative_ephemeris.attitude_phi                = -1.0;
    c_.relative_ephemeris.velocity_dx                 = -1.0;
    c_.relative_ephemeris.velocity_dy                 = -1.0;
    c_.relative_ephemeris.velocity_dz                 = -1.0;

    c_.ephemeris_reference_identifier = 0xDEDEDEDE;

    c_.gps_ascii.oui             = 0xFFFFFFFF;
    c_.gps_ascii.number_of_words = 3;
    c_.gps_ascii.ascii           = "Lorem ipsum!";

    std::array<uint32_t, 2> l1                                                 = {0x12345678, 0x23456789};
    std::array<uint32_t, 2> l2                                                 = {0x3456789A, 0x456789AB};
    std::array<uint32_t, 2> l3                                                 = {0x56789ABC, 0x6789ABCD};
    std::array<uint32_t, 2> l4                                                 = {0x789ABCDE, 0x89ABCDEF};
    std::array<uint32_t, 2> l5                                                 = {0x9ABCDEF0, 0xABCDEF01};
    c_.context_association_lists.has.asynchronous_channel_tag_list             = true;
    c_.context_association_lists.source_list_size                              = l1.size();
    c_.context_association_lists.system_list_size                              = l2.size();
    c_.context_association_lists.vector_component_list_size                    = l3.size();
    c_.context_association_lists.asynchronous_channel_list_size                = l4.size();
    c_.context_association_lists.source_context_association_list               = l1.data();
    c_.context_association_lists.system_context_association_list               = l2.data();
    c_.context_association_lists.vector_component_context_association_list     = l3.data();
    c_.context_association_lists.asynchronous_channel_context_association_list = l4.data();
    c_.context_association_lists.asynchronous_channel_tag_list                 = l5.data();

    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 91, true), VRT_ERR_BUFFER_SIZE);
    ASSERT_EQ(vrt_write_if_context(&c_, buf_.data(), 92, false), 92);
    ASSERT_EQ(Hex(buf_[0]), Hex(0xFFFFFF00)); /* Context indicators */
    ASSERT_EQ(Hex(buf_[1]), Hex(0xACACACAC)); /* Reference point identifier */
    ASSERT_EQ(Hex(buf_[2]), Hex(0xFFFFFFFF)); /* Bandwidth */
    ASSERT_EQ(Hex(buf_[3]), Hex(0xFFF00000));
    ASSERT_EQ(Hex(buf_[4]), Hex(0xFFFFFFFF)); /* IF reference frequency */
    ASSERT_EQ(Hex(buf_[5]), Hex(0xFFF00000));
    ASSERT_EQ(Hex(buf_[6]), Hex(0xFFFFFFFF)); /* RF reference frequency */
    ASSERT_EQ(Hex(buf_[7]), Hex(0xFFF00000));
    ASSERT_EQ(Hex(buf_[8]), Hex(0xFFFFFFFF)); /* RF reference frequency offset */
    ASSERT_EQ(Hex(buf_[9]), Hex(0xFFF00000));
    ASSERT_EQ(Hex(buf_[10]), Hex(0xFFFFFFFF)); /* IF band offset */
    ASSERT_EQ(Hex(buf_[11]), Hex(0xFFF00000));
    ASSERT_EQ(Hex(buf_[12]), Hex(0x0000FF80)); /* Reference level */
    ASSERT_EQ(Hex(buf_[13]), Hex(0xFF80FF80)); /* Gain stage 1 & 2 */
    ASSERT_EQ(Hex(buf_[14]), Hex(0xBDBDBDBD)); /* Over-range count */
    ASSERT_EQ(Hex(buf_[15]), Hex(0xFFFFFFFF)); /* Sample rate */
    ASSERT_EQ(Hex(buf_[16]), Hex(0xFFF00000));
    ASSERT_EQ(Hex(buf_[17]), Hex(0xABCABCAB)); /* Timestamp adjustment */
    ASSERT_EQ(Hex(buf_[18]), Hex(0xCABCABCA));
    ASSERT_EQ(Hex(buf_[19]), Hex(0xBABABABA)); /* Timestamp calibration time */
    ASSERT_EQ(Hex(buf_[20]), Hex(0x0000FFC0)); /* Temperature */
    ASSERT_EQ(Hex(buf_[21]), Hex(0x00BEFEBE)); /* Device identifier.oui */
    ASSERT_EQ(Hex(buf_[22]), Hex(0x0000BEBA)); /* Device identifier.device_code */
    ASSERT_EQ(Hex(buf_[23]), Hex(0xFF0FF0FF)); /* State and event indicators */
    ASSERT_EQ(Hex(buf_[24]), Hex(0xD6FF0FFF)); /* Data packet payload format */
    ASSERT_EQ(Hex(buf_[25]), Hex(0xFFFFFFFF));
    ASSERT_EQ(Hex(buf_[26]), Hex(0x0FFFFFFF)); /* Formatted GPS geolocation */
    ASSERT_EQ(Hex(buf_[27]), Hex(0xABABBABA));
    ASSERT_EQ(Hex(buf_[28]), Hex(0xBEBEBABA));
    ASSERT_EQ(Hex(buf_[29]), Hex(0xCECECBCB));
    ASSERT_EQ(Hex(buf_[30]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[31]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[32]), Hex(0xFFFFFFE0));
    ASSERT_EQ(Hex(buf_[33]), Hex(0xFFFF0000));
    ASSERT_EQ(Hex(buf_[34]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[35]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[36]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[37]), Hex(0x0FFFFFFF)); /* Formatted INS geolocation */
    ASSERT_EQ(Hex(buf_[38]), Hex(0xABABBABA));
    ASSERT_EQ(Hex(buf_[39]), Hex(0xBEBEBABA));
    ASSERT_EQ(Hex(buf_[40]), Hex(0xCECECBCB));
    ASSERT_EQ(Hex(buf_[41]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[42]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[43]), Hex(0xFFFFFFE0));
    ASSERT_EQ(Hex(buf_[44]), Hex(0xFFFF0000));
    ASSERT_EQ(Hex(buf_[45]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[46]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[47]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[48]), Hex(0x0FFFFFFF)); /* ECEF ephemeris */
    ASSERT_EQ(Hex(buf_[49]), Hex(0xABABBABA));
    ASSERT_EQ(Hex(buf_[50]), Hex(0xBEBEBABA));
    ASSERT_EQ(Hex(buf_[51]), Hex(0xCECECBCB));
    ASSERT_EQ(Hex(buf_[52]), Hex(0xFFFFFFE0));
    ASSERT_EQ(Hex(buf_[53]), Hex(0xFFFFFFE0));
    ASSERT_EQ(Hex(buf_[54]), Hex(0xFFFFFFE0));
    ASSERT_EQ(Hex(buf_[55]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[56]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[57]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[58]), Hex(0xFFFF0000));
    ASSERT_EQ(Hex(buf_[59]), Hex(0xFFFF0000));
    ASSERT_EQ(Hex(buf_[60]), Hex(0xFFFF0000));
    ASSERT_EQ(Hex(buf_[61]), Hex(0x0FFFFFFF)); /* Relative ephemeris */
    ASSERT_EQ(Hex(buf_[62]), Hex(0xABABBABA));
    ASSERT_EQ(Hex(buf_[63]), Hex(0xBEBEBABA));
    ASSERT_EQ(Hex(buf_[64]), Hex(0xCECECBCB));
    ASSERT_EQ(Hex(buf_[65]), Hex(0xFFFFFFE0));
    ASSERT_EQ(Hex(buf_[66]), Hex(0xFFFFFFE0));
    ASSERT_EQ(Hex(buf_[67]), Hex(0xFFFFFFE0));
    ASSERT_EQ(Hex(buf_[68]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[69]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[70]), Hex(0xFFC00000));
    ASSERT_EQ(Hex(buf_[71]), Hex(0xFFFF0000));
    ASSERT_EQ(Hex(buf_[72]), Hex(0xFFFF0000));
    ASSERT_EQ(Hex(buf_[73]), Hex(0xFFFF0000));
    ASSERT_EQ(Hex(buf_[74]), Hex(0xDEDEDEDE)); /* Ephemeris reference identifier */
    ASSERT_EQ(Hex(buf_[75]), Hex(0x00FFFFFF)); /* GPS ASCII */
    ASSERT_EQ(Hex(buf_[76]), Hex(0x00000003));
    ASSERT_EQ(Hex(buf_[77]), Hex(0x65726F4C));
    ASSERT_EQ(Hex(buf_[78]), Hex(0x7069206D));
    ASSERT_EQ(Hex(buf_[79]), Hex(0x216D7573));
    ASSERT_EQ(Hex(buf_[80]), Hex(0x00020002)); /* Context association lists */
    ASSERT_EQ(Hex(buf_[81]), Hex(0x00028002));
    ASSERT_EQ(Hex(buf_[82]), Hex(0x12345678));
    ASSERT_EQ(Hex(buf_[83]), Hex(0x23456789));
    ASSERT_EQ(Hex(buf_[84]), Hex(0x3456789A));
    ASSERT_EQ(Hex(buf_[85]), Hex(0x456789AB));
    ASSERT_EQ(Hex(buf_[86]), Hex(0x56789ABC));
    ASSERT_EQ(Hex(buf_[87]), Hex(0x6789ABCD));
    ASSERT_EQ(Hex(buf_[88]), Hex(0x789ABCDE));
    ASSERT_EQ(Hex(buf_[89]), Hex(0x89ABCDEF));
    ASSERT_EQ(Hex(buf_[90]), Hex(0x9ABCDEF0));
    ASSERT_EQ(Hex(buf_[91]), Hex(0xABCDEF01));
}
