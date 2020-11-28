#ifndef INCLUDE_VRT_VRT_WRITE_H_
#define INCLUDE_VRT_VRT_WRITE_H_

#include <stdbool.h>
#include <stdint.h>

#include "vrt_types.h"
#include "vrt_util.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Low-level function that writes VRT header section.
 *
 * \param header    Header to write.
 * \param buf       Buffer to write to.
 * \param words_buf Size of buf in 32-bit words.
 * \param validate  True if validation should be done. If false, only buffer size is validated.
 *
 * \return Number of written 32-bit words (always 1), or a negative number if error.
 * \retval VRT_ERR_BUFFER_SIZE          Buffer is too small.
 * \retval VRT_ERR_INVALID_PACKET_TYPE  Packet type is an invalid value.
 * \retval VRT_ERR_INVALID_TSM          TSM is an invalid value.
 * \retval VRT_ERR_TSM_IN_DATA          Data packet has TSM bit set.
 * \retval VRT_ERR_INVALID_TSI          TSI is an invalid value.
 * \retval VRT_ERR_INVALID_TSF          TSF is an invalid value.
 * \retval VRT_ERR_BOUNDS_PACKET_COUNT  Packet count is outside valid bounds (> 0x0F).
 *
 * \note May require output buffer data to be byte swapped if platform endianess isn't big endian (network order).
 */
VRT_WARN_UNUSED
int32_t vrt_write_header(const vrt_header* header, void* buf, int32_t words_buf, bool validate);

/**
 * Low-level function that writes VRT fields section.
 *
 * \param header    Header.
 * \param fields    Fields to write.
 * \param buf       Buffer to write to. This must point to the start of the fields section, i.e. directly following the
 *                  header word.
 * \param words_buf Size of buf in 32-bit words.
 * \param validate  True if validation should be done. If false, only buffer size is validated.
 *
 * \return Number of written 32-bit words, or a negative number if error.
 * \retval VRT_ERR_BUFFER_SIZE      Buffer is too small.
 * \retval VRT_ERR_BOUNDS_REAL_TIME TSF is VRT_TSF_REAL TIME but picoseconds is outside valid bounds
 *                                  (> 999999999999 ps).
 * \retval VRT_ERR_BOUNDS_OUI       OUI is outside valid bounds (> 0x00FFFFFF).
 */
VRT_WARN_UNUSED
int32_t vrt_write_fields(const vrt_header* header,
                         const vrt_fields* fields,
                         void*             buf,
                         int32_t           words_buf,
                         bool              validate);

/**
 * Low-level function that writes VRT trailer section.
 *
 * \param trailer    Trailer to write.
 * \param buf        Buffer to write to. This must point to the position of the trailer word, i.e. the last word in the
 *                   packet.
 * \param words_buf  Size of buf in 32-bit words.
 * \param validate   True if validation should be done. If false, only buffer size is validated.
 *
 * \return Number of written 32-bit words, or a negative number if error.
 * \retval VRT_ERR_BUFFER_SIZE                              Buffer is too small.
 * \retval VRT_ERR_BOUNDS_ASSOCIATED_CONTEXT_PACKET_COUNT   Associated context packet count is outside valid bounds
 *                                                          (>0x7F).
 */
VRT_WARN_UNUSED
int32_t vrt_write_trailer(const vrt_trailer* trailer, void* buf, int32_t words_buf, bool validate);

/**
 * Low-level function that writes VRT IF context section.
 *
 * \param if_context IF context to write.
 * \param buf        Buffer to write to. This must point to the first context section word in the packet, i.e. directly
 *                   following the fields section.
 * \param words_buf  Size of buf in 32-bit words.
 * \param validate   True if validation should be done. If false, only buffer size is validated.
 *
 * \return Number of written 32-bit words, or a negative number if error.
 * \retval VRT_ERR_BUFFER_SIZE                          Buffer is too small.
 * \retval VRT_ERR_BOUNDS_BANDWIDTH                     Bandwidth is outside valid bounds (< 0 Hz).
 * \retval VRT_ERR_GAIN_STAGE2_SET                      Gain stage 1 must be used instead of stage 2 when only one is
 *                                                      set.
 * \retval VRT_ERR_BOUNDS_SAMPLE_RATE                   Sample rate is outside valid bounds (< 0 Hz).
 * \retval VRT_ERR_BOUNDS_TEMPERATURE                   Temperature is outside valid bounds (< -273.15 degrees).
 * \retval VRT_ERR_BOUNDS_OUI                           OUI is outside valid bounds (> 0x00FFFFFF).
 * \retval VRT_ERR_INVALID_PACKING_METHOD               Packing method is an invalid value.
 * \retval VRT_ERR_INVALID_REAL_OR_COMPLEX              Real/Complex is an invalid value.
 * \retval VRT_ERR_INVALID_DATA_ITEM_FORMAT             Data item format is an invalid value.
 * \retval VRT_ERR_BOUNDS_EVENT_TAG_SIZE                Event tag size is outside valid bounds (> 0x07).
 * \retval VRT_ERR_BOUNDS_CHANNEL_TAG_SIZE              Channel tag size is outside valid bounds (> 0x0F).
 * \retval VRT_ERR_BOUNDS_ITEM_PACKING_FIELD_SIZE       Item packing fields size is outside valid bounds (> 0x3F).
 * \retval VRT_ERR_BOUNDS_DATA_ITEM_SIZE                Data item size is outside valid bounds (> 0x3F).
 * \retval VRT_ERR_INVALID_TSI                          TSI is an invalid value.
 * \retval VRT_ERR_INVALID_TSF                          TSF is an invalid value.
 * \retval VRT_ERR_SET_INTEGER_SECOND_TIMESTAMP         Integer second timestamp is not 0xFFFFFFFF when TSI is
 *                                                      VRT_TSI_UNDEFINED.
 * \retval VRT_ERR_SET_FRACTIONAL_SECOND_TIMESTAMP      Integer second timestamp is not 0xFFFFFFFF when TSI is
 *                                                      VRT_TSI_UNDEFINED.
 * \retval VRT_ERR_BOUNDS_REAL_TIME                     TSF is VRT_TSF_REAL TIME but picoseconds is outside valid bounds
 *                                                      (> 999999999999 ps).
 * \retval VRT_ERR_BOUNDS_LATITUDE                      Latitude is outside valid bounds (< -90 or > 90 degrees).
 * \retval VRT_ERR_BOUNDS_LONGITUDE                     Longitude is outside valid bounds (< -180 or > 180 degrees).
 * \retval VRT_ERR_BOUNDS_SPEED_OVER_GROUND             Speed over ground is outside valid bounds (< 0 m/s).
 * \retval VRT_ERR_BOUNDS_HEADING_ANGLE                 Heading angle outside valid bounds (< 0 or > 359.999999761582
 *                                                      degrees).
 * \retval VRT_ERR_BOUNDS_TRACK_ANGLE                   Track angle is outside valid bounds (< 0 or > 359.999999761582
 *                                                      degrees).
 * \retval VRT_ERR_BOUNDS_MAGNETIC_VARIATION            Magnetic variation is outside valid bounds (< -180 or > 180
 *                                                      degrees).
 * \retval VRT_ERR_BOUNDS_SOURCE_LIST_SIZE              Source list size is outside valid bounds (> 0x01FF).
 * \retval VRT_ERR_BOUNDS_SYSTEM_LIST_SIZE              System list size is outside valid bounds (> 0x01FF).
 * \retval VRT_ERR_BOUNDS_CHANNEL_LIST_SIZE             Channel list size is outside valid bounds (> 0x7FFF).
 *
 * \warning Fields represented as double but with an underlying 64-bit fixed precision format, i.e. bandwidth,
 *          if_reference_frequency, rf_reference_frequency, rf_reference_frequency_offset, if_band_offset, and
 *          sample_rate, may in rare cases lose precision since double only has 53 bits of precision.
 */
VRT_WARN_UNUSED
int32_t vrt_write_if_context(const vrt_if_context* if_context, void* buf, int32_t words_buf, bool validate);

/**
 * Higher-level function that writes a full VRT packet.
 *
 * \param packet     Packet to write.
 * \param buf        Buffer to write to.
 * \param words_buf  Size of buf in 32-bit words.
 * \param validate   True if validation should be done. If false, only buffer size is validated.
 *
 * \return Number of written 32-bit words, or a negative number if error.
 * \retval VRT_ERR_BUFFER_SIZE                            Buffer is too small.
 * \retval VRT_ERR_INVALID_PACKET_TYPE                    Packet type is an invalid value.
 * \retval VRT_ERR_INVALID_TSM                            TSM is an invalid value.
 * \retval VRT_ERR_TSM_IN_DATA                            Data packet has TSM bit set.
 * \retval VRT_ERR_INVALID_TSI                            TSI is an invalid value.
 * \retval VRT_ERR_INVALID_TSF                            TSF is an invalid value.
 * \retval VRT_ERR_BOUNDS_PACKET_COUNT                    Packet count is outside valid bounds (> 0x0F).
 * \retval VRT_ERR_BOUNDS_REAL_TIME                       TSF is VRT_TSF_REAL TIME but picoseconds is outside valid
 *                                                        bounds
 *                                                        (> 999999999999 ps).
 * \retval VRT_ERR_BOUNDS_OUI                             OUI is outside valid bounds (> 0x00FFFFFF).
 * \retval VRT_ERR_BOUNDS_ASSOCIATED_CONTEXT_PACKET_COUNT Associated context packet count is outside valid bounds
 *                                                        (>0x7F).
 * \retval VRT_ERR_GAIN_STAGE2_SET                        Gain stage 1 must be used instead of stage 2 when only one is
 *                                                        set.
 * \retval VRT_ERR_BOUNDS_SAMPLE_RATE                     Sample rate is outside valid bounds (< 0 Hz).
 * \retval VRT_ERR_BOUNDS_TEMPERATURE                     Temperature is outside valid bounds (< -273.15 degrees).
 * \retval VRT_ERR_INVALID_PACKING_METHOD                 Packing method is an invalid value.
 * \retval VRT_ERR_INVALID_REAL_OR_COMPLEX                Real/Complex is an invalid value.
 * \retval VRT_ERR_INVALID_DATA_ITEM_FORMAT               Data item format is an invalid value.
 * \retval VRT_ERR_BOUNDS_EVENT_TAG_SIZE                  Event tag size is outside valid bounds (> 0x07).
 * \retval VRT_ERR_BOUNDS_CHANNEL_TAG_SIZE                Channel tag size is outside valid bounds (> 0x0F).
 * \retval VRT_ERR_BOUNDS_ITEM_PACKING_FIELD_SIZE         Item packing fields size is outside valid bounds (> 0x3F).
 * \retval VRT_ERR_BOUNDS_DATA_ITEM_SIZE                  Data item size is outside valid bounds (> 0x3F).
 * \retval VRT_ERR_SET_INTEGER_SECOND_TIMESTAMP           Integer second timestamp is not 0xFFFFFFFF when TSI is
 *                                                        VRT_TSI_UNDEFINED.
 * \retval VRT_ERR_SET_FRACTIONAL_SECOND_TIMESTAMP        Integer second timestamp is not 0xFFFFFFFF when TSI is
 *                                                        VRT_TSI_UNDEFINED.
 * \retval VRT_ERR_BOUNDS_LATITUDE                        Latitude is outside valid bounds (< -90 or > 90 degrees).
 * \retval VRT_ERR_BOUNDS_LONGITUDE                       Longitude is outside valid bounds (< -180 or > 180 degrees).
 * \retval VRT_ERR_BOUNDS_SPEED_OVER_GROUND               Speed over ground is outside valid bounds (< 0 m/s).
 * \retval VRT_ERR_BOUNDS_HEADING_ANGLE                   Heading angle outside valid bounds (< 0 or > 359.999999761582
 *                                                        degrees).
 * \retval VRT_ERR_BOUNDS_TRACK_ANGLE                     Track angle is outside valid bounds (< 0 or > 359.999999761582
 *                                                        degrees).
 * \retval VRT_ERR_BOUNDS_MAGNETIC_VARIATION              Magnetic variation is outside valid bounds (< -180 or > 180
 *                                                        degrees).
 * \retval VRT_ERR_BOUNDS_SOURCE_LIST_SIZE                Source list size is outside valid bounds (> 0x01FF).
 * \retval VRT_ERR_BOUNDS_SYSTEM_LIST_SIZE                System list size is outside valid bounds (> 0x01FF).
 * \retval VRT_ERR_BOUNDS_CHANNEL_LIST_SIZE               Channel list size is outside valid bounds (> 0x7FFF).
 * \retval VRT_ERR_BOUNDS_PACKET_SIZE                     Calculated packet size is outside valid bounds (> 0xFFFF).
 *
 * \note Calculates packet_size field in header to match the number of written words. There is no need to calculate it
 *       beforehand. It is not modified in the const packet parameter, though.
 * \note May require output buffer data to be byte swapped if platform endianess isn't big endian (network order).
 * \note Will copy body data. For a zero-copy write, use the low-level routines.
 */
VRT_WARN_UNUSED
int32_t vrt_write_packet(const vrt_packet* packet, void* buf, int32_t words_buf, bool validate);

#ifdef __cplusplus
}
#endif

#endif
