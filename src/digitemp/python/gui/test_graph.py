#!/usr/bin/env python

# ------------------------------------------------------------------------
# DigiTemp Python Graphing Application
# Copyright 2003 by Brian C. Lane <bcl@brianlane.com>
# All Rights Reserved
#
# Released under GPL v2.0
#
# ==============================[ History ]===============================
# 07/11/2003   Need to catch a resize event so I an recalculate the scale
# bcl          for the display.
#              Still don't know how to scroll. 
#              drawing is documented here:
#              http://www.gnome.org/~james/pygtk-docs/class-gdkdrawable.html
#
#              How do I get a gc without a widget so I can draw on the 
#              pixmap? And so I can copy it to scroll it left or right.
#
# 07/10/2003   It is now reading temperatures. It still needs to be able
# bcl          to check the return value from DigiTemp for errors.
#              I need to add plotting of temps and scrolling of the graph
#              next.
#              Eventually there will be a menu of sensors and dialog boxes
#              to let the user enter descriptive names for the sensors,
#              and assign colors and alarms.
#
# 06/20/2003   I have timers working, need overlaying pixmaps and getting
# bcl          temperatures from DigiTemp.
#              How do I draw one image over another, with transparency?
#              Also need to catch resize events and redraw. Hmm, which 
#              means that I need to keep track of a screen's worth of
#              datapoints. 
#              If it has to restart when resized that isn't too bad, I 
#              can always change that later.
# -----------------------------------------------------------------------
# Old notes:
#
# I think I can draw with this, but how do I hook up other events, like a
# timer to go and read temperatures every 10 seconds or whenever?
# Really ought to be multi-threaded.
# Python has a thread module too... Need to look into it later

# Need a routine to draw the background for the graph, based on the canvas
# size and range of temperatures to display.

# Need to use 2 pixmaps, one for the graph and one for the background. The
# graph pixmap will get scrolled to the right or left and then overlaied
# onto the background (using a transparent color).

import string, os, sys, time, gtk
bg_pixmap = None
pixmap = None
readtemp_timer = 0
dt = {}

# Default temperature limits of the pixmap in Centigrade
graph_min = -10
graph_max = 60


# Initalize the 1-wire network, print out whatever it says
def init_1wire():
   cmd = "/home/bcl/digitemp/digitemp -s/dev/ttyUSB0 -i -q -o\"T %R %N %.2C\" -O\"C %R %N %n %C\""
   for outline in os.popen(cmd).readlines():
      outline = outline[:-1];
      print outline

   # Ought to be able to check return code for success/failure


# Read all attached sensors
def read_sensors():
   cmd = "/home/bcl/digitemp/digitemp -a -q -o1"
   for outline in os.popen(cmd).readlines():
      outline = outline[:-1]
#      print outline
      S = string.split( outline, " " )

      if S[0] == 'T':
         # Add the temperature reading to a dictionary
         dt[S[1]] = S[3]

      if S[0] == 'C':
         # Add the counter reading to a dictionary
         dt[S[1]] = S[3]

   return 1

# Plot a new temperature point on the graph
# Scroll the graph the right direction and add the new plot
def plottemp( centigrade ):
	global pixmap

        # Get the current image width and height
        (width, height) = pixmap.get_size()

        # Scroll the image to the left
        image = pixmap.get_image( 1,0,width-1,height)
        pixmap.draw_image( gc, image, 0, 0, width-1, height, 
                           0, 0, width-1, height )

# Read the temperature from the attached DigiTemp system
# and plot the new temperatures
def readtemp( ):
	global readtemp_timer

        if( read_sensors() == 1 ):
           for sensor in dt:
              print dt[sensor]
              plottemp( dt[sensor] )

        # Restart the timer
        readtemp_timer = gtk.timeout_add( 1000, readtemp)


# Draw a graph on the background
def draw_graph_bg():
	global pixmap


# Draw the initial window?
def configure_event(widget, event):
        global pixmap
        win = widget.window
        width, height = win.get_size()

        # Create the graph pixmap
        pixmap = gtk.gdk.Pixmap(win, width, height)
        pixmap.draw_rectangle(widget.get_style().white_gc, gtk.TRUE,
                              0, 0, width, height)

        # Create the background pixmap
        bg_pixmap = gtk.gdk.Pixmap(win, width, height)
        draw_graph_bg()
        return gtk.TRUE

def expose_event(widget, event):
        x, y, width, height = event.area
        gc = widget.get_style().fg_gc[gtk.STATE_NORMAL]
        widget.window.draw_drawable(gc, pixmap, x, y, x, y, width, height)
        return gtk.FALSE

# Draw something on the display
def draw_brush(widget, x, y):
        rect = (x-5, y-5, 10, 10)
        pixmap.draw_rectangle(widget.get_style().black_gc, gtk.TRUE,
                              x-5, y-5, 10, 10)
        widget.queue_draw()


def main():
	if( init_1wire() == 0 ):
	  print "1-wire initialization failed\n";
	  exit;

        win = gtk.Window()
        win.set_name("DigiTemp Realtime Graph")
        win.connect("destroy", gtk.mainquit)
        win.set_border_width(5)

        vbox = gtk.VBox(spacing=3)
        win.add(vbox)
        vbox.show()

        drawing_area = gtk.DrawingArea()
        drawing_area.set_size_request(400, 150)
        vbox.pack_start(drawing_area)
        drawing_area.show()

        drawing_area.connect("expose_event", expose_event)
        drawing_area.connect("configure_event", configure_event)

        drawing_area.set_events(gtk.gdk.EXPOSURE_MASK )

        button = gtk.Button("Quit")
        vbox.pack_start(button, expand=gtk.FALSE, fill=gtk.FALSE)
        button.connect("clicked", lambda widget, win=win: win.destroy())
        button.show()
        win.show()

        # Read the temperatures in 1 second
        readtemp_timer = gtk.timeout_add( 1000, readtemp)

        gtk.main()

if __name__ == '__main__':
	main()
