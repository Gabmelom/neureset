#ifndef DEVICE_H
#define DEVICE_H

#include <QDateTime>
#include <QTimer>
#include <Headset.h>
#include <mainwindow.h>
#include "SessionLog.h"

#include "QVector"

class Headset;

class Device : public QObject
{
    Q_OBJECT
public:
    Device(MainWindow *window, QListWidget *list);
    ~Device();

    void replaceBattery();
    void startSession();
    void pauseSession();
    void stopSession();
    QVector<int> readBaseline();    //test version before thread stuff is implemented
    bool applyTherapy();


    QDateTime* getDate();
    bool getHeadsetConn();
    int getBatteryLife();
    QVector<SessionLog*> getLogs();
    SessionLog* getCurrSession();


    void setPower(bool value);

    void uploadSessionLog();

private:
    int sessionNum = 0;
    bool headsetConn;
    bool pcConn;
    int batteryLife;
    bool powerState;
    QDateTime *currDate;
    QTimer *sessionTimer;
    Headset *headset;
    MainWindow *window;
    QListWidget *list;
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
