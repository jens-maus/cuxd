/* -----------------------------------------------------------------------
   DigiTemp

   Copyright 1996-2009 by Brian C. Lane <bcl@brianlane.com>
   All Rights Reserved

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the Free
   Software Foundation; either version 2 of the License, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
   more details.

   You should have received a copy of the GNU General Public License along
   with this program; if not, write to the Free Software Foundation, Inc.,
   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA
   ----------------------------------------------------------------------- */
#define BANNER_1     "DigiTemp v3.6.0 Copyright 1996-2009 by Brian C. Lane\n"
#define BANNER_2     "GNU Public License v2.0 - http://www.digitemp.com\n"
#define BANNER_3     "Compiled for %s\n\n"

#define OPT_INIT     0x0001
#define OPT_VERBOSE  0x0002
#define OPT_SINGLE   0x0004
#define OPT_ALL      0x0008
#define OPT_QUIET    0x0010
#define OPT_WALK     0x0020
#define OPT_DS2438   0x0040
#define OPT_SORT     0x0080
#define OPT_TEST     0x0100


/* Family codes for supported devices */
#define DS1820_FAMILY	0x10
#define DS1822_FAMILY	0x22
#define DS18B20_FAMILY	0x28
#define DS28EA00_FAMILY 0x42
#define DS1923_FAMILY   0x41
#define DS2406_FAMILY   0x12
#define DS2422_FAMILY	0x1C
#define DS2423_FAMILY	0x1D
#define DS2438_FAMILY   0x26
// and non-supported yet, but coming soon
#define DS2408_FAMILY	0x29
#define DS2413_FAMILY	0x3A


/* Coupler related definitions */
#define SWITCH_FAMILY      0x1F
#define MAXDEVICES         15
#define ALL_LINES_OFF      0
#define DIRECT_MAIN_ON     1
#define AUXILARY_ON        2
#define STATUS_RW          3

/* Exit codes */
#define EXIT_OK   0      /* No errors                          */
#define EXIT_ERR  1      /* Couldnt read temp                  */
#define EXIT_NORC 2      /* Couldn't read the rc file          */
#define EXIT_HELP 3      /* Exit and showing help              */
#define EXIT_NOPERM 4    /* No permission to use serial port   */
#define EXIT_LOCKED 5	 /* Serial port is locked              */
#define EXIT_DEVERR 6    /* Error getting serial device        */
#define EXIT_NOPORT 7    /* Port device file doesn't exists    */

/* Number of tries to read a sensor before giving up */
#define MAX_READ_TRIES	3

struct _roms {
        unsigned char   *roms;                  /* Array of 8 bytes     */
        int             max;                    /* Maximum number       */
};

struct _coupler {
  unsigned char SN[8];			/* Serial # of this Coupler */
  unsigned int num_main;		/* # of devices on main */
  unsigned int num_aux;			/* # of devices on aux */
  
  unsigned char *main;			/* Array of 8 byte serial nums */
  unsigned char *aux;			/* Array of 8 byte serial nums */
  
  struct _coupler *next;
};

/* Prototypes */
void usage();
void free_coupler();
float c2f( float temp );
int build_tf( char *time_format, char *format, int sensor, 
              float temp_c, int humidity, unsigned char *sn );
int build_cf( char *time_format, char *format, int sensor, int page,
              unsigned long count, unsigned char *sn );
int log_string( char *line );
int log_temp( int sensor, float temp_c, unsigned char *sn );
int log_counter( int sensor, int page, unsigned long counter, unsigned char *sn );
int log_humidity( int sensor, double temp_c, int humidity, unsigned char *sn );
int cmpSN( unsigned char *sn1, unsigned char *sn2, int branch );
void show_scratchpad( unsigned char *scratchpad, int sensor_family );
int read_temperature( int sensor_family, int sensor );
int read_counter( int sensor_family, int sensor );
int read_ds2438( int sensor_family, int sensor );
int read_humidity( int sensor_family, int sensor );
int read_device( struct _roms *sensor_list, int sensor );
int read_all( struct _roms *sensor_list );
int read_rcfile( char *fname, struct _roms *sensor_list );
int write_rcfile( char *fname, struct _roms *sensor_list );
void printSN( unsigned char *TempSN, int crlf );
int Walk1Wire();
int sercmp( unsigned char *sn1, unsigned char *sn2 );
int Init1WireLan( struct _roms *sensor_list );
int read_pio_ds28ea00( int sensor_family, int sensor );

/* From ds2438.c */
int get_ibl_type(int portnum, unsigned char page, int offset);

/* Local Variables: */
/* mode: C */
/* compile-command: "cd ..; make -k" */
/* End: */
