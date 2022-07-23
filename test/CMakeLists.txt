cmake_minimum_required(VERSION 3.0)

project(
  test
  LANGUAGES C CXX
  DESCRIPTION "Test suite for libvrt.")

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Name target
set(TARGET_NAME run_unit_tests)

# Check if options are set
if(${CLANG_TIDY})
  message(STATUS "Enabling clang-tidy for C++")
  set(CMAKE_CXX_CLANG_TIDY
      "clang-tidy;-checks=*,-readability-magic-numbers,-cppcoreguidelines-avoid-magic-numbers,-cert-err58-cpp,-google-readability-avoid-underscore-in-googletest-name,-fuchsia-statically-constructed-objects,-fuchsia-default-arguments-calls,-cppcoreguidelines-owning-memory,-hicpp-special-member-functions,-cppcoreguidelines-special-member-functions,-modernize-use-trailing-return-type,-cppcoreguidelines-non-private-member-variables-in-classes,-google-readability-function-size,-hicpp-function-size,-hicpp-signed-bitwise,-readability-function-size,-misc-non-private-member-variables-in-classes,-cppcoreguidelines-pro-type-reinterpret-cast,-cppcoreguidelines-pro-type-const-cast,-cppcoreguidelines-pro-bounds-constant-array-index,-cppcoreguidelines-pro-bounds-pointer-arithmetic"
  )
endif()
if(${CPPLINT})
  message(STATUS "Enabling cpplint for C++")
  set(CMAKE_CXX_CPPLINT
      "cpplint;--linelength=120;--filter=-legal/copyright,-build/include_subdir,-build/include_order,-whitespace/indent"
  )
endif()
if(${IWYU})
  message(STATUS "Enabling Include-what-you-use for C++")
  set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE "/usr/local/bin/include-what-you-use")
endif()

# Add test source files
file(GLOB SRC_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
add_executable(${TARGET_NAME} ${SRC_FILES} "${CMAKE_CURRENT_SOURCE_DIR}/../src/timestamp_to_calendar.c")

# Setup testing
enable_testing()
find_package(GTest REQUIRED) # Install with 'sudo apt install libgtest-dev'
target_include_directories(${TARGET_NAME} PUBLIC ${GTEST_INCLUDE_DIR})

# Set warning levels
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(
      ${TARGET_NAME}
      PRIVATE -Weverything
              -Wno-c++98-compat-pedantic
              -Wno-padded
              -Wno-newline-eof
              -Wno-global-constructors
              -Wno-weak-vtables
              -Wno-used-but-marked-unused)
  elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -Wpedantic
                                                  -Wshadow)
  elseif(CMAKE_CXX_COMPILER_ID MATCHES "Intel")
    target_compile_options(${TARGET_NAME} PRIVATE -w3) # /W5 on Windows
  elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(${TARGET_NAME} PRIVATE /W4)
  endif()
endif()

# Add include directory
target_include_directories(${TARGET_NAME}
                           PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../include/")

# Link executable
target_link_libraries(${TARGET_NAME} vrt ${GTEST_LIBRARIES} pthread)

# Add test
add_test(name ${TARGET_NAME} COMMAND ${TARGET_NAME})
