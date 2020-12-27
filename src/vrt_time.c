#include "vrt_time.h"

#include <stdint.h>

#include "timestamp_to_calendar.h"
#include "vrt_error_code.h"
#include "vrt_types.h"

/**
 * Number of picoseconds in a second.
 */
static const uint64_t PS_IN_S = 1000000000000;

/**
 * Calculate time difference from sample count.
 *
 * \param tsi2        Timestamp integer 2.
 * \param is2         Integer second timestamp 2.
 * \param fs2         Fractional second timestamp 2.
 * \param is1         Integer second timestamp 1.
 * \param fs1         Fractional second timestamp 1.
 * \param sample_rate Sample rate [Hz].
 * \param diff        Time difference [out].
 *
 * \return 0, or error code if error.
 */
static int diff_sample_count(enum vrt_tsi     tsi2,
                             uint32_t         is2,
                             uint64_t         fs2,
                             uint32_t         is1,
                             uint64_t         fs1,
                             double           sample_rate,
                             struct vrt_time* diff) {
    /* Sanity check */
    if (sample_rate <= 0.0) {
        return VRT_ERR_MISSING_SAMPLE_RATE;
    }
    uint64_t sample_rate_i = (uint64_t)sample_rate;
    if (fs2 >= sample_rate_i || fs1 >= sample_rate_i) {
        return VRT_ERR_BOUNDS_SAMPLE_COUNT;
    }

    diff->s = 0;
    if (tsi2 != VRT_TSI_NONE) {
        diff->s = is2 - is1;
    }
    uint64_t d = fs2 - fs1;

    /* Handle negative values */
    if (fs2 < fs1) {
        diff->s--;
        d += sample_rate_i;
    }

    diff->ps = (uint64_t)(d / sample_rate * PS_IN_S);

    return 0;
}

/**
 * Calculate time difference from real time.
 *
 * \param tsi2  Timestamp integer 2.
 * \param is2   Integer second timestamp 2.
 * \param fs2   Fractional second timestamp 2.
 * \param is1   Integer second timestamp 1.
 * \param fs1   Fractional second timestamp 1.
 * \param diff  Time difference [out].
 *
 * \return 0, or error code if error.
 */
static int diff_real_time(enum vrt_tsi     tsi2,
                          uint32_t         is2,
                          uint64_t         fs2,
                          uint32_t         is1,
                          uint64_t         fs1,
                          struct vrt_time* diff) {
    /* Sanity check */
    if (fs2 >= PS_IN_S || fs1 >= PS_IN_S) {
        return VRT_ERR_BOUNDS_REAL_TIME;
    }

    diff->s = 0;
    if (tsi2 != VRT_TSI_NONE) {
        diff->s = is2 - is1;
    }
    diff->ps = fs2 - fs1;

    /* Handle negative values */
    if (fs2 < fs1) {
        diff->s--;
        diff->ps += PS_IN_S;
    }

    return 0;
}

/**
 * Calculate time difference from free running count.
 *
 * \param tsi2        Timestamp integer 2.
 * \param is2         Integer second timestamp 2.
 * \param fs2         Fractional second timestamp 2.
 * \param is1         Integer second timestamp 1.
 * \param fs1         Fractional second timestamp 1.
 * \param sample_rate Sample rate [Hz].
 * \param diff        Time difference [out].
 *
 * \return 0, or error code if error.
 */
static int diff_free_running_count(enum vrt_tsi     tsi2,
                                   uint32_t         is2,
                                   uint64_t         fs2,
                                   uint32_t         is1,
                                   uint64_t         fs1,
                                   double           sample_rate,
                                   struct vrt_time* diff) {
    /* Sanity check */
    if (sample_rate <= 0.0) {
        return VRT_ERR_MISSING_SAMPLE_RATE;
    }

    /* Cannot use floating point modulo */
    uint64_t sample_rate_i = (uint64_t)sample_rate;

    uint64_t d = fs2 < fs1 ? fs1 - fs2 : fs2 - fs1;
    diff->s    = d / sample_rate_i;

    /* TSI may not be set here */
    int32_t diff_s_tsi = is2 - is1;

    /* Handle negative values */
    if (fs2 < fs1) {
        diff->s = -diff->s - 1;
        diff_s_tsi--;
    }

    /* Check that integer seconds and its calculation matches */
    if (tsi2 != VRT_TSI_NONE) {
        if (diff->s != diff_s_tsi) {
            return VRT_ERR_INTEGER_SECONDS_MISMATCH;
        }
    }

    diff->ps = (uint64_t)((double)(d % sample_rate_i) / sample_rate * PS_IN_S);

    return 0;
}

/**
 * Neither TSI nor TSF are set.
 *
 * \param diff Time difference [out].
 *
 * \return 0, or error code if error.
 */
static int diff_none(struct vrt_time* diff) {
    diff->s  = 0;
    diff->ps = 0;

    return 0;
}

/**
 * Calculate time difference assuming TSI is set and not TSF.
 *
 * \param is2    Integer second timestamp 2.
 * \param is1    Integer second timestamp 1.
 * \param diff   Time difference [out].
 *
 * \return 0, or error code if error.
 */
static int diff_tsi(uint32_t is2, uint32_t is1, struct vrt_time* diff) {
    diff->s  = (int32_t)(is2 - is1);
    diff->ps = 0;

    return 0;
}

/**
 * Calculate time difference assuming TSF is set and not TSI.
 *
 * \param tsi2        Timestamp integer 2.
 * \param tsf2        Timestamp fractional 2.
 * \param is2         Integer second timestamp 2.
 * \param fs2         Fractional second timestamp 2.
 * \param is1         Integer second timestamp 1.
 * \param fs1         Fractional second timestamp 1.
 * \param sample_rate Sample rate [Hz].
 * \param diff        Time difference [out].
 *
 * \return 0, or error code if error.
 */
static int diff_tsf(enum vrt_tsi     tsi2,
                    enum vrt_tsf     tsf2,
                    uint32_t         is2,
                    uint64_t         fs2,
                    uint32_t         is1,
                    uint64_t         fs1,
                    double           sample_rate,
                    struct vrt_time* diff) {
    switch (tsf2) {
        case VRT_TSF_NONE: {
            /* Should never end up here */
            break;
        }
        case VRT_TSF_SAMPLE_COUNT: {
            return diff_sample_count(tsi2, is2, fs2, is1, fs1, sample_rate, diff);
        }
        case VRT_TSF_REAL_TIME: {
            return diff_real_time(tsi2, is2, fs2, is1, fs1, diff);
        }
        case VRT_TSF_FREE_RUNNING_COUNT: {
            return diff_free_running_count(tsi2, is2, fs2, is1, fs1, sample_rate, diff);
        }
    }

    return 0;
}

/**
 * Calculate time difference assuming TSI and TSF are set.
 *
 * \param tsi2        Timestamp integer 2.
 * \param tsf2        Timestamp fractional 2.
 * \param is2         Integer second timestamp 2.
 * \param fs2         Fractional second timestamp 2.
 * \param is1         Integer second timestamp 1.
 * \param fs1         Fractional second timestamp 1.
 * \param sample_rate Sample rate [Hz].
 * \param diff        Time difference [out].
 *
 * \return 0, or error code if error.
 */
static int diff_tsi_and_tsf(enum vrt_tsi     tsi2,
                            enum vrt_tsf     tsf2,
                            uint32_t         is2,
                            uint64_t         fs2,
                            uint32_t         is1,
                            uint64_t         fs1,
                            double           sample_rate,
                            struct vrt_time* diff) {
    switch (tsf2) {
        case VRT_TSF_NONE: {
            /* Should never end up here */
            break;
        }
        case VRT_TSF_SAMPLE_COUNT: {
            return diff_sample_count(tsi2, is2, fs2, is1, fs1, sample_rate, diff);
        }
        case VRT_TSF_REAL_TIME: {
            return diff_real_time(tsi2, is2, fs2, is1, fs1, diff);
        }
        case VRT_TSF_FREE_RUNNING_COUNT: {
            /* Ignore integer timestamp here since it is less precise */
            return diff_free_running_count(tsi2, is2, fs2, is1, fs1, sample_rate, diff);
        }
    }

    return 0;
}

int vrt_time_difference_fields(const struct vrt_header* h2,
                               const struct vrt_fields* f2,
                               const struct vrt_header* h1,
                               const struct vrt_fields* f1,
                               double                   sample_rate,
                               struct vrt_time*         diff) {
    enum vrt_tsi tsi2 = h2->tsi;
    enum vrt_tsf tsf2 = h2->tsf;
    uint32_t     is2  = f2->integer_seconds_timestamp;
    uint64_t     fs2  = f2->fractional_seconds_timestamp;
    enum vrt_tsi tsi1 = h1->tsi;
    enum vrt_tsf tsf1 = h1->tsf;
    uint32_t     is1  = f1->integer_seconds_timestamp;
    uint64_t     fs1  = f1->fractional_seconds_timestamp;

    /* Sanity check */
    if (tsi2 != tsi1 || tsf2 != tsf1) {
        return VRT_ERR_MISMATCH_TIME_TYPES;
    }

    if (tsi2 == VRT_TSI_NONE && tsf2 == VRT_TSF_NONE) {
        return diff_none(diff);
    }
    if (tsi2 != VRT_TSI_NONE && tsf2 == VRT_TSF_NONE) {
        return diff_tsi(is2, is1, diff);
    }
    if (tsi2 == VRT_TSI_NONE && tsf2 != VRT_TSF_NONE) {
        return diff_tsf(tsi2, tsf2, is2, fs2, is1, fs1, sample_rate, diff);
    }
    /* tsi2 != VRT_TSI_NONE && tsf2 != VRT_TSF_NONE */
    return diff_tsi_and_tsf(tsi2, tsf2, is2, fs2, is1, fs1, sample_rate, diff);
}

int vrt_time_calendar_fields(const struct vrt_header*  header,
                             const struct vrt_fields*  fields,
                             double                    sample_rate,
                             struct vrt_calendar_time* cal_time) {
    return timestamp_to_calendar(header->tsi, header->tsf, fields->integer_seconds_timestamp,
                                 fields->fractional_seconds_timestamp, sample_rate, cal_time);
}

int vrt_time_difference(const struct vrt_packet* p2,
                        const struct vrt_packet* p1,
                        double                   sample_rate,
                        struct vrt_time*         diff) {
    return vrt_time_difference_fields(&p2->header, &p2->fields, &p1->header, &p1->fields, sample_rate, diff);
}

int vrt_time_calendar_gps_geolocation(const struct vrt_if_context* if_context,
                                      double                       sample_rate,
                                      struct vrt_calendar_time*    cal_time) {
    if (!if_context->has.formatted_gps_geolocation) {
        return VRT_ERR_EXPECTED_FIELD;
    }
    const struct vrt_formatted_geolocation* geo = &if_context->formatted_gps_geolocation;
    return timestamp_to_calendar(geo->tsi, geo->tsf, geo->integer_second_timestamp, geo->fractional_second_timestamp,
                                 sample_rate, cal_time);
}

int vrt_time_calendar_ins_geolocation(const struct vrt_if_context* if_context,
                                      double                       sample_rate,
                                      struct vrt_calendar_time*    cal_time) {
    if (!if_context->has.formatted_ins_geolocation) {
        return VRT_ERR_EXPECTED_FIELD;
    }
    const struct vrt_formatted_geolocation* geo = &if_context->formatted_ins_geolocation;
    return timestamp_to_calendar(geo->tsi, geo->tsf, geo->integer_second_timestamp, geo->fractional_second_timestamp,
                                 sample_rate, cal_time);
}

int vrt_time_calendar_ecef_ephemeris(const struct vrt_if_context* if_context,
                                     double                       sample_rate,
                                     struct vrt_calendar_time*    cal_time) {
    if (!if_context->has.ecef_ephemeris) {
        return VRT_ERR_EXPECTED_FIELD;
    }
    const struct vrt_ephemeris* eph = &if_context->ecef_ephemeris;
    return timestamp_to_calendar(eph->tsi, eph->tsf, eph->integer_second_timestamp, eph->fractional_second_timestamp,
                                 sample_rate, cal_time);
}

int vrt_time_calendar_relative_ephemeris(const struct vrt_if_context* if_context,
                                         double                       sample_rate,
                                         struct vrt_calendar_time*    cal_time) {
    if (!if_context->has.relative_ephemeris) {
        return VRT_ERR_EXPECTED_FIELD;
    }
    const struct vrt_ephemeris* eph = &if_context->relative_ephemeris;
    return timestamp_to_calendar(eph->tsi, eph->tsf, eph->integer_second_timestamp, eph->fractional_second_timestamp,
                                 sample_rate, cal_time);
}

int vrt_time_calendar_calibration(const struct vrt_header*     header,
                                  const struct vrt_if_context* if_context,
                                  struct vrt_calendar_time*    cal_time) {
    if (!if_context->has.timestamp_calibration_time) {
        return VRT_ERR_EXPECTED_FIELD;
    }
    return timestamp_to_calendar(header->tsi, VRT_TSF_NONE, if_context->timestamp_calibration_time, 0, 0.0, cal_time);
}

int vrt_time_calendar(const struct vrt_packet* packet, double sample_rate, struct vrt_calendar_time* cal_time) {
    return timestamp_to_calendar(packet->header.tsi, packet->header.tsf, packet->fields.integer_seconds_timestamp,
                                 packet->fields.fractional_seconds_timestamp, sample_rate, cal_time);
}
