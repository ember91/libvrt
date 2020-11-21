# libvrt

C implementation of an encoder and decoder of the ANSI/VITA 49.0 Radio Transport (VRT) standard.

**Features:**
* Well tested - High test code coverage, analyzed with static analysis tools
* Compatible - Tested with GCC, Clang, Visual Studio and ICC
* Well documented - Doxygen style
* Low-level - A high degree of control
* Portable - Written in plain C99
* Flexible - Read and write only the parts of a packet you want
* Easy to install - Use CMake, with no external dependencies
* Complete - Implements full VRT standard
* Heap friendly - No heap allocations

This library uses a packet terminology as follows:
| Header: 1 word       |
|----------------------|
| Fields: 0 - 6 words  |
| Data/context section |
| Trailer: 0 - 1 words |

Where a 'word' in this context is 32 bits long.

## Getting Started

In project root folder (`libvrt/`):
```bash
mkdir Release
cd Release
cmake ..
make -j4
sudo make install
```
This installs a static library. It can now be linked, e.g. with:
| Compiler       | Command                             |
|----------------|-------------------------------------|
| gcc            | gcc example.c -lvrt -o example      |
| clang          | clang example.c -lvrt -o example    |
| icc            |                                     |
| Visual Studio  | cl example.c vrt.lib                |

Generate a packet with:
```c++
/*
 * Generate signal and write VRT IF data packet to file. Note that this won't generate a big endian-format packet if on
 * a little endian platform, so the generated packet may be non-standard.
 */

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <vrt/vrt_init.h>
#include <vrt/vrt_string.h>
#include <vrt/vrt_util.h>
#include <vrt/vrt_write.h>

/* Size of packet in 32-bit words */
#define SIZE 515
/* Sample rate [Hz] */
#define SAMPLE_RATE 44100.0F
/* Center frequency [Hz] */
#define CENTER_FREQUENCY 10000.0F
/* M_PI in math.h is nonstandard :( */
#define PI 3.1415926F

int main() {
    /* Packet data buffer */
    uint32_t b[SIZE];

    /* Generate signal data */
    float s[SIZE - 3];
    for (int i = 0; i < SIZE - 3; ++i) {
        s[i] = sinf(2.0F * PI * CENTER_FREQUENCY * (float)i / SAMPLE_RATE);
    }

    /* Initialize to reasonable values */
    vrt_header  h;
    vrt_fields  f;
    vrt_trailer t;
    vrt_init_header(&h);
    vrt_init_fields(&f);
    vrt_init_trailer(&t);

    /* Configure */
    h.packet_type        = VRT_PT_IF_DATA_WITH_STREAM_ID;
    h.has.trailer        = true;
    h.packet_size        = SIZE;
    f.stream_id          = 0xDEADBEEF;
    t.has.reference_lock = true;
    t.reference_lock     = true;

    /* Write header */
    int32_t offset = 0;
    int32_t rv     = vrt_write_header(&h, b + offset, SIZE - offset, true);
    if (rv < 0) {
        fprintf(stderr, "Failed to write header: %s\n", vrt_string_error(rv));
        return EXIT_FAILURE;
    }
    offset += rv;

    /* Write fields, which in this case is Stream ID */
    rv = vrt_write_fields(&h, &f, b + offset, SIZE - offset, true);
    if (rv < 0) {
        fprintf(stderr, "Failed to write fields section: %s\n", vrt_string_error(rv));
        return EXIT_FAILURE;
    }
    offset += rv;

    /* Copy signal data from signal to packet buffer.
     * This could also have been written directly into the buffer. */
    memcpy(b + offset, s, sizeof(float) * (SIZE - 3));
    offset += SIZE - 3;

    /* Write trailer */
    rv = vrt_write_trailer(&t, b + offset, SIZE - offset, true);
    if (rv < 0) {
        fprintf(stderr, "Failed to write trailer: %s\n", vrt_string_error(rv));
        return EXIT_FAILURE;
    }

    /* Write generated packet to file */
    FILE* fp = fopen("signal.vrt", "wb");
    if (fp == NULL) {
        fprintf(stderr, "Failed to open file\n");
        return EXIT_FAILURE;
    }
    if (fwrite(b, sizeof(uint32_t) * SIZE, 1, fp) != 1) {
        fprintf(stderr, "Failed to write to file\n");
        fclose(fp);
        return EXIT_FAILURE;
    }
    fclose(fp);

    /* Warn if not standards compliant */
    if (vrt_is_platform_little_endian()) {
        printf("Warning: Written packet is little endian. It is NOT compliant with the VRT standard.\n");
    }

    return EXIT_SUCCESS;
}
```
Examples can be found in [example/](example/).

Useful functions for initializing structs to default values are:
```
vrt_init_header(header)
vrt_init_fields(fields)
vrt_init_trailer(trailer)
vrt_init_if_context(if_context)
```
For counting size of a packet:
```
vrt_words_fields(header)
vrt_words_trailer(header)
vrt_words_if_context(if_context)
```
For reading:
```
vrt_read_header(buf, buf_words, header, validate)
vrt_read_fields(header, buf, buf_words, fields, validate)
vrt_read_trailer(buf, buf_words, trailer)
vrt_read_if_context(buf, buf_words, if_context, validate)
```
For writing:
```
vrt_write_header(header, buf, buf_words, validate)
vrt_write_fields(header, fields, buf, buf_words, validate)
vrt_write_trailer(const trailer, buf, buf_words, validate)
vrt_write_if_context(const if_context, buf, buf_words, validate)
```
For making string representations:
```
vrt_string_packet_type(packet_type)
vrt_string_tsi(tsi)
vrt_string_tsf(tsf)
vrt_string_real_or_complex(real_or_complex)
vrt_string_data_item_format(data_item_format)
```
And others:
```
vrt_is_context(type)
vrt_has_stream_id(type)
vrt_is_platform_little_endian()
```
More documentation is found in the source code, or in the generated documentation found in `doc/index.html`.

Some functions return negative numbers if there's an error.

### Prerequisites

* C99 compiler, such as GCC
* CMake
* Build system such as GNU Make
* (C++ compiler and Google test framework, for the tests)

### Notes

To follow the standard fully one must byte swap before reading and after writing on little endian platforms such as x86 and most ARM CPUs. As of now, there's no support for that in this library. Obviously, header, fields section, context, and trailer words must be swapped with 4 byte swaps while the data section depends on the data type.

## Running tests

Compile and run the test suite:
```bash
mkdir Debug
cd Debug
cmake -DCMAKE_BUILD_TYPE=Debug -DTEST=On ..
make -j4
./test/run_unit_tests
```

## Author

**Emil Berg**

## Other libraries

Tools using this library may pop up in the future.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details
