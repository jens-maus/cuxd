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
//---------------------------------------------------------------------------
//
//  ioutil.c - I/O Utility functions
//
//  Version:   2.00
//  History:
//

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <ctype.h>
#include "ownet.h"

#ifdef __MC68K__
#include <PalmOS.h>
#include <Window.h>
#endif

// External functions
extern int key_abort(void);

// typedef
typedef void Sigfunc(int);

// local function prototypes
int EnterString(char *, char *, int, int);
int EnterNum(char *, int, long *, long, long);
int EnterHex(char *, int, ulong *);
int ToHex(char ch);
int getkeystroke(void);
int key_abort(void);
void ExitProg(char *, int);
int getData(uchar*, int, SMALLINT);
void PrintHex(uchar*, int);
void PrintChars(uchar*, int);
void PrintSerialNum(uchar*);

#ifndef __MC68K__
static void sig_ctrlc(int signo);
#else
int key_aborted = FALSE;
#endif

// global state of interrupt
static int got_interrupt=0;

//----------------------------------------------------------------------
// Enter a string
// Leave the same if no characters entered
//
int EnterString(char *msg, char *buf, int min, int max)
{
   int len,deflen;
   char ch,defbuf[80];

   // check for abort
   if (key_abort())
      return -1;

   // remember the start length
   deflen = strlen(buf);
   if (deflen < 80)
      sprintf(defbuf, "%s", buf);
   else
      defbuf[0] = 0;

   // prompt
   if (deflen < 30)
      printf("%s (%s): ",msg,buf);
   else
      printf("%s (%s):\n",msg,buf);


   // loop to get the file
   len = 0;
   for (;;)
   {
      // get a key
      ch = getkeystroke();

      // check for abort
      if (key_abort())
         return -1;

      // check the backspace key
      if (ch == 0x08)
      {
         if (len > 0)
         {
            // got a valid backspace
            len--;
            printf("\x008 \x008");
         }
      }
      // escape key
      else if (ch == 0x1B)
      {
         printf("\n");
         return -1;
      }
      // caraige return
      if (ch == 0x0A)
      {
         // check for special case (keep default)
         if ((len == 0) && (deflen > min))
         {
            sprintf(buf,"%s",defbuf);
            printf("\n");
            return deflen;
         }
         else if (len < min)
            continue;
         else
         {
            printf("\n");
            return len;
         }
      }
      // valid key
      else if (len < max)
      {
         printf("%c",ch);
         // record the char
         buf[len++] = ch;
      }
   }
}

//----------------------------------------------------------------------
// Enter a decimal string and convert it to an unsigned long
// Prompt again if not within min/max inclusive.
//
int EnterNum(char *msg, int numchars, long *value, long min, long max)
{
   short tmp,cnt,isneg=FALSE;
   char ch;

   // check for abort
   if (key_abort())
      return FALSE;

   // loop while not in correct range
   do
   {
      printf("%s (%ld): ",msg,*value);

      // loop for each character read
      cnt = 0;
      for (;;)
      {
         ch = getkeystroke();

         // check for abort
         if (key_abort())
            return FALSE;

         // negative flag
         if (ch == '-')
         {
            if (!isneg)
            {
               isneg = TRUE;
               printf("-");
               cnt++;
            }
         }
         // backspace
         if (ch == 0x08)
         {
            if (cnt)
            {
               if (isneg && (cnt == 1))
                  isneg = FALSE;
               else
                  *value /= 10;
               printf("%c %c",ch,ch);
               cnt--;
            }
         }
         // escape
         if (ch == 0x1B)
         {
            printf("  Aborted\n\n");
            return FALSE;
         }
         // enter
         if (ch == 0x0A)
         {
            printf("\n");
            break;
         }
         // number
         else if ((ch >= '0') && (ch <= '9'))
         {
            if (cnt == 0)
               *value = 0;

            if (cnt < numchars)
            {
               printf("%c",ch);
               tmp = ch - 0x30;
               *value *= 10;
               *value += tmp;
               cnt++;
            }
         }
      }

      if (isneg)
         *value = -*value;
   }
   while ((*value < min) || (*value > max));

   return TRUE;
}

//----------------------------------------------------------------------
// Enter a hex string and convert it to an unsigned long
// (1-8) characters
//
int EnterHex(char *msg, int numchars, ulong *value)
{
   int tmp,cnt;
   int ch;

   // prompt
   printf("%s (enter hex, up to %d chars):",msg,numchars);

   *value = 0;

   cnt = 0;
   do
   {
      ch = getkeystroke();

      if (ch == 0x08)
      {
         printf("%c %c",ch,ch);
         if (cnt)
            cnt--;
      }
      else if (ch == 0x1B)
      {
         printf("  Aborted\n\n");
         return FALSE;
      }
      // caraige return
      else if (ch == 0x0A)
      {
         printf("\n");
         return TRUE;
      }
      else
      {
         tmp = ToHex((char)ch);
         if (tmp)
         {
            printf("%c",ch);
            *value <<= 4;
            *value |= tmp;
            cnt++;
         }
      }
   }
   while (cnt < numchars);

   printf("\n");

   return TRUE;
}

//------------------------------------------------------------------------
// Convert 1 hex character to binary.  If not a hex character then
// return 0.
//
int ToHex(char ch)
{
   if ((ch >= '0') && (ch <= '9'))
      return ch - 0x30;
   else if ((ch >= 'A') && (ch <= 'F'))
      return ch - 0x37;
   else if ((ch >= 'a') && (ch <= 'f'))
      return ch - 0x57;
   else
      return 0;
}

//------------------------------------------------------------------------
// Get a character
//
int getkeystroke(void)
{
   return fgetc(stdin);
}

//------------------------------------------------------------------------
//  Check if key abort has occurred
//
int key_abort(void)
{
#ifdef __MC68K__
   EventType event;

   EvtGetEvent(&event,1);

   if(event.eType == penDownEvent)
   		key_aborted = TRUE;

   return key_aborted;
#else
   static int didsetup=0;

   if (!didsetup)
   {
      if (signal(SIGINT,sig_ctrlc) == SIG_ERR)
          printf("could not setup ctrl-c handler\n");

      didsetup = 1;
   }

   return got_interrupt;
#endif
}

#ifndef __MC68K__
//------------------------------------------------------------------------
//  Key abort interrupt handler
//
static void sig_ctrlc(int signo)
{
   // set abort flag
   got_interrupt = 1;

   // print warning (may take awhile to abort)
   printf("<< CTRL-C key abort >>");
}
#endif

//------------------------------------------------------------------------
// Print message and exit program
//
void ExitProg(char *msg, int exit_code)
{
#ifndef __MC68K__
   printf("%s\n",msg);
   exit(exit_code);
#else
	// Add code to print exit criteria for Palm/Visor
#endif
}

/**
 * Retrieve user input from the console in the form of hex or text.
 *
 * write_buff  the buffer for the data to be written into.
 *
 * @return length of arra.
 */
int getData(uchar *write_buff, int max_len, SMALLINT gethex)
{
   char ch;
   char hexchar[3];
   int  cnt = 0;
   int  done = FALSE;

   if(!gethex)
   {
      do
      {
         ch = (char) getchar();

         if(!isspace(ch) && cnt<max_len)
            write_buff[cnt++] = (uchar) ch;
         else if(ch == ' ' && cnt<max_len)
            write_buff[cnt++] = (uchar) ch;
         else if(cnt > 0)
            done = TRUE;
      }
      while(!done);
   }
   else
   {
      hexchar[2] = 0;
      do
      {
         ch = (char) getchar();

         if(!isspace(ch))
         {
            hexchar[0] = ch;
            hexchar[1] = (char) getchar();

            if(cnt<max_len)
               write_buff[cnt++] = (uchar) strtol(&hexchar[0],NULL,16);
         }
         else if((ch != ' ') && (cnt > 0))
            done = TRUE;
      }
      while(!done);
   }

   return cnt;
}

void PrintHex(uchar* buffer, int cnt)
{
   int i;
   for(i=0; i<cnt; i++)
   {
      if((i&15)==0)
         printf("\n  %02X", buffer[i]);
      else
         printf(".%02X", buffer[i]);
   }
   printf("\n");
}

void PrintChars(uchar* buffer, int cnt)
{
   int i;
   for(i=0; i<cnt; i++)
   {
      printf("%c", buffer[i]);
   }
}

void PrintSerialNum(uchar* buffer)
{
   int i;
   for(i=7; i>=0; i--)
   {
      printf("%02X", buffer[i]);
   }
}


