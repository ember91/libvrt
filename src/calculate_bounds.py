#!/usr/bin/python3

# Calculate bounds for floating point fields such as bandwidth and attitude

import enum
import math

# Floating point type
class TypeFp(enum.Enum):
    Float = 0
    Double = 1


# Integer type
class TypeInt(enum.Enum):
    Int16 = 3
    Int32 = 1
    Uint32 = 2
    Int64 = 0


# Calculate lower and upper bounds and print to terminal
# name:     Field name
# type_fp:  Floating point type in deserialized representation
# type_int: Integer type in serialized representation
# radix:    Radix position from lsb
# lower:    Override when lower limit does not need to be calculated
# upper:    Override when upper limit does not need to be calculated
def pr(name, type_fp, type_int=None, radix=None, lower=None, upper=None):
    # Calculate information from integer data types
    bits = 0
    sign = True
    if type_int == TypeInt.Int16:
        bits = 16
        sign = 1
    elif type_int == TypeInt.Int32:
        bits = 32
        sign = 1
    elif type_int == TypeInt.Uint32:
        bits = 32
        sign = 0
    elif type_int == TypeInt.Int64:
        bits = 64
        sign = 1

    # Calculate lower and upper limits
    if lower is None:
        if sign == 1:
            lower = -math.pow(2, bits - radix - sign)
        else:
            lower = 0.0
    if upper is None:
        upper = math.pow(2, bits - radix - sign) - math.pow(2, -radix)

    # String fixing and printout
    name_type = type_fp.name.lower()
    name_var = name.upper().replace(" ", "_")
    suffix = "F" if type_fp == TypeFp.Float else ""
    print(f"static const {name_type} VRT_MIN_{name_var} = {lower}{suffix};")
    print(f"static const {name_type} VRT_MAX_{name_var} = {upper}{suffix};")


# Print all floating point fields
pr("Bandwidth", TypeFp.Double, TypeInt.Int64, radix=20, lower=0.0)
pr("IF Reference Frequency", TypeFp.Double, TypeInt.Int64, radix=20)
pr("RF Reference Frequency", TypeFp.Double, TypeInt.Int64, radix=20)
pr("RF Reference Frequency Offset", TypeFp.Double, TypeInt.Int64, radix=20)
pr("IF Band Offset", TypeFp.Double, TypeInt.Int64, radix=20)
pr("Reference Level", TypeFp.Float, TypeInt.Int16, radix=7)
pr("Gain", TypeFp.Float, TypeInt.Int16, radix=7)
pr("Sample Rate", TypeFp.Double, TypeInt.Int64, radix=20, lower=0.0)
pr("Temperature", TypeFp.Float, TypeInt.Int16, radix=6, lower=-273.15)
pr("Latitude", TypeFp.Double, lower=-90.0, upper=90.0)
pr("Longitude", TypeFp.Double, lower=-180.0, upper=180.0)
pr("Altitude", TypeFp.Double, TypeInt.Int32, radix=5)
pr("Speed Over Ground", TypeFp.Double, TypeInt.Uint32, radix=16)
pr(
    "Heading Angle",
    TypeFp.Double,
    lower=0.0,
    upper=360.0 - math.pow(2, -22),
)
pr(
    "Track Angle",
    TypeFp.Double,
    lower=0.0,
    upper=360.0 - math.pow(2, -22),
)
pr(
    "Magnetic Variation",
    TypeFp.Double,
    lower=-180.0,
    upper=180.0,
)
pr("Position", TypeFp.Double, TypeInt.Int32, radix=5)
pr("Attitude", TypeFp.Double, TypeInt.Int32, radix=22)
pr("Velocity", TypeFp.Double, TypeInt.Int32, radix=16)
