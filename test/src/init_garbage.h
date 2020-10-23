#ifndef TEST_SRC_INIT_GARBAGE_H_
#define TEST_SRC_INIT_GARBAGE_H_

#include <vrt/vrt_types.h>

/**
 * Initialize header to an easy to spot, non-default sequence.
 *
 * \param h Header.
 */
void init_header_garbage(vrt_header* h);

/**
 * Initialize fields section to an easy to spot, non-default sequence.
 *
 * \param f Fields.
 */
void init_fields_garbage(vrt_fields* f);

/**
 * Initialize trailer to an easy to spot, non-default sequence.
 *
 * \param t Trailer.
 */
void init_trailer_garbage(vrt_trailer* t);

/**
 * Initialize context to an easy to spot, non-default sequence.
 *
 * \param c Context.
 */
void init_context_garbage(vrt_context* c);

#endif
