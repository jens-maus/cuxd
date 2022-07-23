#!/bin/tclsh
#
# (C) '2021 by Uwe Langhammer
# script for reading sytem.Name()
#
# all parameters are optional!
#
# usage: systemname.tcl [<filename>]
#
load tclrega.so

set filename [lindex $argv 0]

set cmd "ret = system.Name();";

if {[catch {array set values [rega_script $cmd]} fid]} {
  puts stderr $fid
} else {
  if {[info exists values(ret)]} {
    if {[string length $filename] > 1 } {
      set fo [open $filename "w"]
      puts -nonewline $fo $values(ret)
      close $fo
    } else {
      puts $values(ret)
    }
  }
}
