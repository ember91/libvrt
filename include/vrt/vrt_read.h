#ifndef INCLUDE_VRT_VRT_READ_H_
#define INCLUDE_VRT_VRT_READ_H_

#include <stdbool.h>
#include <stdint.h>

#include "vrt_util.h"

#ifdef __cplusplus
extern "C" {
#endif

struct vrt_fields;
struct vrt_header;
struct vrt_if_context;
struct vrt_packet;
struct vrt_trailer;

/**
 * Low-level function that reads VRT header section.
 *
 * \param buf       Buffer to read from.
 * \param words_buf Size of buf in 32-bit words.
 * \param header    Header to read into.
 * \param validate  True if validation shall be done. If false, only buffer size is validated.
 *
 * \return Number of read 32-bit words, or a negative number if error.
 * \retval VRT_ERR_BUFFER_SIZE          Buffer is too small.
 * \retval VRT_ERR_RESERVED             One or multiple reserved bits are set.
 * \retval VRT_ERR_INVALID_PACKET_TYPE  Packet type is an invalid value.
 * \retval VRT_ERR_TRAILER_IN_CONTEXT   Context packet has trailer bit set.
 * \retval VRT_ERR_TSM_IN_DATA          Data packet has TSM bit set.
 *
 * \note Requires input buffer data to be byte swapped if platform endianess isn't big endian (network order).
 */
VRT_WARN_UNUSED
int32_t vrt_read_header(const void* buf, int32_t words_buf, struct vrt_header* header, bool validate);

/**
 * Low-level function that reads VRT fields section.
 *
 * \param header    Header.
 * \param buf       Buffer to read from. This must point to the first field in the sequence, i.e. directly following the
 *                  header word.
 * \param words_buf Size of buf in 32-bit words.
 * \param fields    Fields section to read into.
 * \param validate  True if validation shall be done. If false, only buffer size is validated.
 *
 * \return Number of read 32-bit words, or a negative number if error.
 * \retval VRT_ERR_BUFFER_SIZE      Buffer is too small.
 * \retval VRT_ERR_RESERVED         One or multiple reserved bits are set.
 * \retval VRT_ERR_BOUNDS_REAL_TIME TSF is VRT_TSF_REAL_TIME but fractional timestamp is outside valid bounds
 *                                  (> 999999999999 ps).
 */
VRT_WARN_UNUSED
int32_t vrt_read_fields(const struct vrt_header* header,
                        const void*              buf,
                        int32_t                  words_buf,
                        struct vrt_fields*       fields,
                        bool                     validate);

/**
 * Low-level function that reads VRT trailer section.
 *
 * \param buf       Buffer to read from. This must point to the position of the trailer word, i.e. the last word in the
 *                  packet.
 * \param words_buf Size of buf in 32-bit words.
 * \param trailer   Trailer to read into.
 *
 * \return Number of read 32-bit words, i.e. 1, or a negative number if error.
 * \retval VRT_ERR_BUFFER_SIZE Buffer is too small.
 *
 * \note Unlike other read functions, there is nothing to validate, so no 'validate' parameter here.
 */
VRT_WARN_UNUSED
int32_t vrt_read_trailer(const void* buf, int32_t words_buf, struct vrt_trailer* trailer);

/**
 * Low-level function that reads VRT IF context section.
 *
 * \param buf        Buffer to read from.
 * \param words_buf  Size of buf in 32-bit words.
 * \param if_context IF context struct to read into.
 * \param validate   True if validation shall be done. If false, only buffer size is validated.
 *
 * \return Number of read 32-bit words, or a negative number if error.
 * \retval VRT_ERR_BUFFER_SIZE                          Buffer is too small.
 * \retval VRT_ERR_RESERVED                             One or multiple reserved bits are set.
 * \retval VRT_ERR_BOUNDS_BANDWIDTH                     Bandwidth is outside valid bounds (< 0 Hz).
 * \retval VRT_ERR_GAIN_STAGE2_SET                      Gain stage 1 must be used instead of stage 2 when only one of
 *                                                      them is set.
 * \retval VRT_ERR_BOUNDS_SAMPLE_RATE                   Sample rate is outside valid bounds (< 0 Hz).
 * \retval VRT_ERR_BOUNDS_TEMPERATURE                   Temperature is outside valid bounds (< -273.15 degrees).
 * \retval VRT_ERR_INVALID_REAL_OR_COMPLEX              Real/Complex is an invalid value.
 * \retval VRT_ERR_INVALID_DATA_ITEM_FORMAT             Data item format is an invalid value.
 * \retval VRT_ERR_SET_INTEGER_SECOND_TIMESTAMP         Integer second timestamp is not 0xFFFFFFFF when TSI is
 *                                                      VRT_TSI_UNDEFINED.
 * \retval VRT_ERR_SET_FRACTIONAL_SECOND_TIMESTAMP      Fractional second timestamp is not 0xFFFFFFFFFFFFFFFF when TSF
 *                                                      is VRT_TSF_UNDEFINED.
 * \retval VRT_ERR_BOUNDS_REAL_TIME                     TSF is VRT_TSF_REAL_TIME but fractional timestamp is outside
 *                                                      valid bounds (> 999999999999 ps).
 * \retval VRT_ERR_BOUNDS_LATITUDE                      Latitude is outside valid bounds (< -90 or > 90 degrees).
 * \retval VRT_ERR_BOUNDS_LONGITUDE                     Longitude is outside valid bounds (< -180 or > 180 degrees).
 * \retval VRT_ERR_BOUNDS_HEADING_ANGLE                 Heading angle is outside valid bounds (< 0 or > ~360
 *                                                      degrees).
 * \retval VRT_ERR_BOUNDS_TRACK_ANGLE                   Track angle is outside valid bounds (< 0 or > ~360
 *                                                      degrees).
 * \retval VRT_ERR_BOUNDS_MAGNETIC_VARIATION            Magnetic variation is outside valid bounds (< -180 or > 180
 *                                                      degrees).
 *
 * \warning Fields represented as double but with an underlying 64-bit fixed precision format, i.e. bandwidth,
 *          if_reference_frequency, rf_reference_frequency, rf_reference_frequency_offset, if_band_offset, and
 *          sample_rate, may in rare cases lose precision since double only has 53 bits of precision.
 */
VRT_WARN_UNUSED
int32_t vrt_read_if_context(const void* buf, int32_t words_buf, struct vrt_if_context* if_context, bool validate);

/**
 * Higher-level function that reads a full VRT packet.
 *
 * \param buf        Buffer to read from.
 * \param words_buf  Size of buf in 32-bit words.
 * \param packet     Packet to read into.
 * \param validate   True if validation shall be done. If false, only buffer size is validated.
 *
 * \return Number of read 32-bit words, or a negative number if error.
 * \retval VRT_ERR_BUFFER_SIZE                          Buffer is too small.
 * \retval VRT_ERR_RESERVED                             One or multiple reserved bits are set.
 * \retval VRT_ERR_INVALID_PACKET_TYPE                  Packet type is an invalid value.
 * \retval VRT_ERR_TRAILER_IN_CONTEXT                   Context packet has trailer bit set.
 * \retval VRT_ERR_TSM_IN_DATA                          Data packet has TSM bit set.
 * \retval VRT_ERR_BOUNDS_REAL_TIME                     TSF is VRT_TSF_REAL_TIME but fractional timestamp is outside
 *                                                      valid bounds (> 999999999999 ps).
 * \retval VRT_ERR_BOUNDS_BANDWIDTH                     Bandwidth is outside valid bounds (< 0 Hz).
 * \retval VRT_ERR_GAIN_STAGE2_SET                      Gain stage 1 must be used instead of stage 2 when only one of
 *                                                      them is set.
 * \retval VRT_ERR_BOUNDS_SAMPLE_RATE                   Sample rate is outside valid bounds (< 0 Hz).
 * \retval VRT_ERR_BOUNDS_TEMPERATURE                   Temperature is outside valid bounds (< -273.15 degrees).
 * \retval VRT_ERR_INVALID_REAL_OR_COMPLEX              Real/Complex is an invalid value.
 * \retval VRT_ERR_INVALID_DATA_ITEM_FORMAT             Data item format is an invalid value.
 * \retval VRT_ERR_SET_INTEGER_SECOND_TIMESTAMP         Integer second timestamp is not 0xFFFFFFFF when TSI is
 *                                                      VRT_TSI_UNDEFINED.
 * \retval VRT_ERR_SET_FRACTIONAL_SECOND_TIMESTAMP      Fractional second timestamp is not 0xFFFFFFFFFFFFFFFF when TSF
 *                                                      is VRT_TSF_UNDEFINED.
 * \retval VRT_ERR_BOUNDS_LATITUDE                      Latitude is outside valid bounds (< -90 or > 90 degrees).
 * \retval VRT_ERR_BOUNDS_LONGITUDE                     Longitude is outside valid bounds (< -180 or > 180 degrees).
 * \retval VRT_ERR_BOUNDS_HEADING_ANGLE                 Heading angle is outside valid bounds (< 0 or > ~360
 *                                                      degrees).
 * \retval VRT_ERR_BOUNDS_TRACK_ANGLE                   Track angle is outside valid bounds (< 0 or > ~360
 *                                                      degrees).
 * \retval VRT_ERR_BOUNDS_MAGNETIC_VARIATION            Magnetic variation is outside valid bounds (< -180 or > 180
 *                                                      degrees).
 * \retval VRT_ERR_MISMATCH_PACKET_SIZE                 Packet size in header and calculated packet size do not match.
 *
 * \warning The packet body pointer will point into buf, which is why buf isn't const.
 * \warning Fields represented as double but with an underlying 64-bit fixed precision format, i.e. bandwidth,
 *          if_reference_frequency, rf_reference_frequency, rf_reference_frequency_offset, if_band_offset, and
 *          sample_rate, may in rare cases lose precision since double only has 53 bits of precision.
 */
VRT_WARN_UNUSED
int32_t vrt_read_packet(void* buf, int32_t words_buf, struct vrt_packet* packet, bool validate);

#ifdef __cplusplus
}
#endif

#endif
