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

    /* Read header word from file */
    if (fread(b, sizeof(uint32_t), 1, fp) != 1) {
        fprintf(stderr, "Failed to read header word from file '%s'\n", file_path);
        fclose(fp);
        return EXIT_FAILURE;
    }

    /* Parse header */
    int32_t           offset = 0;
    struct vrt_header h;
    int32_t           rv = vrt_read_header(b + offset, SIZE - offset, &h, true);
    if (rv < 0) {
        fprintf(stderr, "Failed to parse header: %s\n", vrt_string_error(rv));
        return EXIT_FAILURE;
    }
    offset += rv;

    /* Print some header information */
    printf("Packet type: %s\n", vrt_string_packet_type(h.packet_type));

    /* Read remaining words from file */
    if (fread(b + 1, sizeof(uint32_t) * (h.packet_size - 1), 1, fp) != 1) {
        fprintf(stderr, "Failed to read remaining words from file '%s'\n", file_path);
        fclose(fp);
        return EXIT_FAILURE;
    }
    fclose(fp);

    /* Parse fields */
    struct vrt_fields f;
    rv = vrt_read_fields(&h, b + offset, SIZE - offset, &f, true);
    if (rv < 0) {
        fprintf(stderr, "Failed to parse fields section: %s\n", vrt_string_error(rv));
        return EXIT_FAILURE;
    }
    offset += rv;

    /* Print some field information */
    if (vrt_has_stream_id(&h)) {
        printf("Stream ID: 0x%x\n", f.stream_id);
    } else {
        printf("No Stream ID\n");
    }

    /* Skip signal data if any */

    if (h.packet_type == VRT_PT_IF_CONTEXT) {
        /* Parse context */
        struct vrt_if_context c;
        rv = vrt_read_if_context(b + offset, SIZE - offset, &c, true);
        if (rv < 0) {
            fprintf(stderr, "Failed to parse IF context section: %s\n", vrt_string_error(rv));
            return EXIT_FAILURE;
        }

        /* Print some IF context information */
        if (c.has.bandwidth) {
            printf("Bandwidth [Hz]: %f\n", c.bandwidth);
        } else {
            printf("No Bandwidth\n");
        }
    }
    if (h.has.trailer) {
        /* Parse trailer */
        struct vrt_trailer t;
        rv = vrt_read_trailer(b + h.packet_size - 1, SIZE - (h.packet_size - 1), &t);
        if (rv < 0) {
            fprintf(stderr, "Failed to parse IF context section: %s\n", vrt_string_error(rv));
            return EXIT_FAILURE;
        }

        /* Print some trailer information */
        if (t.has.reference_lock) {
            printf("Reference lock: %s\n", t.reference_lock ? "true" : "false");
        } else {
            printf("No Reference lock information\n");
        }
    }

    return EXIT_SUCCESS;
}
