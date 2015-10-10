#!/usr/bin/env python

# --------------------------------------------------------------------------
# 07/13/2003   The background needs to be drawn when starting up, it isn't
# bcl          white. Need to record all the points so that the display can
#              be redrawn when another window uncovers it.
#              Add Glade dialog boxes to it for managing the sensors, their
#              names and colors.
#
#              When new data is read it needs to be added to an array of
#              points (for each sensor). The array is then plotted using the
#              points or line segment graphing functions. Hmm, should the
#              reading be saved, and points regenerated? This allows for window
#              resizing without losing any of the historical data.
#
#              I also need to draw a background.
#
#              Dictionary of serial numbers, names and colors
#              Dictionary of serial numbers, array of sensor readings
#
#              Add to the serial number list in the dt dictionary
#              dt[serial#].append( value )
#
#              How much data should be stored? I can always slice on the end
#              of the list based on the display width, but I don't want to
#              fill up memory by never removing old data.
#
# 07/12/2003   Second attempt at a PythonGTK DigiTemp live graphing app.
# bcl
# --------------------------------------------------------------------------

import os
import gtk
import time
import string
import operator
import sys


class DigiTempGraph:

    min_temp = 0
    max_temp = 40
    readtemp_timer = 0
    last_y = 0

    # Sensor serial # and Temperature List
    dt = {}

    # Sensor serial # and name, color info
    sensors = {}

    def __init__(self):
        window = gtk.Window(gtk.WINDOW_TOPLEVEL)
        window.set_title("DigiTemp Graph")
        window.connect("destroy", gtk.mainquit)
        self.area = gtk.DrawingArea()
        self.area.set_size_request(400, 200)
        self.pangolayout = self.area.create_pango_layout("")
        window.add(self.area)
        self.area.connect("expose-event", self.area_expose_cb)
        self.area.show()

        # Setup the colors to be used
        self.colormap = window.get_colormap()
        self.white = self.colormap.alloc_color( "white" )
        self.black = self.colormap.alloc_color( "black" )

        window.show()

        # Start reading the temperature every ? seconds
        self.init_sensors()
        readtemp_timer = gtk.timeout_add( 1000, self.readtemp)


    def area_expose_cb(self, area, event):
        self.style = self.area.get_style()
        self.gc = self.style.fg_gc[gtk.STATE_NORMAL]

        # Redraw the window, something has uncovered it

        return gtk.TRUE

    # Initalize the 1-wire network, print out whatever it says
    def init_sensors( self ):
        cmd = "/home/bcl/digitemp/digitemp -s/dev/ttyUSB0 -i -q -o\"T %R %N %.2C\" -O\"C %R %N %n %C\""
        for outline in os.popen(cmd).readlines():
            outline = outline[:-1];
#            print outline

            S = string.split( outline, ": " )

#            print S[0]
#            print S[1]

            # Setup the sensor dictionary with an empty list
            if( S[0][0:3] == 'ROM' ):
                self.dt[S[1]] = []
                self.sensors[S[1]] = [ S[1], "black" ]
#                print "Added ",S[1]," to the dictionary\n"
                
        # Ought to be able to check return code for success/failure


    # Read all attached sensors
    def read_sensors( self ):
        cmd = "/home/bcl/digitemp/digitemp -a -q -o1"
        for outline in os.popen(cmd).readlines():
            outline = outline[:-1]
            #      print outline

            S = string.split( outline, " " )

            if S[0] == 'T':
                value = string.atof(S[3])
                # Add the temperature reading to a dictionary
                self.dt[S[1]].append( value )

                # Try to auto-size the graph
                if( value > self.max_temp ):
                    self.max_temp = value + 5
                if( value < self.min_temp ):
                    self.min_temp = value - 5

            if S[0] == 'C':
                value = string.atoi( S[3] )
                # Add the counter reading to a dictionary
                self.dt[S[1]].append( value )

        # Should return the digitemp return code
        return 1

    # Read the temperature from the attached DigiTemp system
    # and plot the new temperatures
    def readtemp( self ):
        global readtemp_timer

        if( self.read_sensors() == 1 ):
            for sensor in self.dt:
#                print self.dt[sensor]
                self.plottemp( sensor, self.dt[sensor] )

        # Restart the timer
        readtemp_timer = gtk.timeout_add( 1000, self.readtemp)


    # Plot the temperatures in the drawing area
    def plottemp( self, sensor, temperature ):
        (width, height) = self.area.window.get_size()
        
        # Scroll the display to the left
        self.area.window.draw_drawable( self.gc, self.area.window, 1, 0, 0, 0, width-1, height)

        # Erase the pixel just copied over
        self.gc.set_foreground( self.white )
        self.area.window.draw_point(self.gc, width-1, self.last_y)

        # Plot the new temperature
        self.gc.set_foreground( self.black )
        temp_y = height - int(temperature * (height / (self.max_temp-self.min_temp)))

        self.area.window.draw_point(self.gc, width-1, temp_y)
        self.last_y = temp_y


def main():
    gtk.main()
    return 0

if __name__ == "__main__":
    DigiTempGraph()
    main()
