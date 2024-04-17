#ifndef SESSIONLOG_H
#define SESSIONLOG_H

#include <QVector>
#include <QDateTime>
#include <QString>

using namespace std;

class SessionLog
{
public:
    SessionLog(int sn);
    ~SessionLog();

    void addStartBaselines(QVector<QVector<float>> baseline);
    void addEndBaselines(QVector<QVector<float>> baseline);

    void consoleOut();
    void pushTreatmentFreqs(QVector<QVector<float>>);
    void pushOffset(float);

    void startSession();
    void endSession();
    

    // Getters
    QVector<QVector<float>> getStartBaseline() const { return startBaselines; }
    QVector<QVector<float>> getEndBaseline() const { return endBaselines; }
    QDateTime getStartDateTime() const { return startDateTime; }
    QDateTime getEndDateTime() const { return endDateTime; }
    int getSessionNumber() const { return sessionNumber; }
    float getStartDomFreq() { return startDomFreq; }
    float getEndDomFreq() { return endDomFreq; }

    // Setters
    void setStartDomFreq(float f) { startDomFreq = f; }
    void setEndDomFreq(float f) { endDomFreq = f; }
    void setRound(int r) { currentRound = r; }
    void setSessionNumber(int n) { sessionNumber = n; }

private:
    QVector<QVector<float>> startBaselines;
    QVector<QVector<float>> endBaselines;

    QDateTime startDateTime;
    QDateTime endDateTime;

    int currentRound;
    int sessionNumber;
    float startDomFreq;
    float endDomFreq;
    QVector<QVector<QVector<float>>> treatmentFreqs;  //collection of frequencies throughout the treatments
    QVector<float> offsetFreqs;

};

#endif // SESSIONLOG_H
