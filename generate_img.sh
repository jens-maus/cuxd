#!/bin/sh

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
  tar --owner=root --group=root -czvf ../cuxd_$(cat ../VERSION)_${1}.tar.gz *
  cd ..
  rm -rf tmp
}

# generate all packages now.
generate_pkg ccu1
generate_pkg ccu2
generate_pkg ccurm
