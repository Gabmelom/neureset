#ifndef DEVICE_H
#define DEVICE_H

#include <QDateTime>
#include <QTimer>
#include <QListWidget>
#include <QProgressBar>
#include <QLabel>
#include <QVector>

#include "Headset.h"
#include "SessionLog.h"
#include "PC.h"


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
    Device();
    ~Device();

    void replaceBattery();
    void startSession();
    void pauseSession();
    void resumeSession();
    void stopSession();


    // Getters
    int getBatteryLife() { return batteryLife; }
    bool getPower() { return powerState; }
    bool getHeadsetConn() { return headsetConn; }
    bool getPowerState() { return powerState; }
    bool isOngoing() { return ongoing; }
    E_SESSION_STAGE getSessionStage() { return sessionStage; }
    QDateTime* getDate() { return currDate; }
    SessionLog* getCurrSession() { return currSession; }
    QVector<SessionLog*> getLogs() { return logs; }

    void setDate(QDateTime* date) {currDate = date; }

    void uploadSessionLog();
    void togglePower(); // TODO: decouple, send signal if battery runs out
    void uploadLogs(PC *pc);


private:
    bool ongoing;
    bool headsetConn;
    bool pcConn;
    bool powerState;
    
    int progress;
    int batteryLife;
    int sessionsDone;
    E_SESSION_STAGE sessionStage;

    int offset;
    int rounds;
    float treatmentFrequency;

    Headset *headset;
    SessionLog *currSession;
    QVector<SessionLog*> logs;
    
    QDateTime *currDate;
    QDateTime endTime;
    QTimer sessionTimer;
    QTimer pauseTimer;
    
    float calcDomFreq(QVector<WaveForm>);
    float avgDomFreq(QVector<float>);
    QVector<float> readBaselines(E_SESSION_STAGE stage);
    
    // Device LEDs
    bool redlightOn;
    bool greenlightOn;
    bool bluelightOn;
    void toggleRedlight(bool);
    void toggleGreenlight(bool);
    void toggleBluelight(bool);
    void flashRedlight();

signals:
    void updateProgressBar(int percentage);
    void updateProgressMessage(QString message);
    void updateTreatmentGraph(QVector<WaveForm> waveforms, int site);
    void updateBatteryLevel(int level);
    void uiToggleHeadset(bool);
    void uiTogglePC(bool);
    void uiToggleBluelight(bool);
    void uiToggleRedlight(bool);
    void uiToggleGreenlight(bool);

private slots:
    void readStartBaseline();
    void readTreatmentBaseline();
    void readEndBaseline();
    void treatment();
    void treatmentPart2();
    void pauseTimeout();
    void applyTherapy(float);

    void toggleHeadset();
    void togglePC();
};

#endif // DEVICE_H
