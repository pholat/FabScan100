#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "staticHeaders.h"
#include "fsdialog.h"
#include "fscontrolpanel.h"
#include "usbdialog.h"

QT_BEGIN_NAMESPACE
class QBasicTimer;
class QGLShaderProgram;
QT_END_NAMESPACE

typedef enum _FSState {
    POINT_CLOUD,
    SURFACE_MESH,
    SCANNING
} FSState;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void showDialog(QString dialogText);
    void redraw();
    FSControlPanel* controlPanel;
    void doneScanning();
    void progression(int);


private slots:
    void on_scanButton_clicked();
    void timerEvent(QTimerEvent *e);

    void onSelectSerialPort();
    void onSelectWebCam();
    void openPointCloud();
    void savePointCloud();
    void newPointCloud();
    void showControlPanel();
    void exportSTL();
    void readConfiguration();

    void on_resolutionComboBox_currentIndexChanged(const QString &arg1);

    void on_actionFind_Devices_triggered();
private:
    Ui::MainWindow *ui;
    QBasicTimer *hwTimer; //updates connected hw:arduino,webcam,...
    FSState state;
    FSDialog* dialog;
    UsbDialog *usbdialog;

    void setupMenu();
    void enumerateSerialPorts();
    void enumerateWebCams();
    void applyState(FSState s);
};

#endif // MAINWINDOW_H
