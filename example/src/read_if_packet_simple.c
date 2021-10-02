/*
 * Read VRT IF packet from file and parse it. Note that this will fail to read a big endian-format, i.e. standard
 * conforming, packet on a little endian platform.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <vrt/vrt_read.h>
#include <vrt/vrt_string.h>
#include <vrt/vrt_types.h>
#include <vrt/vrt_util.h>

/* Size of buffer in 32-bit words */
#define SIZE 515

int main() {
    /* Packet data buffer */
    uint32_t b[SIZE];

    /* Warn if not standards compliant */
    if (vrt_is_platform_little_endian()) {
        fprintf(stderr,
                "Warning: Reader is little endian. It will only read packets NOT compliant with the VRT standard.\n");
    }

    /* Open file */
    const char* file_path = "signal.vrt";
    FILE*       fp        = fopen(file_path, "rb");
    if (fp == NULL) {
        fprintf(stderr, "Failed to open file '%s'\n", file_path);
        return EXIT_FAILURE;
    }

    /* Read data from file */
    if (fread(b, sizeof(uint32_t) * SIZE, 1, fp) != 1) {
        fprintf(stderr, "Failed to read data from file '%s'\n", file_path);
        fclose(fp);
        return EXIT_FAILURE;
    }
    fclose(fp);

    /* Parse packet */
    struct vrt_packet p;
    int32_t           rv = vrt_read_packet(b, SIZE, &p, true);
    if (rv < 0) {
        fprintf(stderr, "Failed to parse packet: %s\n", vrt_string_error(rv));
        return EXIT_FAILURE;
    }

    /* Print some header information */
    printf("Packet type: %s\n", vrt_string_packet_type(p.header.packet_type));

    /* Print some field information */
    if (vrt_has_stream_id(&p.header)) {
        printf("Stream ID: 0x%x\n", p.fields.stream_id);
    } else {
        printf("No Stream ID\n");
    }

    /* Print some IF context information */
    if (p.header.packet_type == VRT_PT_IF_CONTEXT) {
        if (p.if_context.has.bandwidth) {
            printf("Bandwidth [Hz]: %f\n", p.if_context.bandwidth);
        } else {
            printf("No Bandwidth\n");
        }
    }

    /* Print some trailer information */
    if (p.header.has.trailer) {
        if (p.trailer.has.reference_lock) {
            printf("Reference lock: %s\n", p.trailer.reference_lock ? "true" : "false");
        } else {
            printf("No Reference lock information\n");
        }
    }

    return EXIT_SUCCESS;
}
