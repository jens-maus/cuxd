#!/bin/tclsh

set checkURL    "http://cuxd.de/cuxd-ccurm.php?$env(QUERY_STRING)"
set downloadURL "http://cuxd.de/cuxd-ccurm.php?$env(QUERY_STRING)"

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
  puts [ exec /usr/bin/wget -qO- --no-check-certificate $downloadURL ]
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
