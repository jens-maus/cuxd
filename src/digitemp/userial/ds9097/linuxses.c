/* Copyright (c) 2003, Erik Rigtorp <erkki@linux.nu>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of Erik Rigtorp nor the names of his contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * 
 * 
 * linuxses.c (0.20)
 * 
 *   * Cleaned up everything.
 * 
 *   * Added printing of error messages.
 * 
 *  -- Erik Rigtorp <erkki@linux.nu>  Thu, 05 Jun 2003 00:57:44 +0200
 */

#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <ownet.h>

/* local function prototypes */
SMALLINT owAcquire(int,char *);
void     owRelease(int);


int fd[MAX_PORTNUM]; /* a list of filedescriptors for serial ports */
struct termios term[MAX_PORTNUM]; /* Current termios settings */
struct termios term_orig[MAX_PORTNUM]; /* backup termios settings */


/* Attempt to acquire a 1-Wire net. Associate 'portnum' with the serial port
 * with name 'port_zstr'. Returns TRUE on success. */
SMALLINT owAcquire(int portnum, char *port_zstr)
{  
   /* Open the serial port */
   if ((fd[portnum] = open(port_zstr, O_RDWR)) == -1)
     {
	OWERROR(OWERROR_GET_SYSTEM_RESOURCE_FAILED);
	perror("owAcquire: failed to open device");
	return FALSE;
     }
   
   /* Get device settings */
   if(tcgetattr(fd[portnum], &term[portnum] ) < 0 )
     {
	OWERROR(OWERROR_SYSTEM_RESOURCE_INIT_FAILED);
	perror("owAcquire: failed to set attributes");
	close(fd[portnum]);
	return FALSE;
     }
   
   /* Save a backup */
   term_orig[portnum] = term[portnum];
   
   /* Reset all settings */
   term[portnum].c_iflag = 0;
   term[portnum].c_oflag = 0;
   term[portnum].c_lflag = 0;   
   term[portnum].c_cflag = 0;   

   /* 1 byte at a time, no timer */
   term[portnum].c_cc[VMIN] = 1;  
   term[portnum].c_cc[VTIME] = 0;
      
   /* 6 data bits, Receiver enabled, Hangup, Dont change "owner" */
   term[portnum].c_cflag |= CS6 | CREAD | HUPCL | CLOCAL;
   
   /* Set input and output speed to 115.2k */
   cfsetispeed(&term[portnum], B115200);
   cfsetospeed(&term[portnum], B115200);
   
   /* set the attributes */
   if(tcsetattr(fd[portnum], TCSANOW, &term[portnum]) < 0 )
     {
	OWERROR(OWERROR_SYSTEM_RESOURCE_INIT_FAILED);
	perror("owAcquire: failed to set attributes");	
	close(fd[portnum]);
	return FALSE;
     }
      
   /* Flush the input and output buffers */
   tcflush(fd[portnum], TCIOFLUSH);
   
   return TRUE;
}

/* Release port 'portnum' */
void owRelease(int portnum)
{
   /* Restore original settings */
   if(tcsetattr(fd[portnum], TCSANOW, &term_orig[portnum]) < 0 )
     {
	/* We failed doing that */
	OWERROR(OWERROR_SYSTEM_RESOURCE_INIT_FAILED);
	perror("owAcquire: failed to set attributes");
	close(fd[portnum]);
     }
   
   /* Close the port */
   if (close(fd[portnum]) < 0)
     {
	/* We failed closing the port */
	OWERROR(OWERROR_SYSTEM_RESOURCE_INIT_FAILED);
	perror("owAcquire: failed to close port");
     }

   /* we should return an error condition here but MAXIMS API is 
    * badly designed */
}


