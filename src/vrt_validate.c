#include <vrt/vrt_validate.h>

#include <stddef.h>

#include <vrt/vrt_common.h>

#include "vrt_fixed_point.h"

int32_t vrt_validate_header(const vrt_header* header) {
    if (header->packet_type < VRT_PT_IF_DATA_WITHOUT_STREAM_ID || header->packet_type > VRT_PT_EXT_CONTEXT) {
        return VRT_ERR_PACKET_TYPE;
    }

    if (vrt_is_context(header->packet_type)) {
        if (header->has.trailer) {
            return VRT_ERR_TRAILER;
        }
    } else {
        if (header->tsm) {
            return VRT_ERR_TSM;
        }
    }

    if (header->tsi < VRT_TSI_UNDEFINED || header->tsi > VRT_TSI_OTHER) {
        return VRT_ERR_TSI;
    }
    if (header->tsf < VRT_TSF_UNDEFINED || header->tsf > VRT_TSF_FREE_RUNNING_COUNT) {
        return VRT_ERR_TSF;
    }

    if (header->packet_count > 0x0F) {
        return VRT_ERR_PACKET_COUNT;
    }

    return 0;
}

int32_t vrt_validate_fields(const vrt_header* header, const vrt_fields* fields) {
    if (header->has.class_id) {
        if (fields->class_id.oui > 0x00FFFFFF) {
            return VRT_ERR_OUI;
        }
    }

    return 0;
}

static int32_t if_context_data_packet_payload_format(const vrt_data_packet_payload_format* f) {
    if (f->real_or_complex < VRT_ROC_REAL || f->real_or_complex > VRT_ROC_COMPLEX_POLAR) {
        return VRT_ERR_REAL_OR_COMPLEX;
    }
    if (f->data_item_format < VRT_DIF_SIGNED_FIXED_POINT ||
        (f->data_item_format > VRT_DIF_SIGNED_VRT_6_BIT_EXPONENT &&
         f->data_item_format < VRT_DIF_IEEE_754_SINGLE_PRECISION_FLOATING_POINT) ||
        f->data_item_format > VRT_DIF_UNSIGNED_VRT_6_BIT_EXPONENT) {
        return VRT_ERR_DATA_ITEM_FORMAT;
    }
    if (f->event_tag_size > 0x07) {
        return VRT_ERR_EVENT_TAG_SIZE;
    }
    if (f->channel_tag_size > 0x0F) {
        return VRT_ERR_CHANNEL_TAG_SIZE;
    }
    if (f->item_packing_field_size > 0x3F) {
        return VRT_ERR_ITEM_PACKING_FIELD_SIZE;
    }
    if (f->data_item_size > 0x3F) {
        return VRT_ERR_DATA_ITEM_SIZE;
    }

    return 0;
}

static int32_t if_context_formatted_geolocation(const vrt_formatted_geolocation* g) {
    if (g->tsi < VRT_TSI_UNDEFINED || g->tsi > VRT_TSI_OTHER) {
        return VRT_ERR_TSI;
    }
    if (g->tsf < VRT_TSF_UNDEFINED || g->tsf > VRT_TSF_FREE_RUNNING_COUNT) {
        return VRT_ERR_TSF;
    }
    if (g->oui > 0x00FFFFFF) {
        return VRT_ERR_OUI;
    }
    if (g->tsi == VRT_TSI_UNDEFINED && g->integer_second_timestamp != 0xFFFFFFFF) {
        return VRT_ERR_INTEGER_SECOND_TIMESTAMP;
    }
    if (g->tsf == VRT_TSF_UNDEFINED && g->fractional_second_timestamp != 0xFFFFFFFFFFFFFFFF) {
        return VRT_ERR_FRACTIONAL_SECOND_TIMESTAMP;
    }
    if (vrt_double_to_fixed_point_i32(g->latitude, 22) != 0x7FFFFFFF && (g->latitude < -90.0 || g->latitude > 90.0)) {
        return VRT_ERR_LATITUDE;
    }
    if (vrt_double_to_fixed_point_i32(g->longitude, 22) != 0x7FFFFFFF &&
        (g->longitude < -180.0 || g->longitude > 180.0)) {
        return VRT_ERR_LONGITUDE;
    }
    if (vrt_double_to_fixed_point_i32(g->heading_angle, 22) != 0x7FFFFFFF &&
        (g->heading_angle < 0.0 || g->heading_angle > 359.999999761582)) {
        return VRT_ERR_HEADING_ANGLE;
    }
    if (vrt_double_to_fixed_point_i32(g->track_angle, 22) != 0x7FFFFFFF &&
        (g->track_angle < 0.0 || g->track_angle > 359.999999761582)) {
        return VRT_ERR_TRACK_ANGLE;
    }
    if (vrt_double_to_fixed_point_i32(g->magnetic_variation, 22) != 0x7FFFFFFF &&
        (g->magnetic_variation < -180.0 || g->magnetic_variation > 180)) {
        return VRT_ERR_MAGNETIC_VARIATION;
    }

    return 0;
}

static int32_t if_context_ephemeris(const vrt_ephemeris* e) {
    /*
     * Unfortunately the standard does not specify that the position, attitude, or velocity can be invalid in any case
     */
    if (e->tsi < VRT_TSI_UNDEFINED || e->tsi > VRT_TSI_OTHER) {
        return VRT_ERR_TSI;
    }
    if (e->tsf < VRT_TSF_UNDEFINED || e->tsf > VRT_TSF_FREE_RUNNING_COUNT) {
        return VRT_ERR_TSF;
    }
    if (e->oui > 0x00FFFFFF) {
        return VRT_ERR_OUI;
    }
    if (e->tsi == VRT_TSI_UNDEFINED && e->integer_second_timestamp != 0xFFFFFFFF) {
        return VRT_ERR_INTEGER_SECOND_TIMESTAMP;
    }
    if (e->tsf == VRT_TSF_UNDEFINED && e->fractional_second_timestamp != 0xFFFFFFFFFFFFFFFF) {
        return VRT_ERR_FRACTIONAL_SECOND_TIMESTAMP;
    }

    return 0;
}

static int32_t if_context_association_lists(const vrt_context_association_lists* l) {
    if (l->source_list_size > 0x01FF) {
        return VRT_ERR_SOURCE_LIST_SIZE;
    }
    if (l->system_list_size > 0x01FF) {
        return VRT_ERR_SYSTEM_LIST_SIZE;
    }
    if (l->asynchronous_channel_list_size > 0x7FFF) {
        return VRT_ERR_CHANNEL_LIST_SIZE;
    }

    return 0;
}

int32_t vrt_validate_if_context(const vrt_if_context* if_context) {
    if (if_context->has.bandwidth) {
        if (if_context->bandwidth < 0.0) {
            return VRT_ERR_BANDWIDTH;
        }
    }
    if (if_context->has.sample_rate) {
        if (if_context->sample_rate < 0.0) {
            return VRT_ERR_SAMPLE_RATE;
        }
    }
    if (if_context->has.temperature) {
        if (if_context->temperature < -273.15) {
            return VRT_ERR_TEMPERATURE;
        }
    }
    if (if_context->has.device_identifier) {
        if (if_context->device_identifier.oui > 0x00FFFFFF) {
            return VRT_ERR_OUI;
        }
    }
    if (if_context->has.data_packet_payload_format) {
        int32_t rv = if_context_data_packet_payload_format(&if_context->data_packet_payload_format);
        if (rv < 0) {
            return rv;
        }
    }
    if (if_context->has.formatted_gps_geolocation) {
        int32_t rv = if_context_formatted_geolocation(&if_context->formatted_gps_geolocation);
        if (rv < 0) {
            return rv;
        }
    }
    if (if_context->has.formatted_ins_geolocation) {
        int32_t rv = if_context_formatted_geolocation(&if_context->formatted_ins_geolocation);
        if (rv < 0) {
            return rv;
        }
    }
    if (if_context->has.ecef_ephemeris) {
        int32_t rv = if_context_ephemeris(&if_context->ecef_ephemeris);
        if (rv < 0) {
            return rv;
        }
    }
    if (if_context->has.relative_ephemeris) {
        int32_t rv = if_context_ephemeris(&if_context->relative_ephemeris);
        if (rv < 0) {
            return rv;
        }
    }
    if (if_context->has.context_association_lists) {
        int32_t rv = if_context_association_lists(&if_context->context_association_lists);
        if (rv < 0) {
            return rv;
        }
    }

    return 0;
}

int32_t vrt_validate_trailer(const vrt_trailer* trailer) {
    /* Not much to validate here. Note that setting fields without their 'has' counterpart is OK. */

    if (trailer->has.associated_context_packet_count) {
        if (trailer->associated_context_packet_count > 0x7F) {
            return VRT_ERR_ASSOCIATED_CONTEXT_PACKET_COUNT;
        }
    }

    return 0;
}
