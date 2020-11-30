#ifndef SRC_VRT_FIXED_POINT_H_
#define SRC_VRT_FIXED_POINT_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __cplusplus
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wbad-function-cast"
#endif

/* Silence warnings when using C++ */
#ifdef __cplusplus
#define VRT_STATIC_CAST(T, X) static_cast<T>(X)
#else
#define VRT_STATIC_CAST(T, X) (T)(X)
#endif

/**
 * Radix position counting from lsb for frequency fields, such as Bandwidth, IF reference frequency, IF reference
 * frequency, RF reference frequency, RF reference frequency offset, IF band offset, and Sample rate.
 */
static const uint32_t VRT_RADIX_FREQUENCY = 20;
/**
 * Radix position counting from lsb for Reference level field.
 */
static const uint32_t VRT_RADIX_REFERENCE_LEVEL = 7;
/**
 * Radix position counting from lsb for Gain stage1 and Gain stage 2 fields.
 */
static const uint32_t VRT_RADIX_GAIN = 7;
/**
 * Radix position counting from lsb for Temperature field.
 */
static const uint32_t VRT_RADIX_TEMPERATURE = 6;
/**
 * Radix position counting from lsb for Angle fields, such as Latitude, Longitude, Heading angle, Track angle, Magnetic
 * variation, Attitude alpha, Attitude beta, and Attitude phi.
 */
static const uint32_t VRT_RADIX_ANGLE = 22;
/**
 * Radix position counting from lsb for Altitude field.
 */
static const uint32_t VRT_RADIX_ALTITUDE = 5;
/**
 * Radix position counting from lsb for speed or velocity fields such as Speed over ground, Velocity dX, Velocity dY,
 * and Velocity dZ.
 */
static const uint32_t VRT_RADIX_SPEED_VELOCITY = 16;
/**
 * Radix position counting from lsb for position fields such as Position X, Position Y, and Position Z.
 */
static const uint32_t VRT_RADIX_POSITION = 5;

/**
 * Round float to nearest int16.
 *
 * \param f Float.
 * \return int16.
 *
 * \note This is implemented in order to reduce dependency on math library for roundf().
 *
 * \warning A number outside range leads to undefined behaviour.
 */
inline int16_t vrt_round_f_to_i16(float f) {
    return VRT_STATIC_CAST(int16_t, f >= 0.0F ? f + 0.5F : f - 0.5F);
}

/**
 * Round double to nearest int32.
 *
 * \param d double.
 * \return int32.
 *
 * \note This is implemented in order to reduce dependency on math library for round().
 *
 * \warning A number outside range leads to undefined behaviour.
 */
inline int32_t vrt_round_d_to_i32(double d) {
    return VRT_STATIC_CAST(int32_t, d >= 0.0 ? d + 0.5 : d - 0.5);
}

/**
 * Round double to nearest uint32.
 *
 * \param d double.
 * \return uint32.
 *
 * \note This is implemented in order to reduce dependency on math library for round().
 *
 * \warning A number outside range leads to undefined behaviour.
 */
inline uint32_t vrt_round_d_to_u32(double d) {
    return VRT_STATIC_CAST(uint32_t, d >= 0.0 ? d + 0.5 : d - 0.5);
}

/**
 * Round double to nearest int64.
 *
 * \param d double.
 * \return int64.
 *
 * \note This is implemented in order to reduce dependency on math library for round().
 *
 * \warning A number outside range leads to undefined behaviour.
 */
inline int64_t vrt_round_d_to_u64(double d) {
    return VRT_STATIC_CAST(int64_t, d >= 0.0 ? d + 0.5 : d - 0.5);
}

/**
 * Convert int16 fixed point to floating point representation.
 *
 * \param fp Fixed point representation.
 * \param r  Position of radix, from least significant bit.
 *
 * \return Floating point representation.
 */
inline float vrt_fixed_point_i16_to_float(int16_t fp, uint32_t r) {
    return VRT_STATIC_CAST(float, fp) / VRT_STATIC_CAST(float, 1U << r);
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
    return VRT_STATIC_CAST(double, fp) / VRT_STATIC_CAST(double, 1U << r);
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
    return VRT_STATIC_CAST(double, fp) / VRT_STATIC_CAST(double, 1U << r);
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
    return VRT_STATIC_CAST(double, fp) / VRT_STATIC_CAST(double, 1U << r);
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
    return vrt_round_f_to_i16(fp * VRT_STATIC_CAST(float, 1U << r));
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
    return vrt_round_d_to_i32(fp * VRT_STATIC_CAST(double, 1U << r));
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
    return vrt_round_d_to_u32(fp * VRT_STATIC_CAST(double, 1U << r));
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
    return vrt_round_d_to_u64(fp * VRT_STATIC_CAST(double, 1U << r));
}

#ifndef __cplusplus
#pragma GCC diagnostic pop
#endif

#ifdef __cplusplus
}
#endif

#endif
