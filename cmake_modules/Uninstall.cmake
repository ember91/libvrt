# Uninstall all files listed in install_manifest.txt

# Lookup manifest
set(MANIFEST "${CMAKE_CURRENT_BINARY_DIR}/install_manifest.txt")
if(NOT EXISTS ${MANIFEST})
    message(FATAL_ERROR "Cannot find install manifest: '${MANIFEST}'")
endif()

# Remove all
file(STRINGS ${MANIFEST} files)
file(REMOVE ${files})
