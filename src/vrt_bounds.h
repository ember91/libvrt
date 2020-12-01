#ifndef SRC_VRT_BOUNDS_H_
#define SRC_VRT_BOUNDS_H_

/* Macro for checking if value is inside bounds */
#define VRT_BOUNDS(min, val, max, ret)        \
    do {                                      \
        if ((val) < (min) || (val) > (max)) { \
            return (ret);                     \
        }                                     \
    } while (false)

/* Constants describing minimum and maximum of floating point fields.
 * Generated with calculate_bounds.py. */
static const double VRT_MIN_BANDWIDTH                     = 0.0;
static const double VRT_MAX_BANDWIDTH                     = 8796093022208.0;
static const double VRT_MIN_IF_REFERENCE_FREQUENCY        = -8796093022208.0;
static const double VRT_MAX_IF_REFERENCE_FREQUENCY        = 8796093022208.0;
static const double VRT_MIN_RF_REFERENCE_FREQUENCY        = -8796093022208.0;
static const double VRT_MAX_RF_REFERENCE_FREQUENCY        = 8796093022208.0;
static const double VRT_MIN_RF_REFERENCE_FREQUENCY_OFFSET = -8796093022208.0;
static const double VRT_MAX_RF_REFERENCE_FREQUENCY_OFFSET = 8796093022208.0;
static const double VRT_MIN_IF_BAND_OFFSET                = -8796093022208.0;
static const double VRT_MAX_IF_BAND_OFFSET                = 8796093022208.0;
static const float  VRT_MIN_REFERENCE_LEVEL               = -256.0F;
static const float  VRT_MAX_REFERENCE_LEVEL               = 255.9921875F;
static const float  VRT_MIN_GAIN                          = -256.0F;
static const float  VRT_MAX_GAIN                          = 255.9921875F;
static const double VRT_MIN_SAMPLE_RATE                   = 0.0;
static const double VRT_MAX_SAMPLE_RATE                   = 8796093022208.0;
static const float  VRT_MIN_TEMPERATURE                   = -273.15F;
static const float  VRT_MAX_TEMPERATURE                   = 511.984375F;
static const double VRT_MIN_LATITUDE                      = -90.0;
static const double VRT_MAX_LATITUDE                      = 90.0;
static const double VRT_MIN_LONGITUDE                     = -180.0;
static const double VRT_MAX_LONGITUDE                     = 180.0;
static const double VRT_MIN_ALTITUDE                      = -67108864.0;
static const double VRT_MAX_ALTITUDE                      = 67108863.96875;
static const double VRT_MIN_SPEED_OVER_GROUND             = 0.0;
static const double VRT_MAX_SPEED_OVER_GROUND             = 65535.99998474121;
static const double VRT_MIN_HEADING_ANGLE                 = 0.0;
static const double VRT_MAX_HEADING_ANGLE                 = 359.9999997615814;
static const double VRT_MIN_TRACK_ANGLE                   = 0.0;
static const double VRT_MAX_TRACK_ANGLE                   = 359.9999997615814;
static const double VRT_MIN_MAGNETIC_VARIATION            = -180.0;
static const double VRT_MAX_MAGNETIC_VARIATION            = 180.0;
static const double VRT_MIN_POSITION                      = -67108864.0;
static const double VRT_MAX_POSITION                      = 67108863.96875;
static const double VRT_MIN_ATTITUDE                      = -512.0;
static const double VRT_MAX_ATTITUDE                      = 511.9999997615814;
static const double VRT_MIN_VELOCITY                      = -32768.0;
static const double VRT_MAX_VELOCITY                      = 32767.99998474121;

#endif
