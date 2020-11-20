#include "vrt_words.h"

#include "vrt_types.h"
#include "vrt_util.h"

#include "vrt_util_internal.h"

uint32_t vrt_words_fields(const vrt_header* header) {
    uint32_t words = 0;
    if (vrt_has_stream_id(header->packet_type)) {
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

uint32_t vrt_words_trailer(const vrt_header* header) {
    /* Context packets cannot have a trailer */
    if (vrt_is_context(header->packet_type)) {
        return 0;
    }
    return vrt_b2u(header->has.trailer);
}

uint32_t vrt_words_if_context(const vrt_if_context* if_context) {
    /* Count non-variable words */
    uint32_t words = vrt_words_if_context_indicator(&if_context->has);

    /* Count variable words */
    if (if_context->has.gps_ascii) {
        words += 2;
        words += if_context->gps_ascii.number_of_words & 0x00FFFFFFU;
    }
    if (if_context->has.context_association_lists) {
        const vrt_context_association_lists* cal = &if_context->context_association_lists;
        const uint16_t                       sz1 = cal->source_list_size & 0x01FFU;
        const uint16_t                       sz2 = cal->system_list_size & 0x01FFU;
        const uint16_t                       sz3 = cal->vector_component_list_size;
        const uint16_t                       sz4 = cal->asynchronous_channel_list_size & 0x7FFFU;

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
