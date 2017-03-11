#!/bin/tclsh
# brief
# Schaltet nach vorgegebener Zeit einen Kanal Ein oder Aus oder ändert einen Wert
# =============================================================
# von MaxWrestler 2012
# Update 2017 - Uwe Langhammer
#
# Aufruf zB.
# /usr/local/addons/cuxd/extra/timer.tcl BidCos-RF.IEQ0504341:1.STATE 1 10
# für Aktor IEQ0504341 Einschalten nach 10 Sekunden
# oder
# /usr/local/addons/cuxd/extra/timer.tcl BidCos-RF.IEQ1234567:1.ALL_LEDS 2863311530 60
# für alle LED´s der Statusanzeige nach 60 Sekunden auf grün.

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

set querystate [getopt argv -s]
set verbose [getopt argv -v]
set ms [getopt argv -ms]

if { $argc < 1 } {
  puts "USAGE: timer.tcl \[-v\] \[-s\] \[-ms\] <interface.address:ch.DP> \[<STATE>\] \[<WAIT_TIME>\] \[<COMPARE>\] \[<ON_TIME>\] \[<RAMP_TIME>\]"
  exit 1
}

set address [lindex $argv 0]
set state [lindex $argv 1]
set wait_in_seconds [lindex $argv 2]
set check_value [lindex $argv 3]
set set_ontime [lindex $argv 4]
set set_ramptime [lindex $argv 5]

if { $querystate } {
  set querystate "State()"
} else {
  set querystate "Value()"
}

if { $argc < 2 } {

  append cmd "var ret = dom.GetObject(\"$address\").$querystate;"
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

  if {[string is double $wait_in_seconds] && ($wait_in_seconds > 0)} {
    if { $ms } { set wait_in_seconds [expr $wait_in_seconds / 1000.0] }
    puts "WAIT ${wait_in_seconds}s"
    set wait_in_1000s [expr $wait_in_seconds * 1000]
    after [expr int($wait_in_1000s)]
  }

  if {([string length $check_value] > 0) && ([expr int($check_value)] != 0)} {
    set cmd "if (dom.GetObject(\"$address\").$querystate != $state)\{";
  }

  if {[string is double $set_ontime] && ($set_ontime > 0)} {
    if { $ms } { set set_ontime [expr $set_ontime / 1000.0] }
    puts "ON_TIME ${set_ontime}s"
    regsub -- {[^\.]+$} $address "ON_TIME" ontime
    append cmd "dom.GetObject(\"$ontime\").State(\"$set_ontime\");"
  }

  if {[string is double $set_ramptime] && ($set_ramptime > 0)} {
    if { $ms } { set set_ramptime [expr $set_ramptime / 1000.0] }
    puts "RAMP_TIME ${set_ramptime}s"
    regsub -- {[^\.]+$} $address "RAMP_TIME" ramptime
    append cmd "dom.GetObject(\"$ramptime\").State(\"$set_ramptime\");"
  }
 
  append cmd "dom.GetObject(\"$address\").State(\"$state\");"

  if {([string length $check_value] > 0) && ([expr int($check_value)] != 0)} {
    append cmd "\}"
  }

  if { $verbose } { puts $cmd }
  
  append cmd "var ret = 1;"

  array set values [rega_script $cmd]

  if { [info exists values(ret)] && ($values(ret) == 1) } {
    puts "OK"
    exit 0
  } else {
    puts "ERROR"
    exit 1
  }
}
