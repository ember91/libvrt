/** \mainpage VRT protocol
 *
 * Encoder and decoder of the ANSI/VITA 49.0 Radio Transport (VRT) standard.
 */

#ifndef INCLUDE_VRT_VRT_COMMON_H_
#define INCLUDE_VRT_VRT_COMMON_H_

#include <stdint.h>

#include <vrt/vrt_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Silence warnings when using C++ */
#ifdef __cplusplus
#define STATIC_CAST(T, X) static_cast<T>(X)
#else
#define STATIC_CAST(T, X) (T)(X)
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
    VRT_ERR_PACKET_TYPE = -3,
    /**
     * Error return code. Context packet has trailer bit set.
     */
    VRT_ERR_TRAILER = -4,
    /**
     * Error return code. Data packet has TSM bit set.
     */
    VRT_ERR_TSM = -5,
    /**
     * Error return code. TSI is an invalid value.
     */
    VRT_ERR_TSI = -6,
    /**
     * Error return code. TSF is an invalid value.
     */
    VRT_ERR_TSF = -7,
    /**
     * Error return code. Fractional seconds Real time is active but picoseconds is outside bounds.
     */
    VRT_ERR_REAL_TIME = -8,
    /**
     * Error return code. Packet count is outside valid bounds (> 0x0F).
     */
    VRT_ERR_PACKET_COUNT = -9,
    /**
     * Error return code. OUI is outside valid bounds (> 0x00FFFFFF).
     */
    VRT_ERR_OUI = -10,
    /**
     * Error return code. Associated context packet count is outside valid bounds (> 0x7F).
     */
    VRT_ERR_ASSOCIATED_CONTEXT_PACKET_COUNT = -11,
    /**
     * Error return code. Bandwidth is negative.
     */
    VRT_ERR_BANDWIDTH = -12,
    /**
     * Error return code. Sample rate is negative.
     */
    VRT_ERR_SAMPLE_RATE = -13,
    /**
     * Error return code. Temperature is below absolute zero (< -273.15).
     */
    VRT_ERR_TEMPERATURE = -14,
    /**
     * Error return code. Packing method is an invalid value.
     */
    VRT_ERR_PACKING_METHOD = -15,
    /**
     * Error return code. Real/Complex is an invalid value.
     */
    VRT_ERR_REAL_OR_COMPLEX = -16,
    /**
     * Error return code. Data item format is an invalid value.
     */
    VRT_ERR_DATA_ITEM_FORMAT = -17,
    /**
     * Error return code. Event tag size is outside valid bounds (> 0x07).
     */
    VRT_ERR_EVENT_TAG_SIZE = -18,
    /**
     * Error return code. Channel tag size is outside valid bounds (> 0x0F).
     */
    VRT_ERR_CHANNEL_TAG_SIZE = -19,
    /**
     * Error return code. Item packing fields size is outside valid bounds (> 0x3F).
     */
    VRT_ERR_ITEM_PACKING_FIELD_SIZE = -20,
    /**
     * Error return code. Data item size is outside valid bounds (> 0x3F).
     */
    VRT_ERR_DATA_ITEM_SIZE = -21,
    /**
     * Error return code. Integer second timestamp is not 0xFFFFFFFF when TSI is VRT_TSI_UNDEFINED.
     */
    VRT_ERR_INTEGER_SECOND_TIMESTAMP = -22,
    /**
     * Error return code. Fractional second timestamp is not 0xFFFFFFFFFFFFFFFF when TSF is
     */
    VRT_ERR_FRACTIONAL_SECOND_TIMESTAMP = -23,
    /**
     * Error return code. Latitude is outside valid bounds (< -90 or > 90).
     */
    VRT_ERR_LATITUDE = -24,
    /**
     * Error return code. Longitude is outside valid bounds (< -180 or > 180).
     */
    VRT_ERR_LONGITUDE = -25,
    /**
     * Error return code. Speed over ground is negative.
     */
    VRT_ERR_SPEED_OVER_GROUND = -26,
    /**
     * Error return code. Heading angle outside valid bounds (< 0 or > 359.999999761582).
     */
    VRT_ERR_HEADING_ANGLE = -27,
    /**
     * Error return code. Track angle is outside valid bounds (< 0 or > 359.999999761582).
     */
    VRT_ERR_TRACK_ANGLE = -28,
    /**
     * Error return code. Magnetic variation is outside valid bounds (< -180 or > 180).
     */
    VRT_ERR_MAGNETIC_VARIATION = -29,
    /**
     * Error return code. Source list size is outside valid bounds (> 0x01FF).
     */
    VRT_ERR_SOURCE_LIST_SIZE = -30,
    /**
     * Error return code. System list size is outside valid bounds (> 0x01FF).
     */
    VRT_ERR_SYSTEM_LIST_SIZE = -31,
    /**
     * Error return code. Channel list size is outside valid bounds (> 0x7FFF).
     */
    VRT_ERR_CHANNEL_LIST_SIZE = -32
} vrt_error_code;

/* There is no point in adding max sizes for e.g. data body and IF/Ext context, since they can be UINT16_MAX - 1. */

/**
 * Size of header in 32-bit words. Always 1.
 */
static const uint16_t VRT_WORDS_HEADER = 1;
/**
 * Maximum size of fields section in 32-bit words.
 */
static const uint16_t VRT_WORDS_MAX_FIELDS = 6;
/**
 * Maximum size of trailer in 32-bit words.
 */
static const uint16_t VRT_WORDS_MAX_TRAILER = 1;
/**
 * Maximum size of a VRT packet in 32-bit words.
 */
static const uint16_t VRT_WORDS_MAX_PACKET = UINT16_MAX;

/**
 * Initialize header to a reasonable default state.
 *
 * \param header Header.
 *
 * \note Packet size must still be set explicitly to the actual packet size in 32-bit words.
 */
void vrt_init_header(vrt_header* header);

/**
 * Initialize fields section to a reasonable default state.
 *
 * \param fields Fields.
 */
void vrt_init_fields(vrt_fields* fields);

/**
 * Initialize trailer to a reasonable default state.
 *
 * \param trailer Trailer.
 */
void vrt_init_trailer(vrt_trailer* trailer);

/**
 * Initialize IF context to a reasonable default state.
 *
 * \param if_context IF context.
 */
void vrt_init_if_context(vrt_if_context* if_context);

/**
 * Calculate size of fields section in 32-bit words.
 *
 * \param header Header.
 *
 * \return Number of 32-bit words the fields section consists of.
 */
uint32_t vrt_words_fields(const vrt_header* header);

/**
 * Calculate size of trailer in 32-bit words.
 *
 * \param header Header, which contains trailer information.
 *
 * \return Number of 32-bit words it consists of.
 *
 * \note If packet type is missing support for the trailer type, such as a context packet, this returns 0.
 */
uint32_t vrt_words_trailer(const vrt_header* header);

/**
 * Calculate size of IF context section in 32-bit words.
 *
 * \param if_context IF context.
 *
 * \return Number of 32-bit words it consists of.
 */
uint32_t vrt_words_if_context(const vrt_if_context* if_context);

/**
 * Check if a specific packet type means it is a context packet.
 *
 * \param type Packet type.
 *
 * \return True if it is a IF or Ext context packet.
 *
 * \warning Undefined behaviour if type is outside bounds.
 */
inline bool vrt_is_context(vrt_packet_type type) {
    return (STATIC_CAST(uint32_t, type) & 0x4U) != 0;
}

/**
 * Check if a specific packet type means it has the Stream ID field.
 *
 * \param type Packet type.
 *
 * \return True if it has the Stream ID field.
 *
 * \warning Undefined behaviour if type is outside bounds.
 */
inline bool vrt_has_stream_id(vrt_packet_type type) {
    return (STATIC_CAST(uint32_t, type) & 0x5U) != 0;
}

/**
 * Check if platform is little endian. May be used to detect if byte swap is required for VRT data.
 *
 * \return True if little endian.
 */
inline bool vrt_is_platform_little_endian() {
    volatile uint32_t i = 0x01234567; /* Ensure written to memory */
    return *(uint8_t*)(&i) == 0x67;
}

#ifdef __cplusplus
}
#endif

#endif
