#include "timestamp_to_calendar.h"

#include "vrt_error_code.h"
#include "vrt_types.h"

/**
 * Number of picoseconds in a second.
 */
static const uint64_t PS_IN_S = 1000000000000;

/**
 * Offset between GPS and UTC time [s].
 */
static const uint32_t UTC_GPS_OFFSET = 315964800;

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
 * Calculate calendar representation date from seconds.
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
    int32_t v_tm_sec  = 0;
    int32_t v_tm_min  = 0;
    int32_t v_tm_hour = 0;
    int32_t v_tm_mon  = 0;
    int32_t v_tm_wday = 0;
    int64_t v_tm_tday = 0;

    int     leap   = 0;
    int64_t m      = 0;
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
    if (v_tm_wday < 0) {
        v_tm_wday += 7;
    }
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

int timestamp_to_calendar(enum vrt_tsi              tsi,
                          enum vrt_tsf              tsf,
                          uint32_t                  ts_int,
                          uint64_t                  ts_frac,
                          double                    sample_rate,
                          struct vrt_calendar_time* cal_time) {
    /* Start with TSI */
    switch (tsi) {
        case VRT_TSI_UTC: {
            int rv = vrt_gmtime(ts_int, cal_time);
            if (rv < 0) {
                return rv;
            }
            break;
        }
        case VRT_TSI_GPS: {
            int rv = vrt_gmtime(ts_int + UTC_GPS_OFFSET, cal_time);
            if (rv < 0) {
                return rv;
            }
            break;
        }
        case VRT_TSI_NONE:
        case VRT_TSI_OTHER:
        default: {
            return VRT_ERR_INVALID_TSI;
        }
    }

    /* TSF*/
    switch (tsf) {
        case VRT_TSF_NONE: {
            cal_time->ps = 0;
            break;
        }
        case VRT_TSF_SAMPLE_COUNT: {
            if (sample_rate <= 0.0) {
                return VRT_ERR_MISSING_SAMPLE_RATE;
            }
            if (ts_frac >= (uint64_t)sample_rate) {
                return VRT_ERR_BOUNDS_SAMPLE_COUNT;
            }
            cal_time->ps = (uint64_t)(ts_frac / sample_rate * PS_IN_S);
            break;
        }
        case VRT_TSF_REAL_TIME: {
            if (ts_frac >= PS_IN_S) {
                return VRT_ERR_BOUNDS_REAL_TIME;
            }
            cal_time->ps = ts_frac;
            break;
        }
        case VRT_TSF_FREE_RUNNING_COUNT:
        default: {
            return VRT_ERR_INVALID_TSF;
        }
    }

    return 0;
}
