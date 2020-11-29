#ifndef INCLUDE_VRT_VRT_ERROR_CODE_H_
#define INCLUDE_VRT_VRT_ERROR_CODE_H_

#include "vrt_error_code.h"
#include "vrt_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Error code constants.
 */
typedef enum vrt_error_code {
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
     * TSF is VRT_TSF_REAL TIME but picoseconds is outside valid bounds (> 999999999999 ps).
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
     * Bandwidth is outside valid bounds (< 0 Hz).
     */
    VRT_ERR_BOUNDS_BANDWIDTH = -13,
    /**
     * Gain stage 1 must be used instead of stage 2 when only one is set.
     */
    VRT_ERR_GAIN_STAGE2_SET = -14,
    /**
     * Sample rate is outside valid bounds (< 0 Hz).
     */
    VRT_ERR_BOUNDS_SAMPLE_RATE = -15,
    /**
     * Temperature is outside valid bounds (< -273.15 degrees C).
     */
    VRT_ERR_BOUNDS_TEMPERATURE = -16,
    /**
     * Packing method is an invalid value.
     */
    VRT_ERR_INVALID_PACKING_METHOD = -17,
    /**
     * Real/Complex is an invalid value.
     */
    VRT_ERR_INVALID_REAL_OR_COMPLEX = -18,
    /**
     * Data item format is an invalid value.
     */
    VRT_ERR_INVALID_DATA_ITEM_FORMAT = -19,
    /**
     * Event tag size is outside valid bounds (> 0x07).
     */
    VRT_ERR_BOUNDS_EVENT_TAG_SIZE = -20,
    /**
     * Channel tag size is outside valid bounds (> 0x0F).
     */
    VRT_ERR_BOUNDS_CHANNEL_TAG_SIZE = -21,
    /**
     * Item packing fields size is outside valid bounds (> 0x3F).
     */
    VRT_ERR_BOUNDS_ITEM_PACKING_FIELD_SIZE = -22,
    /**
     * Data item size is outside valid bounds (> 0x3F).
     */
    VRT_ERR_BOUNDS_DATA_ITEM_SIZE = -23,
    /**
     * Integer second timestamp is not 0xFFFFFFFF when TSI is VRT_TSI_UNDEFINED.
     */
    VRT_ERR_SET_INTEGER_SECOND_TIMESTAMP = -24,
    /**
     * Fractional second timestamp is not 0xFFFFFFFFFFFFFFFF when TSF is VRT_TSF_UNDEFINED.
     */
    VRT_ERR_SET_FRACTIONAL_SECOND_TIMESTAMP = -25,
    /**
     * Latitude is outside valid bounds (< -90 or > 90 degrees).
     */
    VRT_ERR_BOUNDS_LATITUDE = -26,
    /**
     * Longitude is outside valid bounds (< -180 or > 180 degrees).
     */
    VRT_ERR_BOUNDS_LONGITUDE = -27,
    /**
     * Speed over ground is outside valid bounds (< 0 m/s).
     */
    VRT_ERR_BOUNDS_SPEED_OVER_GROUND = -28,
    /**
     * Heading angle outside valid bounds (< 0 or > 359.999999761582 degrees).
     */
    VRT_ERR_BOUNDS_HEADING_ANGLE = -29,
    /**
     * Track angle is outside valid bounds (< 0 or > 359.999999761582 degrees).
     */
    VRT_ERR_BOUNDS_TRACK_ANGLE = -30,
    /**
     * Magnetic variation is outside valid bounds (< -180 or > 180 degrees).
     */
    VRT_ERR_BOUNDS_MAGNETIC_VARIATION = -31,
    /**
     * Source list size is outside valid bounds (> 0x01FF).
     */
    VRT_ERR_BOUNDS_SOURCE_LIST_SIZE = -32,
    /**
     * System list size is outside valid bounds (> 0x01FF).
     */
    VRT_ERR_BOUNDS_SYSTEM_LIST_SIZE = -33,
    /**
     * Channel list size is outside valid bounds (> 0x7FFF).
     */
    VRT_ERR_BOUNDS_CHANNEL_LIST_SIZE = -34,
    /**
     * Calculated packet size is outside valid bounds (> 0xFFFF).
     */
    VRT_ERR_BOUNDS_PACKET_SIZE = -35,
    /**
     * Packet size in header and calculated packet size do not match.
     */
    VRT_ERR_PACKET_SIZE_MISMATCH = -36
} vrt_error_code;

#ifdef __cplusplus
}
#endif

#endif
