#ifndef SRC_VRT_FIXED_POINT_H_
#define SRC_VRT_FIXED_POINT_H_

#include <math.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __cplusplus
#pragma GCC diagnostic ignored "-Wbad-function-cast"
#pragma GCC diagnostic push
#endif

/* Silence warnings when using C++ */
#ifdef __cplusplus
#define STATIC_CAST(T, X) static_cast<T>(X)
#else
#define STATIC_CAST(T, X) (T)(X)
#endif

/**
 * Convert int16 fixed point to floating point representation.
 *
 * \param fp Fixed point representation.
 * \param r  Position of radix, from least significant bit.
 *
 * \return Floating point representation.
 */
inline float vrt_fixed_point_i16_to_float(int16_t fp, uint32_t r) {
    return STATIC_CAST(float, fp) / STATIC_CAST(float, 1U << r);
}

/**
 * Convert int32 fixed point to floating point representation.
 *
 * \param fp Fixed point representation.
 * \param r  Position of radix, from least significant bit.
 *
 * \return Floating point representation.
 */
inline double vrt_fixed_point_i32_to_double(int32_t fp, uint32_t r) {
    return STATIC_CAST(double, fp) / STATIC_CAST(double, 1U << r);
}

/**
 * Convert uint32 fixed point to floating point representation.
 *
 * \param fp Fixed point representation.
 * \param r  Position of radix, from least significant bit.
 *
 * \return Floating point representation.
 */
inline double vrt_fixed_point_u32_to_double(uint32_t fp, uint32_t r) {
    return STATIC_CAST(double, fp) / STATIC_CAST(double, 1U << r);
}

/**
 * Convert int64 fixed point to floating point representation.
 *
 * \param fp Fixed point representation.
 * \param r  Position of radix, from least significant bit.
 *
 * \return Floating point representation.
 *
 * \warning This may lead to loss of precision, since double has 52 bits of precision but the fixed point format 64 bits
 *          of precision. Thus, there is a loss of precision of 12 bits. In practice, this should not be any issue,
 *          though.
 */
inline double vrt_fixed_point_i64_to_double(int64_t fp, uint32_t r) {
    return STATIC_CAST(double, fp) / STATIC_CAST(double, 1U << r);
}

/**
 * Convert floating point to int16 fixed point representation.
 *
 * \param fp Floating point representation.
 * \param r  Position of radix, from least significant bit.
 *
 * \return Fixed point representation.
 */
inline int16_t vrt_float_to_fixed_point_i16(float fp, uint32_t r) {
    return STATIC_CAST(int16_t, roundf(fp * STATIC_CAST(float, 1U << r)));
}

/**
 * Convert floating point to int32 fixed point representation.
 *
 * \param fp Floating point representation.
 * \param r  Position of radix, from least significant bit.
 *
 * \return Fixed point representation.
 */
inline int32_t vrt_double_to_fixed_point_i32(double fp, uint32_t r) {
    return STATIC_CAST(int32_t, round(fp * STATIC_CAST(double, 1U << r)));
}

/**
 * Convert floating point to uint32 fixed point representation.
 *
 * \param fp Floating point representation.
 * \param r  Position of radix, from least significant bit.
 *
 * \return Fixed point representation.
 */
inline uint32_t vrt_double_to_fixed_point_u32(double fp, uint32_t r) {
    return STATIC_CAST(uint32_t, round(fp * STATIC_CAST(double, 1U << r)));
}

/**
 * Convert floating point to int64 fixed point representation.
 *
 * \param fp Fixed point representation.
 * \param r  Position of radix, from least significant bit.
 *
 * \return Fixed point representation.
 */
inline int64_t vrt_double_to_fixed_point_i64(double fp, uint32_t r) {
    return STATIC_CAST(int64_t, round(fp * STATIC_CAST(double, 1U << r)));
}

#ifndef __cplusplus
#pragma GCC diagnostic pop
#endif

#ifdef __cplusplus
}
#endif

#endif
