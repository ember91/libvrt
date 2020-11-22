* Add int32_t vrt_write_data(vrt_header* h, vrt_fields* f, uint32_t* data, uint16_t* data_words, vrt_trailer* t)
* Add int32_t vrt_write_if_context(vrt_header* h, vrt_fields* f, uint32_t* data, uint16_t* data_words, vrt_trailer* t)
* Add int32_t vrt_read_all(vrt_header* h, vrt_fields* f, uint32_t* body, uint16_t* body_words, vrt_if_context* c, vrt_trailer* t)
* Add int32_t vrt_words_body(vrt_header* h), which calculates number of words body consists of

* Trailer: Permission 6.1.7-1: The user-defined indicators may be used for any purpose. They may be used together as well
as individually.
* IF context: Observation 7.1.5.17-2: The user-defined bits in positions [7..0] may be used to indicate eight independent binary
states, or up to 256 mutually-exclusive states, or any combination of independent and mutually-exclusive states that
can be represented in the 8-bit field.

* Check that float, double... are inside bounds when writing and validating
# Future
* Add support for 128 bit float
* Add single header library?
