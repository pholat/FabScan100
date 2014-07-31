#ifndef USBDIALOG_H
#define USBDIALOG_H

#include <QDialog>
#include "/usr/include/libusb-1.0/libusb.h"
#include "fsdefines.h"


namespace Ui {
class UsbDialog;
}

class UsbDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UsbDialog(QWidget *parent = 0);
    ~UsbDialog();

private:
    Ui::UsbDialog *ui;

public slots:
    void usbFindDev();
    void usbdevice_handle();

private slots:
    void on_pushButton_3_clicked();
    void on_pushButton_clicked();
    void on_listWidget_activated(const QModelIndex &index);
    void on_pushButton_2_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
};

#endif // USBDIALOG_H
