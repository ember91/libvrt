#
# - Try to find cpplint
#
# Once done this will define
#
#   CPPLINT_FOUND - system has cpplint
#   CPPLINT_PROGRAM, the cpplint executable.
#
find_program(CPPLINT_PROGRAM
             NAMES cpplint
             PATHS /usr/bin
                   /usr/local/bin
                   ${HOME}/.local/bin
                   ${HOME}/Library/Python/2.7/bin)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Cpplint DEFAULT_MSG CPPLINT_PROGRAM)
