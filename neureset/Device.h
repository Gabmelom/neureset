#ifndef DEVICE_H
#define DEVICE_H

#include <QDateTime>
#include <QTimer>

class Device
{
public:
    Device();
    ~Device();

    void replaceBattery();
    void startSession();
    void pauseSession();
    void stopSession();
    void power(bool value);

private:
    bool headsetConn;
    bool pcConn;
    int battery;
    QDateTime *currDate;
    QTimer *sessionTimer;

public slots:
    void readBaseline();

signals:
    int readEEG(int site);
    void updateProgressBar(int percentage);
    int readBaselineSig();
};

#endif // DEVICE_H
