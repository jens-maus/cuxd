#******************************************************************
#** File: Makefile
#** Description: the makefile for logfilter project
#******************************************************************

CROSS = /usr/bin/

CFLAGS = -O3 -Wall -pedantic -std=gnu99
CC = $(CROSS)gcc
STRIP = $(CROSS)strip --strip-unneeded

all: logfilter

logfilter: logfilter.c Makefile
	$(CC) $(CFLAGS) -o logfilter logfilter.c
	$(STRIP) logfilter

clean:
	rm -f logfilter
