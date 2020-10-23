#ifndef SRC_VRT_UTIL_H_
#define SRC_VRT_UTIL_H_

#include <stdint.h>

#include <vrt/vrt_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Convert bool to uint32.
 *
 * \param b Bool to convert.
 *
 * \return 1 if true and 0 otherwise.
 */
inline uint32_t vrt_b2u(bool b) {
    return b ? 1 : 0;
}

/**
 * Convert uint32 to bool.
 *
 * \param u uint32 to convert.
 *
 * \return false if 0 and true otherwise.
 */
inline uint32_t vrt_u2b(uint32_t u) {
    return u != 0;
}

/**
 * Check if a specific packet type means it has the Stream ID field.
 *
 * \param type Packet type.
 *
 * \return True if it has the Stream ID field.
 */
inline bool vrt_has_stream_id(vrt_packet_type type) {
    return ((uint32_t)type & 0x5U) != 0;
}

/**
 * Check if a packet has the integer timestamp.
 *
 * \param t TSI field.
 *
 * \return true if it has the integer timestamp.
 */
inline bool vrt_has_integer_timestamp(vrt_tsi t) {
    return t != VRT_TSI_NONE;
}

/**
 * Check if a packet has the fractional timestamp.
 *
 * \param t TSF field.
 *
 * \return true if it has the fractional timestamp.
 */
inline bool vrt_has_fractional_timestamp(vrt_tsf t) {
    return t != VRT_TSF_NONE;
}

/**
 * Calculate partial size in 32-bit words of context section, from the information available in the context indicator
 * section.
 *
 * \note This does not take the variable sizes from the GPS ASCII as well as Context association lists fields into
 *       account.
 *
 * \param ind Context indicator field.
 *
 * \return Number of 32-bit words it consists of.
 */
uint32_t vrt_words_context_indicator(const vrt_context_indicators* ind);

#ifdef __cplusplus
}
#endif

#endif
