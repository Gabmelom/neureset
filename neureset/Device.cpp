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

    QVector<int> startBase = readBaseline();

    //sessLog->addStartBaselines(startBase);

    if (applyTherapy()){

    }
}

void Device::pauseSession(){
    //pause the timer
    //pause any calls to the headset
}

void Device::stopSession(){
    //reset state
}

void Device::setPower(bool val){
    powerState = val;
}

QVector<int> Device::readBaseline(){
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
    //over all sites
    //caall the headset to get the baselines
    //use the baselines to get the incremented freq
    //repeat over x intervals
    //end of roound stuff (if it exists)
    return true;    //if the treatment round was successful, not sure if there are  fail cases yet (maybe prelimitory safeety checking)
}

int readEEG(int site){

}

int readBaselineSig(){

}
