#ifndef DEVICE_H
#define DEVICE_H

#include <QDateTime>
#include <QTimer>
#include <QListWidget>
#include <Headset.h>
#include <QListWidget>
#include <QProgressBar>
#include <QLabel>
#include "SessionLog.h"
#include "PC.h"

#include "QVector"

class Headset;

#define ROUNDS 4

enum E_SESSION_STAGE{
    NO_STAGE,
    START_SESSION,
    READ_START_BASELINE,
    TREATMENT,
    READ_TREATMENT_BASELINE,
    TREATMENT_PART_2,
    READ_END_BASELINE
};

class Device : public QObject
{
    Q_OBJECT
public:
    Device(QListWidget *list, QProgressBar *progress,
           QLabel *redlight, QLabel *bluelight, QLabel *greenlight);
    ~Device();

    void replaceBattery();
    void toggleHeadsetConn();
    void startSession();
    void pauseSession();
    void resumeSession();
    void stopSession();
    QVector<int> readBaseline();    //test version before thread stuff is implemented
    bool applyTherapy();


    QDateTime* getDate();
    bool getPower();
    bool getHeadsetConn();
    int getBatteryLife();
    bool getPowerState();
    QVector<SessionLog*> getLogs();
    SessionLog* getCurrSession();

    //void setPower(bool value);

    void uploadSessionLog();

    void togglePower();
    bool isOngoing();
    int getSessionStage();
    void uploadSessionLog(int selected);


private:
    int sessionNum;
    int sessionStage;
    bool ongoing;
    bool headsetConn;
    bool pcConn;
    bool bluelightOn;
    bool redlightOn;
    bool greenlightOn;
    int batteryLife;
    bool powerState;

    int offset;
    int rounds;
    float domFreq;
    float startBaseFreq;

    QDateTime *currDate;
    QTimer sessionTimer;
    Headset *headset;
    PC *pc;

    QListWidget *list;
    QProgressBar *progress;
    SessionLog *currSession;    //the current treatment session, stored in the object so it can be accessed outside the startSession function
    QVector<SessionLog*> logs;
    QLabel* redlight;
    QLabel* bluelight;
    QLabel* greenlight;


    QTimer pauseTimer;

    float calcDomFreq(QVector<QVector<int>>);
    void turnOnBluelight();
    void turnOffBluelight();
    void turnOffRedlight();
    void turnOffGreenlight();
    void turnOnGreenlight();


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
    void flashRedlight();
    //void toggleHeadsetConn();
};

#endif // DEVICE_H
