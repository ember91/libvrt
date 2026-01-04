cmake_minimum_required(VERSION 3.15)

project(
    vrt
    LANGUAGES C
    DESCRIPTION "Read/write library of packets following the VITA Radio Transport (VRT) standard, i.e. VITA-49.0."
)

include(cmake_modules/my_add_library.cmake)

# Search for modules in local module directory as well
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules")

# Default to 'Release'
if(NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE
        Release
        CACHE STRING
        "Choose the type of build. Options are: Debug, Release, RelWithDebInfo, MinSizeRel."
        FORCE
    )
endif()

# Add options
option(CPPLINT "Use google style guide linter" OFF)
option(CLANG_TIDY "Enable clang-tidy" OFF)
option(DOCUMENTATION "Build doxygen documentation" OFF)
option(IWYU "Include what you use" OFF)
option(TEST "Compile test suite" OFF)
option(EXAMPLE "Compile example suite" OFF)
option(GCOV "Generate code coverage report" OFF)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

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

    message(STATUS "Enabling clang-tidy")
    # Note that CMAKE_CXX_CLANG_TIDY is set in CMakeLists.txt in 'test' directory
    set(CMAKE_C_CLANG_TIDY
        "${ClangTidy_EXE};-checks=*,-readability-magic-numbers,-cppcoreguidelines-avoid-magic-numbers,-hicpp-signed-bitwise,-android-cloexec-fopen"
    )
endif()
if(${CPPLINT})
    find_package(Cpplint REQUIRED)

    # Note that CMAKE_CXX_CPPLINT is set in CMakeLists.txt in 'test' directory
    message(STATUS "Enabling cpplint")
    set(CMAKE_C_CPPLINT
        "${CPPLINT_PROGRAM};--linelength=120;--filter=-legal/copyright,-build/include_subdir,-readability/casting,-build/include_order"
    )
endif()
if(DOCUMENTATION)
    if(DOXYGEN_FOUND AND BREATHE_FOUND AND SPHINX_FOUND AND EXHALE_FOUND)
        # Copy all configuration files with file extension ".in"
        file(GLOB TEMPLATE_FILES CONFIGURE_DEPENDS "doc/*.in")
        foreach(file IN LISTS TEMPLATE_FILES)
            cmake_path(GET file STEM file_no_ext)
            configure_file("${file}" "${CMAKE_CURRENT_BINARY_DIR}/${file_no_ext}")
        endforeach()

        # Option ALL allows building documentation together with the application
        add_custom_target(
            doxygen
            ALL
            COMMAND "${DOXYGEN_EXECUTABLE}" "${DOXYGEN_OUT}"
            WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
            VERBATIM
        )

        if(NOT DEFINED SPHINX_THEME)
            set(SPHINX_THEME default)
        endif()

        if(NOT DEFINED SPHINX_THEME_DIR)
            set(SPHINX_THEME_DIR)
        endif()

        add_custom_target(
            sphinx
            ALL
            "${SPHINX_EXECUTABLE}" -q -b html -c "${CMAKE_CURRENT_BINARY_DIR}/doc" -d
            "${CMAKE_CURRENT_BINARY_DIR}/doc/.doctrees" "${CMAKE_CURRENT_BINARY_DIR}/doc"
            "${CMAKE_CURRENT_SOURCE_DIR}/doc/html"
            COMMENT "Building HTML documentation with Sphinx"
            VERBATIM
        )
    endif()
endif()
if(${IWYU})
    find_package(IWYU REQUIRED)

    message(STATUS "Enabling Include-what-you-use")
    set(CMAKE_C_INCLUDE_WHAT_YOU_USE "${IWYU_PROGRAM}")
endif()
if(${TEST})
    message(STATUS "Building test suite")
    add_subdirectory(test)
endif()
if(${EXAMPLE})
    message(STATUS "Building example suite")
    add_subdirectory(example)
endif()

my_add_library(vrt STATIC)

# Copy documentation as well
if(UNIX)
    install(DIRECTORY "doc/html" DESTINATION "share/doc/${PROJECT_NAME}")
    install(FILES "doc/index.html" DESTINATION "share/doc/${PROJECT_NAME}")
endif()

# Add uninstall target
add_custom_target(uninstall "${CMAKE_COMMAND}" -P "${CMAKE_MODULE_PATH}/Uninstall.cmake")
