#ifndef SRC_VRT_TIMESTAMP_TO_CALENDAR_H_
#define SRC_VRT_TIMESTAMP_TO_CALENDAR_H_

#include <stdint.h>

#include "vrt_types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct vrt_calendar_time;

/**
 * Convert time stamp to calendar format.
 *
 * \param tsi         TSI.
 * \param tsf         TSF.
 * \param ts_int      Integer seconds timestamp.
 * \param ts_frac     Fractional second timestamp.
 * \param sample_rate Sample rate [Hz].
 * \param cal_time    Calendar time [out].
 *
 * \return 0, or error code if error.
 */
int timestamp_to_calendar(enum vrt_tsi              tsi,
                          enum vrt_tsf              tsf,
                          uint32_t                  ts_int,
                          uint64_t                  ts_frac,
                          double                    sample_rate,
                          struct vrt_calendar_time* cal_time);

#endif

#ifdef __cplusplus
}
#endif
