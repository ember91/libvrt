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
    struct vrt_header h1;
    struct vrt_fields f1;
    struct vrt_header h2;
    struct vrt_fields f2;
    vrt_init_header(&h1);
    vrt_init_fields(&f1);
    vrt_init_header(&h2);
    vrt_init_fields(&f2);

    /* Configure packet 1 */
    h1.tsi                          = VRT_TSI_UTC;
    h1.tsf                          = VRT_TSF_SAMPLE_COUNT;
    f1.integer_seconds_timestamp    = 5;
    f1.fractional_seconds_timestamp = 7000000;

    /* Configure packet 2 */
    h2.tsi                          = VRT_TSI_UTC;
    h2.tsf                          = VRT_TSF_SAMPLE_COUNT;
    f2.integer_seconds_timestamp    = 8;
    f2.fractional_seconds_timestamp = 9000000;

    /* Calculate time difference */
    struct vrt_time diff;
    int32_t         rv = vrt_time_difference_fields(&h2, &f2, &h1, &f1, SAMPLE_RATE, &diff);
    if (rv < 0) {
        fprintf(stderr, "Failed to calculate time difference: %s\n", vrt_string_error(rv));
        return EXIT_FAILURE;
    }

    /* Display time difference between packets */
    printf("Time difference of %" PRIi32 ".%012" PRIu64 " s\n", diff.s, diff.ps);

    return EXIT_SUCCESS;
}
