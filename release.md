List of things to do for a new library release.

* Full recompile with:
  * GCC
  * clang
  * ICC
  * MSVC
* In both Debug and Release
* Compile tests
* Compile examples
* Warnings:
  * gcc -Wall -Wextra ...
  * clang -Weverything ...
* Run all tests: `./test/run_unit_tests`
* Generate new documentation
* Inspect new documentation
* Static analysis:
  * clang-tidy
  * cpplint
  * include-what-you-use
* Code coverage:
  * gcov?
* Update README
* Ensure new files are included in git commit
* Update github version tag
