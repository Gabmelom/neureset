#include "SessionLog.h"
#include <QDebug>

SessionLog::SessionLog(int sn) : sessionNumber(sn)
{
}

SessionLog::~SessionLog()
{

}

void SessionLog::startSession()
{
    startDateTime = QDateTime::currentDateTime();
    qInfo()<<"Session "<<sessionNumber<<" started";
}

void SessionLog::endSession()
{
    endDateTime = QDateTime::currentDateTime();
    qInfo()<<"Session "<<sessionNumber<<" ended";
}

void SessionLog::addStartBaselines(QVector<QVector<float>> baseline)
{
    startBaselines = baseline;
    qInfo()<<"store start baseline";
}

void SessionLog::addEndBaselines(QVector<QVector<float>> baseline)
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

void SessionLog::pushTreatmentFreqs(QVector<QVector<float>> freqs){    //4 pairs of freq amp for each wavelength
    treatmentFreqs.push_back(freqs);
}
void SessionLog::pushOffset(float freq){
    offsetFreqs.push_back(freq);
}
