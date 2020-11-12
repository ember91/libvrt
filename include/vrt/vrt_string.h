#ifndef INCLUDE_VRT_TO_STRING_H_
#define INCLUDE_VRT_TO_STRING_H_

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
 * \param real_or_complex Data item format.
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
