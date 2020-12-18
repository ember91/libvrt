#ifndef INCLUDE_VRT_VRT_TIME_H_
#define INCLUDE_VRT_VRT_TIME_H_

#include <stdint.h>

#include "vrt_types.h"
#include "vrt_util.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Time duration in whole and fractional seconds.
 *
 * \note ps is always positive. So -3.5 is represented as s = -4, ps = 5e11.
 */
struct vrt_time {
    int32_t  s;  /**< Seconds. */
    uint64_t ps; /**< Picoseconds. Positive. */
};

/**
 * Calculate positive or negative time duration between two packets in whole and fractional seconds.
 *
 * \note Ignores any IF context sample rate field in packet. Use sample_rate parameter instead.
 *
 * \note If both TSI and TSF are NONE, it is regarded as no time difference.
 *
 * \param p1            Packet 1.
 * \param p2            Packet 2.
 * \param sample_rate   Sample rate [Hz].
 * \param diff          Time difference [out].
 *
 * \note Time duration is positive if p1 happens after p2.
 *
 * \return 0, or error code if error.
 * \retval VRT_ERR_MISMATCH_TIME_TYPES      TSI and/or TSF timestamps differ between packets.
 * \retval VRT_ERR_MISSING_SAMPLE_RATE      Sample rate is required but is not provided.
 * \retval VRT_ERR_BOUNDS_SAMPLE_COUNT      Fractional timestamp is outside valid bounds (>= sample rate).
 * \retval VRT_ERR_BOUNDS_REAL_TIME         TSF is VRT_TSF_REAL_TIME but picoseconds is outside valid bounds
 *                                          (> 999999999999 ps).
 * \retval VRT_ERR_INTEGER_SECONDS_MISMATCH Timestamp integer seconds and calculated seconds from the Free running count
 *                                          fractional part differ.
 */
VRT_WARN_UNUSED
int vrt_time_duration(const struct vrt_packet* p1,
                      const struct vrt_packet* p2,
                      double                   sample_rate,
                      struct vrt_time*         diff);

#ifdef __cplusplus
}
#endif

#endif
