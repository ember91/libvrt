find_program(IWYU_PROGRAM
             NAMES include-what-you-use
             PATHS /usr/bin
                   /usr/local/bin
                   ${HOME}/.local/bin
                   ${HOME}/Library/Python/2.7/bin)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(IWYU DEFAULT_MSG IWYU_PROGRAM)
