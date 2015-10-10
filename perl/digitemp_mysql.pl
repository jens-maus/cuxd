#!/usr/bin/perl -W

# DigiTemp MySQL logging script
# Copyright 2002 by Brian C. Lane <bcl@brianlane.com>
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
# -------------------------[ HISTORY ]-------------------------------------
# 01/08/2004  The storage definition should have been decimal(6,2) instead
# bcl         of decimal(3,2). 
#             See http://www.mysql.com/doc/en/Numeric_types.html for a 
#             good description of how decimal(a,b) works.
#
# 08/18/2002  Putting together this MySQL logging script for the new 
# bcl         release of DigiTemp.
#
# -------------------------------------------------------------------------
# CREATE table digitemp (
#   dtKey int(11) NOT NULL auto_increment,
#   time timestamp NOT NULL,
#   SerialNumber varchar(17) NOT NULL,
#   Fahrenheit decimal(6,2) NOT NULL,
#   PRIMARY KEY (dtKey),
#   KEY serial_key (SerialNumber),
#   KEY time_key (time)
# );
#
# GRANT SELECT,INSERT ON digitemp.* TO dt_logger@localhost
# IDENTIFIED BY 'TopSekRet';
#
# -------------------------------------------------------------------------
use DBI;


# Database info
my $db_name     = "digitemp";
my $db_user     = "dt_logger";
my $db_pass     = "TopSekRet";

# The DigiTemp Configuration file to use
my $digitemp_rcfile = "/home/brian/digitemp.cfg";
my $digitemp_binary = "/home/brian/bin/digitemp";


my $debug = 0;

# Connect to the database
my $dbh = DBI->connect("dbi:mysql:$db_name","$db_user","$db_pass")
          or die "I cannot connect to dbi:mysql:$db_name as $db_user - $DBI::errstr\n";


# Gather information from DigiTemp
# Read the output from digitemp
# Output in form SerialNumber<SPACE>Temperature in Fahrenheit
open( DIGITEMP, "$digitemp_binary -q -a -o\"%R %.2F\" -c $digitemp_rcfile |" );

while( <DIGITEMP> )
{
  print "$_\n" if($debug);
  chomp;

  ($serialnumber,$temperature) = split(/ /);

  my $sql="INSERT INTO digitemp SET SerialNumber='$serialnumber',Fahrenheit=$temperature";
  print "SQL: $sql\n" if($debug);
  $dbh->do($sql) or die "Can't execute statement $sql because: $DBI::errstr";
}

close( DIGITEMP );

$dbh->disconnect;
