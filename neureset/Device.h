#ifndef DEVICE_H
#define DEVICE_H

#include <QDateTime>
#include <QTimer>
#include <Headset.h>

#include "QVector"

class Device : public QObject
{
    Q_OBJECT
public:
    Device();
    ~Device();

    void replaceBattery();
    void startSession();
    void pauseSession();
    void stopSession();
    QVector<int> readBaseline();    //test version before thread stuff is implemented
    bool applyTherapy();
    void setPower(bool value);

private:
    bool headsetConn;
    bool pcConn;
    int batteryLife;
    bool powerState;
    QDateTime *currDate;
    QTimer *sessionTimer;
    Headset *headset;

//public slots:
//    QVector<int> readBaseline();

signals:
    int readEEG(int site);
    void updateProgressBar(int percentage);
    int readBaselineSig();
};

#endif // DEVICE_H
