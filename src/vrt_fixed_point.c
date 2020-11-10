#include "vrt_fixed_point.h"

/* Defined inline in header */
extern inline int16_t  vrt_round_f_to_i16(float f);
extern inline uint32_t vrt_round_d_to_u32(double d);
extern inline int32_t  vrt_round_d_to_i32(double d);
extern inline int64_t  vrt_round_d_to_u64(double d);
extern inline float    vrt_fixed_point_i16_to_float(int16_t fp, uint32_t r);
extern inline double   vrt_fixed_point_i32_to_double(int32_t fp, uint32_t r);
extern inline double   vrt_fixed_point_u32_to_double(uint32_t fp, uint32_t r);
extern inline double   vrt_fixed_point_i64_to_double(int64_t fp, uint32_t r);
extern inline int16_t  vrt_float_to_fixed_point_i16(float fp, uint32_t r);
extern inline int32_t  vrt_double_to_fixed_point_i32(double fp, uint32_t r);
extern inline uint32_t vrt_double_to_fixed_point_u32(double fp, uint32_t r);
extern inline int64_t  vrt_double_to_fixed_point_i64(double fp, uint32_t r);
