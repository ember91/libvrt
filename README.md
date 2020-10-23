# libvrt

C implementation of an encoder and decoder of the ANSI/VITA 49.0 Radio Transport (VRT) standard.

**Features:**
* Well tested - High test code coverage, analyzed with static analysis tools
* Compatible - Tested with GCC, Clang, Visual Studio and ICC
* Well [documented](doc/html/index.html) - Doxygen style
* Low-level - A high degree of control
* Portable - Written in plain C99
* Flexible - Read and write only the parts of a packet you want
* Easy to install - Use CMake, with no external dependencies
* Complete - Implements full VRT standard
* Heap friendly - No heap allocations

This library uses a packet terminology as follows:
| Header - 1 word          |
|--------------------------|
| Fields - 0 to 6 words    |
| Data/context section     |
| Trailer - 1 word         |

Where a 'word' in this context is 32 bits long.

## Getting Started

In project root folder (`libvrt/`):
```
mkdir Release
cd Release
cmake ..
make -j4
sudo make install
```
This installs a static library. It can now be linked, e.g. with:
| Compiler       | Command                                 |
|----------------|-----------------------------------------|
| gcc            | gcc example.c -lvrt -lm -o example      |
| clang          | clang example.c -lvrt -lm -o example    |
| Visual Studio  | cl example.c vrt.lib                    |

*Note that you may need to link with a math library, with e.g. `-lm`, due to the inclusion of math.h.*

Generate a packet with:
```
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <vrt/vrt_common.h>
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
        s[i] = sinf(2.0F * PI * CENTER_FREQUENCY * i / SAMPLE_RATE);
    }

    /* Initialize to reasonable values */
    vrt_header  h;
    vrt_fields  f;
    vrt_trailer t;
    vrt_init_header(&h);
    vrt_init_fields(&f);
    vrt_init_trailer(&t);

    /* Configure */
    h.packet_type    = VRT_PT_IF_DATA_WITH_STREAM_ID;
    h.has.trailer    = true;
    h.packet_size    = SIZE;
    f.stream_id      = 0xDEADBEEF;
    t.reference_lock = true;

    /* Write header */
    int32_t offset = 0;
    int32_t rv     = vrt_write_header(&h, b + offset, SIZE);
    if (rv == VRT_ERR) {
        fprintf(stderr, "Failed to write header\n");
        return EXIT_FAILURE;
    }
    offset += rv;

    /* Write fields, which in this case is Stream ID */
    rv = vrt_write_fields(&h, &f, b + offset, SIZE - offset);
    if (rv == VRT_ERR) {
        fprintf(stderr, "Failed to write fields section\n");
        return EXIT_FAILURE;
    }
    offset += rv;

    /* Copy signal data from signal to packet buffer.
     * This could also have been written directly into the buffer. */
    memcpy(b + offset, s, sizeof(float) * (SIZE - 3));
    offset += SIZE - 3;

    /* Write trailer */
    rv = vrt_write_trailer(&t, b + offset, SIZE - offset);
    if (rv == VRT_ERR) {
        fprintf(stderr, "Failed to write trailer\n");
        return EXIT_FAILURE;
    }

    /* Write generated packet to file */
    FILE* fp = fopen("signal.vrt", "w");
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

    return EXIT_SUCCESS;
}
```
This example can be found in [examples/write_if_data.c](examples/write_if_data.c).

Useful functions for initializing structs to default values are:
```
vrt_init_header(header)
vrt_init_fields(fields)
vrt_init_trailer(trailer)
vrt_init_context(context)
```
For counting size of a packet:
```
vrt_words_fields(header)
vrt_words_trailer(header)
vrt_words_context(context)
```
For reading:
```
vrt_read_header(buf, buf_words, header)
vrt_read_fields(header, buf, buf_words, fields)
vrt_read_trailer(buf, buf_words, trailer)
vrt_read_context(buf, buf_words, context)
```
For writing:
```
vrt_write_header(header, buf, buf_words)
vrt_write_fields(header, fields, buf, buf_words)
vrt_write_trailer(const trailer, buf, buf_words)
vrt_write_context(const context, buf, buf_words)
```
And others:
```
vrt_is_context(type)
```
More documentation is found in the source code, or in the generated [documentation](doc/html/index.html).

Some functions may return `VRT_ERR` (-1) if there's an error.

### Prerequisites

* C99 compiler, such as GCC
* CMake
* Build system such as GNU Make
* (Google test framework, for running the tests)

### Notes

To follow the standard fully one must byte swap before reading and after writing on little endian platforms such as x86 and most ARM CPUs. As of now, there's no support for that in this library. Obviously, header, context, and trailer words must be swapped with 4 byte swaps while the data section depends on the data type.

You may need to link to the math library (`-lm` in GCC and clang), since this library uses some math functions.

## Running tests

Compile and run the test suite:
```
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

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details
