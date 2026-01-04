project(test LANGUAGES C CXX DESCRIPTION "Test suite for libvrt.")

include("${CMAKE_SOURCE_DIR}/cmake_modules/my_add_executable.cmake")
include(GoogleTest)

# Name target
set(target run_unit_tests)

# Add test source files
my_add_executable("${target}")

# Setup testing
enable_testing()
find_package(GTest REQUIRED) # Install with 'sudo apt install libgtest-dev'

# Link executable
target_link_libraries("${target}" vrt ${GTEST_LIBRARIES} pthread)

# Add test
gtest_discover_tests("${target}")
