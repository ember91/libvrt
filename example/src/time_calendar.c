/*
 * Calculate calendar time for a packet.
 */

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <vrt/vrt_init.h>
#include <vrt/vrt_string.h>
#include <vrt/vrt_time.h>
#include <vrt/vrt_types.h>

/* Sample rate [Hz] */
#define SAMPLE_RATE 16e6

int main() {
    /* Initialize to reasonable values */
    struct vrt_packet p;
    vrt_init_packet(&p);

    /* Configure packet */
    p.header.tsi                          = VRT_TSI_UTC;
    p.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p.fields.integer_seconds_timestamp    = 5;
    p.fields.integer_seconds_timestamp    = 1608751092;
    p.fields.fractional_seconds_timestamp = 1e6;

    /* Calculate calendar time */
    struct vrt_calendar_time time;
    int32_t                  rv = vrt_time_calendar(&p, SAMPLE_RATE, &time);
    if (rv < 0) {
        fprintf(stderr, "Failed to calculate calendar time: %s\n", vrt_string_error(rv));
        return EXIT_FAILURE;
    }

    /* Display time difference between packets */
    printf("Calendar time: %" PRIi32 "-%02" PRIi32 "-%02" PRIi32 " %" PRIi32 ":%" PRIi32 ":%" PRIi32 ".%012" PRIu64
           ". Week day %" PRIi32 " of day of year %" PRIi32 ".\n",
           1900 + time.year, 1 + time.mon, time.mday, time.hour, time.min, time.sec, time.ps, 1 + time.wday,
           1 + time.yday);

    return EXIT_SUCCESS;
}
