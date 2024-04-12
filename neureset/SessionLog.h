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

    void addStartBaselines(QVector<QVector<int>> baseline);
    void addEndBaselines(QVector<QVector<int>> baseline);

    void consoleOut();
    void pushTreatmentFreqs(QVector<QVector<int>>);
    void pushOffset(float);

    void startSession();
    void endSession();
    

    // Getters
    QVector<QVector<int>> getStartBaseline() const { return startBaselines; }
    QVector<QVector<int>> getEndBaseline() const { return endBaselines; }
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
    QVector<QVector<int>> startBaselines;
    QVector<QVector<int>> endBaselines;
    QDateTime startDateTime;
    QDateTime endDateTime;

    int currentRound;
    int sessionNumber;
    float startDomFreq;
    float endDomFreq;
    QVector<QVector<QVector<int>>> treatmentFreqs;  //collection of frequencies throughout the treatments
    QVector<float> offsetFreqs;

};

#endif // SESSIONLOG_H
