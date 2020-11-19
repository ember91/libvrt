#ifndef INCLUDE_VRT_VRT_UTIL_H_
#define INCLUDE_VRT_VRT_UTIL_H_

#include <stdint.h>

#include <vrt/vrt_types.h>

#ifdef __cplusplus
extern "C" {
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
    return *REINTERPRET_CAST(volatile uint8_t*, &i) == 0x67;
}

#ifdef __cplusplus
}
#endif

#endif
