#!/bin/tclsh
#
# (C) '2017 by Uwe Langhammer
# script for reading RFD duty cycles
#
# all parameters are optional!
#
# usage: dutycycle.tcl [<filename>] [<sleep_sec>]
#
load tclrpc.so

set filename [lindex $argv 0]
set sleep [lindex $argv 1]

set sleep1000s 0
if {[string is double $sleep] && ($sleep > 0)} {
  set sleep1000s [expr int([expr $sleep * 1000])]
}

while { 1 } {
  set r ""
  if {[catch {set r [xmlrpc http://127.0.0.1:2001/ listBidcosInterfaces]} fid]} {
    puts stderr $fid
  }
  set lines ""
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

