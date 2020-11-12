* Compile with -fvisibility=hidden?
* context should be renamed to if_data_context to separate it from ext context
* Remember to validate that packet_size = header + fields for context packets only
* Use CMAKE_CURRENT_SOURCE_DIR
* Rename VRT_SIZE_HEADER and similar constants to VRT_WORDS_HEADER?
* Breathe and sphinx?
* Add tests that write to buffer and then read from the same buffer
* Add read example
* Add vrt_validate_header(), vrt_validate_fields(), vrt_validate_context(), vrt_validate_trailer(), ...
* Add int32_t vrt_write_data(vrt_header* h, vrt_fields* f, uint32_t* data, uint16_t* data_words, vrt_trailer* t)
* Add int32_t vrt_write_context(vrt_header* h, vrt_fields* f, uint32_t* data, uint16_t* data_words, vrt_trailer* t)
* Add int32_t vrt_read_all(vrt_header* h, vrt_fields* f, uint32_t* body, uint16_t* body_words, vrt_context* c, vrt_trailer* t)
* Add int32_t vrt_words_body(vrt_header* h), which calculates number of words body consists of
* float128 instead of double for fixed point?
* Fuzzing
* Byte swapping functionality?
