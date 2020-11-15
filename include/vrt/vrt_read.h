#ifndef INCLUDE_VRT_VRT_READ_H_
#define INCLUDE_VRT_VRT_READ_H_

#include <vrt/vrt_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Low-level function that reads VRT header section.
 *
 * \param buf       Buffer to read from.
 * \param buf_words Size of buf in 32-bit words.
 * \param header    Header to read into.
 *
 * \return Number of read 32-bit words, or a negative number if error.
 * \retval VRT_ERR_BUF_SIZE Buffer is too small.
 *
 * \note Requires input buffer data to be byte swapped if platform endianess isn't big endian (network order).
 *
 * \warning This does not do any validation, i.e. it doesn't check if the field values make any sense.
 */
int32_t vrt_read_header(const void* buf, uint32_t buf_words, vrt_header* header);

/**
 * Low-level function that reads VRT fields section.
 *
 * \param header    Header.
 * \param buf       Buffer to read from. This must point to the first field in the sequence, i.e. directly following the
 *                  header word.
 * \param buf_words Size of buf in 32-bit words.
 * \param fields    Fields to read into.
 *
 * \return Number of read 32-bit words, or a negative number if error.
 * \retval VRT_ERR_BUF_SIZE Buffer is too small.
 *
 * \warning This does not do any validation, i.e. it doesn't check if the field values make any sense.
 */
int32_t vrt_read_fields(const vrt_header* header, const void* buf, uint32_t buf_words, vrt_fields* fields);

/**
 * Low-level function that reads VRT trailer section.
 *
 * \param buf       Buffer to read from. This must point to the position of the trailer word, i.e. the last word in the
 *                  packet.
 * \param buf_words Size of buf in 32-bit words.
 * \param trailer   Trailer to read into.
 *
 * \return Number of read 32-bit words, i.e. 1, or a negative number if error.
 * \retval VRT_ERR_BUF_SIZE Buffer is too small.
 *
 * \warning This does not do any validation, i.e. it doesn't check if the field values make any sense.
 */
int32_t vrt_read_trailer(const void* buf, uint32_t buf_words, vrt_trailer* trailer);

/**
 * Low-level function that reads VRT IF context section.
 *
 * \param buf        Buffer to read from.
 * \param buf_words  Size of buf in 32-bit words. Starting from the header word.
 * \param if_context IF context struct to read into.
 *
 * \return Number of read 32-bit words, or a negative number if error.
 * \retval VRT_ERR_BUF_SIZE Buffer is too small.
 *
 * \warning This does not do any validation, i.e. it doesn't check if the field values make any sense.
 * \warning Fields represented as double but with an underlying 64-bit fixed precision format, i.e. bandwidth,
 *          if_reference_frequency, rf_reference_frequency, rf_reference_frequency_offset, if_band_offset, and
 *          sample_rate, may in rare cases lose precision since double only has 53 bits of precision.
 */
int32_t vrt_read_if_context(const void* buf, uint32_t buf_words, vrt_if_context* if_context);

#ifdef __cplusplus
}
#endif

#endif
