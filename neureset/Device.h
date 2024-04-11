#ifndef DEVICE_H
#define DEVICE_H

#include <QDateTime>
#include <QTimer>
#include <Headset.h>
#include <mainwindow.h>
#include "SessionLog.h"

#include "QVector"

class Device : public QObject
{
    Q_OBJECT
public:
    Device(MainWindow *window);
    ~Device();

    void replaceBattery();
    void startSession();
    void pauseSession();
    void stopSession();
    QVector<int> readBaseline();    //test version before thread stuff is implemented
    bool applyTherapy();

    void setPower(bool value);
    bool getHeadsetConn();
    int getBatteryLife();
    QVector<SessionLog*> getLogs();
    SessionLog* getCurrSession();

    void uploadSessionLog();

private:
    bool headsetConn;
    bool pcConn;
    int batteryLife;
    bool powerState;
    QDateTime *currDate;
    QTimer *sessionTimer;
    Headset *headset;
    MainWindow *window;
    SessionLog *currSession;    //the current treatment session, stored in the object so it can be accessed outside the startSession function
    QVector<SessionLog*> logs;

    float calcDomFreq(QVector<QVector<int>>);

//public slots:
//    QVector<int> readBaseline();

signals:
    int readEEG(int site);
    void updateProgressBar(int percentage);
    int readBaselineSig();
};

#endif // DEVICE_H
