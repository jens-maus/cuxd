#!/bin/tclsh

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
  if { [info exists version ] } {
    puts "<meta http-equiv='refresh' content='0; url=https://github.com/jens-maus/cuxd/releases/tag/$version' />"
  } else {
    puts "<meta http-equiv='refresh' content='0; url=https://github.com/jens-maus/cuxd/releases' />"
  }
} else {
  set version [ exec /usr/bin/wget -qO- --no-check-certificate https://raw.githubusercontent.com/jens-maus/cuxd/master/VERSION ]
  puts $version
}
