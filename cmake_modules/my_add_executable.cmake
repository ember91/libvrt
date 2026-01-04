include("${CMAKE_CURRENT_LIST_DIR}/setup_target.cmake")

function(my_add_executable target)
    add_executable("${target}")
    setup_target("${target}" ${ARGN})
endfunction()
