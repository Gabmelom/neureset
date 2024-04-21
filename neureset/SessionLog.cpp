#include "SessionLog.h"
#include <QDebug>
#include <QDateTime>

SessionLog::SessionLog(int sn) : sessionNumber(sn)
{
    startDomFreq = 0;
}

SessionLog::~SessionLog()
{

}

void SessionLog::startSession(QDateTime *date)
{
    startDateTime = *date;
}

void SessionLog::endSession(QDateTime *date)
{
    endDateTime = *date;
}

void SessionLog::addTreatmentFreq(float freq){
    treatmentFreqs.push_back(freq);
}

void SessionLog::consoleOut(){
    qDebug()<<"Session "<<sessionNumber<<" log: ";
    qDebug()<<"Start datetime "<<startDateTime.toString();
    qDebug()<<"End datetime "<<endDateTime.toString();
    qDebug()<<"Start dominant frequency: "<<startDomFreq<<" Hz";
    qDebug()<<"End dominant frequency: "<<endDomFreq<<" Hz";
    qDebug()<<"Base treatment frequency: " << baseTreatmentFreq;
    qDebug()<<"Start baselines: ";
    for (int i = 0; i < startBaselines.size(); i++){
        qDebug()<<"Site "<<i+1<<": "<<startBaselines[i];
    }
    qDebug()<<"End baselines: ";
    for (int i = 0; i < endBaselines.size(); i++){
        qDebug()<<"Site "<<i+1<<": "<<endBaselines[i];
    }
    qDebug()<<"Offset treatment frequencies: ";
    for (int i = 0; i < treatmentFreqs.size(); i++){
        qDebug()<<"Round "<<i+1<<": "<<treatmentFreqs[i] << " Hz";
    }    
}
