/** \mainpage VRT protocol
 *
 * Encoder and decoder of the ANSI/VITA 49.0 Radio Transport (VRT) standard.
 */

#ifndef INCLUDE_VRT_COMMON_H_
#define INCLUDE_VRT_COMMON_H_

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

/* There is no point in adding max sizes for e.g. data body and IF/Ext context, since they can be UINT16_MAX - 1. */

/**
 * Size of header in 32-bit words. Always 1.
 */
static const uint16_t VRT_SIZE_HEADER = 1;
/**
 * Maximum size of fields section in 32-bit words.
 */
static const uint16_t VRT_MAX_SIZE_FIELDS = 6;
/**
 * Maximum size of trailer in 32-bit words.
 */
static const uint16_t VRT_MAX_SIZE_TRAILER = 1;
/**
 * Maximum size of a VRT packet in 32-bit words.
 */
static const uint16_t VRT_MAX_SIZE_PACKET = UINT16_MAX;

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
    return ((uint32_t)type & 0x5U) != 0;
}

/**
 * Check if platform is big endian.
 *
 * \return True if big endian.
 */
inline bool is_platform_little_endian() {
    volatile uint32_t i = 0x01234567; /* Ensure written to memory */
    return (*((uint8_t*)(&i))) == 0x67;
}

#ifdef __cplusplus
}
#endif

#endif
