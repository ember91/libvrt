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
