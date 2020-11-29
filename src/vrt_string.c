#include "vrt_string.h"

#include "vrt_error_code.h"
#include "vrt_types.h"

const char* vrt_string_packet_type(vrt_packet_type packet_type) {
    switch (packet_type) {
        case VRT_PT_IF_DATA_WITHOUT_STREAM_ID:
            return "IF Data packet without Stream Identifier";
        case VRT_PT_IF_DATA_WITH_STREAM_ID:
            return "IF Data packet with Stream Identifier";
        case VRT_PT_EXT_DATA_WITHOUT_STREAM_ID:
            return "Extension Data packet without Stream Identifier";
        case VRT_PT_EXT_DATA_WITH_STREAM_ID:
            return "Extension Data packet with Stream Identifier";
        case VRT_PT_IF_CONTEXT:
            return "IF Context packet";
        case VRT_PT_EXT_CONTEXT:
            return "Extension Context packet";
        default:
            return "Unknown";
    }
}

const char* vrt_string_tsm(vrt_tsm tsm) {
    switch (tsm) {
        case VRT_TSM_FINE:
            return "Fine";
        case VRT_TSM_COARSE:
            return "Coarse";
        default:
            return "Unknown";
    }
}

const char* vrt_string_tsi(vrt_tsi tsi) {
    switch (tsi) {
        case VRT_TSI_NONE:
            return "None/Undefined";
        case VRT_TSI_UTC:
            return "UTC";
        case VRT_TSI_GPS:
            return "GPS";
        case VRT_TSI_OTHER:
            return "Other";
        default:
            return "Unknown";
    }
}

const char* vrt_string_tsf(vrt_tsf tsf) {
    switch (tsf) {
        case VRT_TSF_NONE:
            return "None/Undefined";
        case VRT_TSF_SAMPLE_COUNT:
            return "Sample Count";
        case VRT_TSF_REAL_TIME:
            return "Real Time";
        case VRT_TSF_FREE_RUNNING_COUNT:
            return "Free Running Count";
        default:
            return "Unknown";
    }
}

const char* vrt_string_agc_or_mgc(vrt_agc_or_mgc agc_or_mgc) {
    switch (agc_or_mgc) {
        case VRT_AOM_MGC:
            return "MGC";
        case VRT_AOM_AGC:
            return "AGC";
        default:
            return "Unknown";
    }
}

const char* vrt_string_packing_method(vrt_packing_method packing_method) {
    switch (packing_method) {
        case VRT_PM_PROCESSING_EFFICIENT:
            return "Processing Efficient";
        case VRT_PM_LINK_EFFICIENT:
            return "Link Efficient";
        default:
            return "Unknown";
    }
}

const char* vrt_string_real_or_complex(vrt_real_complex real_or_complex) {
    switch (real_or_complex) {
        case VRT_ROC_REAL:
            return "Real";
        case VRT_ROC_COMPLEX_CARTESIAN:
            return "Complex, Cartesian";
        case VRT_ROC_COMPLEX_POLAR:
            return "Complex, Polar";
        default:
            return "Unknown";
    }
}

const char* vrt_string_data_item_format(vrt_data_item_format data_item_format) {
    switch (data_item_format) {
        case VRT_DIF_SIGNED_FIXED_POINT:
            return "Signed Fixed-Point";
        case VRT_DIF_SIGNED_VRT_1_BIT_EXPONENT:
            return "Signed VRT, 1-bit exponent";
        case VRT_DIF_SIGNED_VRT_2_BIT_EXPONENT:
            return "Signed VRT, 2-bit exponent";
        case VRT_DIF_SIGNED_VRT_3_BIT_EXPONENT:
            return "Signed VRT, 3-bit exponent";
        case VRT_DIF_SIGNED_VRT_4_BIT_EXPONENT:
            return "Signed VRT, 4-bit exponent";
        case VRT_DIF_SIGNED_VRT_5_BIT_EXPONENT:
            return "Signed VRT, 5-bit exponent";
        case VRT_DIF_SIGNED_VRT_6_BIT_EXPONENT:
            return "Signed VRT, 6-bit exponent";
        case VRT_DIF_IEEE_754_SINGLE_PRECISION_FLOATING_POINT:
            return "IEEE-754 Single-Precision Floating-Point";
        case VRT_DIF_IEEE_754_DOUBLE_PRECISION_FLOATING_POINT:
            return "IEEE-754 Double-Precision Floating-Point";
        case VRT_DIF_UNSIGNED_FIXED_POINT:
            return "Unsigned Fixed-Point";
        case VRT_DIF_UNSIGNED_VRT_1_BIT_EXPONENT:
            return "Unsigned VRT, 1-bit exponent";
        case VRT_DIF_UNSIGNED_VRT_2_BIT_EXPONENT:
            return "Unsigned VRT, 2-bit exponent";
        case VRT_DIF_UNSIGNED_VRT_3_BIT_EXPONENT:
            return "Unsigned VRT, 3-bit exponent";
        case VRT_DIF_UNSIGNED_VRT_4_BIT_EXPONENT:
            return "Unsigned VRT, 4-bit exponent";
        case VRT_DIF_UNSIGNED_VRT_5_BIT_EXPONENT:
            return "Unsigned VRT, 5-bit exponent";
        case VRT_DIF_UNSIGNED_VRT_6_BIT_EXPONENT:
            return "Unsigned VRT, 6-bit exponent";
        default:
            return "Unknown";
    }
}

const char* vrt_string_error(int32_t error /* Do NOT use vrt_error_code, since it isn't returned by any function */) {
    /* No error */
    if (error >= 0) {
        return "Success";
    }

    switch (error) {
        case VRT_ERR_BUFFER_SIZE:
            return "Buffer is too small";
        case VRT_ERR_RESERVED:
            return "One or multiple reserved bits are set";
        case VRT_ERR_INVALID_PACKET_TYPE:
            return "Packet type is an invalid value";
        case VRT_ERR_TRAILER_IN_CONTEXT:
            return "Context packet has trailer bit set";
        case VRT_ERR_TSM_IN_DATA:
            return "Data packet has TSM bit set";
        case VRT_ERR_INVALID_TSM:
            return "TSM is an invalid value";
        case VRT_ERR_INVALID_TSI:
            return "TSI is an invalid value";
        case VRT_ERR_INVALID_TSF:
            return "TSF is an invalid value";
        case VRT_ERR_BOUNDS_REAL_TIME:
            return "TSF is VRT_TSF_REAL TIME but picoseconds is outside valid bounds (> 999999999999 ps)";
        case VRT_ERR_BOUNDS_PACKET_COUNT:
            return "Packet count is outside valid bounds (> 0x0F)";
        case VRT_ERR_BOUNDS_OUI:
            return "OUI is outside valid bounds (> 0x00FFFFFF)";
        case VRT_ERR_BOUNDS_ASSOCIATED_CONTEXT_PACKET_COUNT:
            return "Associated context packet count is outside valid bounds (> 0x7F)";
        case VRT_ERR_BOUNDS_BANDWIDTH:
            return "Bandwidth is outside valid bounds (< 0 Hz)";
        case VRT_ERR_GAIN_STAGE2_SET:
            return "Gain stage 1 must be used instead of stage 2 when only one is set";
        case VRT_ERR_BOUNDS_SAMPLE_RATE:
            return "Sample rate is outside valid bounds (< 0 Hz)";
        case VRT_ERR_BOUNDS_TEMPERATURE:
            return "Temperature is outside valid bounds (< -273.15 degrees C)";
        case VRT_ERR_INVALID_PACKING_METHOD:
            return "Packing method is an invalid value";
        case VRT_ERR_INVALID_REAL_OR_COMPLEX:
            return "Real/Complex is an invalid value";
        case VRT_ERR_INVALID_DATA_ITEM_FORMAT:
            return "Data item format is an invalid value";
        case VRT_ERR_BOUNDS_EVENT_TAG_SIZE:
            return "Event tag size is outside valid bounds (> 0x07)";
        case VRT_ERR_BOUNDS_CHANNEL_TAG_SIZE:
            return "Channel tag size is outside valid bounds (> 0x0F)";
        case VRT_ERR_BOUNDS_ITEM_PACKING_FIELD_SIZE:
            return "Item packing fields size is outside valid bounds (> 0x3F)";
        case VRT_ERR_BOUNDS_DATA_ITEM_SIZE:
            return "Data item size is outside valid bounds (> 0x3F)";
        case VRT_ERR_SET_INTEGER_SECOND_TIMESTAMP:
            return "Integer second timestamp is not 0xFFFFFFFF when TSI is VRT_TSI_UNDEFINED";
        case VRT_ERR_SET_FRACTIONAL_SECOND_TIMESTAMP:
            return "Fractional second timestamp is not 0xFFFFFFFFFFFFFFFF when TSF is VRT_TSF_UNDEFINED";
        case VRT_ERR_BOUNDS_LATITUDE:
            return "Latitude is outside valid bounds (< -90 or > 90 degrees)";
        case VRT_ERR_BOUNDS_LONGITUDE:
            return "Longitude is outside valid bounds (< -180 or > 180 degrees)";
        case VRT_ERR_BOUNDS_SPEED_OVER_GROUND:
            return "Speed over ground is outside valid bounds (< 0 m/s)";
        case VRT_ERR_BOUNDS_HEADING_ANGLE:
            return "Heading angle outside valid bounds (< 0 or > 359.999999761582 degrees)";
        case VRT_ERR_BOUNDS_TRACK_ANGLE:
            return "Track angle is outside valid bounds (< 0 or > 359.999999761582 degrees)";
        case VRT_ERR_BOUNDS_MAGNETIC_VARIATION:
            return "Magnetic variation is outside valid bounds (< -180 or > 180 degrees)";
        case VRT_ERR_BOUNDS_SOURCE_LIST_SIZE:
            return "Source list size is outside valid bounds (> 0x01FF)";
        case VRT_ERR_BOUNDS_SYSTEM_LIST_SIZE:
            return "System list size is outside valid bounds (> 0x01FF)";
        case VRT_ERR_BOUNDS_CHANNEL_LIST_SIZE:
            return "Channel list size is outside valid bounds (> 0x7FFF)";
        case VRT_ERR_BOUNDS_PACKET_SIZE:
            return "Calculated packet size is outside valid bounds (> 0xFFFF)";
        case VRT_ERR_PACKET_SIZE_MISMATCH:
            return "Packet size in header and calculated packet size do not match";
        default:
            return "Unknown";
    }
}
