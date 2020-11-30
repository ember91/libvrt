#ifndef SRC_VRT_BOUNDS_H_
#define SRC_VRT_BOUNDS_H_

/* Macro for checking if value is inside bounds */
#define VRT_BOUNDS(min, val, max, ret)        \
    do {                                      \
        if ((val) < (min) || (val) > (max)) { \
            return (ret);                     \
        }                                     \
    } while (false)

/* Constants describing minimum and maximum of some floating point values */
static const double VRT_MIN_BANDWIDTH                        = 0.0;
static const double VRT_MAX_BANDWIDTH                        = 8.79e12;
static const double VRT_MINMAX_IF_REFERENCE_FREQUENCY        = 8.79e12;
static const double VRT_MINMAX_RF_REFERENCE_FREQUENCY        = 8.79e12;
static const double VRT_MINMAX_RF_REFERENCE_FREQUENCY_OFFSET = 8.79e12;
static const double VRT_MINMAX_IF_BAND_OFFSET                = 8.79e12;
static const float  VRT_MINMAX_REFERENCE_LEVEL               = 256.0F;
static const float  VRT_MINMAX_GAIN                          = 256.0F;
static const double VRT_MIN_SAMPLE_RATE                      = 0.0;
static const double VRT_MAX_SAMPLE_RATE                      = 8.79e12;
static const float  VRT_MIN_TEMPERATURE                      = -273.15F;
static const float  VRT_MAX_TEMPERATURE                      = 511.984375F;
static const double VRT_MINMAX_LATITUDE                      = 90.0;
static const double VRT_MINMAX_LONGITUDE                     = 180.0;
static const double VRT_MINMAX_ALTITUDE                      = 67108e3;
static const double VRT_MIN_SPEED_OVER_GROUND                = 0.0;
static const double VRT_MAX_SPEED_OVER_GROUND                = 65535.0; /* Misspelling in the standard! */
static const double VRT_MIN_HEADING_ANGLE                    = 0.0;
static const double VRT_MAX_HEADING_ANGLE                    = 359.999999761582;
static const double VRT_MIN_TRACK_ANGLE                      = 0.0;
static const double VRT_MAX_TRACK_ANGLE                      = 359.999999761582;
static const double VRT_MINMAX_MAGNETIC_VARIATION            = 180.0;
static const double VRT_MIN_POSITION                         = -67108864;
static const double VRT_MAX_POSITION                         = 67108863;
static const double VRT_MIN_ATTITUDE                         = -512;
static const double VRT_MAX_ATTITUDE                         = 511;
static const double VRT_MIN_VELOCITY                         = -32768;
static const double VRT_MAX_VELOCITY                         = 32767;

#endif
