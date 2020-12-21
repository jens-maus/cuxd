#!/bin/tclsh

set checkURL    "http://cuxd.de/cuxd-ccux86.php?$env(QUERY_STRING)"
set downloadURL "http://cuxd.de/cuxd-ccux86.php?$env(QUERY_STRING)"

catch {
  set input $env(QUERY_STRING)
  set pairs [split $input &]
  foreach pair $pairs {
    if {0 != [regexp "^(\[^=]*)=(.*)$" $pair dummy varname val]} {
      set $varname $val
    }
  }
}

if { [info exists cmd ] && $cmd == "download"} {
  puts -nonewline "Content-Type: text/html; charset=utf-8\r\n\r\n"
  puts -nonewline "<html><head><meta http-equiv='refresh' content='0; url=$downloadURL' /></head><body></body></html>"
} else {
  catch {
    set newversion [ exec /usr/bin/wget -qO- --no-check-certificate $checkURL ]
  }
  if { [info exists newversion] } {
    puts $newversion
  } else {
    puts "n/a"
  }
}
