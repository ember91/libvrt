cmake_minimum_required(VERSION 3.0)

project(
  examples
  LANGUAGES C
  DESCRIPTION "Example suite for libvrt.")

find_package(StandardMathLibrary)

# Add executables
add_executable(read ${CMAKE_CURRENT_SOURCE_DIR}/src/read_if_data.c)
add_executable(write_simple
               ${CMAKE_CURRENT_SOURCE_DIR}/src/write_if_data_simple.c)
add_executable(write_advanced
               ${CMAKE_CURRENT_SOURCE_DIR}/src/write_if_data_advanced.c)

# Set C standard
set_target_properties(read PROPERTIES C_STANDARD 99)
set_target_properties(write_simple PROPERTIES C_STANDARD 99)
set_target_properties(write_advanced PROPERTIES C_STANDARD 99)

# Set warning levels
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  if(CMAKE_C_COMPILER_ID MATCHES "Clang")
    target_compile_options(read PRIVATE -Weverything -Wno-padded
                                        -Wno-newline-eof)
    target_compile_options(write_simple PRIVATE -Weverything -Wno-padded
                                                -Wno-newline-eof)
    target_compile_options(write_advanced PRIVATE -Weverything -Wno-padded
                                                  -Wno-newline-eof)
  elseif(CMAKE_C_COMPILER_ID MATCHES "GNU")
    target_compile_options(read PRIVATE -Wall -Wextra -Wpedantic -Wshadow)
    target_compile_options(write_simple PRIVATE -Wall -Wextra -Wpedantic
                                                -Wshadow)
    target_compile_options(write_advanced PRIVATE -Wall -Wextra -Wpedantic
                                                  -Wshadow)
  elseif(CMAKE_C_COMPILER_ID MATCHES "Intel")
    target_compile_options(read PRIVATE -w3) # /W5 on Windows
    target_compile_options(write_simple PRIVATE -w3) # /W5 on Windows
    target_compile_options(write_advanced PRIVATE -w3) # /W5 on Windows
  elseif(CMAKE_C_COMPILER_ID MATCHES "MSVC")
    target_compile_options(read PRIVATE /W4)
    target_compile_options(write_simple PRIVATE /W4)
    target_compile_options(write_advanced PRIVATE /W4)
  endif()
endif()

# Add include directory
target_include_directories(read
                           PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../include/")
target_include_directories(write_simple
                           PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../include/")
target_include_directories(write_advanced
                           PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../include/")

# Add libraries to link to
target_link_libraries(read vrt)
target_link_libraries(write_simple vrt ${STANDARD_MATH_LIBRARY})
target_link_libraries(write_advanced vrt ${STANDARD_MATH_LIBRARY})
