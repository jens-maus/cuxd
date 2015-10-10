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
 * linuxlnk.c (0.21)
 * 
 *   * Fixed strong pullup return codes so that it doesn't make higer level
 *     code fail when owLevel is used. It returns the value it is called
 *     with.
 * 
 *  -- Brian Lane <bcl@brianlane.com>  2/3/2004
 *
 * linuxlnk.c (0.20)
 * 
 *   * Cleaned up this mess.
 * 
 *   * Added lots of comments.
 * 
 *  -- Erik Rigtorp <erkki@linux.nu>  Thu, 05 Jun 2003 15:07:08 +0200
 * 
 * linuxlnk.c (0.13)
 * 
 *   * Incorporated changes from Jari Kirma <kirma@cs.hut.fi>. These 
 *     changes improves performace several times. By using the UARTs 
 *     FIFO it sends several bits of data to the network and then reads 
 *     the result. My old code sent one bit and read one bit at a time.
 *     Thanks Jari!
 *
 *  -- Erik Rigtorp <erkki@linux.nu>  Thu, 05 Jun 2003 00:57:44 +0200
 * 
 * linuxlnk.c (0.10)
 *   
 *   * First working implementation, derived from work by Brian C. Lane.
 *
 *  -- Erik Rigtorp <erkki@linux.nu>  Unknown
 *   
 */

#include <unistd.h>
#include <sys/select.h>
#include <errno.h>
#include <termios.h>
#include <sys/time.h>
#include <time.h>

#ifdef LINUX
#include <linux/serial.h>
#endif

#include <ownet.h>

/* The UART_FIFO_SIZE defines the amount of bytes that are written before
 * reading the reply. Any positive value should work and 16 is probably low
 * enough to avoid losing bytes in even most extreme situations on all modern
 * UARTs, and values bigger than that will probably work on almost any
 * system... The value affects readout performance asymptotically, value of 1
 * should give equivalent performance with the old implementation. 
 *   -- Jari Kirma 
 *
 * Note: Each bit sent to the 1-wire net requeires 1 byte to be sent to 
 *       the uart.
 */
#define UART_FIFO_SIZE 160

#define TIMEOUT 5
/* exportable link-level functions */
SMALLINT owTouchReset(int);
SMALLINT owTouchBit(int,SMALLINT);
SMALLINT owTouchByte(int,SMALLINT);
SMALLINT owWriteByte(int,SMALLINT);
SMALLINT owReadByte(int);
SMALLINT owSpeed(int,SMALLINT);
SMALLINT owLevel(int,SMALLINT);
SMALLINT owProgramPulse(int);
void msDelay(int);
long msGettick(void);
SMALLINT hasPowerDelivery(int);
SMALLINT hasOverDrive(int);
SMALLINT hasProgramPulse(int);
SMALLINT owWriteBytePower(int,SMALLINT);
SMALLINT owReadBitPower(int, SMALLINT);

extern int fd[MAX_PORTNUM];

/* Reset all of the devices on the 1-Wire Net and return the result. Returns
 * TRUE if presense pulse(s) was detected and devices(s) reset, otherwise
 * the reset has failed and it returns FALSE. */
SMALLINT owTouchReset(int portnum)
{
   fd_set readset;
   struct timeval timeout_tv;
   
   int stat = 0;
   unsigned char wbuff;
   unsigned char result = 0;
   //extern int global_msec, global_msec_max;
   extern struct termios term[MAX_PORTNUM];

   /* Flush the input and output buffers */
   tcflush(fd[portnum], TCIOFLUSH);
   
   /* 8 data bits */
   term[portnum].c_cflag |= CS8; 
   
   /* Set input and output speed to 9.6k */
   cfsetispeed(&term[portnum], B9600);
   cfsetospeed(&term[portnum], B9600);
   
   if(tcsetattr(fd[portnum], TCSANOW, &term[portnum]) < 0 )
     {
	OWERROR(OWERROR_SYSTEM_RESOURCE_INIT_FAILED);
	perror("owTouchReset: Error with tcsetattr 1");
	close(fd[portnum]);
	return FALSE;
     }
   
   /* Send the reset pulse */
   wbuff = 0xF0;
   write(fd[portnum], &wbuff, 1);
   
   /* Set timeout */
   timeout_tv.tv_usec = 0;
   timeout_tv.tv_sec = TIMEOUT;

   /* Clear set */
   FD_ZERO(&readset);
   /* Add filedescriptor to set */
   FD_SET(fd[portnum], &readset);
   
   /* Read byte if it doesn't timeout first */
   if (select(fd[portnum]+1, &readset, NULL, NULL, &timeout_tv) > 0 )
     {
	/* Is there something to read? */
	if (FD_ISSET(fd[portnum], &readset))
	  {
	     /* Read in the result */
	     read(fd[portnum], &result, 1);
	     
	     if (result == 0)
	       /* Data line is a short to ground */
	       stat = FALSE;
	     
	     if (result != 0xF0)
	       {  
		  /* Got a response */
		  /* Here we should check for alarms and errors */
		  stat = TRUE;
	       } else {
		  /* No device responding */
		  OWERROR(OWERROR_NO_DEVICES_ON_NET);
		  stat = FALSE;
	       }
	  }
     } else {
	/* Timed out*/
	stat = FALSE;
     }
   
  
   /* Set input and output speed to 115.2k */
   cfsetispeed(&term[portnum], B115200);
   cfsetospeed(&term[portnum], B115200);
   
   /* set to 6 data bits */
   term[portnum].c_cflag |= CS6; 

   if (tcsetattr(fd[portnum], TCSANOW, &term[portnum] ) < 0 )
     {
	OWERROR(OWERROR_SYSTEM_RESOURCE_INIT_FAILED);
	perror("Reset: Error with tcsetattr 2");
	close(fd[portnum]);
	return FALSE;
     }

   return stat;
}




void owTouchBlock( int portnum, int timeout, int nbits, uchar *transfer_buf)
{
   fd_set readset;
   char *buf;
   struct timeval timeout_tv;

   unsigned char inch = 0;
   unsigned int i;
   unsigned int bit_counter = 0;
   unsigned int nretrieved_bits;
   unsigned int nread_bits;
   
   int nbits2;

   if (nbits == 0)
     return;

   /* Allocate 'nbits' number of bytes for buf */
   /* alloca() should be faster then malloc but ... */
   buf = alloca(nbits);
   if (buf == NULL) {
      fprintf(stderr, "owTouchBlock: Could not allocate %d bytes of memory.\n", nbits);
      return;
   }

   /* Flush the input and output buffers */
   tcflush(fd[portnum], TCIOFLUSH);
   
   /* Construct string of bytes representing bits to be sent */
   for (i = 0; i < nbits; i++)
     buf[i] = (transfer_buf[i/8] & (1 << (i & 0x7))) ? 0xFF : 0x00;
   
   /* send and receive blocks of UART_FIFO_SIZE or less */
   while (nbits != 0) {
      /* Make sure we don't send buffers bigger then UART_FIFO_SIZE */
      nbits2 = (nbits < UART_FIFO_SIZE) ? nbits : UART_FIFO_SIZE;
      nbits -= nbits2;
      
      /* write nbits2 bits of buffer to network */
      write(fd[portnum], buf, nbits2);
      
      /* read N bytes (bits) paired with above write */
      nretrieved_bits = 0;

      while (nretrieved_bits < nbits2) {
	 /* Initialize readset */
	 FD_ZERO(&readset);
	 FD_SET(fd[portnum], &readset);
	 
	 /* Initialize timeout */
	 timeout_tv.tv_usec = 0;
	 timeout_tv.tv_sec = timeout;
	 
	 /* Read bytes if it doesn't timeout first */
	 if (select(fd[portnum]+1, &readset, NULL, NULL, &timeout_tv) > 0)
	   {
	      /* Is there something to read? */
	      if (FD_ISSET(fd[portnum], &readset)) {
		 /* read it into buf */
		 nread_bits = read(fd[portnum], buf, nbits2 - nretrieved_bits);
		 
		 /* loop over the buffer and extract the least significant bits
		  * and put it in inch */
		 for (i = 0; i < nread_bits; i++) {
		    inch >>= 1;
		    /* mask bit 1 of buf, if bit is a '1' set bit 8 of inch,
		     * if bit is a '0' leave bit 8 of inch unset */
		    inch |= (buf[i] & 0x01) ? 0x80 : 0x00;
		    nretrieved_bits++;
		    bit_counter++;
		    
		    if ((bit_counter % 8) == 0) {
		       /* we have a full byte */
		       *transfer_buf = inch;
		       transfer_buf++;
		       inch = 0;
		    }
		 }
	      } else {
		 /* Wicked?, some signal or something */
		 printf("b0rked\n");
		 return;
	      }
	   } else {
	      /* Timed out */
	      return;
	   }
      }
      buf += nbits2;
   }
   
   if ((bit_counter % 8) != 0)
     /* this is not a full byte */
     *transfer_buf = inch;
}


int owTouchBits( int portnum, int timeout, int nbits, SMALLINT outch  )
{
  uchar r = outch;

  owTouchBlock(portnum, timeout, nbits, &r);

  return r;
}

//--------------------------------------------------------------------------
// Send 1 bit of communication to the 1-Wire Net and return the
// result 1 bit read from the 1-Wire Net.  The parameter 'sendbit'
// least significant bit is used and the least significant bit
// of the result is the return bit.
//
// 'portnum'    - number 0 to MAX_PORTNUM-1.  This number is provided to
//                indicate the symbolic port number.
// 'sendbit'    - the least significant bit is the bit to send
//
// Returns: 0:   0 bit read from sendbit
//          1:   1 bit read from sendbit
//
SMALLINT owTouchBit(int portnum, SMALLINT sbit)
{
   //unsigned char c = 0;
  unsigned char sendbit;
   unsigned char inbit = 0;
   fd_set readset;
   struct timeval tv;

   /* Flush the input and output buffers */
   tcflush( fd[portnum], TCIOFLUSH );
   
   /* Get the bit ready to be sent */
   sendbit = (sbit & 0x01) ? 0xFF : 0x00;
     
   /* Send the bits/get the bits */
    write( fd[portnum], &sendbit, 1 );		/* Send the bit			*/

	/* Get the incoming bit if there is one */
	
	/* Initialize readset */
	FD_ZERO(&readset);
	FD_SET(fd[portnum], &readset);
	
	/* Set timeout*/
	tv.tv_usec = 0;
	tv.tv_sec = 5;
	
	/* Read byte if it doesn't timeout first */
	if( select( fd[portnum]+1, &readset, NULL, NULL, &tv ) > 0 )
	  {
	     /* Is there something to read? */
	    if( FD_ISSET( fd[portnum], &readset ) ) {
	      read(fd[portnum], &inbit, 1);
		  inbit &= 0x01;
	    } else 
	       {
		  /* Wicked?, some signal or something */
		  printf("b0rked\n");
	       }
	     
	  } else {
	     return 0xFF;
	  }  
 
   //printf("TouchBit: sendbit: %02X, inbit: %02X\n", sendbit, inbit);
   return inbit;
}


/* Send 8 bits of communication to the 1-Wire Net and return the
 * result 8 bits read from the 1-Wire Net.  The parameter 'sendbyte'
 * least significant 8 bits are used and the least significant 8 bits
 * of the result is the return byte. */
SMALLINT owTouchByte(int portnum, SMALLINT sendbyte)
{
   return owTouchBits(portnum, TIMEOUT, 8, sendbyte);
}


/* Send 8 bits of communication to the 1-Wire Net and verify that the
 * 8 bits read from the 1-Wire Net is the same (write operation).
 * The parameter 'sendbyte' least significant 8 bits are used. 
 * Returns TRUE on sucess and FALSE if operation failed */
SMALLINT owWriteByte(int portnum, SMALLINT sendbyte)
{
   return (owTouchByte(portnum,sendbyte) == sendbyte) ? TRUE : FALSE;
}

/* Send 8 bits of read communication to the 1-Wire Net and and return the
 * result 8 bits read from the 1-Wire Net. */
SMALLINT owReadByte(int portnum)
{
   return owTouchByte(portnum,0xFF);
}

/* Set the 1-Wire net communication speed.
 * The passive adapter only supports normal speed. Returns MODE_NORMAL wich 
 * means that current speed is normal */
SMALLINT owSpeed(int portnum, SMALLINT new_speed)
{
   return MODE_NORMAL;
}


/* The passive adapter has only one line level, and that level depends on the 
 * serial port. */
SMALLINT owLevel(int portnum, SMALLINT new_level)
{
   return new_level;
}

/* No support for eeprom programming on the passive adapter */
SMALLINT owProgramPulse(int portnum)
{
   return FALSE;
}

/* Delay for at least 'len' ms */
void msDelay(int len)
{
   struct timespec s;              // Set aside memory space on the stack
   
   s.tv_sec = len / 1000;
   s.tv_nsec = (len - (s.tv_sec * 1000)) * 1000000;
   nanosleep(&s, NULL);
}

/* Get the current millisecond tick count.  Does not have to represent
 * an actual time, it just needs to be an incrementing timer. */
long msGettick(void)
{
   //struct timezone tmzone;
   //struct timeval  tmval;
   //long ms;
   
   //gettimeofday(&tmval,&tmzone);
   //ms = (tmval.tv_sec & 0xFFFF) * 1000 + tmval.tv_usec / 1000;
   return clock() / 1000;;
}

//--------------------------------------------------------------------------
// Send 8 bits of communication to the 1-Wire Net and verify that the
// 8 bits read from the 1-Wire Net is the same (write operation).  
// The parameter 'sendbyte' least significant 8 bits are used.  After the
// 8 bits are sent change the level of the 1-Wire net.
//
// 'portnum'  - number 0 to MAX_PORTNUM-1.  This number was provided to
//              OpenCOM to indicate the port number.
// 'sendbyte' - 8 bits to send (least significant byte)
//
// Returns:  TRUE: bytes written and echo was the same
//           FALSE: echo was not the same 
//
SMALLINT owWriteBytePower(int portnum, SMALLINT sendbyte)
{
   // replace if platform has better implementation (faster response)
   if (!hasPowerDelivery(portnum))
     return FALSE;
   
   if(owTouchByte(portnum,sendbyte) != sendbyte)
     return FALSE;
   
   if(owLevel(portnum,MODE_STRONG5) != MODE_STRONG5)
     return FALSE;
   
   return TRUE;
}

//--------------------------------------------------------------------------
// Send 1 bit of communication to the 1-Wire Net and verify that the
// response matches the 'applyPowerResponse' bit and apply power delivery
// to the 1-Wire net.  Note that some implementations may apply the power
// first and then turn it off if the response is incorrect.
//
// 'portnum'  - number 0 to MAX_PORTNUM-1.  This number was provided to
//              OpenCOM to indicate the port number.
// 'applyPowerResponse' - 1 bit response to check, if correct then start
//                        power delivery 
//
// Returns:  TRUE: bit written and response correct, strong pullup now on
//           FALSE: response incorrect
//
SMALLINT owReadBitPower(int portnum, SMALLINT applyPowerResponse)
{
   // replace if platform has better implementation (faster response)
   if (!hasPowerDelivery(portnum))
     return FALSE;
   
   if(owTouchBit(portnum, 0x01) != applyPowerResponse)
     return FALSE;
   
   if(owLevel(portnum, MODE_STRONG5) != MODE_STRONG5)
     return FALSE;
   
   return TRUE;
}

/* I'm not realy sure about this but it works for digitemp. If it
 * means the adapter has a separate power line this should be false. */
SMALLINT hasPowerDelivery(int portnum)
{
   return TRUE;
}

/* Passive adapter dosen't suport OverDrive due to limitations in 
 * the way data is transfered. */
SMALLINT hasOverDrive(int portnum)
{
   return FALSE;
}

/* Passive adapter doesn't support eeprom programming */
SMALLINT hasProgramPulse(int portnum)
{
   return FALSE;
}
