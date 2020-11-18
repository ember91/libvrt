* Cannot run sudo make install with sphinx
* Improve documentation in sphinx
* Should error codes really be in common.h?
* In write test, break out common initializations of buffer for GPS, INS, ECEF ephemeris and Relative ephemeris. 
* Compile with -fvisibility=hidden?
* Header only library
* Add tests that write to buffer and then read from the same buffer. vrt_compare_...() functions may be useful for this.

* Add read example
* Add int32_t vrt_write_data(vrt_header* h, vrt_fields* f, uint32_t* data, uint16_t* data_words, vrt_trailer* t)
* Add int32_t vrt_write_if_context(vrt_header* h, vrt_fields* f, uint32_t* data, uint16_t* data_words, vrt_trailer* t)
* Add int32_t vrt_read_all(vrt_header* h, vrt_fields* f, uint32_t* body, uint16_t* body_words, vrt_if_context* c, vrt_trailer* t)
* Add int32_t vrt_words_body(vrt_header* h), which calculates number of words body consists of

* Check that float, double... are inside bound when writing and validating

* cmake format
* Rerun static analysis tools
* Compile again with other compilers
* Not following google style guide...
