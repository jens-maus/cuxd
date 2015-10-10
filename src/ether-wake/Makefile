#******************************************************************
#** File: Makefile
#** Description: the makefile for ether-wake project
#******************************************************************

CROSS = /usr/bin/

CFLAGS = -O3 -Wall -pedantic -std=gnu99
CC = $(CROSS)gcc
STRIP = $(CROSS)strip --strip-unneeded

all: ether-wake

ether-wake: ether-wake.c
	$(CC) $(CFLAGS) -o ether-wake ether-wake.c
	$(STRIP) ether-wake

clean:
	rm -f ether-wake
