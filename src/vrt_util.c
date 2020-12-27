#include "vrt_util.h"

struct vrt_header;

/* Defined inline in header */
extern bool vrt_is_context(const struct vrt_header* header);
extern bool vrt_has_stream_id(const struct vrt_header* header);
extern bool vrt_is_platform_little_endian();
