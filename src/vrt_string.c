#include <vrt/vrt_string.h>

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
        case VRT_TSI_NONE:
            return "None/Undefined";
        case VRT_TSI_UTC:
            return "Sample Count";
        case VRT_TSI_GPS:
            return "Real Time";
        case VRT_TSI_OTHER:
            return "Free Running";
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
