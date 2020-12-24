/*
 * Calculate time difference between two packets.
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
    struct vrt_packet p1;
    struct vrt_packet p2;
    vrt_init_packet(&p1);
    vrt_init_packet(&p2);

    /* Configure packet 1 */
    p1.header.tsi                          = VRT_TSI_UTC;
    p1.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p1.fields.integer_seconds_timestamp    = 5;
    p1.fields.fractional_seconds_timestamp = 7000;

    /* Configure packet 2 */
    p2.header.tsi                          = VRT_TSI_UTC;
    p2.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p2.fields.integer_seconds_timestamp    = 8;
    p2.fields.fractional_seconds_timestamp = 9000;

    /* Calculate time difference */
    struct vrt_time diff;
    int32_t         rv = vrt_time_difference(&p2, &p1, SAMPLE_RATE, &diff);
    if (rv < 0) {
        fprintf(stderr, "Failed to calculate time difference: %s\n", vrt_string_error(rv));
        return EXIT_FAILURE;
    }

    /* Display time difference between packets */
    printf("Time difference of %" PRIi32 " s, %" PRIu64 " ps\n", diff.s, diff.ps);

    return EXIT_SUCCESS;
}
