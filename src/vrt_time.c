#include "vrt_time.h"
#include "vrt_error_code.h"
#include "vrt_types.h"

/**
 * Number of picoseconds in a second.
 */
static const uint64_t PS_IN_S = 1000000000000;

/**
 * Calculate time difference from sample count.
 *
 * \param p2          Packet 2.
 * \param p1          Packet 1.
 * \param sample_rate Sample rate [Hz].
 * \param diff        Time difference [out].
 *
 * \return 0, or error code if error.
 */
static int sample_count(const struct vrt_packet* p2,
                        const struct vrt_packet* p1,
                        double                   sample_rate,
                        struct vrt_time*         diff) {
    /* Sanity check */
    if (sample_rate <= 0) {
        return VRT_ERR_MISSING_SAMPLE_RATE;
    }
    if (p2->fields.fractional_seconds_timestamp >= sample_rate ||
        p1->fields.fractional_seconds_timestamp >= sample_rate) {
        return VRT_ERR_BOUNDS_SAMPLE_COUNT;
    }

    diff->s = 0;
    if (p2->header.tsi != VRT_TSI_NONE) {
        diff->s = p2->fields.integer_seconds_timestamp - p1->fields.integer_seconds_timestamp;
    }
    uint64_t d = p2->fields.fractional_seconds_timestamp - p1->fields.fractional_seconds_timestamp;

    /* Handle negative values */
    if (p2->fields.fractional_seconds_timestamp < p1->fields.fractional_seconds_timestamp) {
        diff->s--;
        d += (uint64_t)sample_rate;
    }

    diff->ps = d / sample_rate * PS_IN_S;

    return 0;
}

/**
 * Calculate time difference from real time.
 *
 * \param p2          Packet 2.
 * \param p1          Packet 1.
 * \param sample_rate Sample rate [Hz].
 * \param diff        Time difference [out].
 *
 * \return 0, or error code if error.
 */
static int real_time(const struct vrt_packet* p2, const struct vrt_packet* p1, struct vrt_time* diff) {
    /* Sanity check */
    if (p2->fields.fractional_seconds_timestamp >= PS_IN_S || p1->fields.fractional_seconds_timestamp >= PS_IN_S) {
        return VRT_ERR_BOUNDS_REAL_TIME;
    }

    diff->s = 0;
    if (p2->header.tsi != VRT_TSI_NONE) {
        diff->s = p2->fields.integer_seconds_timestamp - p1->fields.integer_seconds_timestamp;
    }
    diff->ps = p2->fields.fractional_seconds_timestamp - p1->fields.fractional_seconds_timestamp;

    /* Handle negative values */
    if (p2->fields.fractional_seconds_timestamp < p1->fields.fractional_seconds_timestamp) {
        diff->s--;
        diff->ps += PS_IN_S;
    }

    return 0;
}

/**
 * Calculate time difference from free running count.
 *
 * \param p2          Packet 2.
 * \param p1          Packet 1.
 * \param sample_rate Sample rate [Hz].
 * \param diff        Time difference [out].
 *
 * \return 0, or error code if error.
 */
static int free_running_count(const struct vrt_packet* p2,
                              const struct vrt_packet* p1,
                              double                   sample_rate,
                              struct vrt_time*         diff) {
    /* Sanity check */
    if (sample_rate <= 0) {
        return VRT_ERR_MISSING_SAMPLE_RATE;
    }

    /* Cannot use floating point modulo */
    uint64_t sample_rate_i = (uint64_t)sample_rate;

    uint64_t d = p2->fields.fractional_seconds_timestamp < p1->fields.fractional_seconds_timestamp
                     ? p1->fields.fractional_seconds_timestamp - p2->fields.fractional_seconds_timestamp
                     : p2->fields.fractional_seconds_timestamp - p1->fields.fractional_seconds_timestamp;
    diff->s = d / sample_rate_i;

    /* TSI may not be set here */
    int32_t diff_s_tsi = p2->fields.integer_seconds_timestamp - p1->fields.integer_seconds_timestamp;

    /* Handle negative values */
    if (p2->fields.fractional_seconds_timestamp < p1->fields.fractional_seconds_timestamp) {
        diff->s = -diff->s - 1;
        diff_s_tsi--;
    }

    /* Check that integer seconds and its calculation matches */
    if (p2->header.tsi != VRT_TSI_NONE) {
        if (diff->s != diff_s_tsi) {
            return VRT_ERR_INTEGER_SECONDS_MISMATCH;
        }
    }

    diff->ps = (double)(d % sample_rate_i) / sample_rate * PS_IN_S;

    return 0;
}

/**
 * Neither TSI nor TSF are set.
 *
 * \param diff        Time difference [out].
 *
 * \return 0, or error code if error.
 */
static int none(struct vrt_time* diff) {
    diff->s  = 0;
    diff->ps = 0;

    return 0;
}

/**
 * Calculate time difference assuming TSI is set and not TSF.
 *
 * \param p2          Packet 2.
 * \param p1          Packet 1.
 * \param sample_rate Sample rate [Hz].
 * \param diff        Time difference [out].
 *
 * \return 0, or error code if error.
 */
static int tsi(const struct vrt_packet* p2, const struct vrt_packet* p1, struct vrt_time* diff) {
    diff->s  = (int32_t)(p2->fields.integer_seconds_timestamp - p1->fields.integer_seconds_timestamp);
    diff->ps = 0;

    return 0;
}

/**
 * Calculate time difference assuming TSF is set and not TSI.
 *
 * \param p2          Packet 2.
 * \param p1          Packet 1.
 * \param sample_rate Sample rate [Hz].
 * \param diff        Time difference [out].
 *
 * \return 0, or error code if error.
 */
static int tsf(const struct vrt_packet* p2, const struct vrt_packet* p1, double sample_rate, struct vrt_time* diff) {
    switch (p2->header.tsf) {
        case VRT_TSF_NONE: {
            /* Should never end up here */
            break;
        }
        case VRT_TSF_SAMPLE_COUNT: {
            return sample_count(p2, p1, sample_rate, diff);
        }
        case VRT_TSF_REAL_TIME: {
            return real_time(p2, p1, diff);
        }
        case VRT_TSF_FREE_RUNNING_COUNT: {
            return free_running_count(p2, p1, sample_rate, diff);
        }
    }

    return 0;
}

/**
 * Calculate time difference assuming TSI and TSF are set.
 *
 * \param p2          Packet 2.
 * \param p1          Packet 1.
 * \param sample_rate Sample rate [Hz].
 * \param diff        Time difference [out].
 *
 * \return 0, or error code if error.
 */
static int tsi_and_tsf(const struct vrt_packet* p2,
                       const struct vrt_packet* p1,
                       double                   sample_rate,
                       struct vrt_time*         diff) {
    switch (p2->header.tsf) {
        case VRT_TSF_NONE: {
            /* Should never end up here */
            break;
        }
        case VRT_TSF_SAMPLE_COUNT: {
            return sample_count(p2, p1, sample_rate, diff);
        }
        case VRT_TSF_REAL_TIME: {
            return real_time(p2, p1, diff);
        }
        case VRT_TSF_FREE_RUNNING_COUNT: {
            /* Ignore integer timestamp here since it is less precise */
            return free_running_count(p2, p1, sample_rate, diff);
        }
    }

    return 0;
}

int vrt_time_difference(const struct vrt_packet* p2,
                        const struct vrt_packet* p1,
                        double                   sample_rate,
                        struct vrt_time*         diff) {
    /* Sanity check */
    if (p2->header.tsi != p1->header.tsi || p2->header.tsf != p1->header.tsf) {
        return VRT_ERR_MISMATCH_TIME_TYPES;
    }

    if (p2->header.tsi == VRT_TSI_NONE && p2->header.tsf == VRT_TSF_NONE) {
        return none(diff);
    }
    if (p2->header.tsi != VRT_TSI_NONE && p2->header.tsf == VRT_TSF_NONE) {
        return tsi(p2, p1, diff);
    }
    if (p2->header.tsi == VRT_TSI_NONE && p2->header.tsf != VRT_TSF_NONE) {
        return tsf(p2, p1, sample_rate, diff);
    }
    /* p2->header.tsi != VRT_TSI_NONE && p2->header.tsf != VRT_TSF_NONE */
    return tsi_and_tsf(p2, p1, sample_rate, diff);
}
