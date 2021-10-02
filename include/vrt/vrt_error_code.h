#ifndef INCLUDE_VRT_VRT_ERROR_CODE_H_
#define INCLUDE_VRT_VRT_ERROR_CODE_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Error code constants.
 */
enum vrt_error_code {
    /**
     * Buffer is too small.
     */
    VRT_ERR_BUFFER_SIZE = -1,
    /**
     * One or multiple reserved bits are set.
     */
    VRT_ERR_RESERVED = -2,
    /**
     * Packet type is an invalid value.
     */
    VRT_ERR_INVALID_PACKET_TYPE = -3,
    /**
     * Context packet has trailer bit set.
     */
    VRT_ERR_TRAILER_IN_CONTEXT = -4,
    /**
     * Data packet has TSM bit set.
     */
    VRT_ERR_TSM_IN_DATA = -5,
    /**
     * TSM is an invalid value.
     */
    VRT_ERR_INVALID_TSM = -6,
    /**
     * TSI is an invalid value.
     */
    VRT_ERR_INVALID_TSI = -7,
    /**
     * TSF is an invalid value.
     */
    VRT_ERR_INVALID_TSF = -8,
    /**
     * TSF is VRT_TSF_REAL_TIME but fractional timestamp is outside valid bounds (> 999999999999 ps).
     */
    VRT_ERR_BOUNDS_REAL_TIME = -9,
    /**
     * Packet count is outside valid bounds (> 0x0F).
     */
    VRT_ERR_BOUNDS_PACKET_COUNT = -10,
    /**
     * OUI is outside valid bounds (> 0x00FFFFFF).
     */
    VRT_ERR_BOUNDS_OUI = -11,
    /**
     * Associated context packet count is outside valid bounds (> 0x7F).
     */
    VRT_ERR_BOUNDS_ASSOCIATED_CONTEXT_PACKET_COUNT = -12,
    /**
     * Bandwidth is outside valid bounds (< 0 Hz or > ~8.79 THz).
     */
    VRT_ERR_BOUNDS_BANDWIDTH = -13,
    /**
     * IF reference frequency is outside valid bounds (< ~-8.79 or > ~8.79 THz).
     */
    VRT_ERR_BOUNDS_IF_REFERENCE_FREQUENCY = -14,
    /**
     * RF reference frequency is outside valid bounds (< ~-8.79 or > ~8.79 THz).
     */
    VRT_ERR_BOUNDS_RF_REFERENCE_FREQUENCY = -15,
    /**
     * RF reference frequency offset is outside valid bounds (< ~-8.79 or > ~8.79 THz).
     */
    VRT_ERR_BOUNDS_RF_REFERENCE_FREQUENCY_OFFSET = -16,
    /**
     * IF band offset is outside valid bounds (< ~-8.79 or > ~8.79 THz).
     */
    VRT_ERR_BOUNDS_IF_BAND_OFFSET = -17,
    /**
     * Reference level is outside valid bounds (< -256 or > ~256 dBm).
     */
    VRT_ERR_BOUNDS_REFERENCE_LEVEL = -18,
    /**
     * Gain stage 1 or 2 is outside valid bounds (< -256 or > ~256 dB).
     */
    VRT_ERR_BOUNDS_GAIN = -19,
    /**
     * Gain stage 1 must be used instead of stage 2 when only one of them is set.
     */
    VRT_ERR_GAIN_STAGE2_SET = -20,
    /**
     * Sample rate is outside valid bounds (< 0 Hz or > ~8.79 THz).
     */
    VRT_ERR_BOUNDS_SAMPLE_RATE = -21,
    /**
     * Temperature is outside valid bounds (< -273.15 or > ~512 degrees C).
     */
    VRT_ERR_BOUNDS_TEMPERATURE = -22,
    /**
     * Packing method is an invalid value.
     */
    VRT_ERR_INVALID_PACKING_METHOD = -23,
    /**
     * Real/Complex is an invalid value.
     */
    VRT_ERR_INVALID_REAL_OR_COMPLEX = -24,
    /**
     * Data item format is an invalid value.
     */
    VRT_ERR_INVALID_DATA_ITEM_FORMAT = -25,
    /**
     * Event tag size is outside valid bounds (> 0x07).
     */
    VRT_ERR_BOUNDS_EVENT_TAG_SIZE = -26,
    /**
     * Channel tag size is outside valid bounds (> 0x0F).
     */
    VRT_ERR_BOUNDS_CHANNEL_TAG_SIZE = -27,
    /**
     * Item packing fields size is outside valid bounds (> 0x3F).
     */
    VRT_ERR_BOUNDS_ITEM_PACKING_FIELD_SIZE = -28,
    /**
     * Data item size is outside valid bounds (> 0x3F).
     */
    VRT_ERR_BOUNDS_DATA_ITEM_SIZE = -29,
    /**
     * Integer second timestamp is not 0xFFFFFFFF when TSI is VRT_TSI_UNDEFINED.
     */
    VRT_ERR_SET_INTEGER_SECOND_TIMESTAMP = -30,
    /**
     * Fractional second timestamp is not 0xFFFFFFFFFFFFFFFF when TSF is VRT_TSF_UNDEFINED.
     */
    VRT_ERR_SET_FRACTIONAL_SECOND_TIMESTAMP = -31,
    /**
     * Latitude is outside valid bounds (< -90 or > 90 degrees).
     */
    VRT_ERR_BOUNDS_LATITUDE = -32,
    /**
     * Longitude is outside valid bounds (< -180 or > 180 degrees).
     */
    VRT_ERR_BOUNDS_LONGITUDE = -33,
    /**
     * Altitude is outside valid bounds (< ~-67108 or > ~67108 km).
     */
    VRT_ERR_BOUNDS_ALTITUDE = -34,
    /**
     * Speed over ground is outside valid bounds (< 0 or > ~65536 m/s).
     */
    VRT_ERR_BOUNDS_SPEED_OVER_GROUND = -35,
    /**
     * Heading angle is outside valid bounds (< 0 or > ~360 degrees).
     */
    VRT_ERR_BOUNDS_HEADING_ANGLE = -36,
    /**
     * Track angle is outside valid bounds (< 0 or > ~360 degrees).
     */
    VRT_ERR_BOUNDS_TRACK_ANGLE = -37,
    /**
     * Magnetic variation is outside valid bounds (< -180 or > 180 degrees).
     */
    VRT_ERR_BOUNDS_MAGNETIC_VARIATION = -38,
    /**
     * Position x, y, or z is outside valid bounds (< ~-67108 or > ~67108 km).
     */
    VRT_ERR_BOUNDS_POSITION = -39,
    /**
     * Attitude alpha, beta, or gamma is outside valid bounds (< -512 or > ~512 degrees).
     */
    VRT_ERR_BOUNDS_ATTITUDE = -40,
    /**
     * Velocity dx, dy, or dz is outside valid bounds (< -32768 or > ~32768 m/s).
     */
    VRT_ERR_BOUNDS_VELOCITY = -41,
    /**
     * Source list size is outside valid bounds (> 0x01FF).
     */
    VRT_ERR_BOUNDS_SOURCE_LIST_SIZE = -42,
    /**
     * System list size is outside valid bounds (> 0x01FF).
     */
    VRT_ERR_BOUNDS_SYSTEM_LIST_SIZE = -43,
    /**
     * Channel list size is outside valid bounds (> 0x7FFF).
     */
    VRT_ERR_BOUNDS_CHANNEL_LIST_SIZE = -44,
    /**
     * Calculated packet size is outside valid bounds (> 0xFFFF).
     */
    VRT_ERR_BOUNDS_PACKET_SIZE = -45,
    /**
     * Packet size in header and calculated packet size do not match.
     */
    VRT_ERR_MISMATCH_PACKET_SIZE = -46,
    /**
     * TSI and/or TSF timestamps differ between packets.
     */
    VRT_ERR_MISMATCH_TIME_TYPES = -47,
    /**
     * Sample rate is required but is not provided (<= 0).
     */
    VRT_ERR_MISSING_SAMPLE_RATE = -48,
    /**
     * Fractional timestamp is outside valid bounds (>= sample rate).
     */
    VRT_ERR_BOUNDS_SAMPLE_COUNT = -49,
    /**
     * Timestamp integer seconds and calculated seconds from the Free running count fractional part differ.
     */
    VRT_ERR_INTEGER_SECONDS_MISMATCH = -50,
    /**
     * Expected a field that was not present.
     */
    VRT_ERR_EXPECTED_FIELD = -51
};

#ifdef __cplusplus
}
#endif

#endif
