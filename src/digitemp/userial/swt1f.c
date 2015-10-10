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
//  swt1f.c - Does commands on the DS2409 device
//
//  version 2.00-digitemp
//    Fixed problem of not finding more than 1 device on main branch by
//    changing owBranchNext to use Smart Main instead of Main On command
//
//  version 2.00
//  - future version will have branch of a branch searches
//

// Include files
#include <stdio.h>
#include "ownet.h"

// external One Wire functions from nework layer
extern SMALLINT owAccess(int);
extern SMALLINT owFirst(int,SMALLINT,SMALLINT);
extern SMALLINT owNext(int,SMALLINT,SMALLINT);
extern void     owSerialNum(int,uchar *,SMALLINT);

// external One Wire functions from transaction layer
extern SMALLINT owBlock(int,SMALLINT,uchar *,SMALLINT);

// Local subroutines
int SetSwitch1F(int,uchar *,int,int,uchar *,int);
int SwitchStateToString1F(int, char *);
int FindBranchDevice(int,uchar *,uchar BranchSN[][8],int,int);
int owBranchFirst(int,uchar *,int,int);
int owBranchNext(int,uchar *,int,int);

//----------------------------------------------------------------------
//	SUBROUTINE - SetSwitch1F
//
//  This routine sets the main and auxilary on and off for DS2409.
//
// 'portnum'     - number 0 to MAX_PORTNUM-1.  This number is provided to
//                 indicate the symbolic port number.
// 'SerialNum'   - Serial Number of DS2409 to set the switch state
// 'Swtch'       - '0' Sets Main and Auxilary off
//                 '1' Sets Main on
//                 '2' Sets Auxilary on
//                 '3' Read Status Info Byte
//                 '4' Smart On Main
// 'NumExtra'    - The number of extra bytes for a command.
// 'InfoByte'    - Returns the info byte and other information depending
//                 on the command.  The InfoByte size changes depending on
//                 the NumExtra which is buffer length * (NumExtra + 1)
// 'rst'         - True then reset the search for devices
//                 False don't reset search
//
//  Returns: TRUE(1)  State of DS2409 set and verified
//           FALSE(0) could not set the DS2409, perhaps device is not
//                    in contact
//
int SetSwitch1F(int portnum, uchar *SerialNum, int Swtch, int NumExtra,
                    uchar *InfoByte, int rst)
{
   int send_cnt,i,cmd;
   uchar send_block[50];

   if(owAccess(portnum))
   {
      send_cnt = 0;
      // add the match command
      send_block[send_cnt++] = 0x55;

      for (i = 0; i < 8; i++)
         send_block[send_cnt++] = SerialNum[i];

      // the command
      switch(Swtch)
      {
         case 0: // All lines off
            send_block[send_cnt++] = 0x66;
            cmd = 0x66;
            break;

         case 1: // Direct on Main
            send_block[send_cnt++] = 0xA5;
            cmd = 0xA5;
            break;

         case 2: // Smart on Auxilary
            send_block[send_cnt++] = 0x33;
            cmd = 0x33;
            break;

         case 3: // Status Read/Write
            send_block[send_cnt++] = 0x5A;
            cmd = 0x5A;

            // bytes 0-2: don't care
            // bytes 3-4: write control 0 to change status
            // byte 5: 0 = auto-control, 1 = manual mode
            // byte 6: 0 = main, 1 = auxiliary
            // byte 7: value to be written to control output, manual mode only
            // 0x00 default value
            *InfoByte = 0x00;
            send_block[send_cnt++] = *InfoByte;
            break;

         case 4: // Smart on Main
            send_block[send_cnt++] = 0xCC;
            cmd = 0xCC;
            break;

         default:
            return FALSE;
      }

      // extra bytes and confirmation
      for(i=0; i<=NumExtra; i++)
         send_block[send_cnt++] = 0xFF;

      // send the command string
      if(owBlock(portnum,rst,send_block,send_cnt))
      {
         // returned information for the info byte and command
         for(i=0; i<=NumExtra; i++)
            *(InfoByte+(NumExtra-i)) = send_block[send_cnt - (i+2)];

         // Set because for the read/write command the confirmation
         // byte is the same as the status byte
         if (Swtch == 3)
            cmd = send_block[send_cnt - 2];

         if (send_block[send_cnt - 1] == cmd)
            return TRUE;
      }
   }

   return FALSE;
}

//----------------------------------------------------------------------
// SUBROUTINE = FindBranchDevices
//
// This routine will find all the branches on a certain DS2409 device and
// will return the serial numbers down that branch
//
// 'portnum'        - number 0 to MAX_PORTNUM-1.  This number is provided to
//                    indicate the symbolic port number.
// 'Branch[8]'      - The serial number of the branch to look down
// 'BranchSN[][8]'  - The list of serial numbers on the branch
// 'MAXDEVICES'     - The Max devices on the branch
// 'MainBr'         - True to search down the main branch, False for Aux.
//
// Returns: Returns the number of devices found on the branch
//
int FindBranchDevice(int portnum, uchar Branch[8], uchar BranchSN[][8], int MAXDEVICES,
                     int MainBr)
{
   int NumDevices = 0;
   short result;

   result = owBranchFirst(portnum, &Branch[0], FALSE, MainBr);

   while(result)
   {
      owSerialNum(portnum,BranchSN[NumDevices], TRUE);
      NumDevices++;

      result = owBranchNext(portnum, &Branch[0], FALSE, MainBr);
   }

   return NumDevices;
}

//----------------------------------------------------------------------
// SUBROUTINE - owBranchFirst
//
// This routine is used like owFirst but for devices on a branch.
//
// 'portnum' - number 0 to MAX_PORTNUM-1.  This number is provided to
//             indicate the symbolic port number.
// 'BrSN'    - This is the DS2409 device branch
// 'AlarmD'  - True or False, to do Alarm search or false for regular search
// 'FirMain' - True then search main branch, False search Aux. branch
//
// Returns:   TRUE (1) : when a 1-Wire device was found and it's
//                        Serial Number placed in the global SerialNum
//            FALSE (0): There are no devices on the 1-Wire Net.
//
int owBranchFirst(int portnum, uchar BrSN[8], int AlarmD, int FirMain)
{
   int smart_main = 4;
   int smart_aux = 2;
   int numextra = 2;
   uchar extra[3];


   if(FirMain)
   {
      if(SetSwitch1F(portnum, &BrSN[0], smart_main, numextra, extra, TRUE))
         if(extra[2] != 0xFF)
            return owFirst(portnum,FALSE, AlarmD);
   }
   else
   {
      if(SetSwitch1F(portnum, &BrSN[0], smart_aux, numextra, extra, TRUE))
         if(extra[2] != 0xFF)
            return owFirst(portnum,FALSE, AlarmD);
   }

   return FALSE;
}

//----------------------------------------------------------------------
// SUBROUTINE - owBranchNext
//
// This routine is used like owFirst but for devices on a branch.
//
// 'portnum'    - number 0 to MAX_PORTNUM-1.  This number is provided to
//                indicate the symbolic port number.
// 'BrSN'       - This is the DS2409 device branch
// 'AlarmD'     - True or False, to do Alarm search or false for regular search
// 'NextMain'   - True then search main branch, False search Aux. branch
//
// Returns:   TRUE (1) : when a 1-Wire device was found and it's
//                        Serial Number placed in the global SerialNum
//            FALSE (0): There are no devices on the 1-Wire Net.
//
int owBranchNext(int portnum, uchar BrSN[8], int AlarmD, int NextMain)
{
   int smart_main = 4;		/* Was 1, Main On [bcl - 01132002 */
   int smart_aux = 2;
   int numextra = 2;
   uchar extra[3];

   if(NextMain)
   {
      if(SetSwitch1F(portnum, &BrSN[0], smart_main, numextra, extra, TRUE))
         return owNext(portnum,FALSE, AlarmD);
   }
   else
   {
      if(SetSwitch1F(portnum, &BrSN[0], smart_aux, numextra, extra, TRUE))
         return owNext(portnum,FALSE, AlarmD);
   }

   return FALSE;
}

//----------------------------------------------------------------------
//	SUBROUTINE - SwitchStateToString
//
//  This routine uses the info byte to return a string with all the data.
//
// 'infobyte'   - This is the information byte data from the hardware.
// 'outstr'     - This will be the output string.  It gets set in the
//				      the procedure.
//
// Returns      - Returns the number of characters in the string
//
int SwitchStateToString1F(int infobyte, char *outstr)
{
   int cnt = 0;

   if(infobyte & 0x80)
   {
      cnt += sprintf(outstr+cnt, "%s", "Manual mode\n");
      if(infobyte & 0x40)
         cnt += sprintf(outstr+cnt, "%s", "Output transistor on\n");
      else
         cnt += sprintf(outstr+cnt, "%s", "Output transistor off\n");
   }
   else
   {
      cnt += sprintf(outstr+cnt, "%s", "Auto-control mode\n");
      if(infobyte & 0x40)
         cnt += sprintf(outstr+cnt, "%s", "Output association with Auxillary\n");
      else
         cnt += sprintf(outstr+cnt, "%s", "Output association with Main\n");
   }

   if(infobyte & 0x20)
      cnt += sprintf(outstr+cnt, "%s",
         "Negative edge sensed since inactive on Main\n");
   else
      cnt += sprintf(outstr+cnt, "%s", "No event on Main\n");

   if(infobyte & 0x10)
      cnt += sprintf(outstr+cnt, "%s",
         "Negative edge sensed since inactive on Aux.\n");
   else
      cnt += sprintf(outstr+cnt, "%s", "No event on Aux.\n");

   if(infobyte & 0x08)
      cnt += sprintf(outstr+cnt, "%s", "Voltage High on Aux. output\n");
   else
      cnt += sprintf(outstr+cnt, "%s", "Voltage Low on Aux. output\n");

   if(infobyte & 0x04)
      cnt += sprintf(outstr+cnt, "%s", "Inactive status of Aux. output\n");
   else
      cnt += sprintf(outstr+cnt, "%s", "Active status of Aux. output\n");

   if(infobyte & 0x02)
      cnt += sprintf(outstr+cnt, "%s", "Voltage High on Main output\n");
   else
      cnt += sprintf(outstr+cnt, "%s", "Voltage Low on Main output\n");

   if(infobyte & 0x01)
      cnt += sprintf(outstr+cnt, "%s", "Inactive status on Main output\n");
   else
      cnt += sprintf(outstr+cnt, "%s", "Active status on Main output\n");

   return cnt;
}
