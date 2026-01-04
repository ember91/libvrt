function(setup_target target)
    cmake_parse_arguments(PARSE_ARGV 1 arg "NO_GLOB" "" "")

    if(NOT arg_NO_GLOB)
        file(GLOB files_src CONFIGURE_DEPENDS "src/*.c" "src/*.cpp")
        target_sources("${target}" PRIVATE ${files_src})
    endif()

    if(${GCOV})
        if(CMAKE_BUILD_TYPE STREQUAL "Debug")
            if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
                target_compile_options("${target}" PUBLIC -fprofile-arcs -ftest-coverage)
                target_link_libraries("${target}" INTERFACE gcov)
            else()
                message(WARNING "Can only generate code coverage symbols with GCC")
            endif()
        else()
            message(WARNING "Can only generate code coverage symbols when Debug is enabled")
        endif()
    endif()

    if(CMAKE_C_COMPILER_ID MATCHES "^Clang|GNU$")
        target_compile_options("${target}" PRIVATE -Wall -Wextra -Wpedantic -Wshadow)
    elseif(CMAKE_C_COMPILER_ID STREQUAL "Intel")
        target_compile_options("${target}" PRIVATE -w3) # /W5 on Windows
    elseif(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
        target_compile_options("${target}" PRIVATE /W4)
    endif()

    set_target_properties("${target}" PROPERTIES C_STANDARD 99 CXX_STANDARD 17)

    target_include_directories("${target}" PUBLIC include PRIVATE src)

    install(TARGETS "${target}" DESTINATION lib)
    file(GLOB FILES_HEADER CONFIGURE_DEPENDS "include/${target}/*.h")
    install(FILES ${FILES_HEADER} DESTINATION "include/${target}")
endfunction()
