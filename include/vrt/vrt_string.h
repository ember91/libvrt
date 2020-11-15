#ifndef INCLUDE_VRT_VRT_STRING_H_
#define INCLUDE_VRT_VRT_STRING_H_

#include <vrt/vrt_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Get a string representation of Packet type enum.
 *
 * \param packet_type Packet type.
 *
 * \return String representation.
 *
 * \note Inputting an unknown packet type returns "Unknown".
 */
const char* vrt_string_packet_type(vrt_packet_type packet_type);

/**
 * Get a string representation of TSM (Timestamp mode) enum.
 *
 * \param tsm TSM.
 *
 * \return String representation.
 *
 * \note Inputting an unknown TSM returns "Unknown".
 */
const char* vrt_string_tsm(vrt_tsm tsm);

/**
 * Get a string representation of TSI (Timestamp integer) enum.
 *
 * \param tsi TSI.
 *
 * \return String representation.
 *
 * \note Inputting an unknown TSI returns "Unknown".
 */
const char* vrt_string_tsi(vrt_tsi tsi);

/**
 * Get a string representation of TSF (Timestamp fractional) enum.
 *
 * \param tsf TSF.
 *
 * \return String representation.
 *
 * \note Inputting an unknown TSF returns "Unknown".
 */
const char* vrt_string_tsf(vrt_tsf tsf);

/**
 * Get a string representation of AGC/MGC enum.
 *
 * \param agc_or_mgc AGC/MGC.
 *
 * \return String representation.
 *
 * \note Inputting an unknown AGC/MGC returns "Unknown".
 */
const char* vrt_string_agc_or_mgc(vrt_agc_or_mgc agc_or_mgc);

/**
 * Get a string representation of Packing method enum.
 *
 * \param packing_method Packing method.
 *
 * \return String representation.
 *
 * \note Inputting an unknown Packing method returns "Unknown".
 */
const char* vrt_string_packing_method(vrt_packing_method packing_method);

/**
 * Get a string representation of Real/Complex enum.
 *
 * \param real_or_complex Real/Complex.
 *
 * \return String representation.
 *
 * \note Inputting an unknown Real/Complex returns "Unknown".
 */
const char* vrt_string_real_or_complex(vrt_real_complex real_or_complex);

/**
 * Get a string representation of Data item format enum.
 *
 * \param data_item_format Data item format.
 *
 * \return String representation.
 *
 * \note Inputting an unknown Data item format returns "Unknown".
 */
const char* vrt_string_data_item_format(vrt_data_item_format data_item_format);

#ifdef __cplusplus
}
#endif

#endif
