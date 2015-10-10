#!/usr/bin/env python
#
# DigiTemp temperature and q-wire home weather station logging
# Copyright 2003 by Brian C. Lane <bcl@brianlane.com>
# http://www.digitemp.com
#
# Requires DigiTemp v3.1.0 or later for counter support
# Requires Python MySQLdb module and Round Robin Database binaries
#
# Logs data to a MySQL database and to a RRDb graph log
#
# Create a MySQL database called weather and give a weather user permission
# to access it.
# GRANT SELECT,INSERT,CREATE ON weather.* TO weather@localhost IDENTIFIED BY 'password';
#
# CREATE TABLE temperature (
#   TemperatureKey bigint UNSIGNED NOT NULL auto_increment,
#   SerialNumber varchar(20) NOT NULL,
#   RecTime timestamp NOT NULL,
#   C float NOT NULL, 
#   PRIMARY KEY(SerialNumber),
#   KEY(TemperatureKey)
# );
#
# CREATE TABLE counter (
#   CounterKey bigint UNSIGNED NOT NULL auto_increment, 
#   SerialNumber varchar(20) NOT NULL,
#   RecTime timestamp NOT NULL,
#   CounterNum tinyint UNSIGNED NOT NULL,
#   CounterValue mediumint UNSIGNED NOT NULL,
#   PRIMARY KEY(SerialNumber),
#   KEY(CounterKey)
# );
# 

import string, os, sys, time
import MySQLdb

timefmt = '%Y-%m-%d %H:%M:%S'

# Connect to the database
try:
  mydb = MySQLdb.Connect(host='localhost',user='weather',passwd='password',db='weather')
except DatabaseError:
  print "Problem connecting to database"
  sys.exit(-1)

cursor=mydb.cursor()

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

rrdtool_path = "/usr/local/rrdtool/bin/rrdtool"
rrd_path     = "/home/brian/temperature/"

def c2f( c ):
  f = 32.0 + ((c * 9.0) / 5.0)
  return f

rrd_sensors = {}

# grab the latest readings for the sensors
for skey in sensors.keys():
  sql = "SELECT UNIX_TIMESTAMP(rectime), C from temperature where serialnumber='"+skey+"' ORDER BY rectime DESC LIMIT 1"
#  print sql
  cursor.execute(sql)
  result = cursor.fetchone()
#  print result
  rrd_sensors[sensors[skey]] = result

# grab the counters
rrd_counters = {}

# grab the latest readings for the sensors
for ckey in counters.keys():
  sql = "SELECT UNIX_TIMESTAMP(rectime), countervalue from counter where serialnumber='"+ckey+"' AND counternum=0 ORDER BY rectime DESC LIMIT 1"
#  print sql
  cursor.execute(sql)
  result = cursor.fetchone()
#  print result
  rrd_counters[counters[ckey]] = result

# Log the temperatures to the RRD
# rrdtool update drink.rrd time:value
rrd = "%s update %s/drink.rrd %ld:%0.2f"  % (rrdtool_path, rrd_path, rrd_sensors['Drink'][0], rrd_sensors['Drink'][1])
os.system(rrd)
#print rrd

# rrdtool update outside.rrd time:value
rrd = "%s update %s/outside.rrd %ld:%0.2f"  % (rrdtool_path, rrd_path, rrd_sensors['Attic'][0], rrd_sensors['Attic'][1])
os.system(rrd)
#print rrd

# rrdtool update room.rrd time:value
rrd = "%s update %s/room.rrd %ld:%0.2f"  % (rrdtool_path, rrd_path, rrd_sensors['Office'][0], rrd_sensors['Office'][1])
os.system(rrd)
#print rrd

# rrdtool update room.rrd time:value
rrd = "%s update %s/kermit.rrd %ld:%0.2f"  % (rrdtool_path, rrd_path, rrd_sensors['DS1822'][0], rrd_sensors['DS1822'][1])
os.system(rrd)
#print rrd

# Log the counter values
# rrdtool update rain.rrd time:value
rrd = "%s update %s/rain.rrd %ld:%ld"  % (rrdtool_path, rrd_path, rrd_counters['Rain'][0], rrd_counters['Rain'][1])
os.system(rrd)
#print rrd

# rrdtool update wind.rrd rime:value
rrd = "%s update %s/wind.rrd %ld:%ld"  % (rrdtool_path, rrd_path, rrd_counters['Wind'][0], rrd_counters['Wind'][1])
os.system(rrd)
#print rrd


# Write a new .signature file
outfile = open('/home/user/.signature','w')
sig = "--[Inside %0.1fF]--[Outside %0.1fF]--[Kermit %0.1fF]--[Coaster %0.1fF]--\n" % (c2f(rrd_sensors['Office'][1]),c2f(rrd_sensors['Attic'][1]),c2f(rrd_sensors['DS1822'][1]),c2f(rrd_sensors['Drink'][1]))
outfile.write(sig)
outfile.close()

mydb.close()
