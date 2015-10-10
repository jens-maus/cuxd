#!/bin/env python

# Initalize the indicated serial port and continuously read the temperatures
# and graph them in a window. Show the discovered sensors and allow the user
# to select the color associated with it. 
# Also allow them to control the refresh rate.

""" DigiTemp PyGTK Graph display """

import string, os, sys, time, gtk


dt = {}

# Initalize the 1-wire network, print out whatever it says
def init_1wire():
   cmd = "digitemp -i -q -o\"T %R %N %.2C\" -O\"C %R %N %n %C\""
   for outline in os.popen(cmd).readlines()
      outline = outline[:-1];
      print outline

   # Ought to be able to check return code for success/failure


# Read all attached sensors
def read_sensors():
   cmd = "digitemp -a -q -o1"
   for outline in os.popen(cmd).readlines():
      outline = outline[:-1]
      S = string.split( outline, " " )

      if S[0] == 'T':
         # Add the temperature reading to a dictionary
         dt[S[1]] = S[3]

      if S[0] == 'C':
         # Add the counter reading to a dictionary
         dt[S[1]] = S[3]


         