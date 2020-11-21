include(FindPkgConfig)
include(FindPackageHandleStandardArgs)

# Find include directory
find_path(Libvrt_INCLUDE_DIR NAMES vrt)

# Find library
find_library(Libvrt_LIBRARY NAMES vrt)

if(Libvrt_INCLUDE_DIR AND Libvrt_LIBRARY)
  set(Libvrt_FOUND yes)
else()
  set(Libvrt_FOUND no)
endif()
