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
if(${CPPLINT})
  message(STATUS "Enabling cpplint for C")
  set(CMAKE_C_CPPLINT
      "cpplint;--linelength=120;--filter=-legal/copyright,-build/include_subdir,-build/include_order,-readability/casting"
  )
endif()
if(${IWYU})
  message(STATUS "Enabling Include-what-you-use for C")
  set(CMAKE_C_INCLUDE_WHAT_YOU_USE "/usr/local/bin/include-what-you-use")
endif()

function(add_example source_file)
  string(REGEX REPLACE "\\.[^.]*$" "" target_name ${source_file})

  # Add executable
  add_executable(${target_name}
                ${CMAKE_CURRENT_SOURCE_DIR}/src/${source_file})

  # Set C standard
  set_target_properties(${target_name} PROPERTIES C_STANDARD 99)

  # Set warning levels
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(CMAKE_C_COMPILER_ID MATCHES "Clang")
      target_compile_options(${target_name} PRIVATE -Weverything -Wno-padded
                                                -Wno-newline-eof)
    elseif(CMAKE_C_COMPILER_ID MATCHES "GNU")
      target_compile_options(${target_name} PRIVATE -Wall -Wextra -Wpedantic
                                                -Wshadow)
    elseif(CMAKE_C_COMPILER_ID MATCHES "Intel")
      target_compile_options(${target_name} PRIVATE -w3) # /W5 on Windows
    elseif(CMAKE_C_COMPILER_ID MATCHES "MSVC")
      target_compile_options(${target_name} PRIVATE /W4)
    endif()
  endif()
endfunction()

add_example(read_if_packet_advanced)
add_example(read_if_packet_simple)
add_example(time_calendar)
add_example(time_difference)
add_example(write_if_context_advanced)
add_example(write_if_context_simple)
add_example(write_if_data_advanced)
add_example(write_if_data_simple)

# Add libraries to link to
target_link_libraries(read_if_packet_advanced vrt)
target_link_libraries(read_if_packet_simple vrt)
target_link_libraries(time_difference vrt)
target_link_libraries(time_calendar vrt)
target_link_libraries(write_if_context_advanced vrt)
target_link_libraries(write_if_context_simple vrt)
target_link_libraries(write_if_data_advanced vrt ${STANDARD_MATH_LIBRARY})
target_link_libraries(write_if_data_simple vrt ${STANDARD_MATH_LIBRARY})
