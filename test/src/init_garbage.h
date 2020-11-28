#ifndef TEST_SRC_INIT_GARBAGE_H_
#define TEST_SRC_INIT_GARBAGE_H_

#include <vrt/vrt_types.h>

/**
 * Initialize header to an easy to spot, non-default sequence.
 *
 * \param h Header.
 */
void init_garbage_header(vrt_header* h);

/**
 * Initialize fields section to an easy to spot, non-default sequence.
 *
 * \param f Fields.
 */
void init_garbage_fields(vrt_fields* f);

/**
 * Initialize trailer to an easy to spot, non-default sequence.
 *
 * \param t Trailer.
 */
void init_garbage_trailer(vrt_trailer* t);

/**
 * Initialize IF context to an easy to spot, non-default sequence.
 *
 * \param c IF context.
 */
void init_garbage_if_context(vrt_if_context* c);

/**
 * Initialize VRT packet to an easy to spot, non-default sequence.
 *
 * \param p VRT packet.
 */
void init_garbage_packet(vrt_packet* p);

#endif
