#ifndef INCLUDE_VRT_VRT_TIME_H_
#define INCLUDE_VRT_VRT_TIME_H_

#include <stdint.h>

#include "vrt_types.h"
#include "vrt_util.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Timestamp in whole and fractional seconds.
 *
 * \note ps is always positive. So -3.5 is represented as s = -4, ps = 5e11.
 */
struct vrt_time {
    int32_t  s;  /**< Seconds. */
    uint64_t ps; /**< Picoseconds. Positive. */
};

/**
 * Human comprehensible calendar date/time format. Similar to C standard library struct tm, but with an extra field for
 * picoseconds.
 */
struct vrt_calendar_time {
    int32_t  year; /**< Number of years since 1900. */
    int32_t  mday; /**< Day of month [1, 31]. */
    int32_t  yday; /**< Day in year [0, 365]. */
    int32_t  sec;  /**< Seconds [0, 59]. */
    int32_t  min;  /**< Minutes [0, 59]. */
    int32_t  hour; /**< Hours [0, 23]. */
    int32_t  mon;  /**< Month [0, 11]. */
    int32_t  wday; /**< Day of week, starting with Sunday [0, 6]. */
    uint64_t ps;   /**< Picoseconds [0, 999999999999]. */
};

/**
 * Calculate positive or negative time difference between two packets in whole and fractional seconds.
 *
 * \note Ignores any IF context sample rate field in packet. Use sample_rate parameter instead.
 *
 * \note If both TSI and TSF are NONE, it is regarded as no time difference.
 *
 * \param p2            Packet 2.
 * \param p1            Packet 1.
 * \param sample_rate   Sample rate [Hz].
 * \param diff          Time difference [out].
 *
 * \note Time difference is positive if p2 happens after p1.
 *
 * \return 0, or error code if error.
 * \retval VRT_ERR_MISMATCH_TIME_TYPES      TSI and/or TSF timestamps differ between packets.
 * \retval VRT_ERR_MISSING_SAMPLE_RATE      Sample rate is required but is not provided (<= 0).
 * \retval VRT_ERR_BOUNDS_SAMPLE_COUNT      Fractional timestamp is outside valid bounds (>= sample rate).
 * \retval VRT_ERR_BOUNDS_REAL_TIME         TSF is VRT_TSF_REAL_TIME but picoseconds is outside valid bounds
 *                                          (> 999999999999 ps).
 * \retval VRT_ERR_INTEGER_SECONDS_MISMATCH Timestamp integer seconds and calculated seconds from the Free running count
 *                                          fractional part differ.
 */
VRT_WARN_UNUSED
int vrt_time_difference(const struct vrt_packet* p2,
                        const struct vrt_packet* p1,
                        double                   sample_rate,
                        struct vrt_time*         diff);

/**
 * Generate a human-readable form of packet timestamp. Only applicable when TSI is UTC or GPS.
 *
 * \note GPS timestamps may seem a bit off since they do not take leap seconds in consideration.
 *
 * \param packet      Packet.
 * \param sample_rate Sample rate [Hz].
 * \param cal_time    Human readable form of a time point, e.g. year, month, day in month, ... [out].
 *
 * \return 0, or error code if error.
 * \retval VRT_ERR_INVALID_TSI         TSI is an invalid value.
 * \retval VRT_ERR_INVALID_TSF         TSF is an invalid value.
 * \retval VRT_ERR_MISSING_SAMPLE_RATE Sample rate is required but is not provided (<= 0).
 * \retval VRT_ERR_BOUNDS_REAL_TIME    TSF is VRT_TSF_REAL_TIME but picoseconds is outside valid bounds
 *                                     (> 999999999999 ps).
 */
VRT_WARN_UNUSED
int vrt_time_calendar(const struct vrt_packet* packet, double sample_rate, struct vrt_calendar_time* t);

#ifdef __cplusplus
}
#endif

#endif
