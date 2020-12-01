#include "vrt_words.h"

#include "vrt_types.h"
#include "vrt_util.h"

#include "vrt_util_internal.h"

int32_t vrt_words_fields(const struct vrt_header* header) {
    int32_t words = 0;
    if (vrt_has_stream_id(header)) {
        words += 1;
    }
    if (header->has.class_id) {
        words += 2;
    }
    if (header->tsi != VRT_TSI_NONE) {
        words += 1;
    }
    if (header->tsf != VRT_TSF_NONE) {
        words += 2;
    }
    return words;
}

int32_t vrt_words_trailer(const struct vrt_header* header) {
    /* Context packets cannot have a trailer */
    if (vrt_is_context(header)) {
        return 0;
    }
    return vrt_b2u(header->has.trailer);
}

int32_t vrt_words_if_context(const struct vrt_if_context* if_context) {
    /* Count non-variable words */
    int32_t words = vrt_words_if_context_indicator(&if_context->has);

    /* Count variable words */
    if (if_context->has.gps_ascii) {
        words += 2;
        words += if_context->gps_ascii.number_of_words & 0x00FFFFFFU;
    }
    if (if_context->has.context_association_lists) {
        const struct vrt_context_association_lists* cal = &if_context->context_association_lists;
        const uint16_t                              sz1 = cal->source_list_size & 0x01FFU;
        const uint16_t                              sz2 = cal->system_list_size & 0x01FFU;
        const uint16_t                              sz3 = cal->vector_component_list_size;
        const uint16_t                              sz4 = cal->asynchronous_channel_list_size & 0x7FFFU;

        words += 2;
        words += sz1;
        words += sz2;
        words += sz3;
        if (cal->has.asynchronous_channel_tag_list) {
            words += sz4;
        }
        words += sz4;
    }

    return words;
}

int32_t vrt_words_packet(const struct vrt_packet* packet) {
    int32_t words = VRT_WORDS_HEADER;
    words += vrt_words_fields(&packet->header);
    switch (packet->header.packet_type) {
        case VRT_PT_IF_DATA_WITHOUT_STREAM_ID:
        case VRT_PT_IF_DATA_WITH_STREAM_ID:
        case VRT_PT_EXT_DATA_WITHOUT_STREAM_ID:
        case VRT_PT_EXT_DATA_WITH_STREAM_ID:
        case VRT_PT_EXT_CONTEXT: {
            words += packet->words_body;
            break;
        }
        case VRT_PT_IF_CONTEXT: {
            words += vrt_words_if_context(&packet->if_context);
            break;
        }
        default: {
            /* Do nothing here */
            break;
        }
    }
    words += vrt_words_trailer(&packet->header);

    return words;
}
