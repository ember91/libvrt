include(CheckCSourceCompiles)

set(find_standard_math_library_test_program "#include<math.h>
     int main() { sin(0.0); logf(0.0f); }")

# First try compiling/linking the test program without any linker flags

set(CMAKE_REQUIRED_FLAGS "")
set(CMAKE_REQUIRED_LIBRARIES "")
check_c_source_compiles("${find_standard_math_library_test_program}"
                        standard_math_library_linked_to_automatically)

if(standard_math_library_linked_to_automatically)
  # Test program linked successfully without any linker flag.
  set(STANDARD_MATH_LIBRARY "")
  set(STANDARD_MATH_LIBRARY_FOUND TRUE)
else()
  # Test program did not link successfully without any linker flag. Try standard
  # name 'm' for the standard math library.

  set(CMAKE_REQUIRED_LIBRARIES "m")
  check_c_source_compiles("${find_standard_math_library_test_program}"
                          standard_math_library_linked_to_as_m)
  if(standard_math_library_linked_to_as_m)
    # Test program linked successfully when linking to the 'm' library
    set(STANDARD_MATH_LIBRARY "m")
    set(STANDARD_MATH_LIBRARY_FOUND TRUE)
  else()
    # Test program still doesn't link successfully
    set(STANDARD_MATH_LIBRARY_FOUND FALSE)
  endif()
endif()
