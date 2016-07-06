#!/bin/bash

# function that generate a CCU specific
# install package
generate_pkg()
{
  # generate temporary directory
  mkdir -p tmp
  rm -rf tmp/*

  # generate ccu1 package
  cp -a common/* tmp/
  cp -a ${1}/cuxd tmp/
  cd tmp
  tar --owner=root --group=root --exclude=.DS_Store -czvf ../cuxd_$(cat ../VERSION)_${1}.tar.gz *
  cd ..
  rm -rf tmp
}

# generate packages now. If specified on 
# command-line generate this specific package
# only. If not specified at all, generate all
# packages.
if [ -n "$1" ]; then
  generate_pkg $1
else
  generate_pkg ccu1
  generate_pkg ccu2
  generate_pkg ccurm
fi
