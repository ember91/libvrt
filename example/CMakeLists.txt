cmake_minimum_required(VERSION 3.0)

project(
  examples
  LANGUAGES C
  DESCRIPTION "Example suite for libvrt.")

find_package(StandardMathLibrary)

# Check if options are set
if(${CLANG_TIDY})
  message(STATUS "Enabling clang-tidy for C")
  set(CMAKE_C_CLANG_TIDY
      "clang-tidy;-checks=*,-readability-magic-numbers,-cppcoreguidelines-avoid-magic-numbers,-hicpp-signed-bitwise,-android-cloexec-fopen"
  )
endif()
if(${CPPCHECK})
  message(STATUS "Enabling Cppcheck for C")
  set(CMAKE_C_CPPCHECK "cppcheck;--std=c99")
endif()
if(${CPPLINT})
  message(STATUS "Enabling cpplint for C")
  set(CMAKE_C_CPPLINT
      "cpplint;--linelength=120;--filter=-legal/copyright,-build/include_subdir"
  )
endif()
if(${IWYU})
  message(STATUS "Enabling Include-what-you-use for C")
  set(CMAKE_C_INCLUDE_WHAT_YOU_USE "/usr/local/bin/include-what-you-use")
endif()

# Add executables
add_executable(read_simple
               ${CMAKE_CURRENT_SOURCE_DIR}/src/read_if_data_simple.c)
add_executable(read_advanced
               ${CMAKE_CURRENT_SOURCE_DIR}/src/read_if_data_advanced.c)
add_executable(write_simple
               ${CMAKE_CURRENT_SOURCE_DIR}/src/write_if_data_simple.c)
add_executable(write_advanced
               ${CMAKE_CURRENT_SOURCE_DIR}/src/write_if_data_advanced.c)
add_executable(time_difference
               ${CMAKE_CURRENT_SOURCE_DIR}/src/time_difference.c)

# Set C standard
set_target_properties(read_simple PROPERTIES C_STANDARD 99)
set_target_properties(read_advanced PROPERTIES C_STANDARD 99)
set_target_properties(write_simple PROPERTIES C_STANDARD 99)
set_target_properties(write_advanced PROPERTIES C_STANDARD 99)
set_target_properties(time_difference PROPERTIES C_STANDARD 99)

# Set warning levels
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  if(CMAKE_C_COMPILER_ID MATCHES "Clang")
    target_compile_options(read_simple PRIVATE -Weverything -Wno-padded
                                               -Wno-newline-eof)
    target_compile_options(read_advanced PRIVATE -Weverything -Wno-padded
                                                 -Wno-newline-eof)
    target_compile_options(write_simple PRIVATE -Weverything -Wno-padded
                                                -Wno-newline-eof)
    target_compile_options(write_advanced PRIVATE -Weverything -Wno-padded
                                                  -Wno-newline-eof)
    target_compile_options(time_difference PRIVATE -Weverything -Wno-padded
                                                  -Wno-newline-eof)
  elseif(CMAKE_C_COMPILER_ID MATCHES "GNU")
    target_compile_options(read_simple PRIVATE -Wall -Wextra -Wpedantic
                                               -Wshadow)
    target_compile_options(read_advanced PRIVATE -Wall -Wextra -Wpedantic
                                                 -Wshadow)
    target_compile_options(write_simple PRIVATE -Wall -Wextra -Wpedantic
                                                -Wshadow)
    target_compile_options(write_advanced PRIVATE -Wall -Wextra -Wpedantic
                                                  -Wshadow)
    target_compile_options(time_difference PRIVATE -Wall -Wextra -Wpedantic
                                                  -Wshadow)
  elseif(CMAKE_C_COMPILER_ID MATCHES "Intel")
    target_compile_options(read_simple PRIVATE -w3) # /W5 on Windows
    target_compile_options(read_advanced PRIVATE -w3) # /W5 on Windows
    target_compile_options(write_simple PRIVATE -w3) # /W5 on Windows
    target_compile_options(write_advanced PRIVATE -w3) # /W5 on Windows
    target_compile_options(time_difference PRIVATE -w3) # /W5 on Windows
  elseif(CMAKE_C_COMPILER_ID MATCHES "MSVC")
    target_compile_options(read_simple PRIVATE /W4)
    target_compile_options(read_advanced PRIVATE /W4)
    target_compile_options(write_simple PRIVATE /W4)
    target_compile_options(write_advanced PRIVATE /W4)
    target_compile_options(time_difference PRIVATE /W4)
  endif()
endif()

# Add include directory
target_include_directories(read_simple
                           PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../include/")
target_include_directories(read_advanced
                           PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../include/")
target_include_directories(write_simple
                           PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../include/")
target_include_directories(write_advanced
                           PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../include/")
target_include_directories(time_difference
                           PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../include/")

# Add libraries to link to
target_link_libraries(read_simple vrt)
target_link_libraries(read_advanced vrt)
target_link_libraries(write_simple vrt ${STANDARD_MATH_LIBRARY})
target_link_libraries(write_advanced vrt ${STANDARD_MATH_LIBRARY})
target_link_libraries(time_difference vrt)
