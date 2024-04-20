#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <Device.h>
#include "Device.h"

//#include "Device.h"
#include <QDebug>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Device;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    Ui::MainWindow* getUi() { return ui; }
private slots:
    void powerPressed();
    void homePressed();
    void upPressed();
    void downPressed();
    void selectPressed();
    void playPausePressed();
    void stopPressed();
    void updateDate();
    void changeDate();
    void changeTime();
    void changeBattery();
    void toggleHeadset();
    void updateGui();
    void devicePageChanged(int index);
    void pcPageChanged(int index);
    void displayLogDetails(QListWidgetItem*);
    void uploadLogs();
    void pcBackButtonPressed();
    void connToPC();
    void updateProgressMessage(QString message);


private:
    Ui::MainWindow *ui;
    Device *device;
    PC *pc;
    void init();

    int currentDeviceScreen;
    QListWidget* currentDeviceList;

    int currentPCScreen;
    QListWidget* currentPCList;

    void displayDeviceLogs();
    void displayPCLogs();

};
#endif // MAINWINDOW_H
