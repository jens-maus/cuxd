#!/usr/bin/perl -w
#
# DigiTemp v2.1 RRDB Temperature Graph
#
# Copyright 1997-2001 by Brian C. Lane <bcl@brianlane.com> www.brianlane.com
# All Rights Reserved
#
# This program is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the Free
# Software Foundation; either version 2 of the License, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
# more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA
#

# REQUIRES:
#  RRD tool binary
#  CGI (comes with perl5)

use RRDp;
use CGI;

$cgi = new CGI;                   # Load the CGI routines

# Find the RRD executable, look in the standard locations
# If your rrdtool is installed someplace else, fill in the proper location
# below:
if ( -x "/usr/bin/rrdtool") 
{
  RRDp::start "/usr/bin/rrdtool";
} elsif ( -x "/usr/local/bin/rrdtool") {
   RRDp::start "/usr/local/bin/rrdtool";
} elsif ( -x "/usr/local/rrdtool/bin/rrdtool" ) {
   RRDp::start "/usr/local/rrdtool/bin/rrdtool";
} else {
   die "Could not find rrdtool binary\n";
}

# The RRD database to get the data from
$rrd = "/tmp/digitemp.rrd";


# Make the variables easier to use and assign defaults if not set
if( !$cgi->param('starttime') )
{
    $starttime = "-1day";
} else {
    $starttime = $cgi->param('starttime');
}
if( !$cgi->param('endtime') )
{
    $endtime = time;
} else {
    $endtime = $cgi->param('endtime');
}
if( !$cgi->param('width') )
{
    $width = "400";
} else {
    $width = $cgi->param('width');
}
if( !$cgi->param('height') )
{
    $height = "100";
} else {
    $height = $cgi->param('height');
}
if( !$cgi->param('label') )
{
    $label="";
} else {
    $label = $cgi->param('label');
}
$var = $cgi->param('var');

# Diagnostic output
#print STDERR "start  = $starttime\n";
#print STDERR "end    = $endtime\n";
#print STDERR "width  = $width\n";
#print STDERR "height = $height\n";
#print STDERR "var    = $var\n";
#print STDERR "label  = $label\n";


# Output a HTML header for the PNG image to follow
print $cgi->header('image/png');

# Generate the graph
RRDp::cmd "graph - --imgformat PNG",
    "--start '$starttime' --end '$endtime'",
    "--width $width --height $height",
    "DEF:room_c=$rrd:room:AVERAGE",
    "DEF:attic_c=$rrd:attic:AVERAGE",
    "DEF:desk_c=$rrd:desk:AVERAGE",
    "CDEF:room_f=room_c,9,*,5,/,32,+",
    "CDEF:attic_f=attic_c,9,*,5,/,32,+",
    "CDEF:desk_f=drink_c,9,*,5,/,32,+",
    "LINE1:room_f#00FF00:'Computer Room'",
    "LINE1:attic_f#FF0000:'Attic'",
    "LINE1:desk_f#0000FF:'Desk'";

$answer=RRDp::read;

print $$answer;

RRDp::end;
