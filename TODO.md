* Copy paste in example/CMakeLists.txt
* Doxygen/sphinx/breathe Documentation cleanup
* Add int packet_to_calendar_time(packet, sample_rate, cal_time), which parses packet header time to year, month, ... . It must be:
  * thread safe (not use gmtime)
  * Handle leap seconds for UTC and not for UTC.
  * Perhaps also with extra time zone information.
* CMake find clang-tidy, iwyu
* Test coverage

# Future
* Add single header library?
* Make version number tag on github
