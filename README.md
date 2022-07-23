# libvrt

C implementation of an encoder and decoder of the ANSI/VITA 49.0 Radio Transport (VRT) standard.

**Features:**
* Well tested - High test code coverage with more than 900 tests
* Analyzed - With runtime analysis tools such as valgrind
* Compatible - Tested with GCC, Clang, Visual Studio, and ICC
* Well documented - Doxygen style
* Low-level - A high degree of control
* Portable - Written in plain C99
* Flexible - Read and write only the parts of a packet you want
* Easy to install - Use CMake, with no external dependencies except standard library
* Easy to learn - Comes with examples
* Complete - Implements full VRT standard
* Heap friendly - No heap allocations
* No printf - Neither stdout nor stderr output
* Packet validation - But only if you want

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
| icc            | icc example.c -lvrt -o example      |
| Visual Studio  | cl example.c vrt.lib                |

Generate a packet with:
```c++
/*
 * Generate signal and write VRT IF data packet to file. Note that this will not generate a big endian-format, i.e.
 * standard conforming, packet on a little endian platform.
 */

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <vrt/vrt_init.h>
#include <vrt/vrt_string.h>
#include <vrt/vrt_types.h>
#include <vrt/vrt_util.h>
#include <vrt/vrt_write.h>

/* Size of buffer in 32-bit words */
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
    struct vrt_packet p;
    vrt_init_packet(&p);

    /* Configure */
    p.header.packet_type         = VRT_PT_IF_DATA_WITH_STREAM_ID;
    p.header.has.trailer         = true;
    p.header.packet_size         = SIZE;
    p.fields.stream_id           = 0xDEADBEEF;
    p.words_body                 = SIZE - 3;
    p.body                       = s;
    p.trailer.has.reference_lock = true;
    p.trailer.reference_lock     = true;

    /* Write to buffer */
    int32_t rv = vrt_write_packet(&p, b, SIZE, true);
    if (rv < 0) {
        fprintf(stderr, "Failed to write packet: %s\n", vrt_string_error(rv));
        return EXIT_FAILURE;
    }

    /* Write generated packet to file */
    const char* file_path = "signal.vrt";
    FILE*       fp        = fopen(file_path, "wb");
    if (fp == NULL) {
        fprintf(stderr, "Failed to open file '%s'\n", file_path);
        return EXIT_FAILURE;
    }
    if (fwrite(b, sizeof(uint32_t) * SIZE, 1, fp) != 1) {
        fprintf(stderr, "Failed to write to file '%s'\n", file_path);
        fclose(fp);
        return EXIT_FAILURE;
    }
    fclose(fp);

    /* Warn if not standards compliant */
    if (vrt_is_platform_little_endian()) {
        fprintf(stderr, "Warning: Written packet is little endian. It is NOT compliant with the VRT standard.\n");
    }

    return EXIT_SUCCESS;
}
```
Examples can be found [here](example/src).

For initializing a packet struct to default values:
```
vrt_init_packet(packet, buf, words_buf, validate)
```
For calculating the size of a packet:
```
vrt_words_packet(packet)
```
For reading:
```
vrt_read_packet(buf, words_buf, packet, validate)
```
For writing:
```
vrt_write_packet(packet, buf, words_buf, validate)
```
For calculating time between packets:
```
vrt_time_difference_fields(header2, fields2, header1, fields1, sample_rate, diff)
```
For converting a packet timestamp to a calendar representation:
```
vrt_time_calendar_fields(packet, sample_rate, time)
```
For getting a string representation of an error:
```
vrt_string_error(error)
```

**Other useful lower level functions:**\
For initializing structs to default values:
```
vrt_init_header(header)
vrt_init_fields(fields)
vrt_init_trailer(trailer)
vrt_init_if_context(if_context)
```
For counting serialized struct sizes:
```
vrt_words_fields(header)
vrt_words_trailer(header)
vrt_words_if_context(if_context)
```
For reading:
```
vrt_read_header(buf, words_buf, header, validate)
vrt_read_fields(header, buf, words_buf, fields, validate)
vrt_read_trailer(buf, words_buf, trailer)
vrt_read_if_context(buf, words_buf, if_context, validate)
```
For writing:
```
vrt_write_header(header, buf, words_buf, validate)
vrt_write_fields(header, fields, buf, words_buf, validate)
vrt_write_trailer(const trailer, buf, words_buf, validate)
vrt_write_if_context(const if_context, buf, words_buf, validate)
```
For making string representations:
```
vrt_string_packet_type(packet_type)
vrt_string_tsi(tsi)
vrt_string_tsf(tsf)
vrt_string_real_or_complex(real_or_complex)
vrt_string_data_item_format(data_item_format)
```
For handling time:
```
vrt_time_difference_fields(header2, fields2, header1, fields1, sample_rate, diff)
vrt_time_calendar_fields(header, fields, sample_rate, time)
vrt_time_calendar_gps_geolocation(if_context, sample_rate, time)
vrt_time_calendar_ins_geolocation(if_context, sample_rate, time)
vrt_time_calendar_ecef_ephemeris(if_context, sample_rate, time)
vrt_time_calendar_relative_ephemeris(if_context, sample_rate, time)
vrt_time_calendar_calibration(header, if_context, sample_rate, time)
```
And other helper functions:
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
* Build system, such as GNU Make

**For tests:**
* C++17 compiler
* Google test framework

**For documentation generation:**
* Doxygen
* Sphinx
* Breathe
* Exhale

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

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details
