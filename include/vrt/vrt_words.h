#ifndef INCLUDE_VRT_VRT_WORDS_H_
#define INCLUDE_VRT_VRT_WORDS_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct vrt_header;
struct vrt_if_context;
struct vrt_packet;

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
 * Size of trailer in 32-bit words. Always 1.
 */
static const uint16_t VRT_WORDS_TRAILER = 1;
/**
 * Maximum size of a VRT packet in 32-bit words.
 */
static const uint16_t VRT_WORDS_MAX_PACKET = UINT16_MAX;

/**
 * Calculate size of fields section in 32-bit words.
 *
 * \param header Header.
 *
 * \return Number of 32-bit words the fields section consists of.
 */
int32_t vrt_words_fields(const struct vrt_header* header);

/**
 * Calculate size of trailer in 32-bit words.
 *
 * \param header Header, which contains trailer information.
 *
 * \return Number of 32-bit words it consists of.
 *
 * \note If packet type is missing support for the trailer type, such as a context packet, this returns 0.
 */
int32_t vrt_words_trailer(const struct vrt_header* header);

/**
 * Calculate size of IF context section in 32-bit words.
 *
 * \param if_context IF context.
 *
 * \return Number of 32-bit words it consists of.
 */
int32_t vrt_words_if_context(const struct vrt_if_context* if_context);

/**
 * Calculate size of VRT packet in 32-bit words.
 *
 * \note This does not look at the packet_size field in the header.
 *
 * \param packet VRT packet.
 *
 * \return Number of 32-bit words it consists of.
 */
int32_t vrt_words_packet(const struct vrt_packet* packet);

#ifdef __cplusplus
}
#endif

#endif
