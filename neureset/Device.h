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
    Device(QProgressBar *progress, QLabel *redlight, QLabel *bluelight, QLabel *greenlight);
    ~Device();

    void replaceBattery();
    void startSession();
    void pauseSession();
    void resumeSession();
    void stopSession();


    // Getters
    int getBatteryLife() { return batteryLife; }
    int getSessionStage() { return sessionStage; }
    bool getPower() { return powerState; }
    bool getHeadsetConn() { return headsetConn; }
    bool getPowerState() { return powerState; }
    bool isOngoing() { return ongoing; }
    QDateTime* getDate() { return currDate; }
    SessionLog* getCurrSession() { return currSession; }
    QVector<SessionLog*> getLogs() { return logs; }

    void uploadSessionLog();
    void togglePower();
    void uploadLogs(PC *pc);


private:
    bool ongoing;
    bool headsetConn;
    bool pcConn;
    bool bluelightOn;
    bool redlightOn;
    bool greenlightOn;
    bool powerState;
    
    int sessionStage;
    int batteryLife;
    int sessionsDone;

    int offset;
    int rounds;
    float treatmentFrequency;

    Headset *headset;
    SessionLog *currSession;
    QVector<SessionLog*> logs;
    
    QDateTime *currDate;
    QTimer sessionTimer;
    QTimer pauseTimer;
    
    float calcDomFreq(QVector<WaveForm>);
    float avgDomFreq(QVector<float>);
    QVector<float> readBaselines(bool graph=false);
    QString bandToString(FREQ_BAND band); // TODO: Move to own enum class
    
    // TODO : Decouple ui from class
    QProgressBar *progress;
    QLabel* redlight;
    QLabel* bluelight;
    QLabel* greenlight;

    // TODO: Change these to signals
    void turnOnBluelight();
    void turnOffBluelight();
    void turnOffRedlight();
    void turnOffGreenlight();
    void turnOnGreenlight();

signals:
    void updateProgressBar(int percentage);
    void updateProgressMessage(QString message);
    void updateTreatmentGraph(QVector<WaveForm> waveforms, int site);

    void updateBatteryLevel(int level);
    void uiToggleHeadset(bool);
    void uiTogglePC(bool);

private slots:
    void readStartBaseline();
    void readTreatmentBaseline();
    void readEndBaseline();
    void treatment();
    void treatmentPart2();
    void pauseTimeout();
    void flashRedlight();
    void applyTherapy(float);

    void toggleHeadset();
    void togglePC();

    //void toggleHeadsetConn();
};

#endif // DEVICE_H
