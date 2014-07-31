#ifndef FSSERIAL_H
#define FSSERIAL_H

#include <QObject>
#include "qextserialport-1.2rc/src/qextserialport.h"
#include "qextserialport-1.2rc/src/qextserialenumerator.h"

//#include "staticHeaders.h"

#include <usb.h>
#include "usb_fun.h"

class FSSerial : public QObject
{

Q_OBJECT
public:
    FSSerial();
    QString *serialPortPath;
    bool connectToSerialPort();
    void writeChar(char c);
    void writeChars(char* c, int size=0);

private slots:
    void onReadyRead();
    void onDsrChanged(bool);

private:
    QextSerialPort *serialPort;

};

#endif // FSSERIAL_H
