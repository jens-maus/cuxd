#!/bin/tclsh
#
# Version 1.0 * (C) '2017 by Uwe Langhammer
# script for controling blinds
#
# usage: blind.tcl [-v] [-ms] [-stop] <interface.address:ch[.DP]> [<STATE>] [<ON_TIME>|<STEP>] [<RAMP_TIME>]
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
set stop_cmd [getopt argv -stop]

if { $argc < 1 } {
  puts "USAGE: blind.tcl \[-v\] \[-ms\] \[-stop\] <interface.address:ch\[.DP\]> \[<STATE>\] \[<ON_TIME>|<STEP>\] \[<RAMP_TIME>\]"
  exit 1
}

set address [lindex $argv 0]
set state [lindex $argv 1]
set set_ontime [lindex $argv 2]
set set_ramptime [lindex $argv 3]

if {[string is double $set_ontime]} { set set_ontime [expr $set_ontime + 0] }

if {[regexp -- {\.[^\.]+\:\d+$} $address]} {
  append address ".LEVEL"
}

if { $argc < 2 } {

  append cmd "var ret = dom.GetObject(\"$address\").Value();"
  if { $verbose } { puts $cmd }
  array set values [rega_script $cmd]

  if { [info exists values(ret)] } {
    puts $values(ret)
    exit 0
  } else {
    puts "ERROR"
    exit 1
  }

} else {
  regsub -- {\.[^\.]+$} $address "" addr
  
  if { $stop_cmd } {
    append cmd "if (dom.GetObject(\"$addr.WORKING\").Value()) \{"
    append cmd "dom.GetObject(\"$addr.STOP\").State(1);"
    append cmd "\} else \{"
  }
  
  if {[regexp -- {^CUxD\.} $address]} {
    if {[string is double $set_ontime] && ($set_ontime > 0)} {
      if { $ms } { set set_ontime [expr $set_ontime / 1000.0] }
      puts "ON_TIME ${set_ontime}s"
      append cmd "dom.GetObject(\"$addr.ON_TIME\").State(\"$set_ontime\");"
    }
  
    if {[string is double $set_ramptime] && ($set_ramptime > 0)} {
      if { $ms } { set set_ramptime [expr $set_ramptime / 1000.0] }
      puts "RAMP_TIME ${set_ramptime}s"
      append cmd "dom.GetObject(\"$addr.RAMP_TIME\").State(\"$set_ramptime\");"
    }
  } elseif {[regexp -- {\.LEVEL$} $address]} {
    if {[string is double $set_ontime] && (($set_ontime > 0) || ($set_ontime < 0))} {
      if {($state < 0.5)} { set set_ontime [expr {-$set_ontime}] }
      if {($set_ontime > 0)} { set set_ontime [concat "+" $set_ontime] }
      append cmd "dom.GetObject(\"$address\").State(dom.GetObject(\"$address\").Value()$set_ontime);"
      set level_set 1
    }
  }

  if {![info exists level_set]} {
    append cmd "dom.GetObject(\"$address\").State(\"$state\");"
  }
  
  if { $stop_cmd } {
    append cmd "\}" 
  }

  if { $verbose } { puts $cmd }
  
  set cmd [concat "if (dom.GetObject(\"$addr.WORKING\")) {" $cmd]
  append cmd "var ret = 1;}"
  
  array set values [rega_script $cmd]
  
  if { [info exists values(ret)] && ($values(ret) == 1) } {
    puts "OK"
    exit 0
  } else {
    puts "ERROR"
    exit 1
  }
}
