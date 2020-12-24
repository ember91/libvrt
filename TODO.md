* Add int packet_to_calendar_time(packet, sample_rate, cal_time), which parses packet header time to year, month, ... . It must be:
  * Thread safe (not use gmtime).
  * Handle leap seconds for UTC and not for GPS.
  * Perhaps also with extra time zone information.
* Add example for calendar_time

# Future
* Make version number tag on github
