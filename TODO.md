# Version 2.0
* Remove vrt_time_calendar() in favour of vrt_time_fields()
* Rename VRT_ERR_INTEGER_SECONDS_MISMATCH to VRT_ERR_MISMATCH_INTEGER_SECONDS for consistency
* Remove vrt_time_difference() in favour of vrt_time_difference_fields()
* Fix consistency error. Sometimes it's integer_seconds_timestamp and sometimes integer_second_timestamp
* Make validate parameter in read/write an enum
