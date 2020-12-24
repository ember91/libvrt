#include "vrt_time.h"
#include "vrt_error_code.h"
#include "vrt_types.h"

/**
 * Number of picoseconds in a second.
 */
static const uint64_t PS_IN_S = 1000000000000;

static const uint32_t UTC_GPS_OFFSET = 315964800;

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
    if (sample_rate <= 0.0) {
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
    if (sample_rate <= 0.0) {
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

/**
 * Days per month without and with leap days respectively.
 */
static const int days_in_month[2][12] = {
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
};

/**
 * Cumulative days per month without and with leap days respectively.
 */
static const int julian_days_by_month[2][12] = {
    {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334},
    {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335},
};

/**
 * Days per year without and with leap days respectively.
 */
static const int length_of_year[2] = {365, 366};

/* Let's assume people are going to be looking for dates in the future.
   Let's provide some cheats so you can skip ahead.
   This has a 4x speed boost when near 2008.
*/
/* Number of days since epoch on Jan 1st, 2008 GMT */
#define CHEAT_DAYS  (1199145600 / 24 / 60 / 60)
#define CHEAT_YEARS 108

/**
 * True if a year is leap year.
 */
#define IS_LEAP(n) ((!(((n) + 1900) % 400) || (!(((n) + 1900) % 4) && (((n) + 1900) % 100))) != 0)

#define WRAP(a, b, m) ((a) = ((a) < 0) ? ((b)--, (a) + (m)) : (a))

/* Some numbers relating to the gregorian cycle */
static const int64_t years_in_gregorian_cycle = 400;
#define days_in_gregorian_cycle ((365 * 400) + 100 - 4 + 1)

/**
 * Calculate human comprehensible date from seconds.
 *
 * A modified copy of gmtime() from the Bionic C standard library. This has two advantages:
 *   It can be rewritten to use a pre-allocated output, making it thread safe.
 *   We know that the time reference is 1970-01-01, which the standard gmtime() does not guarantee.
 *
 * \param in_time  Time since 1970-01-01 [s].
 * \param t        time [out].
 *
 * \return 0, or error code if error.
 */
static int vrt_gmtime(uint32_t in_time, struct vrt_calendar_time* t) {
    int     v_tm_sec, v_tm_min, v_tm_hour, v_tm_mon, v_tm_wday;
    int64_t v_tm_tday;
    int     leap;
    int64_t m;
    int64_t time   = in_time;
    int64_t year   = 70;
    int     cycles = 0;

    v_tm_sec = (int)(time % 60);
    time /= 60;
    v_tm_min = (int)(time % 60);
    time /= 60;
    v_tm_hour = (int)(time % 24);
    time /= 24;
    v_tm_tday = time;

    WRAP(v_tm_sec, v_tm_min, 60);
    WRAP(v_tm_min, v_tm_hour, 60);
    WRAP(v_tm_hour, v_tm_tday, 24);

    v_tm_wday = (int)((v_tm_tday + 4) % 7);
    if (v_tm_wday < 0)
        v_tm_wday += 7;
    m = v_tm_tday;

    if (m >= CHEAT_DAYS) {
        year = CHEAT_YEARS;
        m -= CHEAT_DAYS;
    }

    if (m >= 0) {
        /* Gregorian cycles, this is huge optimization for distant times */
        cycles = (int)(m / (int64_t)days_in_gregorian_cycle);
        if (cycles) {
            m -= (cycles * (int64_t)days_in_gregorian_cycle);
            year += (cycles * years_in_gregorian_cycle);
        }

        /* int64_ts */
        leap = IS_LEAP(year);
        while (m >= (int64_t)length_of_year[leap]) {
            m -= (int64_t)length_of_year[leap];
            year++;
            leap = IS_LEAP(year);
        }

        /* Months */
        v_tm_mon = 0;
        while (m >= (int64_t)days_in_month[leap][v_tm_mon]) {
            m -= (int64_t)days_in_month[leap][v_tm_mon];
            v_tm_mon++;
        }
    } else {
        year--;

        /* Gregorian cycles */
        cycles = (int)((m / (int64_t)days_in_gregorian_cycle) + 1);
        if (cycles) {
            m -= (cycles * (int64_t)days_in_gregorian_cycle);
            year += (cycles * years_in_gregorian_cycle);
        }

        /* int64_ts */
        leap = IS_LEAP(year);
        while (m < (int64_t)-length_of_year[leap]) {
            m += (int64_t)length_of_year[leap];
            year--;
            leap = IS_LEAP(year);
        }

        /* Months */
        v_tm_mon = 11;
        while (m < (int64_t)-days_in_month[leap][v_tm_mon]) {
            m += (int64_t)days_in_month[leap][v_tm_mon];
            v_tm_mon--;
        }
        m += (int64_t)days_in_month[leap][v_tm_mon];
    }

    t->year = year;

    /* At this point m is less than a year so casting to an int is safe */
    t->mday = (int)m + 1;
    t->yday = julian_days_by_month[leap][v_tm_mon] + (int)m;
    t->sec  = v_tm_sec;
    t->min  = v_tm_min;
    t->hour = v_tm_hour;
    t->mon  = v_tm_mon;
    t->wday = v_tm_wday;

    return 0;
}

int vrt_time_calendar(const struct vrt_packet* packet, double sample_rate, struct vrt_calendar_time* cal_time) {
    /* Start with TSI */
    switch (packet->header.tsi) {
        case VRT_TSI_UTC: {
            int rv = vrt_gmtime(packet->fields.integer_seconds_timestamp, cal_time);
            if (rv < 0) {
                return rv;
            }
            break;
        }
        case VRT_TSI_GPS: {
            int rv = vrt_gmtime(packet->fields.integer_seconds_timestamp + UTC_GPS_OFFSET, cal_time);
            if (rv < 0) {
                return rv;
            }
            break;
        }
        default: {
            return VRT_ERR_INVALID_TSI;
        }
    }

    /* TSF*/
    switch (packet->header.tsf) {
        case VRT_TSF_NONE: {
            cal_time->ps = 0;
            break;
        }
        case VRT_TSF_SAMPLE_COUNT: {
            if (sample_rate <= 0.0) {
                return VRT_ERR_MISSING_SAMPLE_RATE;
            }
            cal_time->ps = packet->fields.fractional_seconds_timestamp / sample_rate * PS_IN_S;
            break;
        }
        case VRT_TSF_REAL_TIME: {
            if (packet->fields.fractional_seconds_timestamp >= PS_IN_S) {
                return VRT_ERR_BOUNDS_REAL_TIME;
            }
            cal_time->ps = packet->fields.fractional_seconds_timestamp;
            break;
        }
        default: {
            return VRT_ERR_INVALID_TSF;
        }
    }

    return 0;
}
