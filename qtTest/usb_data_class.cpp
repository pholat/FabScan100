#include "usb_data_class.h"

usb_data_class::usb_data_class()
{
     r = libusb_init(&ctx);
     device_count = libusb_get_device_list(ctx, &device_list);
     usb_to_use = new int[device_count];
}

usb_data_class::~usb_data_class()
{
    delete usb_to_use;
}
