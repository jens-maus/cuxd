#
# Makefile for DigiTemp
#
# Copyright 1996-2005 by Brian C. Lane <bcl@brianlane.com>
# See COPYING for GNU Public License
#
# Please note that this Makefile *needs* GNU make. BSD make won't do.
#
# To disable Lockfile support on Linux, chenage LOCK to no
#

VERSION = 3.6.0

CC	= gcc
CFLAGS	= -I./src -I./userial -O2 -Wall # -g

OBJS		=	src/digitemp.o src/device_name.o src/ds2438.o
HDRS		= 	src/digitemp.h src/device_name.h

# Common userial header/source
HDRS		+=	userial/ownet.h userial/owproto.h userial/ad26.h \
			src/device_name.h src/digitemp.h
OBJS		+=	userial/crcutil.o userial/ioutil.o userial/swt1f.o \
			userial/owerr.o userial/cnt1d.o userial/ad26.o

# DS9097 passive adapter support source
DS9097OBJS	=	userial/ds9097/ownet.o userial/ds9097/linuxlnk.o \
			userial/ds9097/linuxses.o userial/ds9097/owtran.o \
                        src/ds9097.o

# DS9097-U adapter support source
DS9097UOBJS	=	userial/ds9097u/ds2480ut.o userial/ds9097u/ownetu.o \
			userial/ds9097u/owllu.o userial/ds9097u/owsesu.o \
			userial/ds9097u/owtrnu.o userial/ds9097u/linuxlnk.o \
                        src/ds9097u.o

# DS2490 adapter support
DS2490OBJS	=	userial/ds2490/ownet.o userial/ds2490/owtran.o \
			userial/ds2490/usblnk.o userial/ds2490/usbses.o \
			src/ds2490.o

# -----------------------------------------------------------------------
# Sort out what operating system is being run and modify CFLAGS and LIBS
#
# If you add a new OSTYPE here please email it to me so that I can add
# it to the distribution in the next release
# -----------------------------------------------------------------------
SYSTYPE := $(shell uname -s)

ifeq ($(SYSTYPE), Linux)
  CFLAGS += -DLINUX

  # Set LOCK to yes for serial port locking support
  LOCK = no

endif

ifneq (, $(findstring CYGWIN,$(SYSTYPE)))
  CFLAGS += -DCYGWIN
  LIBS   += -static -static-libgcc
endif

ifeq ($(SYSTYPE), SunOS)
  CFLAGS += -DSOLARIS
  LIBS   += -lposix4
endif

ifeq ($(SYSTYPE), FreeBSD)
  CFLAGS += -DFREEBSD
endif

ifeq ($(SYSTYPE), OpenBSD)
  CFLAGS += -DOPENBSD
endif

# Untested, but should work.
ifeq ($(SYSTYPE), NetBSD)
  CFLAGS += -DNETBSD
endif

ifeq ($(SYSTYPE), Darwin)
  CFLAGS += -DDARWIN
endif

ifeq ($(SYSTYPE), AIX)
  CFLAGS += -DAIX
endif

ifeq ($(LOCK), yes)
  CFLAGS += -DLOCKDEV
  LIBS   += -llockdev
endif


# USB specific flags
ds2490:  CFLAGS += -DOWUSB
ds2490:  LIBS   += -lusb


help:
	@echo "  SYSTYPE = $(SYSTYPE)"
	@echo "  CFLAGS = $(CFLAGS)"
	@echo "  LIBS   = $(LIBS)"
	@echo ""
	@echo "Pick one of the following targets:"
	@echo -e "\tmake ds9097\t- Build version for DS9097 (passive)"
	@echo -e "\tmake ds9097u\t- Build version for DS9097U"
	@echo -e "\tmake ds2490\t- Build version for DS2490 (USB) (edit Makefile) (BROKEN)"
	@echo " "
	@echo ""
	@echo "Please note: You must use GNU make to compile digitemp"
	@echo ""

all:		help


# Build the Linux executable
ds9097:		$(OBJS) $(HDRS) $(ONEWIREOBJS) $(ONEWIREHDRS) $(DS9097OBJS)
		$(CC) $(OBJS) $(ONEWIREOBJS) $(DS9097OBJS) -o digitemp_DS9097 $(LIBS)

ds9097u:	$(OBJS) $(HDRS) $(ONEWIREOBJS) $(ONEWIREHDRS) $(DS9097UOBJS)
		$(CC) $(OBJS) $(ONEWIREOBJS) $(DS9097UOBJS) -o digitemp_DS9097U $(LIBS)

ds2490:		$(OBJS) $(HDRS) $(ONEWIREOBJS) $(ONEWIREHDRS) $(DS2490OBJS)
		$(CC) $(OBJS) $(ONEWIREOBJS) $(DS2490OBJS) -o digitemp_DS2490 $(LIBS)


# Clean up the object files and the sub-directory for distributions
clean:
		rm -f *~ src/*~ userial/*~ userial/ds9097/*~ userial/ds9097u/*~ userial/ds2490/*~
		rm -f $(OBJS) $(ONEWIREOBJS) $(DS9097OBJS) $(DS9097UOBJS) $(DS2490OBJS)
		rm -f core *.asc 
		rm -f perl/*~ rrdb/*~ .digitemprc digitemp-$(VERSION)-1.spec
		rm -rf digitemp-$(VERSION)

# Sign the binaries using gpg (www.gnupg.org)
# My key is available from www.brianlane.com
sign:
		gpg -ba digitemp_DS*
		echo

# Install digitemp into /usr/local/bin
install:	digitemp
		install -b -o root -g bin digitemp /usr/local/bin

# Build the archive of everything
archive:	clean
		cd .. && tar --exclude *.o --exclude .svn -cvzf digitemp-$(VERSION).tar.gz digitemp-$(VERSION)/* 

# Build the source distribution
source:		archive

dist:		ds9097 ds9097u ds2490 sign archive

dist_ds9097:	ds9097 sign archive
		cd .. && mv digitemp-$(VERSION).tar.gz digitemp-$(VERSION)-ds9097.tar.gz

dist_ds9097u:	ds9097u sign archive
		cd .. && mv digitemp-$(VERSION).tar.gz digitemp-$(VERSION)-ds9097u.tar.gz

dist_ds2490:	ds2490 sign archive
		cd .. && mv digitemp-$(VERSION).tar.gz digitemp-$(VERSION)-ds2490.tar.gz
