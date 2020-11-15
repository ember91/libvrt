#ifndef INCLUDE_VRT_VRT_VALIDATE_H_
#define INCLUDE_VRT_VRT_VALIDATE_H_

#include <vrt/vrt_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Validate header against specification.
 *
 * \note This does not check that Packet size is the sum of all members.
 *
 * \param header Header.
 *
 * \return Error code (< 0) if it isn't valid. 0 otherwise.
 *
 * \retval VRT_ERR_PACKET_TYPE     Packet type is an invalid value.
 * \retval VRT_ERR_TRAILER         Context packet has trailer bit set.
 * \retval VRT_ERR_TSM             Data packet has TSM bit set.
 * \retval VRT_ERR_DATA_TSI        TSI is an invalid value.
 * \retval VRT_ERR_DATA_TSF        TSF is an invalid value.
 * \retval VRT_ERR_PACKET_COUNT    Packet count is outside valid bounds (> 0x0F).
 */
int32_t vrt_validate_header(const vrt_header* header);

/**
 * Validate fields against specification.
 *
 * \param header Header.
 * \param fields Fields.
 *
 * \return int32_t Error code (< 0) if it isn't valid. 0 otherwise.
 *
 * \retval VRT_ERR_OUI OUI is outside valid bounds (> 0x00FFFFFF).
 */
int32_t vrt_validate_fields(const vrt_header* header, const vrt_fields* fields);

/**
 * Validate IF context against specification.
 *
 * \param if_context IF context.
 *
 * \return int32_t Error code (< 0) if it isn't valid. 0 otherwise.
 *
 * \retval VRT_ERR_BANDWIDTH                   Bandwidth is negative.
 * \retval VRT_ERR_SAMPLE_RATE                 Sample rate is negative.
 * \retval VRT_ERR_TEMPERATURE                 Temperature is below absolute zero (< -273.15).
 * \retval VRT_ERR_OUI                         OUI is outside valid bounds (> 0x00FFFFFF).
 * \retval VRT_ERR_REAL_OR_COMPLEX             Real/Complex is an invalid value.
 * \retval VRT_ERR_DATA_ITEM_FORMAT            Data item format is an invalid value.
 * \retval VRT_ERR_EVENT_TAG_SIZE              Event tag size is outside valid bounds (> 0x07).
 * \retval VRT_ERR_CHANNEL_TAG_SIZE            Channel tag size is outside valid bounds (> 0x0F).
 * \retval VRT_ERR_ITEM_PACKING_FIELD_SIZE     Item packing fields size is outside valid bounds (> 0x3F).
 * \retval VRT_ERR_DATA_ITEM_SIZE              Data item size is outside valid bounds (> 0x3F).
 * \retval VRT_ERR_OUI                         OUI is outside valid bounds (> 0x00FFFFFF).
 * \retval VRT_ERR_TSI                         TSI is an invalid value.
 * \retval VRT_ERR_TSF                         TSF is an invalid value.
 * \retval VRT_ERR_INTEGER_SECOND_TIMESTAMP    Integer second timestamp is not 0xFFFFFFFF when TSI is VRT_TSI_UNDEFINED.
 * \retval VRT_ERR_FRACTIONAL_SECOND_TIMESTAMP Fractional second timestamp is not 0xFFFFFFFFFFFFFFFF when TSF is
 *         VRT_TSF_UNDEFINED.
 * \retval VRT_ERR_LATITUDE                    Latitude is outside valid bounds (< -90 or > 90).
 * \retval VRT_ERR_LONGITUDE                   Longitude is outside valid bounds (< -180 or > 180).
 * \retval VRT_ERR_HEADING_ANGLE               Heading angle outside valid bounds (< 0 or > 359.999999761582).
 * \retval VRT_ERR_TRACK_ANGLE                 Track angle is outside valid bounds (< 0 or > 359.999999761582).
 * \retval VRT_ERR_MAGNETIC_VARIATION          Magnetic variation is outside valid bounds (< -180 or > 180).
 * \retval VRT_ERR_SOURCE_LIST_SIZE            Source list size is outside valid bounds (> 0x01FF).
 * \retval VRT_ERR_SYSTEM_LIST_SIZE            System list size is outside valid bounds (> 0x01FF).
 * \retval VRT_ERR_CHANNEL_LIST_SIZE           Channel list size is outside valid bounds (> 0x7FFF).
 */
int32_t vrt_validate_if_context(const vrt_if_context* if_context);

/**
 * Validate trailer against specification.
 *
 * \param trailer Trailer.
 *
 * \return int32_t Error code (< 0) if it isn't valid. 0 otherwise.
 *
 * \retval VRT_ERR_ASSOCIATED_CONTEXT_PACKET_COUNT Associated context packet count is outside valid bounds (> 0x7F).
 */
int32_t vrt_validate_trailer(const vrt_trailer* trailer);

#ifdef __cplusplus
}
#endif

#endif
