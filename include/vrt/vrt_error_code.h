#ifndef INCLUDE_VRT_VRT_ERROR_CODE_H_
#define INCLUDE_VRT_VRT_ERROR_CODE_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Error code constants.
 */
typedef enum vrt_error_code {
    /**
     * Error return code. Buffer is too small.
     */
    VRT_ERR_BUF_SIZE = -1,
    /**
     * Error return code. One or multiple reserved bits are set.
     */
    VRT_ERR_RESERVED = -2,
    /**
     * Error return code. Packet type is an invalid value.
     */
    VRT_ERR_INVALID_PACKET_TYPE = -3,
    /**
     * Error return code. Context packet has trailer bit set.
     */
    VRT_ERR_TRAILER_IN_CONTEXT = -4,
    /**
     * Error return code. Data packet has TSM bit set.
     */
    VRT_ERR_TSM_IN_DATA = -5,
    /**
     * Error return code. TSM is an invalid value.
     */
    VRT_ERR_INVALID_TSM = -6,
    /**
     * Error return code. TSI is an invalid value.
     */
    VRT_ERR_INVALID_TSI = -7,
    /**
     * Error return code. TSF is an invalid value.
     */
    VRT_ERR_INVALID_TSF = -8,
    /**
     * Error return code. TSF is VRT_TSF_REAL TIME but picoseconds is outside valid bounds (> 999999999999 ps).
     */
    VRT_ERR_BOUNDS_REAL_TIME = -9,
    /**
     * Error return code. Packet count is outside valid bounds (> 0x0F).
     */
    VRT_ERR_BOUNDS_PACKET_COUNT = -10,
    /**
     * Error return code. OUI is outside valid bounds (> 0x00FFFFFF).
     */
    VRT_ERR_BOUNDS_OUI = -11,
    /**
     * Error return code. Associated context packet count is outside valid bounds (> 0x7F).
     */
    VRT_ERR_BOUNDS_ASSOCIATED_CONTEXT_PACKET_COUNT = -12,
    /**
     * Error return code. Bandwidth is outside valid bounds (< 0 Hz).
     */
    VRT_ERR_BOUNDS_BANDWIDTH = -13,
    /**
     * Error return code. Sample rate is outside valid bounds (< 0 Hz).
     */
    VRT_ERR_BOUNDS_SAMPLE_RATE = -14,
    /**
     * Error return code. Temperature is outside valid bounds (< -273.15 degrees C).
     */
    VRT_ERR_BOUNDS_TEMPERATURE = -15,
    /**
     * Error return code. Packing method is an invalid value.
     */
    VRT_ERR_INVALID_PACKING_METHOD = -16,
    /**
     * Error return code. Real/Complex is an invalid value.
     */
    VRT_ERR_INVALID_REAL_OR_COMPLEX = -17,
    /**
     * Error return code. Data item format is an invalid value.
     */
    VRT_ERR_INVALID_DATA_ITEM_FORMAT = -18,
    /**
     * Error return code. Event tag size is outside valid bounds (> 0x07).
     */
    VRT_ERR_BOUNDS_EVENT_TAG_SIZE = -19,
    /**
     * Error return code. Channel tag size is outside valid bounds (> 0x0F).
     */
    VRT_ERR_BOUNDS_CHANNEL_TAG_SIZE = -20,
    /**
     * Error return code. Item packing fields size is outside valid bounds (> 0x3F).
     */
    VRT_ERR_BOUNDS_ITEM_PACKING_FIELD_SIZE = -21,
    /**
     * Error return code. Data item size is outside valid bounds (> 0x3F).
     */
    VRT_ERR_BOUNDS_DATA_ITEM_SIZE = -22,
    /**
     * Error return code. Integer second timestamp is not 0xFFFFFFFF when TSI is VRT_TSI_UNDEFINED.
     */
    VRT_ERR_SET_INTEGER_SECOND_TIMESTAMP = -23,
    /**
     * Error return code. Fractional second timestamp is not 0xFFFFFFFFFFFFFFFF when TSF is VRT_TSF_UNDEFINED.
     */
    VRT_ERR_SET_FRACTIONAL_SECOND_TIMESTAMP = -24,
    /**
     * Error return code. Latitude is outside valid bounds (< -90 or > 90 degrees).
     */
    VRT_ERR_BOUNDS_LATITUDE = -25,
    /**
     * Error return code. Longitude is outside valid bounds (< -180 or > 180 degrees).
     */
    VRT_ERR_BOUNDS_LONGITUDE = -26,
    /**
     * Error return code. Speed over ground is outside valid bounds (< 0 m/s).
     */
    VRT_ERR_BOUNDS_SPEED_OVER_GROUND = -27,
    /**
     * Error return code. Heading angle outside valid bounds (< 0 or > 359.999999761582 degrees).
     */
    VRT_ERR_BOUNDS_HEADING_ANGLE = -28,
    /**
     * Error return code. Track angle is outside valid bounds (< 0 or > 359.999999761582 degrees).
     */
    VRT_ERR_BOUNDS_TRACK_ANGLE = -29,
    /**
     * Error return code. Magnetic variation is outside valid bounds (< -180 or > 180 degrees).
     */
    VRT_ERR_BOUNDS_MAGNETIC_VARIATION = -30,
    /**
     * Error return code. Source list size is outside valid bounds (> 0x01FF).
     */
    VRT_ERR_BOUNDS_SOURCE_LIST_SIZE = -31,
    /**
     * Error return code. System list size is outside valid bounds (> 0x01FF).
     */
    VRT_ERR_BOUNDS_SYSTEM_LIST_SIZE = -32,
    /**
     * Error return code. Channel list size is outside valid bounds (> 0x7FFF).
     */
    VRT_ERR_BOUNDS_CHANNEL_LIST_SIZE = -33
} vrt_error_code;

#ifdef __cplusplus
}
#endif

#endif