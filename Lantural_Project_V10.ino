// Daniel M. Levine and Ian T. Moritz
// 1 April 2016

//TO-DO
//end of cycle alert

#define Bulb_ON 0
#define Bulb_OFF 1

#include <Time.h>
#include <Wire.h>
#include <DS3232RTC.h>
#include <TimeAlarms.h>

int bulbs[] = {10, 9, 8, 7, 6, 5, 4}; //Bulb related to pin 10 is most Eastern bulb and Bulb related to pin 4 is most Western bulb
int bulbCount = 7;

int i = 0; // A counter
int j = 0; //A variable used for hourly alarms to account for military time
int delay0 = 1100;  //For hourly alarms, pause after all lights go off and count starts
int delay1 = 800;  //For hourly alarms, pauses during counting sequence
int delay2 = 2200; //For hourly alarms, how long 'hour' stays lit #itslit
int scene_number = 0; //A variable used for (Switch)
int map_number = 0;
int the_month = 0; // The month from 1 to 12
int the_day = 0; // The day from 1 to 31
int the_hour = 0; // The hour from 0 to 23
int the_minute = 0; // The minute from 0 to 59
int the_second = 0; // The second from 0 to 59
int days_in_each_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
int doy = 0; // The day of the year from 1 to 365
int tod = 0; // The time of the day from 0 to 1440
int todays_sunrise = 0; // The time of the day of today's sunrise in minutes
int todays_sunset = 0; // The time of the day of today's sunset in minutes

int sunrise_times[] = {
  447,  // 1 January 2016 | 0
  447,  // 2 January 2016 | 1
  447,  // 3 January 2016 | 2
  447,  // 4 January 2016 | 3
  447,  // 5 January 2016 | 4
  447,  // 6 January 2016 | 5
  447,  // 7 January 2016 | 6
  447,  // 8 January 2016 | 7
  447,  // 9 January 2016 | 8
  447,  // 10 January 2016 | 9
  446,  // 11 January 2016 | 10
  446,  // 12 January 2016 | 11
  446,  // 13 January 2016 | 12
  446,  // 14 January 2016 | 13
  445,  // 15 January 2016 | 14
  445,  // 16 January 2016 | 15
  445,  // 17 January 2016 | 16
  444,  // 18 January 2016 | 17
  444,  // 19 January 2016 | 18
  443,  // 20 January 2016 | 19
  443,  // 21 January 2016 | 20
  442,  // 22 January 2016 | 21
  441,  // 23 January 2016 | 22
  441,  // 24 January 2016 | 23
  440,  // 25 January 2016 | 24
  439,  // 26 January 2016 | 25
  439,  // 27 January 2016 | 26
  438,  // 28 January 2016 | 27
  437,  // 29 January 2016 | 28
  436,  // 30 January 2016 | 29
  436,  // 31 January 2016 | 30
  435,  // 1 Febraury 2016 | 31
  434,  // 2 Febraury 2016 | 32
  433,  // 3 Febraury 2016 | 33
  432,  // 4 Febraury 2016 | 34
  431,  // 5 Febraury 2016 | 35
  430,  // 6 Febraury 2016 | 36
  429,  // 7 Febraury 2016 | 37
  428,  // 8 Febraury 2016 | 38
  427,  // 9 Febraury 2016 | 39
  426,  // 10 Febraury 2016 | 40
  425,  // 11 Febraury 2016 | 41
  423,  // 12 Febraury 2016 | 42
  422,  // 13 Febraury 2016 | 43
  421,  // 14 Febraury 2016 | 44
  420,  // 15 Febraury 2016 | 45
  419,  // 16 Febraury 2016 | 46
  417,  // 17 Febraury 2016 | 47
  416,  // 18 Febraury 2016 | 48
  415,  // 19 Febraury 2016 | 49
  414,  // 20 Febraury 2016 | 50
  412,  // 21 Febraury 2016 | 51
  411,  // 22 Febraury 2016 | 52
  410,  // 23 Febraury 2016 | 53
  408,  // 24 Febraury 2016 | 54
  407,  // 25 Febraury 2016 | 55
  406,  // 26 Febraury 2016 | 56
  404,  // 27 Febraury 2016 | 57
  403,  // 28 Febraury 2016 | 58
  401,  // 29 Febraury 2016 | 59
  400,  // 1 March 2016 | 60
  398,  // 2 March 2016 | 61
  397,  // 3 March 2016 | 62
  395,  // 4 March 2016 | 63
  394,  // 5 March 2016 | 64
  392,  // 6 March 2016 | 65
  391,  // 7 March 2016 | 66
  389,  // 8 March 2016 | 67
  388,  // 9 March 2016 | 68
  386,  // 10 March 2016 | 69
  385,  // 11 March 2016 | 70
  383,  // 12 March 2016 | 71
  442,  // 13 March 2016 | 72
  440,  // 14 March 2016 | 73
  439,  // 15 March 2016 | 74
  437,  // 16 March 2016 | 75
  436,  // 17 March 2016 | 76
  434,  // 18 March 2016 | 77
  432,  // 19 March 2016 | 78
  431,  // 20 March 2016 | 79
  429,  // 21 March 2016 | 80
  428,  // 22 March 2016 | 81
  426,  // 23 March 2016 | 82
  425,  // 24 March 2016 | 83
  423,  // 25 March 2016 | 84
  421,  // 26 March 2016 | 85
  420,  // 27 March 2016 | 86
  418,  // 28 March 2016 | 87
  417,  // 29 March 2016 | 88
  415,  // 30 March 2016 | 89
  414,  // 31 March 2016 | 90
  412,  // 1 April 2016 | 91
  411,  // 2 April 2016 | 92
  409,  // 3 April 2016 | 93
  407,  // 4 April 2016 | 94
  406,  // 5 April 2016 | 95
  404,  // 6 April 2016 | 96
  403,  // 7 April 2016 | 97
  401,  // 8 April 2016 | 98
  400,  // 9 April 2016 | 99
  398,  // 10 April 2016 | 100
  397,  // 11 April 2016 | 101
  395,  // 12 April 2016 | 102
  394,  // 13 April 2016 | 103
  392,  // 14 April 2016 | 104
  391,  // 15 April 2016 | 105
  390,  // 16 April 2016 | 106
  388,  // 17 April 2016 | 107
  387,  // 18 April 2016 | 108
  385,  // 19 April 2016 | 109
  384,  // 20 April 2016 | 110
  383,  // 21 April 2016 | 111
  381,  // 22 April 2016 | 112
  380,  // 23 April 2016 | 113
  379,  // 24 April 2016 | 114
  377,  // 25 April 2016 | 115
  376,  // 26 April 2016 | 116
  375,  // 27 April 2016 | 117
  373,  // 28 April 2016 | 118
  372,  // 29 April 2016 | 119
  371,  // 30 April 2016 | 120
  370,  // 1 May 2016 | 121
  369,  // 2 May 2016 | 122
  367,  // 3 May 2016 | 123
  366,  // 4 May 2016 | 124
  365,  // 5 May 2016 | 125
  364,  // 6 May 2016 | 126
  363,  // 7 May 2016 | 127
  362,  // 8 May 2016 | 128
  361,  // 9 May 2016 | 129
  360,  // 10 May 2016 | 130
  359,  // 11 May 2016 | 131
  358,  // 12 May 2016 | 132
  357,  // 13 May 2016 | 133
  356,  // 14 May 2016 | 134
  355,  // 15 May 2016 | 135
  354,  // 16 May 2016 | 136
  354,  // 17 May 2016 | 137
  353,  // 18 May 2016 | 138
  352,  // 19 May 2016 | 139
  351,  // 20 May 2016 | 140
  351,  // 21 May 2016 | 141
  350,  // 22 May 2016 | 142
  349,  // 23 May 2016 | 143
  349,  // 24 May 2016 | 144
  348,  // 25 May 2016 | 145
  347,  // 26 May 2016 | 146
  347,  // 27 May 2016 | 147
  346,  // 28 May 2016 | 148
  346,  // 29 May 2016 | 149
  345,  // 30 May 2016 | 150
  345,  // 31 May 2016 | 151
  345,  // 1 June 2016 | 152
  344,  // 2 June 2016 | 153
  344,  // 3 June 2016 | 154
  344,  // 4 June 2016 | 155
  343,  // 5 June 2016 | 156
  343,  // 6 June 2016 | 157
  343,  // 7 June 2016 | 158
  343,  // 8 June 2016 | 159
  343,  // 9 June 2016 | 160
  342,  // 10 June 2016 | 161
  342,  // 11 June 2016 | 162
  342,  // 12 June 2016 | 163
  342,  // 13 June 2016 | 164
  342,  // 14 June 2016 | 165
  342,  // 15 June 2016 | 166
  342,  // 16 June 2016 | 167
  342,  // 17 June 2016 | 168
  343,  // 18 June 2016 | 169
  343,  // 19 June 2016 | 170
  343,  // 20 June 2016 | 171
  343,  // 21 June 2016 | 172
  343,  // 22 June 2016 | 173
  344,  // 23 June 2016 | 174
  344,  // 24 June 2016 | 175
  344,  // 25 June 2016 | 176
  345,  // 26 June 2016 | 177
  345,  // 27 June 2016 | 178
  345,  // 28 June 2016 | 179
  346,  // 29 June 2016 | 180
  346,  // 30 June 2016 | 181
};
int sunset_times[] = {
  1017, // 1 January 2016 | 0
  1017, // 2 January 2016 | 1
  1018, // 3 January 2016 | 2
  1019, // 4 January 2016 | 3
  1020, // 5 January 2016 | 4
  1021, // 6 January 2016 | 5
  1022, // 7 January 2016 | 6
  1023, // 8 January 2016 | 7
  1024, // 9 January 2016 | 8
  1025, // 10 January 2016 | 9
  1026, // 11 January 2016 | 10
  1027, // 12 January 2016 | 11
  1028, // 13 January 2016 | 12
  1029, // 14 January 2016 | 13
  1030, // 15 January 2016 | 14
  1031, // 16 January 2016 | 15
  1032, // 17 January 2016 | 16
  1033, // 18 January 2016 | 17
  1034, // 19 January 2016 | 18
  1035, // 20 January 2016 | 19
  1036, // 21 January 2016 | 20
  1037, // 22 January 2016 | 21
  1039, // 23 January 2016 | 22
  1040, // 24 January 2016 | 23
  1041, // 25 January 2016 | 24
  1042, // 26 January 2016 | 25
  1043, // 27 January 2016 | 26
  1044, // 28 January 2016 | 27
  1046, // 29 January 2016 | 28
  1047, // 30 January 2016 | 29
  1048, // 31 January 2016 | 30
  1049, // 1 Febraury 2016 | 31
  1050, // 2 Febraury 2016 | 32
  1051, // 3 Febraury 2016 | 33
  1052, // 4 Febraury 2016 | 34
  1054, // 5 Febraury 2016 | 35
  1055, // 6 Febraury 2016 | 36
  1056, // 7 Febraury 2016 | 37
  1057, // 8 Febraury 2016 | 38
  1058, // 9 Febraury 2016 | 39
  1059, // 10 Febraury 2016 | 40
  1061, // 11 Febraury 2016 | 41
  1062, // 12 Febraury 2016 | 42
  1063, // 13 Febraury 2016 | 43
  1064, // 14 Febraury 2016 | 44
  1065, // 15 Febraury 2016 | 45
  1066, // 16 Febraury 2016 | 46
  1067, // 17 Febraury 2016 | 47
  1068, // 18 Febraury 2016 | 48
  1070, // 19 Febraury 2016 | 49
  1071, // 20 Febraury 2016 | 50
  1072, // 21 Febraury 2016 | 51
  1073, // 22 Febraury 2016 | 52
  1074, // 23 Febraury 2016 | 53
  1075, // 24 Febraury 2016 | 54
  1076, // 25 Febraury 2016 | 55
  1077, // 26 Febraury 2016 | 56
  1078, // 27 Febraury 2016 | 57
  1079, // 28 Febraury 2016 | 58
  1080, // 29 Febraury 2016 | 59
  1081, // 1 March 2016 | 60
  1082, // 2 March 2016 | 61
  1083, // 3 March 2016 | 62
  1085, // 4 March 2016 | 63
  1086, // 5 March 2016 | 64
  1087, // 6 March 2016 | 65
  1088, // 7 March 2016 | 66
  1089, // 8 March 2016 | 67
  1090, // 9 March 2016 | 68
  1091, // 10 March 2016 | 69
  1092, // 11 March 2016 | 70
  1093, // 12 March 2016 | 71
  1154, // 13 March 2016 | 72
  1155, // 14 March 2016 | 73
  1156, // 15 March 2016 | 74
  1157, // 16 March 2016 | 75
  1158, // 17 March 2016 | 76
  1159, // 18 March 2016 | 77
  1160, // 19 March 2016 | 78
  1161, // 20 March 2016 | 79
  1162, // 21 March 2016 | 80
  1163, // 22 March 2016 | 81
  1164, // 23 March 2016 | 82
  1164, // 24 March 2016 | 83
  1165, // 25 March 2016 | 84
  1166, // 26 March 2016 | 85
  1167, // 27 March 2016 | 86
  1168, // 28 March 2016 | 87
  1169, // 29 March 2016 | 88
  1170, // 30 March 2016 | 89
  1171, // 31 March 2016 | 90
  1172, // 1 April 2016 | 91
  1173, // 2 April 2016 | 92
  1174, // 3 April 2016 | 93
  1175, // 4 April 2016 | 94
  1176, // 5 April 2016 | 95
  1177, // 6 April 2016 | 96
  1178, // 7 April 2016 | 97
  1179, // 8 April 2016 | 98
  1180, // 9 April 2016 | 99
  1181, // 10 April 2016 | 100
  1182, // 11 April 2016 | 101
  1183, // 12 April 2016 | 102
  1184, // 13 April 2016 | 103
  1185, // 14 April 2016 | 104
  1186, // 15 April 2016 | 105
  1187, // 16 April 2016 | 106
  1188, // 17 April 2016 | 107
  1189, // 18 April 2016 | 108
  1190, // 19 April 2016 | 109
  1191, // 20 April 2016 | 110
  1192, // 21 April 2016 | 111
  1192, // 22 April 2016 | 112
  1193, // 23 April 2016 | 113
  1194, // 24 April 2016 | 114
  1195, // 25 April 2016 | 115
  1196, // 26 April 2016 | 116
  1197, // 27 April 2016 | 117
  1198, // 28 April 2016 | 118
  1199, // 29 April 2016 | 119
  1200, // 30 April 2016 | 120
  1201, // 1 May 2016 | 121
  1202, // 2 May 2016 | 122
  1203, // 3 May 2016 | 123
  1204, // 4 May 2016 | 124
  1205, // 5 May 2016 | 125
  1206, // 6 May 2016 | 126
  1207, // 7 May 2016 | 127
  1208, // 8 May 2016 | 128
  1209, // 9 May 2016 | 129
  1210, // 10 May 2016 | 130
  1211, // 11 May 2016 | 131
  1212, // 12 May 2016 | 132
  1212, // 13 May 2016 | 133
  1213, // 14 May 2016 | 134
  1214, // 15 May 2016 | 135
  1215, // 16 May 2016 | 136
  1216, // 17 May 2016 | 137
  1217, // 18 May 2016 | 138
  1218, // 19 May 2016 | 139
  1219, // 20 May 2016 | 140
  1219, // 21 May 2016 | 141
  1220, // 22 May 2016 | 142
  1221, // 23 May 2016 | 143
  1222, // 24 May 2016 | 144
  1223, // 25 May 2016 | 145
  1224, // 26 May 2016 | 146
  1224, // 27 May 2016 | 147
  1225, // 28 May 2016 | 148
  1226, // 29 May 2016 | 149
  1227, // 30 May 2016 | 150
  1227, // 31 May 2016 | 151
  1228, // 1 June 2016 | 152
  1229, // 2 June 2016 | 153
  1229, // 3 June 2016 | 154
  1230, // 4 June 2016 | 155
  1230, // 5 June 2016 | 156
  1231, // 6 June 2016 | 157
  1232, // 7 June 2016 | 158
  1232, // 8 June 2016 | 159
  1233, // 9 June 2016 | 160
  1233, // 10 June 2016 | 161
  1234, // 11 June 2016 | 162
  1234, // 12 June 2016 | 163
  1235, // 13 June 2016 | 164
  1235, // 14 June 2016 | 165
  1235, // 15 June 2016 | 166
  1236, // 16 June 2016 | 167
  1236, // 17 June 2016 | 168
  1236, // 18 June 2016 | 169
  1236, // 19 June 2016 | 170
  1237, // 20 June 2016 | 171
  1237, // 21 June 2016 | 172
  1237, // 22 June 2016 | 173
  1237, // 23 June 2016 | 174
  1237, // 24 June 2016 | 175
  1237, // 25 June 2016 | 176
  1237, // 26 June 2016 | 177
  1237, // 27 June 2016 | 178
  1237, // 28 June 2016 | 179
  1237, // 29 June 2016 | 180
  1237, // 30 June 2016 | 181
};

void setup() {
  Serial.begin(9600);
  setSyncProvider(RTC.get); // Initiates communication with the clock

  for (int thisBulb = 0; thisBulb < bulbCount; thisBulb++) {
    digitalWrite(bulbs[thisBulb], Bulb_OFF);  //Initialize pins so relays are inactive at reset
  }
  for (int thisBulb = 0; thisBulb < bulbCount; thisBulb++) {
    pinMode(bulbs[thisBulb], OUTPUT);  //Sets pins as outputs
  }

}
void loop() {
  digitalClockDisplay();
  Alarm.delay(1000); // wait one second between clock display

  time_t t = now();
  the_month = month(t); // Reads the current month
  the_day = day(t); // Reads the current day of the month
  the_hour = hour(t); // Reads the current hour of the day
  the_minute = minute(t); // Reads the current minute of the hour
  the_second = second(t); // Reads the current second of the minute

  //Triggers hourly alarms

  //if ((the_hour >= 1 && the_hour <= 7) && the_minute == 59 && (the_second == 0 || the_second == 1 || the_second == 2)) {
  //  Serial.println("Hourly alarm triggered");
  //  RepeatsV2();
  //}

  if ((the_hour >= 8 && the_hour <= 12) && the_minute == 59 && (the_second == 0 || the_second == 1 || the_second == 2)) {
    Serial.println("Hourly alarm triggered");
    RepeatsV1();
  }

  if ((the_hour >= 13 && the_hour <= 19) && the_minute == 59 && (the_second == 0 || the_second == 1 || the_second == 2)) {
    Serial.println("Hourly alarm triggered");
    RepeatsV2();
  }

  if ((the_hour >= 20 && the_hour <= 24) && the_minute == 59 && (the_second == 0 || the_second == 1 || the_second == 2)) {
    Serial.println("Hourly alarm triggered");
    RepeatsV1();
  }

  //Sets the day of the year
{ if (the_hour == 0 && the_minute == 0 && the_second == 0) {
      i = 0;
      doy = 0;
    }
    if (the_hour == 0 && the_minute == 0 && i <= 11) { // Recalculates doy at midnight each day, requires no more than 12 loops
      if (the_month - 1 > i) { // Adds the number of days in each preceding month to doy
        doy = doy + days_in_each_month[i];
        i = i + 1;
      }
      if (the_month - 1 == i) { // Runs once the days of all preceding months have been added to doy
        doy = doy + the_day; // Add the current day of the month to doy
        i = 999; // Ensures the doy calculation will not begin again until midnight the following morning
      }
    }

    tod = the_hour * 60 + the_minute; // Calculates the time of the day in minutes each loop
    todays_sunrise = sunrise_times[doy - 1];
    todays_sunset = sunset_times[doy - 1];
    delay(1000); // This ensures that i is only reset to 0 once per day
  }
  //Triggers actions at sunrise and sunset
  if (tod == todays_sunrise && (the_second == 0 || the_second == 1 || the_second == 2)) { // trigger sunrise scene
    Serial.println("The sun rose");
    SunriseAction();
  }
  if (tod == todays_sunset && (the_second == 0 || the_second == 1 || the_second == 2)) { //trigger sunset scene
    Serial.println("The sun set");
    SunsetAction();
  }
  //Maps sunrise and sunset to lighting scences, core of program
  map_number = map(tod, todays_sunrise, todays_sunset, 0, 14);
  //Uses mapped scene to chooses case/scene for switch
  if (tod < todays_sunrise || tod > todays_sunset) {   //Sets all lights on between sunset and sunrise
    scene_number = 300; //default
  }
  else if (map_number == 0 || map_number == 14) { //All lights on
    scene_number = 300;
  }
  else if (map_number >= 1 && map_number <= 6) {
    scene_number = 301;
  }
  else if (map_number == 7) {  //All lights off
    scene_number = 302;
  }
  else if (map_number >= 8 && map_number <= 13) {
    scene_number = 303;
  }
  switch (scene_number) {
    case 300: //All on
      for (int thisBulb = 0; thisBulb < bulbCount; thisBulb++) {
        digitalWrite(bulbs[thisBulb], Bulb_ON);
      }
      break;
    case 301: //Leading half on, trailing half off
      for (int thisBulb = 0; thisBulb < map_number; thisBulb++) {
        digitalWrite(bulbs[thisBulb], Bulb_OFF);
      }
      for (int thisBulb = bulbCount; thisBulb >= map_number; thisBulb--) {
        digitalWrite(bulbs[thisBulb], Bulb_ON);
      }
      break;
    case 302: //All off
      for (int thisBulb = 0; thisBulb < bulbCount; thisBulb++) {
        digitalWrite(bulbs[thisBulb], Bulb_OFF);
      }
      break;
    case 303: //Leading half off, trailing half on
      for (int thisBulb = 0; thisBulb < map_number - bulbCount; thisBulb++) {
        digitalWrite(bulbs[thisBulb], Bulb_ON);
      }
      for (int thisBulb = bulbCount; thisBulb >= map_number - bulbCount; thisBulb--) {
        digitalWrite(bulbs[thisBulb], Bulb_OFF);
      }
      break;
  }
}

void RepeatsV1() {
  if (the_hour > 12) {
    j = the_hour - 12;
  }
  else {
    j = the_hour;
  }
  for (int thisBulb = 0; thisBulb < bulbCount; thisBulb++) {
    digitalWrite(bulbs[thisBulb], Bulb_OFF);
  }
  delay(delay0);
  for (int thisBulb = 0; thisBulb < bulbCount; thisBulb++) {
    digitalWrite(bulbs[thisBulb], Bulb_ON);
    delay(delay1);
  }
  for (int thisBulb = 0; thisBulb < bulbCount; thisBulb++) {
    digitalWrite(bulbs[thisBulb], Bulb_OFF);
  }

  for (int thisBulb = 0; thisBulb < j - bulbCount + 1; thisBulb++) {
    digitalWrite(bulbs[thisBulb], Bulb_ON);
    delay(delay1);
  }
  delay(delay2);
}

void RepeatsV2() {
  if (the_hour > 12) {
    j = the_hour - 12;
  }
  else {
    j = the_hour;
  }
  for (int thisBulb = 0; thisBulb < bulbCount; thisBulb++) {
    digitalWrite(bulbs[thisBulb], Bulb_OFF);
  }
  delay(delay0);
  for (int thisBulb = 0; thisBulb < j + 1; thisBulb++) {
    digitalWrite(bulbs[thisBulb], Bulb_ON);
    delay(delay1);
  }
  delay(delay2);
}

void SunriseAction() {
  digitalWrite(bulbs[3], Bulb_ON);
  delay(800);
  digitalWrite(bulbs[2], Bulb_ON);
  digitalWrite(bulbs[4], Bulb_ON);
  delay(900);
  digitalWrite(bulbs[1], Bulb_ON);
  digitalWrite(bulbs[5], Bulb_ON);
  delay(1050);
  digitalWrite(bulbs[0], Bulb_ON);
  digitalWrite(bulbs[6], Bulb_ON);
  digitalWrite(bulbs[3], Bulb_ON);
  delay(delay1);
  delay(800);
  digitalWrite(bulbs[2], Bulb_ON);
  digitalWrite(bulbs[4], Bulb_ON);
  delay(900);
  digitalWrite(bulbs[1], Bulb_ON);
  digitalWrite(bulbs[5], Bulb_ON);
  delay(1050);
  digitalWrite(bulbs[0], Bulb_ON);
  digitalWrite(bulbs[6], Bulb_ON);
  digitalWrite(bulbs[3], Bulb_ON);
  delay(delay1);
  delay(800);
  digitalWrite(bulbs[2], Bulb_ON);
  digitalWrite(bulbs[4], Bulb_ON);
  delay(900);
  digitalWrite(bulbs[1], Bulb_ON);
  digitalWrite(bulbs[5], Bulb_ON);
  delay(1050);
  digitalWrite(bulbs[0], Bulb_ON);
  digitalWrite(bulbs[6], Bulb_ON);
  delay(delay2);
}

void SunsetAction() {
  digitalWrite(bulbs[0], Bulb_OFF);
  digitalWrite(bulbs[6], Bulb_OFF);
  delay(800);
  digitalWrite(bulbs[1], Bulb_OFF);
  digitalWrite(bulbs[5], Bulb_OFF);
  delay(900);
  digitalWrite(bulbs[2], Bulb_OFF);
  digitalWrite(bulbs[4], Bulb_OFF);
  delay(1050);
  digitalWrite(bulbs[3], Bulb_OFF);
  delay(delay1);
  digitalWrite(bulbs[0], Bulb_OFF);
  digitalWrite(bulbs[6], Bulb_OFF);
  delay(800);
  digitalWrite(bulbs[1], Bulb_OFF);
  digitalWrite(bulbs[5], Bulb_OFF);
  delay(900);
  digitalWrite(bulbs[2], Bulb_OFF);
  digitalWrite(bulbs[4], Bulb_OFF);
  delay(1050);
  digitalWrite(bulbs[3], Bulb_OFF);
  delay(delay1);
  digitalWrite(bulbs[0], Bulb_OFF);
  digitalWrite(bulbs[6], Bulb_OFF);
  delay(800);
  digitalWrite(bulbs[1], Bulb_OFF);
  digitalWrite(bulbs[5], Bulb_OFF);
  delay(900);
  digitalWrite(bulbs[2], Bulb_OFF);
  digitalWrite(bulbs[4], Bulb_OFF);
  delay(1050);
  digitalWrite(bulbs[3], Bulb_OFF);
  delay(delay2);
}

void digitalClockDisplay()
{
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(", ");
  Serial.print("The day of the year is ");
  Serial.print(doy);
  Serial.print(", ");
  Serial.print("The month is ");
  Serial.print(the_month);
  Serial.print(", ");
  Serial.print("The day is ");
  Serial.print(the_day);
  Serial.print(", ");
  Serial.print("The time of day is ");
  Serial.print(tod);
  Serial.print(", ");
  Serial.print("The sunrise today is ");
  Serial.print(todays_sunrise);
  Serial.print(" and the sunset is ");
  Serial.print(todays_sunset);
  Serial.print(", ");
  Serial.print("The map number is ");
  Serial.print(map_number);
  Serial.print(", ");
  Serial.print("The scene is ");
  Serial.println(scene_number);
}

void printDigits(int digits)
{
  Serial.print(":");
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}




