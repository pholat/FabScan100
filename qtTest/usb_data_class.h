#ifndef USB_DATA_CLASS_H
#define USB_DATA_CLASS_H

#include "/usr/include/libusb-1.0/libusb.h"

class usb_data_class
{
public:
    usb_data_class();
    ~usb_data_class();

    int *usb_to_use;
    int r;

    int usb_number;
//    int USB_Flag;

    //libUSB members
    libusb_device_handle *device_handle; //handle to USB device
    libusb_device_descriptor device_descriptor; //the device descriptor
    libusb_device  **device_list; //to store all found USB devices

    libusb_context *ctx; //a libusb context for library intialization
    ssize_t i; //for the device iterator for loop
    ssize_t device_count; //holding number of devices in list
    QString list_entry; //for using library outputs in char * in Qt framework


};

#endif // USB_DATA_CLASS_H
