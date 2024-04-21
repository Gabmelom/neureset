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

    void startSession(QDateTime *date);
    void endSession(QDateTime *date);
    void addTreatmentFreq(float);
    void consoleOut();

    // Getters
    QVector<float> getStartBaselines() const { return startBaselines; }
    QVector<float> getEndBaselines() const { return endBaselines; }
    QDateTime getStartDateTime() const { return startDateTime; }
    QDateTime getEndDateTime() const { return endDateTime; }
    int getSessionNumber() const { return sessionNumber; }
    float getStartDomFreq() const { return startDomFreq; }
    float getEndDomFreq() const { return endDomFreq; }
    float getBaseTreatmentFreq() const { return baseTreatmentFreq; }

    // Setters
    void setStartDomFreq(float f) { startDomFreq = f; }
    void setBaseTreatmentFreq(float f) { baseTreatmentFreq = f; }
    void setEndDomFreq(float f) { endDomFreq = f; }
    void setSessionNumber(int n) { sessionNumber = n; }
    void setStartBaselines(QVector<float> b) { startBaselines = b; }
    void setEndBaselines(QVector<float> b) { endBaselines = b; }

private:
    QVector<float> startBaselines;
    QVector<float> endBaselines;
    QVector<float> treatmentFreqs;

    QDateTime startDateTime;
    QDateTime endDateTime;

    int sessionNumber;
    float startDomFreq;
    float baseTreatmentFreq;
    float endDomFreq;

};

#endif // SESSIONLOG_H
