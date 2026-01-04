include("${CMAKE_CURRENT_LIST_DIR}/setup_target.cmake")

function(my_add_library target)
    add_library("${target}")
    setup_target("${target}" ${ARGN})
endfunction()
