#ifndef INCLUDE_VRT_VRT_UTIL_H_
#define INCLUDE_VRT_VRT_UTIL_H_

#include <stdbool.h>
#include <stdint.h>

#include "vrt_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Force compiler to warn when return values are ignored */
#ifdef __GNUC__
#define VRT_WARN_UNUSED __attribute__((warn_unused_result))
#elif __clang__
#define VRT_WARN_UNUSED __attribute__((warn_unused_result))
#elif _MSC_VER
#define VRT_WARN_UNUSED _Check_return_
#else
#define VRT_WARN_UNUSED
#endif

/* Silence warnings when using C++ */
#ifdef __cplusplus
#define STATIC_CAST(T, X)      static_cast<T>(X)
#define REINTERPRET_CAST(T, X) reinterpret_cast<T>(X)
#else
#define STATIC_CAST(T, X)      (T)(X)
#define REINTERPRET_CAST(T, X) (T)(X)
#endif

/* Macro for checking if value is inside bounds */
#define VRT_BOUNDS(min, val, max, ret)        \
    do {                                      \
        if ((val) < (min) || (val) > (max)) { \
            return (ret);                     \
        }                                     \
    } while (false)

/* Constants describing minimum and maximum of some floating point values */
static const double VRT_MIN_BANDWIDTH                        = 0.0;
static const double VRT_MAX_BANDWIDTH                        = 8.79e12;
static const double VRT_MINMAX_IF_REFERENCE_FREQUENCY        = 8.79e12;
static const double VRT_MINMAX_RF_REFERENCE_FREQUENCY        = 8.79e12;
static const double VRT_MINMAX_RF_REFERENCE_FREQUENCY_OFFSET = 8.79e12;
static const double VRT_MINMAX_IF_BAND_OFFSET                = 8.79e12;
static const float  VRT_MINMAX_REFERENCE_LEVEL               = 256.0F;
static const float  VRT_MINMAX_GAIN                          = 256.0F;
static const double VRT_MIN_SAMPLE_RATE                      = 0.0;
static const double VRT_MAX_SAMPLE_RATE                      = 8.79e12;
static const float  VRT_MIN_TEMPERATURE                      = -273.15F;
static const float  VRT_MAX_TEMPERATURE                      = 511.984375F;
static const double VRT_MINMAX_LATITUDE                      = 90.0;
static const double VRT_MINMAX_LONGITUDE                     = 180.0;
static const double VRT_MINMAX_ALTITUDE                      = 67108e3;
static const double VRT_MIN_SPEED_OVER_GROUND                = 0.0;
static const double VRT_MAX_SPEED_OVER_GROUND                = 65535.0; /* Misspelling in the standard! */
static const double VRT_MIN_HEADING_ANGLE                    = 0.0;
static const double VRT_MAX_HEADING_ANGLE                    = 359.999999761582;
static const double VRT_MIN_TRACK_ANGLE                      = 0.0;
static const double VRT_MAX_TRACK_ANGLE                      = 359.999999761582;
static const double VRT_MINMAX_MAGNETIC_VARIATION            = 180.0;
static const double VRT_MIN_POSITION                         = -67108864;
static const double VRT_MAX_POSITION                         = 67108863;
static const double VRT_MIN_ATTITUDE                         = -512;
static const double VRT_MAX_ATTITUDE                         = 511;
static const double VRT_MIN_VELOCITY                         = -32768;
static const double VRT_MAX_VELOCITY                         = 32767;

/**
 * Check if a header indicates it is a context packet.
 *
 * \param header Packet header.
 *
 * \return True if it is a IF or Ext context packet.
 *
 * \warning Undefined behaviour if type is outside bounds.
 */
inline bool vrt_is_context(const vrt_header* header) {
    return (STATIC_CAST(uint32_t, header->packet_type) & 0x4U) != 0;
}

/**
 * Check if a indicates it has the Stream ID field.
 *
 * \param header Packet header.
 *
 * \return True if it has the Stream ID field.
 *
 * \warning Undefined behaviour if type is outside bounds.
 */
inline bool vrt_has_stream_id(const vrt_header* header) {
    return (STATIC_CAST(uint32_t, header->packet_type) & 0x5U) != 0;
}

/**
 * Check if platform is little endian. May be used to detect if byte swap is required for VRT data.
 *
 * \return True if little endian.
 */
inline bool vrt_is_platform_little_endian() {
    volatile uint32_t i = 0x01234567; /* Ensure written to memory */
    return *REINTERPRET_CAST(volatile uint8_t*, &i) == 0x67;
}

#ifdef __cplusplus
}
#endif

#endif
