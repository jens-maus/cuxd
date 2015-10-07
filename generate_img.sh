#!/bin/sh
mkdir -p tmp
cp -a common/* tmp/
cp -a ccu1 tmp/
cp -a ccu2 tmp/
cp -a ccupi tmp/
cd tmp
tar -czvf ../cuxd_1.4.tar.gz *
cd ..
rm -rf tmp
