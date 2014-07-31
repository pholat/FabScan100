#ifndef USBGLOBAL_H
#define USBGLOBAL_H

/* Global data is being used so it could be used between windows - in class these are extern
 * to assure no doubling. It's not in class, as it should be, yet. (In use in usbdialog.h)
 *  */

#include "/usr/include/libusb-1.0/libusb.h"
int usb_number; int USB_Flag=0;
//libUSB members
libusb_device_handle *device_handle; //handle to USB device
libusb_device_descriptor device_descriptor; //the device descriptor
libusb_device  **device_list; //to store all found USB devices

#endif // USBGLOBAL_H
