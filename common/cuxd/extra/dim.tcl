#!/bin/tclsh
#
# Version 1.0 * (C) '2017 by Uwe Langhammer
# script to dim up/down
#
# usage: dim.tcl [-v] [-ms] <interface.address:ch.DP> [[-]<LEVELDIFF>] [<DELAYTIME>]
#  STOP: dim.tcl <interface.address:ch.DP> 0
#

load tclrega.so

proc getopt {_argv name {_var ""}} {
  upvar 1 $_argv argv $_var var
  set pos [lsearch -regexp $argv ^$name]
  if {$pos>=0} {
    if {$_var != ""} {
      set var 1
    }
    set argv [lreplace $argv $pos $pos]
    incr ::argc -1
    return 1
  }
  return 0
}

set verbose [getopt argv -v]
set ms [getopt argv -ms]

if { $argc < 1 } {
  puts "USAGE: dim.tcl \[-v\] \[-ms\] <interface.address:ch.DP> \[[-]<LEVELDIFF>\] \[<DELAYTIME\]"
  exit 1
}

set address [lindex $argv 0]
set leveldiff [lindex $argv 1]
set delaytime [lindex $argv 2]
set pidfilename "/tmp/dim_$address"
set run 1

if { $argc >= 2 } {
  if {[string is double $leveldiff] && ($leveldiff != 0)} {
  } else {
    puts "STOP"
    set run 0
  }
} else {
  puts "STOP"
  set run 0
}

if { $run } {
  set cmd "var ret = dom.GetObject(\"$address\").Value();"
  if { $verbose } { puts $cmd }
  array set values [rega_script $cmd]
#  array set values [list ret 0.5]

  if { [info exists values(ret)] } {
    if {[string is double $values(ret)]} {
#      puts $values(ret)
      set new_level $values(ret)
      puts "STEP $leveldiff"
      if {[string is double $delaytime] && ($delaytime > 0)} {
        if { ! $ms } { set delaytime [expr $delaytime * 1000] }
        puts "DELAY ${delaytime}ms"
      }

      set pidfile [open $pidfilename w]
	  puts $pidfile [pid]
      close $pidfile 

      set pidfilemtime [file mtime $pidfilename]

      while { $run } {
        set new_level [ expr { $new_level + $leveldiff } ]
        if { ($new_level >= 0.999) } {
          set new_level 1
        }
        if { ($new_level <= 0.001) } {
          set new_level 0
        }
#        puts $new_level
        set cmd "dom.GetObject(\"$address\").State($new_level);"
        if { $verbose } { puts $cmd }
        array set values [rega_script $cmd]
        if { ($new_level == 0) || ($new_level == 1) } {
          set run 0
        } else {
          if {[string is double $delaytime] && ($delaytime > 0)} {
            after [expr int($delaytime)]
			if { [file exists $pidfilename] } {
	          if { $pidfilemtime != [file mtime $pidfilename] } {
                puts "ERROR: dim level changed"
                exit 1
		      }
			} else {
			  puts "STOP"
			  exit 0
			}
          } else {
            set run 0
          }
		}
      }
    }
  } else {
    puts "ERROR"
    exit 1
  }
}
if { !$run } {
  file delete $pidfilename
}
exit 0