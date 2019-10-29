#!/bin/tclsh
#
# (C) '2018 by Uwe Langhammer
# script for reading RFD duty cycles
#
# all parameters are optional!
#
# usage: dutycycle.tcl <port>[,<port>...] [<filename>] [<sleep_sec>]
#
load tclrpc.so

set ports [lindex $argv 0]
set filename [lindex $argv 1]
set sleep [lindex $argv 2]

if { $argc <  1 } {
  puts "USAGE: dutycycle.tcl <port>\[,<port>...\] \[<filename>i\] \[<sleep_sec>\]"
  exit 1
}

set sleep1000s 0
if {[string is double $sleep] && ($sleep > 0)} {
  set sleep1000s [expr int([expr $sleep * 1000])]
}

set portlist [split $ports ","]

while { 1 } {
  set lines ""
  foreach port $portlist {
    set r ""
    if {[catch {set r [xmlrpc http://127.0.0.1:$port/ listBidcosInterfaces]} fid]} {
      puts stderr $fid
    }
    if {[string length $r] > 20 } {
      foreach gateway $r {
        array set gw $gateway
        set records {ADDRESS TYPE DUTY_CYCLE}
        foreach rec $records {
          if {[info exists gw($rec)]} {
            append lines "$gw($rec)\t"
          } else {
            append lines "\t"
          }
        }
        append lines "\n"
      }
    }
  }
  if {[string length $filename] > 1 } {
    set fo [open $filename "w"] 
    puts -nonewline $fo $lines
    close $fo
  } else {
    puts $lines
  }
  if { $sleep1000s > 0 } {
    after $sleep1000s
  } else {
    break
  }
}

