#ifndef INCLUDE_VRT_VRT_WRITE_H_
#define INCLUDE_VRT_VRT_WRITE_H_

#include <stdint.h>

#include <vrt/vrt_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Low-level function that writes VRT header section.
 *
 * \param header    Header to write.
 * \param buf       Buffer to write to.
 * \param buf_words Size of buf in 32-bit words.
 * \param validate  True if validation should be done. If false, only buffer size is validated.
 *
 * \return Number of written 32-bit words (always 1), or a negative number if error.
 * \retval VRT_ERR_BUF_SIZE     Buffer is too small.
 * \retval VRT_ERR_PACKET_TYPE  Packet type is an invalid value.
 * \retval VRT_ERR_TSM          Data packet has TSM bit set.
 * \retval VRT_ERR_TSI          TSI is an invalid value.
 * \retval VRT_ERR_TSF          TSF is an invalid value.
 * \retval VRT_ERR_PACKET_COUNT Packet count is outside valid bounds (> 0x0F).
 *
 * \note May requires output buffer data to be byte swapped if platform endianess isn't big endian (network order).
 */
int32_t vrt_write_header(const vrt_header* header, void* buf, uint32_t buf_words, bool validate);

/**
 * Low-level function that writes VRT fields section.
 *
 * \param header    Header.
 * \param fields    Fields to write.
 * \param buf       Buffer to write to. This must point to the start of the fields section, i.e. directly following the
 *                  header word.
 * \param buf_words Size of buf in 32-bit words.
 * \param validate  True if validation should be done. If false, only buffer size is validated.
 *
 * \return Number of written 32-bit words, or a negative number if error.
 * \retval VRT_ERR_BUF_SIZE  Buffer is too small.
 * \retval VRT_ERR_REAL_TIME Fractional seconds Real time is activated but picoseconds is outside bounds.
 * \retval VRT_ERR_OUI       OUI is outside valid bounds (> 0x00FFFFFF).
 */
int32_t vrt_write_fields(const vrt_header* header,
                         const vrt_fields* fields,
                         void*             buf,
                         uint32_t          buf_words,
                         bool              validate);

/**
 * Low-level function that writes VRT trailer section.
 *
 * \param trailer    Trailer to write.
 * \param buf        Buffer to write to. This must point to the position of the trailer word, i.e. the last word in the
 *                   packet.
 * \param buf_words  Size of buf in 32-bit words.
 * \param validate  True if validation should be done. If false, only buffer size is validated.
 *
 * \return Number of written 32-bit words, or a negative number if error.
 * \retval VRT_ERR_BUF_SIZE                        Buffer is too small.
 * \retval VRT_ERR_ASSOCIATED_CONTEXT_PACKET_COUNT Associated context packet count is outside valid bounds (> 0x7F).
 */
int32_t vrt_write_trailer(const vrt_trailer* trailer, void* buf, uint32_t buf_words, bool validate);

/**
 * Low-level function that writes VRT IF context section.
 *
 * \param if_context IF context to write.
 * \param buf        Buffer to write to. This must point to the first context section word in the packet, i.e. directly
 *                   following the fields section.
 * \param buf_words  Size of buf in 32-bit words.
 * \param validate  True if validation should be done. If false, only buffer size is validated.
 *
 * \return Number of written 32-bit words, or a negative number if error.
 * \retval VRT_ERR_BUF_SIZE                    Buffer is too small.
 * \retval VRT_ERR_BANDWIDTH                   Bandwidth is negative.
 * \retval VRT_ERR_SAMPLE_RATE                 Sample rate is negative.
 * \retval VRT_ERR_TEMPERATURE                 Temperature is below absolute zero (< -273.15).
 * \retval VRT_ERR_OUI                         OUI is outside valid bounds (> 0x00FFFFFF).
 * \retval VRT_ERR_PACKING_METHOD              Packing method is an invalid value.
 * \retval VRT_ERR_REAL_OR_COMPLEX             Real/Complex is an invalid value.
 * \retval VRT_ERR_DATA_ITEM_FORMAT            Data item format is an invalid value.
 * \retval VRT_ERR_EVENT_TAG_SIZE              Event tag size is outside valid bounds (> 0x07).
 * \retval VRT_ERR_CHANNEL_TAG_SIZE            Channel tag size is outside valid bounds (> 0x0F).
 * \retval VRT_ERR_ITEM_PACKING_FIELD_SIZE     Item packing fields size is outside valid bounds (> 0x3F).
 * \retval VRT_ERR_DATA_ITEM_SIZE              Data item size is outside valid bounds (> 0x3F).
 * \retval VRT_ERR_TSI                         TSI is an invalid value.
 * \retval VRT_ERR_TSF                         TSF is an invalid value.
 * \retval VRT_ERR_INTEGER_SECOND_TIMESTAMP    Integer second timestamp is not 0xFFFFFFFF when TSI is VRT_TSI_UNDEFINED.
 * \retval VRT_ERR_FRACTIONAL_SECOND_TIMESTAMP Fractional second timestamp is not 0xFFFFFFFFFFFFFFFF when TSF is
 *                                             VRT_TSF_UNDEFINED.
 * \retval VRT_ERR_REAL_TIME                   Fractional seconds Real time is active but picoseconds is outside bounds.
 * \retval VRT_ERR_LATITUDE                    Latitude is outside valid bounds (< -90 or > 90).
 * \retval VRT_ERR_LONGITUDE                   Longitude is outside valid bounds (< -180 or > 180).
 * \retval VRT_ERR_SPEED_OVER_GROUND           Speed over ground is negative.
 * \retval VRT_ERR_HEADING_ANGLE               Heading angle outside valid bounds (< 0 or > 359.999999761582).
 * \retval VRT_ERR_TRACK_ANGLE                 Track angle is outside valid bounds (< 0 or > 359.999999761582).
 * \retval VRT_ERR_MAGNETIC_VARIATION          Magnetic variation is outside valid bounds (< -180 or > 180).
 * \retval VRT_ERR_SOURCE_LIST_SIZE            Source list size is outside valid bounds (> 0x01FF).
 * \retval VRT_ERR_SYSTEM_LIST_SIZE            System list size is outside valid bounds (> 0x01FF).
 * \retval VRT_ERR_CHANNEL_LIST_SIZE           Channel list size is outside valid bounds (> 0x7FFF).
 *
 * \warning Fields represented as double but with an underlying 64-bit fixed precision format, i.e. bandwidth,
 *          if_reference_frequency, rf_reference_frequency, rf_reference_frequency_offset, if_band_offset, and
 *          sample_rate, may in rare cases lose precision since double only has 53 bits of precision.
 */
int32_t vrt_write_if_context(const vrt_if_context* if_context, void* buf, uint32_t buf_words, bool validate);

#ifdef __cplusplus
}
#endif

#endif
