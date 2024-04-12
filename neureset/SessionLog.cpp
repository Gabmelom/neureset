#include "SessionLog.h"
#include <QDebug>

SessionLog::SessionLog() {

}

SessionLog::~SessionLog()
{

}

void SessionLog::addStartBaselines(QVector<QVector<int>> baseline)
{
    startBaselines = baseline;
    qInfo()<<"store start baseline";
}

void SessionLog::addEndBaselines(QVector<QVector<int>> baseline)
{
    endBaselines = baseline;
    qInfo()<<"store end baseline";
}

void SessionLog::consoleOut(){
    qInfo()<<"outputting log";
    qInfo()<<"startDomFreq: "<<startDomFreq;
    qInfo()<<"endDomFreq: "<<endDomFreq;
    //qInfo()<<"treatments:";
    //treatmentFreqs.toStdVector();
    qInfo()<<"offset treatments:    "<<offsetFreqs;
//    for (int i = 0; i < offsetFreqs.length(); i++){
//        qInfo()<<"\r   "<<offsetFreqs[i];
//    }
    //qInfo()<<"";

}

void SessionLog::setStartDomFreq(float freq){
    startDomFreq = freq;
}
void SessionLog::setEndDomFreq(float freq){
    endDomFreq = freq;
}
void SessionLog::setStartDateTime(QString str){
    startTime = str;
}
void SessionLog::setEndDateTime(QString str){
    endTime = str;
}
void SessionLog::setRound(int r){
    currentRound = r;
}
void SessionLog::pushTreatmentFreqs(QVector<QVector<int>> freqs){    //4 pairs of freq amp for each wavelength
    treatmentFreqs.push_back(freqs);
}
void SessionLog::pushOffset(float freq){
    offsetFreqs.push_back(freq);
}
