#ifndef INCLUDE_VRT_VRT_TIME_H_
#define INCLUDE_VRT_VRT_TIME_H_

#include "vrt_util.h"

#ifdef __cplusplus
extern "C" {
#endif

struct vrt_calendar_time;
struct vrt_fields;
struct vrt_header;
struct vrt_if_context;
struct vrt_packet;
struct vrt_time;

/**
 * Calculate positive or negative time difference between fields sections of two packets in whole and fractional
 * seconds.
 *
 * \note Ignores any IF context sample rate field in packet. Use sample_rate parameter instead.
 *
 * \note If both TSI and TSF are NONE, it is regarded as no time difference.
 *
 * \param h2            Header 2.
 * \param f2            Fields section 2.
 * \param h1            Header 1.
 * \param f1            Fields section 1.
 * \param sample_rate   Sample rate [Hz]. May be set to 0 if TSF isn't VRT_TSF_SAMPLE_COUNT.
 * \param diff          Time difference [out].
 *
 * \note Time difference is positive if p2 happens after p1.
 *
 * \return 0, or error code if error.
 * \retval VRT_ERR_MISMATCH_TIME_TYPES      TSI and/or TSF timestamps differ between packets.
 * \retval VRT_ERR_MISSING_SAMPLE_RATE      Sample rate is required but is not provided (<= 0).
 * \retval VRT_ERR_BOUNDS_SAMPLE_COUNT      Fractional timestamp is outside valid bounds (>= sample rate).
 * \retval VRT_ERR_BOUNDS_REAL_TIME         TSF is VRT_TSF_REAL_TIME but fractional timestamp is outside valid bounds
 *                                          (> 999999999999 ps).
 * \retval VRT_ERR_INTEGER_SECONDS_MISMATCH Timestamp integer seconds and calculated seconds from the Free running count
 *                                          fractional part differ.
 */
VRT_WARN_UNUSED
int vrt_time_difference_fields(const struct vrt_header* h2,
                               const struct vrt_fields* f2,
                               const struct vrt_header* h1,
                               const struct vrt_fields* f1,
                               double                   sample_rate,
                               struct vrt_time*         diff);

/**
 * \deprecated{This will be removed in a future version. Use vrt_time_difference_fields instead.}
 */
VRT_DEPRECATED
VRT_WARN_UNUSED
int vrt_time_difference(const struct vrt_packet* p2,
                        const struct vrt_packet* p1,
                        double                   sample_rate,
                        struct vrt_time*         diff);

/**
 * Generate a calendar representation of the fields section timestamp. Only applicable when header TSI is UTC or GPS.
 *
 * \note GPS timestamps may seem a bit off since they do not take leap seconds in consideration.
 *
 * \param header      Header.
 * \param fields      Fields.
 * \param sample_rate Sample rate [Hz]. May be set to 0 if TSF isn't VRT_TSF_SAMPLE_COUNT.
 * \param cal_time    Calendar representation of a time point, i.e. year, month, day in month, ... [out].
 *
 * \return 0, or error code if error.
 * \retval VRT_ERR_INVALID_TSI          TSI is an invalid value.
 * \retval VRT_ERR_INVALID_TSF          TSF is an invalid value.
 * \retval VRT_ERR_MISSING_SAMPLE_RATE  Sample rate is required but is not provided (<= 0).
 * \retval VRT_ERR_BOUNDS_SAMPLE_COUNT  Fractional timestamp is outside valid bounds (>= sample rate).
 * \retval VRT_ERR_BOUNDS_REAL_TIME     TSF is VRT_TSF_REAL_TIME but fractional timestamp is outside valid bounds
 *                                      (> 999999999999 ps).
 * \retval VRT_ERR_EXPECTED_FIELD       Expected a field that was not present.
 */
VRT_WARN_UNUSED
int vrt_time_calendar_fields(const struct vrt_header*  header,
                             const struct vrt_fields*  fields,
                             double                    sample_rate,
                             struct vrt_calendar_time* cal_time);

/**
 * Generate a calendar representation of IF context GPS geolocation timestamp. Only applicable when GPS geolocation is
 * present and GPS geolocation TSI is UTC or GPS.
 *
 * \note GPS timestamps may seem a bit off since they do not take leap seconds in consideration.
 *
 * \param if_context  IF context.
 * \param sample_rate Sample rate [Hz]. May be set to 0 if TSF isn't VRT_TSF_SAMPLE_COUNT.
 * \param cal_time    Calendar representation of a time point, i.e. year, month, day in month, ... [out].
 *
 * \return 0, or error code if error.
 * \retval VRT_ERR_INVALID_TSI          TSI is an invalid value.
 * \retval VRT_ERR_INVALID_TSF          TSF is an invalid value.
 * \retval VRT_ERR_MISSING_SAMPLE_RATE  Sample rate is required but is not provided (<= 0).
 * \retval VRT_ERR_BOUNDS_SAMPLE_COUNT  Fractional timestamp is outside valid bounds (>= sample rate).
 * \retval VRT_ERR_BOUNDS_REAL_TIME     TSF is VRT_TSF_REAL_TIME but fractional timestamp is outside valid bounds
 *                                      (> 999999999999 ps).
 * \retval VRT_ERR_EXPECTED_FIELD       Expected a field that was not present.
 */
VRT_WARN_UNUSED
int vrt_time_calendar_gps_geolocation(const struct vrt_if_context* if_context,
                                      double                       sample_rate,
                                      struct vrt_calendar_time*    cal_time);

/**
 * Generate a calendar representation of IF context INS geolocation timestamp. Only applicable when INS geolocation is
 * present and INS geolocation TSI is UTC or GPS.
 *
 * \note GPS timestamps may seem a bit off since they do not take leap seconds in consideration.
 *
 * \param if_context  IF context.
 * \param sample_rate Sample rate [Hz]. May be set to 0 if TSF isn't VRT_TSF_SAMPLE_COUNT.
 * \param cal_time    Calendar representation of a time point, i.e. year, month, day in month, ... [out].
 *
 * \return 0, or error code if error.
 * \retval VRT_ERR_INVALID_TSI          TSI is an invalid value.
 * \retval VRT_ERR_INVALID_TSF          TSF is an invalid value.
 * \retval VRT_ERR_MISSING_SAMPLE_RATE  Sample rate is required but is not provided (<= 0).
 * \retval VRT_ERR_BOUNDS_SAMPLE_COUNT  Fractional timestamp is outside valid bounds (>= sample rate).
 * \retval VRT_ERR_BOUNDS_REAL_TIME     TSF is VRT_TSF_REAL_TIME but fractional timestamp is outside valid bounds
 *                                      (> 999999999999 ps).
 * \retval VRT_ERR_EXPECTED_FIELD       Expected a field that was not present.
 */
VRT_WARN_UNUSED
int vrt_time_calendar_ins_geolocation(const struct vrt_if_context* if_context,
                                      double                       sample_rate,
                                      struct vrt_calendar_time*    cal_time);

/**
 * Generate a calendar representation of IF context ECEF ephemeris timestamp. Only applicable when ECEF ephemeris is
 * present and ECEF ephemeris TSI is UTC or GPS.
 *
 * \note GPS timestamps may seem a bit off since they do not take leap seconds in consideration.
 *
 * \param if_context  IF context.
 * \param sample_rate Sample rate [Hz]. May be set to 0 if TSF isn't VRT_TSF_SAMPLE_COUNT.
 * \param cal_time    Calendar representation of a time point, i.e. year, month, day in month, ... [out].
 *
 * \return 0, or error code if error.
 * \retval VRT_ERR_INVALID_TSI          TSI is an invalid value.
 * \retval VRT_ERR_INVALID_TSF          TSF is an invalid value.
 * \retval VRT_ERR_MISSING_SAMPLE_RATE  Sample rate is required but is not provided (<= 0).
 * \retval VRT_ERR_BOUNDS_SAMPLE_COUNT  Fractional timestamp is outside valid bounds (>= sample rate).
 * \retval VRT_ERR_BOUNDS_REAL_TIME     TSF is VRT_TSF_REAL_TIME but fractional timestamp is outside valid bounds
 *                                      (> 999999999999 ps).
 * \retval VRT_ERR_EXPECTED_FIELD       Expected a field that was not present.
 */
VRT_WARN_UNUSED
int vrt_time_calendar_ecef_ephemeris(const struct vrt_if_context* if_context,
                                     double                       sample_rate,
                                     struct vrt_calendar_time*    cal_time);

/**
 * Generate a calendar representation of IF context Relative ephemeris timestamp. Only applicable when Relative
 * ephemeris is present and Relative ephemeris TSI is UTC or GPS.
 *
 * \note GPS timestamps may seem a bit off since they do not take leap seconds in consideration.
 *
 * \param if_context  IF context.
 * \param sample_rate Sample rate [Hz]. May be set to 0 if TSF isn't VRT_TSF_SAMPLE_COUNT.
 * \param cal_time    Calendar representation of a time point, i.e. year, month, day in month, ... [out].
 *
 * \return 0, or error code if error.
 * \retval VRT_ERR_INVALID_TSI          TSI is an invalid value.
 * \retval VRT_ERR_INVALID_TSF          TSF is an invalid value.
 * \retval VRT_ERR_MISSING_SAMPLE_RATE  Sample rate is required but is not provided (<= 0).
 * \retval VRT_ERR_BOUNDS_SAMPLE_COUNT  Fractional timestamp is outside valid bounds (>= sample rate).
 * \retval VRT_ERR_BOUNDS_REAL_TIME     TSF is VRT_TSF_REAL_TIME but fractional timestamp is outside valid bounds
 *                                      (> 999999999999 ps).
 * \retval VRT_ERR_EXPECTED_FIELD       Expected a field that was not present.
 */
VRT_WARN_UNUSED
int vrt_time_calendar_relative_ephemeris(const struct vrt_if_context* if_context,
                                         double                       sample_rate,
                                         struct vrt_calendar_time*    cal_time);

/**
 * Generate a calendar representation of Timestamp calibration time. Only applicable when IF calibration time is present
 * and Calibration time TSI is UTC or GPS.
 *
 * \note GPS timestamps may seem a bit off since they do not take leap seconds in consideration.
 *
 * \param header      Header.
 * \param if_context  IF context.
 * \param cal_time    Calendar representation of a time point, i.e. year, month, day in month, ... [out].
 *
 * \note cal_time.ps will always be 0.
 *
 * \return 0, or error code if error.
 * \retval VRT_ERR_INVALID_TSI          TSI is an invalid value.
 * \retval VRT_ERR_INVALID_TSF          TSF is an invalid value.
 * \retval VRT_ERR_MISSING_SAMPLE_RATE  Sample rate is required but is not provided (<= 0).
 * \retval VRT_ERR_BOUNDS_SAMPLE_COUNT  Fractional timestamp is outside valid bounds (>= sample rate).
 * \retval VRT_ERR_BOUNDS_REAL_TIME     TSF is VRT_TSF_REAL_TIME but fractional timestamp is outside valid bounds
 *                                      (> 999999999999 ps).
 * \retval VRT_ERR_EXPECTED_FIELD       Expected a field that was not present.
 */
VRT_WARN_UNUSED
int vrt_time_calendar_calibration(const struct vrt_header*     header,
                                  const struct vrt_if_context* if_context,
                                  struct vrt_calendar_time*    cal_time);

/**
 * \deprecated{This will be removed in a future version. Use vrt_time_fields instead.}
 */
VRT_DEPRECATED
VRT_WARN_UNUSED
int vrt_time_calendar(const struct vrt_packet* packet, double sample_rate, struct vrt_calendar_time* cal_time);

#ifdef __cplusplus
}
#endif

#endif
