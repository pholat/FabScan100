#include "usbdialog.h"
#include "ui_usbdialog.h"
//#include "/usr/include/libusb-1.0/libusb.h"

extern int usb_number;
extern int USB_Flag;
//libUSB members
extern libusb_device_handle *device_handle; //handle to USB device
extern libusb_device_descriptor device_descriptor; //the device descriptor
extern libusb_device  **device_list; //to store all found USB devices

// Table of all possible usb numbers - in progress - shall be new / and delete (as it shall be in usb class storing it all)
int Usb_to_use_numbers[30];
int USB_Flag_conected;

UsbDialog::UsbDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UsbDialog)
{
    ui->setupUi(this);
    ui->progressBar->reset();
}

UsbDialog::~UsbDialog()
{
    libusb_close(device_handle);
    delete ui;
}

void UsbDialog::usbFindDev()
{
    ui->listWidget->clear(); //clearing the list view

//    libusb_device_handle *device_handle; //handle to USB device
//    libusb_device_descriptor device_descriptor; //the device descriptor
//    libusb_device  **device_list; //to store all found USB devices
    libusb_context *ctx; //a libusb context for library intialization
    ssize_t i; //for the device iterator for loop
    ssize_t device_count; //holding number of devices in list
    QString list_entry; //for using library outputs in char * in Qt framework

    int iusbnum=0;

    int r; //for return values
    int progress_bar_value = 0;
    int progress_bar_correction = 0;
    char device_open = 0; //for checking whether a device was opened or not
    unsigned char string_buffer_manufacturer[4096]; //for storing manufacturer descriptor
    unsigned char string_buffer_product[4096]; //for storing product descriptor
    const char *cc_manufacturer; //for the casting to use the string output from libusb in Qt
    const char *cc_product; //for the casting to use the string output from libusb in Qt

    r = libusb_init(&ctx); //initializing the library

    //inform error on status bar
    if(r < 0) ;//ui->statusBar->showMessage("Error: Initializing libusb");

    else
    {
        //detecting all the connected devices
        device_count = libusb_get_device_list(ctx, &device_list);

        //inform error on status bar
        if(device_count <= 0); //ui->statusBar->showMessage("Info: No device found");

        else
        {
            //resetting the progress bar
            ui->progressBar->reset();

            //for loop iterating through found devices
            for(i=0;i<device_count;i++)
            {
                //getting device descriptor
                r = libusb_get_device_descriptor(device_list[i], &device_descriptor);

                //inform error on status bar
                if(r < 0);   //ui->statusBar->showMessage("Error: Failed to get device descriptor");

                else
                {
                    //opening the device
                    r = libusb_open(device_list[i],&device_handle);

                    if(r < 0)
                    {
                        //inform error on status bar
                        //ui->statusBar->showMessage("Error: Opening USB device");
                        //indicating that device is not open
                        device_open = 0;
                    }

                    else
                    {
                        //indicating that device is open
                        device_open = 1;

                        //getting the ASCII text value from the descriptor field
                        r = libusb_get_string_descriptor_ascii(device_handle,device_descriptor.iManufacturer,string_buffer_manufacturer,sizeof(string_buffer_manufacturer));

                        if(r < 0)
                        {
                            //reporting error on the list entry
                            ui->listWidget->addItem("Error: Converting descriptor to ASCII [iManufacturer]");
                            ui->listWidget->item(ui->listWidget->count() - 1)->setForeground(Qt::white);
                            ui->listWidget->item(ui->listWidget->count() - 1)->setBackground(Qt::red);
                        }

                        else
                        {
                            //getting the ASCII text value from the descriptor field
                            r = libusb_get_string_descriptor_ascii(device_handle,device_descriptor.iProduct,string_buffer_product,sizeof(string_buffer_product));
                            if(r < 0)
                            {
                                //reporting error on the list entry
                                ui->listWidget->addItem("Error: Converting descriptor to ASCII [iProduct]");
                                ui->listWidget->item(ui->listWidget->count() - 1)->setForeground(Qt::white);
                                ui->listWidget->item(ui->listWidget->count() - 1)->setBackground(Qt::red);
                            }

                            else
                            {
                                //converting the string to const char*
                                cc_manufacturer = (const char*)string_buffer_manufacturer;
                                QString manufacturer(cc_manufacturer);

                                //converting the string to const char*
                                cc_product = (const char*)string_buffer_product;
                                QString product(cc_product);

                                //constructing the string for list entry
                                list_entry = manufacturer + " " + product;

                                ui->listWidget->horizontalScrollBar();
                                ui->listWidget->verticalScrollBar();
                                ui->listWidget->addItem(list_entry); //adding the list entry
                                //TODO add "roll list" with numbers
                                //ui->lcdNumber->display(i);
                                Usb_to_use_numbers[iusbnum]=i;
                                iusbnum++;

                                //closing opened USB device
                                if (device_open == 1)
                                {
                                    //closing the deivce
                                    libusb_close(device_handle);
                                    //indicating that device is open
                                    device_open = 0;
                                }

                                //calculate and set the progress bar value
                                progress_bar_value = (i+1) * (100/device_count);

                                //correcting value
                                if ( (i+1) == device_count)
                                {
                                    progress_bar_correction = 100 - progress_bar_value;
                                    progress_bar_value = progress_bar_value + progress_bar_correction;
                                }

                                //setting the value
                                ui->progressBar->setValue(progress_bar_value);
                            }
                            ui->progressBar->setValue(100);
                        }
                    }
                }
            }
            //closing opened USB device
            if (device_open == 1)
            {
                //closing the deivce
                libusb_close(device_handle);
                //indicating that device is open
                device_open = 0;
            }
        }
    }

}

void UsbDialog::on_pushButton_clicked()
{
    usbFindDev();
}

void UsbDialog::on_listWidget_activated(const QModelIndex &index)
{
    usb_number=index.row();
    ui->lcdNumber->display(Usb_to_use_numbers[usb_number]);
}

void UsbDialog::on_pushButton_3_clicked()
{
    USB_Flag=1;
    ui->listWidget_2->addItem("USB_Flag set");
}

void UsbDialog::on_pushButton_2_clicked()
{
    //TODO add val = to prevent errors
    libusb_open(device_list[Usb_to_use_numbers[usb_number]],&device_handle);
    USB_Flag_conected=1;
    ui->listWidget_2->addItem("Usb dev connected");
}

void UsbDialog::on_pushButton_4_clicked()
{
    uchar buffer[2];

    if(USB_Flag_conected==0)
    {
        ui->listWidget_2->addItem("Error - no USB connected");
    }
    else
    {
        libusb_control_transfer(device_handle,LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE | LIBUSB_ENDPOINT_IN,
                                MC_TURN_STEPPER_ON , 0, 0, 0, 0, 5000);

        libusb_control_transfer(device_handle,LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE | LIBUSB_ENDPOINT_IN,
                                MC_PERFORM_STEP , 0, 0, buffer, sizeof(buffer), 5000);

        libusb_control_transfer(device_handle,LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE | LIBUSB_ENDPOINT_IN,
                                5 , 0, 0, buffer, sizeof(buffer), 5000);
    }
}

void UsbDialog::on_pushButton_5_clicked()
{
    libusb_close(device_handle);
    USB_Flag=USB_Flag_conected=0;
    ui->listWidget_2->addItem("USB_Flag reset");
}
