* Copy paste in example/CMakeLists.txt
* Doxygen/sphinx/breathe Documentation cleanup
* Add int packet_to_calendar_time(packet, sample_rate, cal_time), which parses packet header time to year, month, ... . It must be:
  * thread safe (not use gmtime)
  * Handle leap seconds for UTC and not for UTC.
  * Perhaps also with extra time zone information.
* Add uninstall option to CMake
* Add missing vrt_string error codes
* Change name of p1 and p2 in vrt_time documentation
* CMake find clang-tidy, iwyu

# Future
* Add single header library?
* Make version number tag on github
