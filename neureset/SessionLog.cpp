#include "SessionLog.h"

SessionLog::SessionLog() {

}

SessionLog::~SessionLog()
{

}

void SessionLog::addStartBaselines(vector<int> baseline)
{

}

void SessionLog::addEndBaselines(vector<int> baseline)
{

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
