#******************************************************************
#** File: Makefile
#** Description: the makefile for update_addon project
#******************************************************************

CROSS = /usr/bin/

CFLAGS = -O3 -Wall -pedantic -std=gnu99
CC = $(CROSS)gcc
STRIP = $(CROSS)strip --strip-unneeded

all: update_addon

update_addon: update_addon.c
	$(CC) $(CFLAGS) -o update_addon update_addon.c
	$(STRIP) update_addon 

clean:
	rm -f update_addon
