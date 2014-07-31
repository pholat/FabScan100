#include "fsserial.h"
#include <QThread>
//#include <usb.h>
//unix:!macx:!symbian: PRE_TARGETDEPS += $$PWD/../../../../../usr/lib/i386-linux-gnu/libusb.a
#include "/usr/include/libusb-1.0/libusb.h"

extern libusb_device_handle *device_handle; //handle to USB device
extern int USB_Flag;


FSSerial::FSSerial()
{
    serialPortPath = new QString();
}

// USB has another window - so no need to add USB_Flag
bool FSSerial::connectToSerialPort()
{
    this->serialPort = new QextSerialPort(*serialPortPath, QextSerialPort::EventDriven);
    serialPort->setBaudRate(BAUD9600);
    serialPort->setFlowControl(FLOW_OFF);
    serialPort->setParity(PAR_NONE);
    serialPort->setDataBits(DATA_8);
    serialPort->setStopBits(STOP_2);

    if (serialPort->open(QIODevice::ReadWrite) == true) {
        connect(serialPort, SIGNAL(readyRead()), this, SLOT(onReadyRead()) );
        connect(serialPort, SIGNAL(dsrChanged(bool)), this, SLOT(onDsrChanged(bool)) );
        if (!(serialPort->lineStatus() & LS_DSR)){
            //qDebug() << "warning: device is not turned on";
            return false;
        }
        //qDebug() << "listening for data on" << serialPort->portName();
        return true;
    }else{
        //qDebug() << "device failed to open:" << serialPort->errorString();
        return true;
    }
}

void FSSerial::onReadyRead()
{
    QByteArray bytes;
    int a = serialPort->bytesAvailable();
    bytes.resize(a);
    serialPort->read(bytes.data(), bytes.size());
    //qDebug() << "#"<<bytes.size() <<"bytes=" << bytes.data();
}

void FSSerial::onDsrChanged(bool)
{
    //qDebug("onDsrChanged");
}

void FSSerial::writeChar(char c)
{
    if(USB_Flag==0) // If HID usb is not in use → than use USB type of write char
    {
        //qDebug() << "writing to serial port: " << (int)((unsigned char)c);
        if( serialPortPath->isEmpty() ) return;
        if( !serialPort->isOpen() ) return;
        if( serialPort->isWritable() ){
            //qDebug("is writable");
            //usleep(100000);
            serialPort->write(&c);
        }else{
           // qDebug("is not writable");
        }
    }else{
        libusb_control_transfer(device_handle,LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE | LIBUSB_ENDPOINT_IN,
                                c , 0, 0, 0, 0, 5000);

    }
}

void FSSerial::writeChars(char* c ,int size)
{
    if(USB_Flag==0)
    {
        if( serialPortPath->isEmpty() ) return;
        if( !serialPort->isOpen() ) return;
        if( serialPort->isWritable() ){
            //qDebug("is writable");
            //usleep(100000);
            serialPort->write(c);
        }else{
            //qDebug("is not writable");
        }
    }else{
        while((size)!=0)
        {
            libusb_control_transfer(device_handle,LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE | LIBUSB_ENDPOINT_IN,
                                    *c , 0, 0, 0, 0, 5000);
            size--;
            c=c++;
        }
    }
}
