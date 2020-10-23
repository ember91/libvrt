cmake_minimum_required(VERSION 3.0)

project(vrt VERSION 1.0 LANGUAGES C DESCRIPTION "Read/write library of packets following the VITA Radio Transport (VRT) standard, i.e. VITA-49.0.")

# Default to 'Release'
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif()

# Name target the same as project
set(TARGET_NAME ${PROJECT_NAME})

# Add options
option(CPPLINT          "Use google style guide linter" OFF)
option(CLANG_TIDY       "Enable clang-tidy"             OFF)
option(DOCUMENTATION    "Build doxygen documentation"   OFF)
option(IWYU             "Include what you use"          OFF)
option(TEST             "Compile test suite"            OFF)

# Check if options are set
if (${CLANG_TIDY})
    message(STATUS "Enabling clang-tidy for C")
    set(CMAKE_C_CLANG_TIDY "clang-tidy;-checks=*,-readability-magic-numbers,-cppcoreguidelines-avoid-magic-numbers,-hicpp-signed-bitwise")
endif()
if (${CPPCHECK})
    message(STATUS "Enabling Cppcheck for C")
    set(CMAKE_C_CPPCHECK "cppcheck;--std=c99")
endif()
if (${CPPLINT})
    message(STATUS "Enabling cpplint for C")
    set(CMAKE_C_CPPLINT "cpplint;--linelength=120;--filter=-legal/copyright,-build/include_subdir")
endif()
find_package(Doxygen)
if (DOCUMENTATION)
    if (DOXYGEN_FOUND)
        # Set input and output files
        set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile.in)
        set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

        # Request to configure the file
        configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
        message(STATUS "Doxygen build started")

        # Option ALL allows building documentation together with the application
        add_custom_target(doc_doxygen ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            VERBATIM)
    else()
        message(WARNING "Doxygen with dot component is required for documentation generation")
    endif()
endif()
if (${IWYU})
    message(STATUS "Enabling Include-what-you-use for C")
    set(CMAKE_C_INCLUDE_WHAT_YOU_USE "/usr/bin/iwyu;--transitive_includes_only")
endif()
if (${TEST})
    message(STATUS "Compiling test suite")
    add_subdirectory(test)
endif()

# Add source files
file(GLOB FILES_SRC src/*.c)
add_library(${TARGET_NAME} STATIC ${FILES_SRC})

# Set warning levels
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    if (CMAKE_C_COMPILER_ID MATCHES "Clang")
        target_compile_options(${TARGET_NAME} PRIVATE -Weverything -Wno-padded -Wno-newline-eof)
    elseif (CMAKE_C_COMPILER_ID MATCHES "GNU")
        target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -Wpedantic -Wshadow)
    elseif (CMAKE_C_COMPILER_ID MATCHES "Intel")
        target_compile_options(${TARGET_NAME} PRIVATE -w3) # /W5 on Windows
    elseif (CMAKE_C_COMPILER_ID MATCHES "MSVC")
        target_compile_options(${TARGET_NAME} PRIVATE /W4)
    endif()
endif()

# Add include directory
target_include_directories(${TARGET_NAME} PUBLIC include/)

# Set C standard
set_target_properties(${TARGET_NAME} PROPERTIES C_STANDARD 99)

# Install library
install(TARGETS ${PROJECT_NAME} DESTINATION lib)

# Install library headers
file(GLOB FILES_HEADER include/${PROJECT_NAME}/*.h)
install(FILES ${FILES_HEADER} DESTINATION include/${PROJECT_NAME})

# Copy documentation as well
if (UNIX)
    install(DIRECTORY doc/html DESTINATION share/doc/${PROJECT_NAME})
    install(FILES doc/index.html DESTINATION share/doc/${PROJECT_NAME})
endif()
