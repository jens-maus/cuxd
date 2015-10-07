#!/bin/sh
#
# (C) '2013 by Uwe Langhammer
# export spool directory to external ftp host
#
# usage: export_ftp.sh user:pass 192.168.12.3 import/ccu /tmp/export
#
export LD_LIBRARY_PATH=/usr/local/addons/cuxd
CURL=/usr/local/addons/cuxd/curl
FTP_USERPASS=$1
FTP_HOST=$2
FTP_DIR=$3
EXPORT_DIR=$4
cd $EXPORT_DIR
for file in *
do
  if [ -f $file ]
  then
    $CURL -s -u ${FTP_USERPASS} -T $file ftp://${FTP_HOST}/${FTP_DIR}/${file}.part
    if [ $? -eq 0 ]
    then
      $CURL -s -u ${FTP_USERPASS} ftp://$FTP_HOST -Q "RNFR ${FTP_DIR}/${file}.part" -Q "RNTO ${FTP_DIR}/${file}" >/dev/null
      if [ $? -eq 0 ]
      then
        rm -f $file
      fi
    fi
  fi
done

