cmake_minimum_required(VERSION 3.0)

project(
  vrt
  LANGUAGES C
  DESCRIPTION
    "Read/write library of packets following the VITA Radio Transport (VRT) standard, i.e. VITA-49.0."
)

# Search for modules in local module directory as well
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
                      "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules")

# Default to 'Release'
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE
      "Release"
      CACHE
        STRING
        "Choose the type of build. Options are: Debug, Release, RelWithDebInfo, MinSizeRel."
        FORCE)
endif()

# Name target the same as project
set(TARGET_NAME "${PROJECT_NAME}")

# Add options
option(CPPLINT "Use google style guide linter" OFF)
option(CLANG_TIDY "Enable clang-tidy" OFF)
option(DOCUMENTATION "Build doxygen documentation" OFF)
option(IWYU "Include what you use" OFF)
option(TEST "Compile test suite" OFF)
option(EXAMPLE "Compile example suite" OFF)
option(GCOV "Generate code coverage report" OFF)

if(DOCUMENTATION)
  # Install with 'sudo apt install doxygen'.
  find_package(Doxygen)
  # Note that Breathe may located with find_package but may not work with sudo
  # make install. Install with 'sudo pip3 install breathe'.
  find_package(Breathe)
  # Note that Sphinx may located with find_package but may not work with sudo make
  # install. Install with 'sudo pip3 install sphinx_rtd_theme'.
  find_package(Sphinx)
  # Install with 'sudo pip3 install exhale'.
  find_package(Exhale)

  if(NOT DOXYGEN_FOUND)
    message(WARNING "Doxygen is required for documentation generation")
  endif()
  if(NOT BREATHE_FOUND)
    message(WARNING "Breathe is required for documentation generation")
  endif()
  if(NOT SPHINX_FOUND)
    message(WARNING "Sphinx is required for documentation generation")
  endif()
  if(NOT EXHALE_FOUND)
    message(WARNING "Exhale is required for documentation generation")
  endif()
endif()

# Check if options are set
if(${CLANG_TIDY})
  find_package(ClangTidy REQUIRED)

  message(STATUS "Enabling clang-tidy for C")
  # Note that CMAKE_CXX_CLANG_TIDY is set in CMakeLists.txt in 'test' directory
  set(CMAKE_C_CLANG_TIDY
      "${ClangTidy_EXE};-checks=*,-readability-magic-numbers,-cppcoreguidelines-avoid-magic-numbers,-hicpp-signed-bitwise,-android-cloexec-fopen"
  )
endif()
if(${CPPLINT})
  find_package(Cpplint REQUIRED)

  # Note that CMAKE_CXX_CPPLINT is set in CMakeLists.txt in 'test' directory
  message(STATUS "Enabling cpplint for C")
  set(CMAKE_C_CPPLINT
      "${CPPLINT_PROGRAM};--linelength=120;--filter=-legal/copyright,-build/include_subdir,-readability/casting,-build/include_order"
  )
endif()
if(DOCUMENTATION)
  if(DOXYGEN_FOUND
     AND BREATHE_FOUND
     AND SPHINX_FOUND
     AND EXHALE_FOUND)
    # Set input and output files
    set(DOXYGEN_IN "${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile.in")
    set(DOXYGEN_OUT "${CMAKE_CURRENT_BINARY_DIR}/doc/Doxyfile")

    # Copy all configuration files with file extension ".in"
    file(
      GLOB TEMPLATE_FILES CONFIGURE_DEPENDS
      RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/"
      "${CMAKE_CURRENT_SOURCE_DIR}/doc/*.in")
    foreach(FILE ${TEMPLATE_FILES})
      string(LENGTH ${FILE} LEN)
      math(EXPR LEN "${LEN} - 3")
      string(SUBSTRING ${FILE} 0 ${LEN} FILE_NO_EXT)
      configure_file(${CMAKE_CURRENT_SOURCE_DIR}/${FILE}
                     ${CMAKE_CURRENT_BINARY_DIR}/${FILE_NO_EXT})
    endforeach()

    # Option ALL allows building documentation together with the application
    add_custom_target(
      doxygen ALL
      COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      VERBATIM)

    if(NOT DEFINED SPHINX_THEME)
      set(SPHINX_THEME default)
    endif()

    if(NOT DEFINED SPHINX_THEME_DIR)
      set(SPHINX_THEME_DIR)
    endif()

    add_custom_target(
      sphinx ALL
      ${SPHINX_EXECUTABLE}
      -q
      -b
      html
      -c
      "${CMAKE_CURRENT_BINARY_DIR}/doc"
      -d
      "${CMAKE_CURRENT_BINARY_DIR}/doc/.doctrees"
      "${CMAKE_CURRENT_BINARY_DIR}/doc"
      "${CMAKE_CURRENT_SOURCE_DIR}/doc/html"
      COMMENT "Building HTML documentation with Sphinx"
      VERBATIM)
  endif()
endif()
if(${IWYU})
  find_package(IWYU REQUIRED)

  message(STATUS "Enabling Include-what-you-use for C")
  set(CMAKE_C_INCLUDE_WHAT_YOU_USE "${IWYU_PROGRAM}")
endif()
if(${TEST})
  message(STATUS "Compiling test suite")
  add_subdirectory(test)
endif()
if(${EXAMPLE})
  message(STATUS "Compiling example suite")
  add_subdirectory(example)
endif()

# Add source files
file(GLOB FILES_SRC CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
add_library(${TARGET_NAME} STATIC ${FILES_SRC})

# Code coverage
if(${GCOV})
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(CMAKE_C_COMPILER_ID MATCHES "GNU")
      target_compile_options(${TARGET_NAME} PUBLIC -fprofile-arcs -ftest-coverage)
      target_link_libraries(${TARGET_NAME} INTERFACE gcov)
    else()
      message(WARNING "Can only generate code coverage symbols with GCC")
    endif()
  else()
    message(WARNING "Can only generate code coverage symbols when Debug is enabled")
  endif()
endif()

# Set warning levels
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  if(CMAKE_C_COMPILER_ID MATCHES "Clang")
    target_compile_options(${TARGET_NAME} PRIVATE -Weverything -Wno-padded
                                                  -Wno-newline-eof)
  elseif(CMAKE_C_COMPILER_ID MATCHES "GNU")
    target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -Wpedantic
                                                  -Wshadow)
  elseif(CMAKE_C_COMPILER_ID MATCHES "Intel")
    target_compile_options(${TARGET_NAME} PRIVATE -w3) # /W5 on Windows
  elseif(CMAKE_C_COMPILER_ID MATCHES "MSVC")
    target_compile_options(${TARGET_NAME} PRIVATE /W4)
  endif()
endif()

# Add include directory
target_include_directories(${TARGET_NAME} PUBLIC include/vrt/)

# Set C standard
set_target_properties(${TARGET_NAME} PROPERTIES C_STANDARD 99)

# Install library
install(TARGETS ${PROJECT_NAME} DESTINATION lib)

# Install library headers
file(GLOB FILES_HEADER CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}/*.h")
install(FILES ${FILES_HEADER} DESTINATION include/${PROJECT_NAME})

# Copy documentation as well
if(UNIX)
  install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/doc/html"
          DESTINATION "share/doc/${PROJECT_NAME}")
  install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/doc/index.html"
          DESTINATION "share/doc/${PROJECT_NAME}")
endif()

# Add uninstall target
add_custom_target(uninstall
    "${CMAKE_COMMAND}" -P "${CMAKE_MODULE_PATH}/Uninstall.cmake"
)
