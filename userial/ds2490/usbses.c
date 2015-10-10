//---------------------------------------------------------------------------
// Copyright (C) 2001 Matthew Dharm, All Rights Reserved.
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
// Matthew is not activily supporting this code but can be reached at:
// mailto:mdharm-usb@one-eyed-alien.net
//---------------------------------------------------------------------------
//
//  usb_sessuin.c - General 1-Wire session layer for the DS2490 on Linux 
//
//  Version: 3.00B
// 

#include "ownet.h"
#include <usb.h>
#include <string.h>
#include "digitemp.h"


SMALLINT owAcquire(int,char *, char *);
void owRelease(int,char *);
static void usb_ds2490_init(void);

struct usb_dev_handle *usb_dev_handle_list[MAX_PORTNUM];
struct usb_device *usb_dev_list[MAX_PORTNUM];
int usb_num_devices = -1;
int initted_flag = 0;

extern int opts;            // Command line options


//---------------------------------------------------------------------------
// Initialize the DS2490
//
void usb_ds2490_init(void)
{
	struct usb_bus *bus;
	struct usb_device *dev;

	// initialize USB subsystem
	usb_init();
	usb_set_debug(0);
	usb_find_busses();
	usb_find_devices();

      	//printf("bus/device  idVendor/idProduct\n");
	for (bus = usb_busses; bus; bus = bus->next) {
		for (dev = bus->devices; dev; dev = dev->next) {
			//printf("%s/%s     %04X/%04X\n", bus->dirname, dev->filename,
			//		dev->descriptor.idVendor, dev->descriptor.idProduct);
			if (dev->descriptor.idVendor == 0x04FA &&
					dev->descriptor.idProduct == 0x2490) {
                ++usb_num_devices;
				usb_dev_list[usb_num_devices] = dev;

				if( !(opts & OPT_QUIET) )
  				    printf("Found DS2490 device #%d at %s/%s\n", usb_num_devices + 1,
						    bus->dirname, dev->filename);
			}
		}
	}

	initted_flag = 1;

}

//---------------------------------------------------------------------------
// Attempt to acquire a 1-Wire net
//
// 'portnum'    - number 0 to MAX_PORTNUM-1.  This number is provided to
//                indicate the symbolic port number.
// 'port_zstr'  - zero terminated port name.  
// 'return_msg' - zero terminated return message. 
//
// Returns: TRUE - success, port opened
//
SMALLINT owAcquire(int portnum, char *port_zstr, char *return_msg)
{
	if (!initted_flag)
		usb_ds2490_init();
	
	/* check the port string */
	if (strcmp (port_zstr, "USB")) {
		strcpy(return_msg, "owAcquire called with invalid port string\n");
	      	return 0;
	}
	
	/* check to see if the portnumber is valid */
	if (usb_num_devices < portnum) {
		strcpy(return_msg, "Attempted to select invalid port number\n");
		return FALSE;
	}

	/* check to see if opening the device is valid */
	if (usb_dev_handle_list[portnum] != NULL) {
		strcpy(return_msg, "Device allready open\n");
		return FALSE;
	}

	/* open the device */
	usb_dev_handle_list[portnum] = usb_open(usb_dev_list[portnum]);
	if (usb_dev_handle_list[portnum] == NULL) {
		strcpy(return_msg, "Failed to open usb device\n");
		printf("%s\n", usb_strerror());
		return FALSE;
	}

	/* set the configuration */
	if (usb_set_configuration(usb_dev_handle_list[portnum], 1)) {
		strcpy(return_msg, "Failed to set configuration\n");
		printf("%s\n", usb_strerror());
		usb_close(usb_dev_handle_list[portnum]);
		return FALSE;
	}

	/* claim the interface */
	if (usb_claim_interface(usb_dev_handle_list[portnum], 0)) {
		strcpy(return_msg, "Failed to claim interface\n");
		printf("%s\n", usb_strerror());
		usb_close(usb_dev_handle_list[portnum]);
		return FALSE;
	}

	/* set the alt interface */
	if (usb_set_altinterface(usb_dev_handle_list[portnum], 3)) {
		strcpy(return_msg, "Failed to set altinterface\n");
		printf("%s\n", usb_strerror());
		usb_release_interface(usb_dev_handle_list[portnum], 0);
		usb_close(usb_dev_handle_list[portnum]);
		return FALSE;
	}

	/* we're all set here! */
	strcpy(return_msg, "DS2490 successfully acquired by USB driver\n");
	return TRUE;
}

//---------------------------------------------------------------------------
// Release the previously acquired a 1-Wire net.
//
// 'portnum'    - number 0 to MAX_PORTNUM-1.  This number is provided to
//                indicate the symbolic port number.
// 'return_msg' - zero terminated return message. 
//
void owRelease(int portnum, char *return_msg)
{
	usb_release_interface(usb_dev_handle_list[portnum], 0);
	usb_close(usb_dev_handle_list[portnum]);
	usb_dev_handle_list[portnum] = NULL;
	strcpy(return_msg, "DS2490 successfully released by USB driver\n");
}


