# Lanturnal
Ian's diurnal chandelier with Arduino 

You need sunset and sunrise times for your location to drop in the code.  Use the Excel document and the output from the below website to input your sunset and sunrise in minutes after midnight.  Keep an eye out for daylights saving time.  http://aa.usno.navy.mil/data/docs/RS_OneYear.php

Hardware:
8 Channel DC 5V Relay Module,
Arduino UNO R3 Board Module,
Light Bulbs,
Light Sockets,
DS3231 Real Time Clock Module,
Wiring etc.

Libraries:
Time.h,
Wire.h,
DS3232RTC.h,
TimeAlarms.h,

Write up: https://medium.com/@Ian_Moritz/the-lanturnal-678f391636bc#.irljd0cma 

Credit: Dan Levine figured out how to set the day of the year. Lines 449 to 469.
