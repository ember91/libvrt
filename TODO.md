* Add vrt_words_packet
* Rename init_header_garbage... to init_garbage_header
* When testing buffer size error, check both with and without validation
* Add tests that test negative buffer size
* vrt_is_context and similar functions should take header as parameter
* Add int32_t vrt_read_packet(vrt_packet* p, void* buf, int32_t words_buf validate)
* Add int32_t vrt_words_body(vrt_header* h), which calculates number of words body consists of

* Trailer: Permission 6.1.7-1: The user-defined indicators may be used for any purpose. They may be used together as well
as individually.
* IF context: Observation 7.1.5.17-2: The user-defined bits in positions [7..0] may be used to indicate eight independent binary
states, or up to 256 mutually-exclusive states, or any combination of independent and mutually-exclusive states that
can be represented in the 8-bit field.

* Check that float, double... are inside bounds when writing and validating
# Future
* Restrict keyword?
* Add support for 128 bit float
* Add single header library?
