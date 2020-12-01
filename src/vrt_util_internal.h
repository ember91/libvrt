#ifndef SRC_VRT_UTIL_H_
#define SRC_VRT_UTIL_H_

#include <stdbool.h>
#include <stdint.h>

#include "vrt_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Value when TSI is unspecified in Formatted GPS/INS geolocation and ECEF/Relative ephemeris.
 */
static const uint32_t VRT_UNSPECIFIED_TSI = 0xFFFFFFFFU;
/**
 * Value when TSF is unspecified in Formatted GPS/INS geolocation and ECEF/Relative ephemeris.
 */
static const uint64_t VRT_UNSPECIFIED_TSF = 0xFFFFFFFFFFFFFFFFU;
/**
 * Value when fixed point number fields are unspecified in Formatted GPS/INS geolocation and ECEF/Relative ephemeris.
 */
static const uint32_t VRT_UNSPECIFIED_FIXED_POINT = 0x7FFFFFFFU;

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
 * Check if a packet has the fractional timestamp.
 *
 * \param t TSF field.
 *
 * \return true if it has the fractional timestamp.
 */
inline bool vrt_has_fractional_timestamp(enum vrt_tsf t) {
    return t != VRT_TSF_NONE;
}

/**
 * Calculate partial size in 32-bit words of IF context section, from the information available in the context indicator
 * section.
 *
 * \note This does not take the variable sizes from the GPS ASCII as well as Context association lists fields into
 *       account.
 *
 * \param ind Context indicator field.
 *
 * \return Number of 32-bit words it consists of.
 */
int32_t vrt_words_if_context_indicator(const struct vrt_context_indicators* ind);

#ifdef __cplusplus
}
#endif

#endif
