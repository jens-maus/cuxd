//---------------------------------------------------------------------------
// Copyright (C) 2000 Dallas Semiconductor Corporation, All Rights Reserved.
// 
// Permission is hereby granted, free of charge, to any person obtaining a 
// copy of this software and associated documentation files (the "Software"), 
// to deal in the Software without restriction, including without limitation 
// the rights to use, copy, modify, merge, publish, distribute, sublicense, 
// and/or sell copies of the Software, and to permit persons to whom the 
// Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included 
// in all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
// MERCHANTABILITY,  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
// IN NO EVENT SHALL DALLAS SEMICONDUCTOR BE LIABLE FOR ANY CLAIM, DAMAGES 
// OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR 
// OTHER DEALINGS IN THE SOFTWARE.
// 
// Except as contained in this notice, the name of Dallas Semiconductor 
// shall not be used except as stated in the Dallas Semiconductor 
// Branding Policy. 
//--------------------------------------------------------------------------
//
//  ad26.c - Reads the voltage on the 1-Wire
//  version 1.00
//

// Include Files
#include <stdio.h>
#include "ownet.h"
#include "ad26.h"
#include "owproto.h"


extern int   owBlock(int,int,uchar *,int);
extern void  setcrc8(int,uchar);
extern uchar docrc8(int,uchar);
extern int   owReadByte(int);
extern int   owWriteByte(int,int);
extern void  output_status(int, char *);
extern void  msDelay(int);
extern void  owSerialNum(int,uchar *,int);
extern int   owAccess(int);


int Volt_AD(int portnum, int vdd);
float Volt_Reading(int portnum, int vdd, int *cad);
double Get_Temperature(int portnum);

int Volt_AD(int portnum, int vdd)
{
   uchar send_block[50];
   uchar test;
   int send_cnt=0;
   int i;
   ushort lastcrc8=255;
   int busybyte; 

   /* 01/08/2004 [bcl] DigiTemp does this before calling the function
    * owSerialNum(portnum,SNum,FALSE);
    */

   // Recall the Status/Configuration page
   // Recall command
   send_block[send_cnt++] = 0xB8;

   // Page to Recall
   send_block[send_cnt++] = 0x00;

   if(!owBlock(portnum,FALSE,send_block,send_cnt))
      return FALSE;

   send_cnt = 0;

   if(owAccess(portnum))
   {
      // Read the Status/Configuration byte
      // Read scratchpad command
      send_block[send_cnt++] = 0xBE;

      // Page for the Status/Configuration byte
      send_block[send_cnt++] = 0x00;

      for(i=0;i<9;i++)
         send_block[send_cnt++] = 0xFF;

      if(owBlock(portnum,FALSE,send_block,send_cnt))
      {
         setcrc8(portnum,0);

         for(i=2;i<send_cnt;i++)
            lastcrc8 = docrc8(portnum,send_block[i]);

         if(lastcrc8 != 0x00)
            return FALSE;
      }//Block
      else
         return FALSE;

      /*
       * Avoid writing to status if needed --ro
       */

      test = send_block[2] & 0x08;
      if(((test == 0x08) && vdd) || ((test == 0x00) && !(vdd)))
         return TRUE;
   }//Access

   if(owAccess(portnum))
   {
      send_cnt = 0;
      // Write the Status/Configuration byte
      // Write scratchpad command
      send_block[send_cnt++] = 0x4E;

      // Write page
      send_block[send_cnt++] = 0x00;

      if(vdd)
         send_block[send_cnt++] = send_block[2] | 0x08;
      else
         send_block[send_cnt++] = send_block[2] & 0xF7;

      for(i=0;i<7;i++)
         send_block[send_cnt++] = send_block[i+4];

      /*
       * Turn on CAD sampling and turn off CA and EE functions 
       * for our use for less glitches
       */

      send_block[2] |= 0x1;
      send_block[2] &= ~0x4;
      send_block[2] &= ~0x2;

      if(owBlock(portnum,FALSE,send_block,send_cnt))
      {
         send_cnt = 0;

         if(owAccess(portnum))
         {
            // Copy the Status/Configuration byte
            // Copy scratchpad command
            send_block[send_cnt++] = 0x48;

            // Copy page
            send_block[send_cnt++] = 0x00;

            if(owBlock(portnum,FALSE,send_block,send_cnt))
            {
               busybyte = owReadByte(portnum);
         
               while(busybyte == 0)
                  busybyte = owReadByte(portnum);

               return TRUE;
            }//Block
         }//Access
      }//Block

   }//Access

   return FALSE;
}
      

float Volt_Reading(int portnum, int vdd, int *cad)
{
   uchar send_block[50];
   int send_cnt=0;
   int i;
   int busybyte; 
   ushort lastcrc8=255;
   ushort volts;
   float ret=-1.0;
   int done = TRUE;
   uchar c;

   do
   {
      if(Volt_AD(portnum,vdd))
      {

         if(owAccess(portnum))
         {
	   /*  Convert V */
            if(!owWriteByte(portnum,0xB4))
            {
/*               output_status(LV_ALWAYS,(char *)"DIDN'T WRITE CORRECTLY\n"); */
               return ret;
            }

            busybyte = owReadByte(portnum);
         
            while(busybyte == 0)
               busybyte = owReadByte(portnum);
         }

         if(owAccess(portnum))
         {
            // Recall the Status/Configuration page
            // Recall command
            send_block[send_cnt++] = 0xB8;

            // Page to Recall
            send_block[send_cnt++] = 0x00;

            if(!owBlock(portnum,FALSE,send_block,send_cnt))
               return ret;
         }

         send_cnt = 0;

         if(owAccess(portnum))
         {
            // Read the Status/Configuration byte
            // Read scratchpad command
            send_block[send_cnt++] = 0xBE;

            // Page for the Status/Configuration byte
            send_block[send_cnt++] = 0x00;

            for(i=0;i<9;i++)
               send_block[send_cnt++] = 0xFF;

            if(owBlock(portnum,FALSE,send_block,send_cnt))
            {
               setcrc8(portnum,0);

               for(i=2;i<send_cnt;i++)
                  lastcrc8 = docrc8(portnum,send_block[i]);

               if(lastcrc8 != 0x00)
                  return ret;

            }
            else
               return ret;    
      
            if((!vdd) && ((send_block[2] & 0x08) == 0x08))
	      continue;
            else
               done = TRUE;

            volts = (send_block[6] << 8) | send_block[5];
            ret = (float) volts/100;

	    if(cad) {

	      /* Get Current reading as well */
	      c = send_block[8] & 0x3;

	      *cad =  (c << 8) | send_block[7];
	      if(send_block[8] & 0x4) *cad =  - *cad;

	      //	      printf("CAD=%d\n", *cad);
	    }
         }//Access
      }
   } while(!done);

   return ret;

}

double Get_Temperature(int portnum)
{
   double ret=-1.0;
   uchar send_block[50];
   int send_cnt=0;
   int i;
   ushort lastcrc8=255;

   /* 01/08/2004 [bcl] DigiTemp does this before calling the function
    * owSerialNum(portnum,SNum,FALSE);
    */

   if(owAccess(portnum))
      // Convert Temperature command
      owWriteByte(portnum,0x44);

   msDelay(10);

   if(owAccess(portnum))
   {
      // Recall the Status/Configuration page
      // Recall command
      send_block[send_cnt++] = 0xB8;

      // Page to Recall
      send_block[send_cnt++] = 0x00;

      if(!owBlock(portnum,FALSE,send_block,send_cnt))
         return FALSE;

      send_cnt = 0;
   }

   if(owAccess(portnum))
   {
      // Read the Status/Configuration byte
      // Read scratchpad command
      send_block[send_cnt++] = 0xBE;

      // Page for the Status/Configuration byte
      send_block[send_cnt++] = 0x00;

      for(i=0;i<9;i++)
         send_block[send_cnt++] = 0xFF;

      if(owBlock(portnum,FALSE,send_block,send_cnt))
      {
         setcrc8(portnum,0);

         for(i=2;i<send_cnt;i++)
            lastcrc8 = docrc8(portnum,send_block[i]);

         if(lastcrc8 != 0x00)
            return ret;

      }
      else
         return ret;    
      
/* Doesn't handle negative properly
      ret = (((send_block[4] << 8) | send_block[3]) >> 3) * 0.03125;
*/
     ret = ((((send_block[4] & 0x7F) << 8) | send_block[3]) >> 3);

     if( send_block[4] & 0x80 )
     {
       /* Negative, take 2's complement and make it negative */
       ret = -1 * (0x1000 - ret);
     }
     ret =  ret * 0.03125;
   }//Access

   return ret;
}

/* ---- DS2406 PIO input by Tomasz R. Surmacz (tsurmacz@ict.pwr.wroc.pl) ---- */
int PIO_Reading(int portnum, int pionum /* TS ignored so far */ )
{
   uchar send_block[50];
   int send_cnt=0;
   int i;
   short unsigned int infoByte, pioByte;
   ushort lastcrc16 = 0;
   int ret=0;
   int done = 3; // max tries if CRC error

   while (done-->0)
   {
         if(owAccess(portnum))
         {
            // Recall the Status/Configuration page
            // Recall command
            send_block[send_cnt++] = 0xF5; // (b8) Channel access

            // Channel Control Byte 1 
            send_block[send_cnt++] = 0xCD; // ALR IM tog ic, CHS1 CHS0 crc1 CRC0
		    // CHS1 CHS0 = 01 = Channels A and B
		    // ic: 0: asynchronous sampling, 1: synchronous
		    // tog IM: 00: Write all bits to channel, 01: read all bits
		    // ALR = 1 ==> reset activity latch
		    // crc1crc0: 0: no crc, 1: after every byte, 2: every 8B, 3: 32B

            // Channel Control Byte 2 - reserved
            send_block[send_cnt++] = 0x00; // reserved

            if(!owBlock(portnum,FALSE,send_block,send_cnt))
               return ret;

	        // receive Channel Info Byte
            infoByte = (send_block[send_cnt++] = owReadByte(portnum)) & 0xff;
		    // bit7: supply indication Vcc (0 = no supply)
		    // bit6: no. of channels, 0 = channel A only
		    // bit5: PIO-B activity latch
		    // bit4: PIO-A activity latch
		    // bit3: PIO-B sensed level
		    // bit2: PIO-A sensed leve
		    // bit1: PIO-B Channel Flip-Flop Q
		    // bit0: PIO-A Channel Flip-Flop Q

	        // Receive PIO Byte(s) until Master TX Reset
            pioByte = send_block[send_cnt++] = owReadByte(portnum);

	        // read and check CRC
            setcrc16(portnum,0);
            for(i=0;i<send_cnt;i++) {
		        lastcrc16 = docrc16(portnum,send_block[i]);
		        // DEBUG:
		        // printf("  crc[%i]: %02x -> %04x (~%04x)\n", i, send_block[i], lastcrc16, lastcrc16 ^0xffff);
		    }

	        lastcrc16 ^= owReadByte(portnum);		// CRC16, lsb
	        lastcrc16 ^= (owReadByte(portnum) << 8);	// CRC16, msb
	        lastcrc16 ^= 0xffff;

	        // DEBUG:
	        // printf("  crc: %04x (~%04x)\n", lastcrc16, lastcrc16 ^0xffff);

		    if(lastcrc16 != 0x0000) {
			    printf("DS2406 crc error\n");
			    continue;
		    }

	        // reset bus and end reading PIO status
	        owTouchReset(portnum);
	        return ret=(infoByte <<8) + pioByte;

	        // returns sampled data in LSB (bits 7-0), status byte in bits 15-8
	        // when reading both ports, (sampled bits are b3a3b2a2b1a1b0a0 or: 
		    // LSB should be 0x00 for A=0, B=0
		    // 0xAA for A=0, B=1 
		    // 0x55 for A=1, B=0
		    // 0xFF for A=1, B=1
	        // status byte: 0x43 == 2-pio device, parasite powered, A=B=0
		    // 0x03 == 1-pio device, parasite powered
		    // 0xC3, 0x83 - Vdd-powered, 2 or 1 pio device
        } // Access
	    // printf("  DS2406 access error\n");
    } // while(done)

    return -1;
}

