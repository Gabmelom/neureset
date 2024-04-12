#ifndef DEVICE_H
#define DEVICE_H

#include <QDateTime>
#include <QTimer>
#include <Headset.h>
#include <QListWidget>
#include "SessionLog.h"
#include "PC.h"

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
    void stopSession();
    QVector<int> readBaseline();    //test version before thread stuff is implemented
    bool applyTherapy();

    void setPower(bool value);
    bool getHeadsetConn();
    int getBatteryLife();
    QVector<SessionLog*> getLogs();
    SessionLog* getCurrSession();

    void uploadSessionLog(int selected);

private:
    int sessionNum = 0;
    bool headsetConn;
    bool pcConn;
    int batteryLife;
    bool powerState;
    QDateTime *currDate;
    QTimer sessionTimer;
    Headset *headset;
    PC *pc;
    QListWidget *list;
    SessionLog *currSession;    //the current treatment session, stored in the object so it can be accessed outside the startSession function
    QVector<SessionLog*> logs;

    float calcDomFreq(QVector<QVector<int>>);

    int offset = 5;
    int rounds = 0;

    float domFreq;
    float startBaseFreq;

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
};

#endif // DEVICE_H
