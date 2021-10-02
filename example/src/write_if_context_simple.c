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
#include <vrt/vrt_write.h>

/* Size of buffer in 32-bit words */
#define SIZE 515

int main() {
    /* Packet data buffer */
    uint32_t b[SIZE];

    /* Initialize to reasonable values */
    struct vrt_packet p;
    vrt_init_packet(&p);

    /* Configure. Note that context packets cannot have a trailer word. */
    p.header.packet_type         = VRT_PT_IF_CONTEXT;
    p.fields.stream_id           = 0xDEADBEEF;
    p.if_context.has.bandwidth   = true;
    p.if_context.has.sample_rate = true;
    p.if_context.has.temperature = true;
    p.if_context.bandwidth       = 2.4e9;
    p.if_context.sample_rate     = 2e6;
    p.if_context.temperature     = 24.0F;

    /* Write to buffer */
    int32_t rv = vrt_write_packet(&p, b, SIZE, true);
    if (rv < 0) {
        fprintf(stderr, "Failed to write packet: %s\n", vrt_string_error(rv));
        return EXIT_FAILURE;
    }
    int32_t pkt_sz = rv;

    /* Write generated packet to file */
    const char* file_path = "context.vrt";
    FILE*       fp        = fopen(file_path, "wb");
    if (fp == NULL) {
        fprintf(stderr, "Failed to open file '%s'\n", file_path);
        return EXIT_FAILURE;
    }
    if (fwrite(b, sizeof(uint32_t) * pkt_sz, 1, fp) != 1) {
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
