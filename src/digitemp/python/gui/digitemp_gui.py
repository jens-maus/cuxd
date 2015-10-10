#!/usr/bin/env python
#
# DigiTemp temperature and 1-wire home weather station graphing
# Copyright 2003 by Brian C. Lane <bcl@brianlane.com>
# http://www.digitemp.com
#
# Requires DigiTemp v3.1.0 or later for counter support
# 

import string, os, sys, time

timefmt = '%Y-%m-%d %H:%M:%S'

# 1. Read the output of DigiTemp
cmd = '/home/brian/temperature/digitemp -c/home/brian/temperature/digitemp.cfg -a -q'

for outline in os.popen(cmd).readlines():
   outline = outline[:-1]
#   print outline

   if outline[0:1] == 'T':
      # Parse the temperature sensor line
      S = string.split(outline, " ")
#      print S      

      # Add the reading to the database
      sql = "INSERT INTO temperature VALUES(NULL, %s, %s, %s)"
      sqltime = time.strftime( timefmt, time.localtime(int(S[2])))
      cursor.execute( sql, (S[1], sqltime, S[3]) );

   if outline[0:1] == 'C':
      # Parse the counter line
      S = string.split(outline, " ")
#      print S

      # Add the reading to the database
      sql = "INSERT INTO counter VALUES(NULL, %s, %s, %s, %s)"
      sqltime = time.strftime( timefmt, time.localtime(int(S[2])))
      cursor.execute( sql, (S[1], sqltime, S[3], S[4]) );

# -----------------------------------------------------------------------
# Do interesting things with the just-logged information
# =======================================================================
# Brian's Sensor Map:
#
#10E8A00E00000055      Room (grey wire)
#10B95E05000800AA      Attic
#10575A050008008F      Desk
#22B9B20500000049      DS1822
#286D1D2D000000EA      DS18B20 (kermit)
#1092B9330008002E      Drink
#1D9CB900000000B3      Rain Gauge
#1DFA15010000005F      Wind Speed
#1009212E0008004B      DT-1A passive sensor
#

# Dict of serial numbers and what to name them
sensors = {'10E8A00E00000055':'Office',
           '10B95E05000800AA':'Attic',
           '10575A050008008F':'Desk',
           '22B9B20500000049':'DS1822',
           '286D1D2D000000EA':'DS18B20',
           '1092B9330008002E':'Drink',
           '1009212E0008004B':'DT1A'
          }

counters= {'1D9CB900000000B3':'Rain',
           '1DFA15010000005F':'Wind'
          }

def c2f( c ):
  f = 32.0 + ((c * 9.0) / 5.0)
  return f

