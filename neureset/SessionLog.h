#ifndef SESSIONLOG_H
#define SESSIONLOG_H

#include <QVector>
#include <QDateTime>
#include <QString>

using namespace std;

class SessionLog
{
public:
    SessionLog();
    ~SessionLog();

    void addStartBaselines(vector<int> baseline);
    void addEndBaselines(vector<int> baseline);

    void consoleOut();

//    bool getOngoing();

    int getCurrentRound();
//    void setOngoing(bool);
    float getstartBaseFreq();
    float getEndBaseFreq();

    void setStartDomFreq(float);
    void setEndDomFreq(float);
    void setStartDateTime(QString);
    void setEndDateTime(QString);
    void setRound(int);
    void pushTreatmentFreqs(QVector<QVector<int>>);
    void pushOffset(float);
private:

/*    bool ongoing; */      //if the session is in progress, good to check the state of the device
    //stores some info on the current session

    QVector<int> startBaselines;
    QVector<int> endBaselines;
    QDateTime startDateTime;
    QDateTime endDateTime;


    float startDomFreq;
    QVector<QVector<QVector<int>>> treatmentFreqs;  //collection of frequencies throughout the treatments
    QVector<float> offsetFreqs;
    float endDomFreq;
    QString startTime;  //using the QDateTime objeect's toString method
    QString endTime;
    int currentRound;

};

#endif // SESSIONLOG_H
