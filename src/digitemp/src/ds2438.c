/* -----------------------------------------------------------------------
   DS2438 routines
   Copyright 2007 by Brian C. Lane
   All Rights Reserved
   Licensed under GPL v2
   ----------------------------------------------------------------------- */
#include <stdio.h>
#include "ownet.h"
#include "ad26.h"

extern int   owBlock(int,int,uchar *,int);
extern void  setcrc8(int,uchar);
extern uchar docrc8(int,uchar);
extern int   owReadByte(int);
extern int   owWriteByte(int,int);
extern void  output_status(int, char *);
extern void  msDelay(int);
extern void  owSerialNum(int,uchar *,int);
extern int   owAccess(int);


/* -----------------------------------------------------------------------
   ----------------------------------------------------------------------- */
int get_ibl_type(int portnum, uchar page, int offset)
{
   uchar send_block[50];
   int send_cnt=0;
   int i;
   ushort lastcrc8=255;

   /* 01/08/2004 [bcl] DigiTemp does this before calling the function
    * owSerialNum(portnum,SNum,FALSE);
    */

   // Recall the Status/Configuration page
   // Recall command
   send_block[send_cnt++] = 0xB8;

   // Page to Recall
   send_block[send_cnt++] = page;

   if(!owBlock(portnum,FALSE,send_block,send_cnt))
      return FALSE;

   send_cnt = 0;

   if(owAccess(portnum))
   {
      // Read the Status/Configuration byte
      // Read scratchpad command
      send_block[send_cnt++] = 0xBE;

      // Page for the Status/Configuration byte
      send_block[send_cnt++] = page;

      for(i=0;i<9;i++)
         send_block[send_cnt++] = 0xFF;

      if(owBlock(portnum,FALSE,send_block,send_cnt))
      {
         setcrc8(portnum,0);

         for(i=2;i<send_cnt;i++)
            lastcrc8 = docrc8(portnum,send_block[i]);

         if(lastcrc8 != 0x00)
            return FALSE;
      } else {
         return FALSE;
      }

      // Return the requested byte
      return send_block[2+offset];
   }//Access

   return -1;
}
