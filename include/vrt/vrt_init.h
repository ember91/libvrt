#ifndef INCLUDE_VRT_VRT_INIT_H_
#define INCLUDE_VRT_VRT_INIT_H_

#include "vrt_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialize header to a reasonable default state.
 *
 * \param header Header.
 *
 * \note Packet size must still be set explicitly to the actual packet size in 32-bit words.
 */
void vrt_init_header(vrt_header* header);

/**
 * Initialize fields section to a reasonable default state.
 *
 * \param fields Fields.
 */
void vrt_init_fields(vrt_fields* fields);

/**
 * Initialize trailer to a reasonable default state.
 *
 * \param trailer Trailer.
 */
void vrt_init_trailer(vrt_trailer* trailer);

/**
 * Initialize IF context to a reasonable default state.
 *
 * \param if_context IF context.
 */
void vrt_init_if_context(vrt_if_context* if_context);

#ifdef __cplusplus
}
#endif

#endif
