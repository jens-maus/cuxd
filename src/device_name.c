/* -----------------------------------------------------------------------
   DigiTemp
      
   Copyright 1996-2003 by Brian C. Lane <bcl@brianlane.com>
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


/* Return the family name passed to it */
char *device_name( unsigned int family )
{
  switch( family )
  {
    case 0x01:
      return "DS2401/DS1990A Serial Number iButton";
      
    case 0x02:
      return "DS1425/DS1991 MultiKey iButton";
      
    case 0x04:
      return "DS2402/DS1994 4K NVRAM memory, clock, timer";

    case 0x05:
      return "DS2405 Addressable Switch";

    case 0x06:
      return "DS1993 4K NVRAM Memory";
      
    case 0x08:
      return "DS1992 1K NVRAM Memory";
  
    case 0x09:
      return "DS2502/DS1982 1Kbit Add only memory";
  
    case 0x0A:
      return "DS1995 16K NVRAM Memory";

    case 0x0B:
      return "DS2505/DS1985 16K EPROM Memory"; 
      
    case 0x0C:
      return "DS1996/x2/x4 64K to 256K NVRAM Memory"; 
      
    case 0x0F:
      return "DS2506/DS1986 64K EEPROM Memory";
      
    case 0x10:
      return "DS1820/DS18S20/DS1920 Temperature Sensor";
    
    case 0x12:
      return "DS2406/2407 Dual Addressable Switch + 1Kbit memory";
    
    case 0x14:
      return "DS2430A/DS1971 256bit EEPROM iButton";
    
    case 0x18:
      return "DS1963S SHA iButton";
      
    case 0x1A:
      return "DS1963L 4kBit MONETARY iButton";
      
    case 0x1C:
      return "DS2422 1Kbit RAM + Counter";
      
    case 0x1D:
      return "DS2423 4Kbit RAM + Counter";
      
    case 0x1F:
      return "DS2409 MicroLAN Coupler";
      
    case 0x20:
      return "DS2450 Quad A/D Converter";
      
    case 0x21:
      return "DS1921/H/Z Thermochron iButton";
      
    case 0x22:
      return "DS1822 Econo-Temperature Sensor";

    case 0x23:
      return "DS2433/DS1973 4K EEPROM Memory";
    
    case 0x24:
      return "DS1425/DS1904 Real Time Clock";
      
    case 0x26:
      return "DS2438 Temperature, A/D Battery Monitor";
      
    case 0x27:
      return "DS2417 Real Time Clock with Interrupt";

    case 0x28:
      return "DS18B20 Temperature Sensor";

    case 0x29:
      return "DS2408 8-Channel Addressable Switch";

    case 0x2C:
      return "DS2890 Single Channel Digital Potentiometer";
      
    case 0x30:
      return "DS2760 Temperature, Current, A/D";
      
    case 0x33:
      return "DS2432/DS1961S 1K EEPROM with SHA-1 Engine";

    case 0x3A:
      return "DS2413 Dual Channel Addressable Switch";

    case 0x41:
      return "DS1923 Hygrochron Temperature/Humidity Logger with 8kB Data Log Memory";

    case 0x42:
      return "DS28EA00 Temperature Sensor with Sequence Detect and PIO";

    case 0x82:
      return "DS1425 Multi iButton";
      
    case 0x84:
      return "DS1427 TIME iButton";
      
    case 0x89:
      return "DS2502/1982 1024bit UniqueWare Add Only Memory";
    
    case 0x8B:
      return "DS2505/1985 16Kbit UniqueWare Add Only Memory";
      
    case 0x8F:
      return "DS2506/1986 64Kbit UniqueWare Add Only Memory";

    case 0x91:
      return "DS1981 512-bit EEPROM Memory UniqueWare Only";
      
    case 0x96:
      return "DS1955/DS1957B Java Cryptographic iButton";

    case 0xEE:
      return "Hobby Boards with Temperature Sensor";

    case 0xEF:
      return "Hobby Boards";

    default:
      return "Unknown Family Code";
  }
}



/* Local Variables: */
/* mode: C */
/* compile-command: "cd ..; make -k" */
/* End: */
