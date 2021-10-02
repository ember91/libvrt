/*
 * Generate VRT IF context packet and write to file. Note that this will not generate a big endian-format, i.e. standard
 * conforming, packet on a little endian platform.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <vrt/vrt_init.h>
#include <vrt/vrt_string.h>
#include <vrt/vrt_types.h>
#include <vrt/vrt_util.h>
#include <vrt/vrt_words.h>
#include <vrt/vrt_write.h>

/* Size of buffer in 32-bit words */
#define SIZE 515

int main() {
    /* Packet data buffer */
    uint32_t b[SIZE];

    /* Initialize to reasonable values. Note that context packets cannot have a trailer word. */
    struct vrt_header     h;
    struct vrt_fields     f;
    struct vrt_if_context c;
    vrt_init_header(&h);
    vrt_init_fields(&f);
    vrt_init_if_context(&c);

    /* Configure */
    h.packet_type     = VRT_PT_IF_CONTEXT;
    f.stream_id       = 0xDEADBEEF;
    c.has.bandwidth   = true;
    c.has.sample_rate = true;
    c.has.temperature = true;
    c.bandwidth       = 2.4e9;
    c.sample_rate     = 2e6;
    c.temperature     = 24.0F;
    h.packet_size     = VRT_WORDS_HEADER + vrt_words_fields(&h) + vrt_words_if_context(&c);

    /* Write header to buffer */
    int32_t offset = 0;
    int32_t rv     = vrt_write_header(&h, b + offset, SIZE - offset, true);
    if (rv < 0) {
        fprintf(stderr, "Failed to write header: %s\n", vrt_string_error(rv));
        return EXIT_FAILURE;
    }
    offset += rv;

    /* Write fields to buffer */
    rv = vrt_write_fields(&h, &f, b + offset, SIZE - offset, true);
    if (rv < 0) {
        fprintf(stderr, "Failed to write fields section: %s\n", vrt_string_error(rv));
        return EXIT_FAILURE;
    }
    offset += rv;

    /* Write IF context to buffer */
    rv = vrt_write_if_context(&c, b + offset, SIZE - offset, true);
    if (rv < 0) {
        fprintf(stderr, "Failed to write IF context section: %s\n", vrt_string_error(rv));
        return EXIT_FAILURE;
    }
    offset += rv;

    /* Write generated packet to file */
    const char* file_path = "context.vrt";
    FILE*       fp        = fopen(file_path, "wb");
    if (fp == NULL) {
        fprintf(stderr, "Failed to open file '%s'\n", file_path);
        return EXIT_FAILURE;
    }
    if (fwrite(b, sizeof(uint32_t) * h.packet_size, 1, fp) != 1) {
        fprintf(stderr, "Failed to write to file '%s'\n", file_path);
        fclose(fp);
        return EXIT_FAILURE;
    }
    fclose(fp);

    /* Warn if not standards compliant */
    if (vrt_is_platform_little_endian()) {
        fprintf(stderr, "Warning: Written packet is little endian. It is NOT compliant with the VRT standard.\n");
    }

    return EXIT_SUCCESS;
}
