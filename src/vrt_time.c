#include "vrt_time.h"
#include "vrt_error_code.h"
#include "vrt_types.h"

/**
 * Number of picoseconds in a second.
 */
static const uint64_t PS_IN_S = 1000000000000;

static int sample_count(const struct vrt_packet* p1,
                        const struct vrt_packet* p2,
                        double                   sample_rate,
                        struct vrt_time*         diff) {
    /* Sanity check */
    if (sample_rate <= 0) {
        return VRT_ERR_MISSING_SAMPLE_RATE;
    }
    if (p1->fields.fractional_seconds_timestamp >= sample_rate ||
        p2->fields.fractional_seconds_timestamp >= sample_rate) {
        return VRT_ERR_BOUNDS_SAMPLE_COUNT;
    }

    diff->s = 0;
    if (p1->header.tsi != VRT_TSI_NONE) {
        diff->s = p1->fields.integer_seconds_timestamp - p2->fields.integer_seconds_timestamp;
    }
    uint64_t d = p1->fields.fractional_seconds_timestamp - p2->fields.fractional_seconds_timestamp;

    /* Handle negative values */
    if (p1->fields.fractional_seconds_timestamp < p2->fields.fractional_seconds_timestamp) {
        diff->s--;
        d += (uint64_t)sample_rate;
    }

    diff->ps = d / sample_rate * PS_IN_S;

    return 0;
}

static int real_time(const struct vrt_packet* p1, const struct vrt_packet* p2, struct vrt_time* diff) {
    /* Sanity check */
    if (p1->fields.fractional_seconds_timestamp >= PS_IN_S || p2->fields.fractional_seconds_timestamp >= PS_IN_S) {
        return VRT_ERR_BOUNDS_REAL_TIME;
    }

    diff->s = 0;
    if (p1->header.tsi != VRT_TSI_NONE) {
        diff->s = p1->fields.integer_seconds_timestamp - p2->fields.integer_seconds_timestamp;
    }
    diff->ps = p1->fields.fractional_seconds_timestamp - p2->fields.fractional_seconds_timestamp;

    /* Handle negative values */
    if (p1->fields.fractional_seconds_timestamp < p2->fields.fractional_seconds_timestamp) {
        diff->s--;
        diff->ps += PS_IN_S;
    }

    return 0;
}

static int free_running_count(const struct vrt_packet* p1,
                              const struct vrt_packet* p2,
                              double                   sample_rate,
                              struct vrt_time*         diff) {
    /* Sanity check */
    if (sample_rate <= 0) {
        return VRT_ERR_MISSING_SAMPLE_RATE;
    }

    /* Cannot use floating point modulo */
    uint64_t sample_rate_i = (uint64_t)sample_rate;

    uint64_t d = p1->fields.fractional_seconds_timestamp < p2->fields.fractional_seconds_timestamp
                     ? p2->fields.fractional_seconds_timestamp - p1->fields.fractional_seconds_timestamp
                     : p1->fields.fractional_seconds_timestamp - p2->fields.fractional_seconds_timestamp;
    diff->s = d / sample_rate_i;

    /* TSI may not be set here */
    int32_t diff_s_tsi = p1->fields.integer_seconds_timestamp - p2->fields.integer_seconds_timestamp;

    /* Handle negative values */
    if (p1->fields.fractional_seconds_timestamp < p2->fields.fractional_seconds_timestamp) {
        diff->s = -diff->s - 1;
        diff_s_tsi--;
    }

    /* Check that integer seconds and its calculation matches */
    if (p1->header.tsi != VRT_TSI_NONE) {
        if (diff->s != diff_s_tsi) {
            return VRT_ERR_INTEGER_SECONDS_MISMATCH;
        }
    }

    diff->ps = (double)(d % sample_rate_i) / sample_rate * PS_IN_S;

    return 0;
}

static int none(struct vrt_time* diff) {
    diff->s  = 0;
    diff->ps = 0;

    return 0;
}

static int tsi(const struct vrt_packet* p1, const struct vrt_packet* p2, struct vrt_time* diff) {
    diff->s  = (int32_t)(p1->fields.integer_seconds_timestamp - p2->fields.integer_seconds_timestamp);
    diff->ps = 0;

    return 0;
}

static int tsf(const struct vrt_packet* p1, const struct vrt_packet* p2, double sample_rate, struct vrt_time* diff) {
    switch (p1->header.tsf) {
        case VRT_TSF_NONE: {
            /* Should never end up here */
            break;
        }
        case VRT_TSF_SAMPLE_COUNT: {
            return sample_count(p1, p2, sample_rate, diff);
        }
        case VRT_TSF_REAL_TIME: {
            return real_time(p1, p2, diff);
        }
        case VRT_TSF_FREE_RUNNING_COUNT: {
            return free_running_count(p1, p2, sample_rate, diff);
        }
    }

    return 0;
}

static int tsi_and_tsf(const struct vrt_packet* p1,
                       const struct vrt_packet* p2,
                       double                   sample_rate,
                       struct vrt_time*         diff) {
    switch (p1->header.tsf) {
        case VRT_TSF_NONE: {
            /* Should never end up here */
            break;
        }
        case VRT_TSF_SAMPLE_COUNT: {
            return sample_count(p1, p2, sample_rate, diff);
        }
        case VRT_TSF_REAL_TIME: {
            return real_time(p1, p2, diff);
        }
        case VRT_TSF_FREE_RUNNING_COUNT: {
            /* Ignore integer timestamp here since it is less precise */
            return free_running_count(p1, p2, sample_rate, diff);
        }
    }

    return 0;
}

int vrt_time_difference(const struct vrt_packet* p1,
                        const struct vrt_packet* p2,
                        double                   sample_rate,
                        struct vrt_time*         diff) {
    /* Sanity check */
    if (p1->header.tsi != p2->header.tsi || p1->header.tsf != p2->header.tsf) {
        return VRT_ERR_MISMATCH_TIME_TYPES;
    }

    if (p1->header.tsi == VRT_TSI_NONE && p1->header.tsf == VRT_TSF_NONE) {
        return none(diff);
    }
    if (p1->header.tsi != VRT_TSI_NONE && p1->header.tsf == VRT_TSF_NONE) {
        return tsi(p1, p2, diff);
    }
    if (p1->header.tsi == VRT_TSI_NONE && p1->header.tsf != VRT_TSF_NONE) {
        return tsf(p1, p2, sample_rate, diff);
    }
    /* p1->header.tsi != VRT_TSI_NONE && p1->header.tsf != VRT_TSF_NONE */
    return tsi_and_tsf(p1, p2, sample_rate, diff);
}
