#!/usr/bin/perl -w
#
# DigiTemp v2.3 RRDB Temperature logger
#
# Copyright 1997-2002 by Brian C. Lane <bcl@brianlane.com> www.brianlane.com
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

# This require the RRDB pipe module for Perl, available with the RRDB source
# from http://ee-staff.ethz.ch/~oetiker/webtools/rrdtool/
use RRDp;

# Convert centigrade to fahrenheit
sub ctof
{
  ($temp_c) = @_;

  return 32 + (($temp_c * 9)/5);
}



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

# The RRD database to put the data into
$wx_rrd = "/tmp/digitemp.rrd";

# Read the output from digitemp
# Output in form 0\troom\tattic\tdrink
open( DIGITEMP, "digitemp -a -q -o2 |" );

while( <DIGITEMP> )
{
#  print "$_\n";
  chomp;

  if( $_ =~ /^nanosleep/i )
  {
    $now = localtime;
    open( ERRLOG, ">>/tmp/dt-error") or die "Error opening dt-error";
    print ERRLOG "nanosleep error at $now\n";
    close( ERRLOG );
    die "nanosleep error";
  } else {

    ($null,$desk_DS18S20,$attic_DS18S20,$room) = split(/\t/);
  }
}

close( DIGITEMP );

# Put the info into the weather rrd database room:drink:attic
RRDp::cmd "update $wx_rrd N:$room:$attic_DS18S20:$desk_DS18S20";
#$answer=RRDp::read;



# This section was used to create up to date email signature files

# Create the email header in Fahrenheit
#open( HEADER, ">/tmp/.header") or die "Error opening .header";

#$now = localtime;
#print HEADER "[$now]--[Inside ";
#printf HEADER "%0.2f",ctof( $room );
#print HEADER "F]--[Outside ";
#printf HEADER "%0.2f",ctof( $attic_DS18S20 );
#print HEADER"F]--[Drink ";
#printf HEADER "%0.2f",ctof( $desk_DS18S20 );
#print HEADER "F]--\n";
#close( HEADER );

exit;
