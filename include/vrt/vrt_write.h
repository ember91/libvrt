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
 *
 * \return Number of written 32-bit words (always 1), or a negative number if error.
 * \retval VRT_ERR_BUF_SIZE Buffer is too small.
 *
 * \note May requires output buffer data to be byte swapped if platform endianess isn't big endian (network order).
 *
 * \warning This does not do any validation, i.e. it doesn't check if the field values make any sense.
 */
int32_t vrt_write_header(const vrt_header* header, void* buf, uint32_t buf_words);

/**
 * Low-level function that writes VRT fields section.
 *
 * \param header    Header.
 * \param fields    Fields to write.
 * \param buf       Buffer to write to. This must point to the start of the fields section, i.e. directly following the
 *                  header word.
 * \param buf_words Size of buf in 32-bit words.
 *
 * \return Number of written 32-bit words, or a negative number if error.
 * \retval VRT_ERR_BUF_SIZE Buffer is too small.
 *
 * \warning This does not do any validation, i.e. it doesn't check if the field values make any sense.
 */
int32_t vrt_write_fields(const vrt_header* header, const vrt_fields* fields, void* buf, uint32_t buf_words);

/**
 * Low-level function that writes VRT trailer section.
 *
 * \param trailer    Trailer to write.
 * \param buf        Buffer to write to. This must point to the position of the trailer word, i.e. the last word in the
 *                   packet.
 * \param buf_words  Size of buf in 32-bit words.
 *
 * \return Number of written 32-bit words, or a negative number if error.
 * \retval VRT_ERR_BUF_SIZE Buffer is too small.
 *
 * \warning This does not do any validation, i.e. it doesn't check if the field values make any sense.
 */
int32_t vrt_write_trailer(const vrt_trailer* trailer, void* buf, uint32_t buf_words);

/**
 * Low-level function that writes VRT IF context section.
 *
 * \param if_context IF context to write.
 * \param buf        Buffer to write to. This must point to the first context section word in the packet, i.e. directly
 *                   following the fields section.
 * \param buf_words  Size of buf in 32-bit words.
 *
 * \return Number of written 32-bit words, or a negative number if error.
 * \retval VRT_ERR_BUF_SIZE Buffer is too small.
 *
 * \warning This does not do any validation, i.e. it doesn't check if the field values make any sense.
 * \warning Fields represented as double but with an underlying 64-bit fixed precision format, i.e. bandwidth,
 *          if_reference_frequency, rf_reference_frequency, rf_reference_frequency_offset, if_band_offset, and
 *          sample_rate, may in rare cases lose precision since double only has 53 bits of precision.
 */
int32_t vrt_write_if_context(const vrt_if_context* if_context, void* buf, uint32_t buf_words);

#ifdef __cplusplus
}
#endif

#endif
