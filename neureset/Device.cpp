#include "Device.h"
#include "Headset.h"
#include "SessionLog.h"

#include "QDebug"
#include "QVector"
#include "QThread"


Device::Device(){
    headset = new Headset(7,this);
    //pcConn = new PC   //immplement after that  class has been maade
    sessionTimer = new QTimer();   //might move creation into start session. needs to be part of the class to be gotten from the UI
    currDate = new QDateTime();
    batteryLife = 100; //stored as an int, should be a flloat once exact calculations are written
    powerState = 0;

}

Device::~Device(){
    delete headset;
    delete sessionTimer;
    delete currDate;
}

void Device::replaceBattery(){
    qDebug("Changing the baattery");
    batteryLife = 100;
}

void Device::startSession(){
    qDebug("Session started");
    //follows ssequeence ddiagram for the main use case

    //stores all important info over the entire session
    SessionLog *sessLog = new SessionLog();

    float startBaseFreq = calcDomFreq(headset->getDomFreq());
    //ssession duratiion is expected to be constant, the only exception is if it is stopped completely
    //sessLog->addStartBaselines(startBaseFreq);    this might change accorrding to sessionLog format
    //the treatment bits, according to the recent test doc
    //should put this function in a thread for timing, pausing, and timer
    int r = 4;  //nummber of rounds of treatments
    int offset = 5;    //offset added top the dominant  frequency
    float domFreq;
    for (int i = 0;i < r; i++){
        qDebug("round ",i);
        domFreq = calcDomFreq(headset->getDomFreq());
        //over 1 second, apply the domFreq+offset every 1/16 seconds on each node
        headset->applyTreatment(domFreq+offset);
        offset+=5;

    }
    float endBaseFreq = calcDomFreq(headset->getDomFreq());
    qDebug("treatment has been proformed. Start baseline: ",startBaseFreq," end baseline  ",endBaseFreq);
}

void Device::pauseSession(){
    //pause the timer
    //pause any calls to the headset
}

void Device::stopSession(){
    //reset state
    //saves the current session log?
}

void Device::setPower(bool val){
    powerState = val;
}

QVector<int> Device::readBaseline(){
    //function for the complicated baseline, to be implemented if it seeems necessary (still looking through Q/A for details)
    QVector<float>  avg;
    avg.fill(0,21);
//    QThread *thread = new QThread();

//    QTimer *timer = new QTimer();

    QVector<int> baseline = headset->readBase();


    //process numbers
    //maybe add them  to the log here, probably should be done in the maain process loop
    return baseline;
}

bool Device::applyTherapy(){
    //might not be necessary since the device can call the headset functionn, this should be used if extra steps are necessary
    //over all sites
    //caall the headset to get the baselines
    //use the baselines to get the incremented freq
    //repeat over x intervals
    //end of roound stuff (if it exists)
    return true;    //if the treatment round was successful, not sure if there are  fail cases yet (maybe prelimitory safeety checking)
}

float Device::calcDomFreq(QVector<QVector<int>> baseFreqs){
    //baseFreqs is a nested vector of freq,amp for the 4 wave  lengths being read
    //caalculates the dominent frequency from the output of headset->getDomFreq
    //equation from the test doc
    int top = (baseFreqs[0][0] * (baseFreqs[0][1] * baseFreqs[0][1]) + baseFreqs[1][0] * (baseFreqs[1][1] * baseFreqs[1][1]) + baseFreqs[2][0] * (baseFreqs[2][1] * baseFreqs[2][1]) + baseFreqs[3][0] * (baseFreqs[3][1] * baseFreqs[3][1]));
    int bot = baseFreqs[0][1] + baseFreqs[1][1] + baseFreqs[2][1]  + baseFreqs[3][1];
    return (top / bot);
}


int readEEG(int site){

}

int readBaselineSig(){

}
