#!/bin/tclsh
#
# Version 1.1 * (C) '2020 by Uwe Langhammer
# script to toggle switches/dimmer
#
# usage: toggle.tcl [-v] [-ms] <interface.address:ch.DP> [<LEVEL>] [<RAMPTIME0>] [<RAMPTIME1>] [<ONTIME>]
#

load tclrega.so

set state_file "/var/cache/cuxd/state_"

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
  puts "USAGE: toggle.tcl \[-v\] \[-ms\] <interface.address:ch.DP> \[<LEVEL>\] \[<RAMPTIME0>\] \[<RAMPTIME1>\] \[<ONTIME>\]"
  exit 1
}

set address [lindex $argv 0]
set state [lindex $argv 1]
set set_ramptime0 [lindex $argv 2]
set set_ramptime1 [lindex $argv 3]
set set_ontime [lindex $argv 4]

if { $argc >= 2 } {
  if {[string is double $state] && ($state > 0)} {
  } else {
    set state 0
  }
} else {
  set state 0
}

if { 1 } {
  set level 0
  if {[regexp -- {\.[^\.]+\:\d+\.LEVEL$} $address]} {
    set level 1
  }
  regsub -- {\.[^\.]+$} $address "" addr

  append cmd "var ret;"

  append state_file $address
  if { [ file exists $state_file ] && ([ expr [ clock seconds ] - [ file mtime $state_file ] ] < 4) } {
    set fp [ open $state_file "r" ]
    gets $fp data
    close $fp
    if { $data } {
      append cmd "if (true) \{"
    } else {
      append cmd "if (false) \{"
    }
  } else {
    append cmd "if (dom.GetObject(\"$address\").Value()) \{"
  }

  if {[string is double $set_ramptime0] && ($set_ramptime0 > 0)} {
    if { $ms } { set set_ramptime0 [expr $set_ramptime0 / 1000.0] }
    puts "ON_TIME ${set_ramptime0}s"
    append cmd "dom.GetObject(\"$addr.RAMP_TIME\").State(\"$set_ramptime0\");"
  }

  append cmd "dom.GetObject(\"$address\").State(0);"
  append cmd "ret = 0;"
  append cmd "\} else \{"

  if {[string is double $set_ramptime1] && ($set_ramptime1 > 0)} {
    if { $ms } { set set_ramptime1 [expr $set_ramptime1 / 1000.0] }
    puts "ON_TIME ${set_ramptime1}s"
    append cmd "dom.GetObject(\"$addr.RAMP_TIME\").State(\"$set_ramptime1\");"
  }

  if {[string is double $set_ontime] && ($set_ontime > 0)} {
    if { $ms } { set set_ontime [expr $set_ontime / 1000.0] }
    puts "ON_TIME ${set_ontime}s"
    append cmd "dom.GetObject(\"$addr.ON_TIME\").State(\"$set_ontime\");"
  }

  if { $level } {
    if { $state } {
      append cmd "dom.GetObject(\"$address\").State($state);"
    } else {
      append cmd "dom.GetObject(\"$addr.OLD_LEVEL\").State(1);"
    }
  } else {
    append cmd "dom.GetObject(\"$address\").State(1);"
  }
  append cmd "ret = 1;"
  append cmd "\}"

  if { $verbose } { puts $cmd }

  array set values [rega_script $cmd]

  if { [info exists values(ret)] && ( $values(ret) != "null" ) } {
    puts "OK"
#    puts $values(ret)
    set fp [ open $state_file "w" ]
    puts -nonewline $fp $values(ret)
    close $fp
    exit 0
  } else {
    puts "ERROR"
    exit 1
  }
}
