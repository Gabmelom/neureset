#ifndef DEVICE_H
#define DEVICE_H

#include <QDateTime>
#include <QTimer>
#include <QListWidget>
#include <Headset.h>
#include "SessionLog.h"

#include "QVector"

#define ROUNDS 4

class Device : public QObject
{
    Q_OBJECT
public:
    Device(QListWidget *list);
    ~Device();

    void replaceBattery();
    void startSession();
    void pauseSession();
    void resumeSession();
    void stopSession();
    QVector<int> readBaseline();    //test version before thread stuff is implemented
    bool applyTherapy();

    void setPower(bool value);
    bool getHeadsetConn();
    int getBatteryLife();
    QVector<SessionLog*> getLogs();
    SessionLog* getCurrSession();

    void uploadSessionLog();
    void togglePower();
    bool isOngoing();

private:
    int sessionNum = -1;
    int sessionStage;
    bool ongoing;
    bool headsetConn;
    bool pcConn;
    int batteryLife;
    bool powerState;
    int offset;
    int rounds;
    float domFreq;
    float startBaseFreq;
    QDateTime *currDate;
    QTimer sessionTimer;
    Headset *headset;
    QListWidget *list;
    SessionLog *currSession;    //the current treatment session, stored in the object so it can be accessed outside the startSession function
    QVector<SessionLog*> logs;

    QTimer pauseTimer;

    float calcDomFreq(QVector<QVector<int>>);



//public slots:
//    QVector<int> readBaseline();

signals:
    int readEEG(int site);
    void updateProgressBar(int percentage);
    int readBaselineSig();

private slots:
    void readStartBaseline();
    void readTreatmentBaseline();
    void treatment();
    void treatmentPart2();
    void readEndBaseline();
    void pauseTimeout();
};

#endif // DEVICE_H
