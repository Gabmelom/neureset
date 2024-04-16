#include "Device.h"
#include "Headset.h"
#include "SessionLog.h"

#include "QDebug"
#include "QVector"
#include "QThread"



Device::Device(QProgressBar* progress,QLabel *r, QLabel *b, QLabel *g, QProgressBar* batteryBar) : progress(progress), redlight(r), bluelight(b), greenlight(g), batteryBar(batteryBar){
    headset = new Headset(7,this);
    currDate = new QDateTime(QDateTime::currentDateTime());
    batteryLife = 100; //stored as an int, should be a flloat once exact calculations are written
    batteryBar->setValue(100);
    powerState = 0;
    headsetConn = 0;
    sessionStage = NO_STAGE;
    turnOffBluelight();
    turnOffRedlight();
    turnOffGreenlight();
    rounds = 0;
    sessionsDone = 0;
    offset = 5;
    pcConn = false;
      
    connect(&pauseTimer, &QTimer::timeout, this, &Device::pauseTimeout);
}

Device::~Device(){
    delete headset;
    delete currDate;
    // Clear logs
    for (int i = 0; i < logs.length(); i++){
        delete logs[i];
    }
}

//admin functions that simmulate  hardware managament

void Device::replaceBattery(){
    qDebug() << "Changing the battery, was at " << batteryLife;
    if (powerState){
        qInfo("turrn off the device  before changing the battery");
    } else {
        batteryLife = 100;
        batteryBar->setValue(batteryLife);
        pauseTimer.stop();
    }

}

// Follows sequence diagram for the main use case
void Device::startSession(){
    if(!powerState){
        qInfo("Device is off");
        return;
    }
    if(headsetConn){
        if (sessionStage != 0){
            resumeSession();
            return;
        }
        turnOnBluelight();
        ongoing = true;

        qDebug("Session started");
        //follows ssequeence ddiagram for the main use case

        currSession = new SessionLog(logs.length() + 1);
        currSession->startSession();

        progress->setValue(15);

        QTimer::singleShot(1000, this, &Device::readStartBaseline);
    }
    else qInfo("Cannot start session without headset connection");
}

void Device::readStartBaseline(){
    if(ongoing && powerState){
        sessionStage = READ_START_BASELINE;
        QVector<QVector<int>> startBaseline = headset->getDomFreq();
        startBaseFreq = calcDomFreq(startBaseline);
        currSession->addStartBaselines(startBaseline);
        currSession->setStartDomFreq(startBaseFreq);
        //ssession duratiion is expected to be constant, the only exception is if it is stopped completely
        //sessLog->addStartBaselines(startBaseFreq);    this might change accorrding to sessionLog format
        //the treatment  bits, according to the recent test doc
        //should put this function in a thread for timing, pausing, and timer
        qDebug() << "starting freq " << startBaseFreq;
        //nummber of rounds of treatments
        //offset added top the dominant frequency. does this change depending on the dominant frequency?

        progress->setValue(28);

        QTimer::singleShot(1000, this, &Device::readTreatmentBaseline);
    }
}

void Device::readTreatmentBaseline(){
    if(ongoing && powerState){
        sessionStage = READ_TREATMENT_BASELINE;
        domFreq = calcDomFreq(headset->getDomFreq());
        qDebug()<<"dom freq for treatment:"<<domFreq;

        QTimer::singleShot(1000, this, &Device::treatment);
    }
}

void Device::treatment(){
    if(ongoing && powerState){
        turnOffGreenlight();
        sessionStage = TREATMENT;
        if (rounds >= ROUNDS)
        {
            readEndBaseline();
        }
        else
        {
            qDebug() << "round " << 1 + rounds;
            qDebug() << "batttery life: " << batteryLife;
            if (batteryLife < 9) {     //19 is for quick testing, something liike 9 gives realistic test results for middle of sessionn. 10 could be used ofr start
                qDebug()<<"Not enough power to continue";
                pauseSession();
                return;
            } else if (batteryLife <= 20) {
                qInfo()<<"battery low: "<<batteryLife<<", please pause and replace";
                //trigger light or different image on UI
            }
//            else if (batteryLife < 10){
//                qInfo()<<"battery is too low to continue: "<<batteryLife<<", stopping current sesssion";
//                stopSession();
//                return;
//            }
            currSession->setRound(1 + rounds);

            QVector<QVector<int>> freqs = headset->getDomFreq();
            //domFreq = calcDomFreq(freqs); //This might or might not be recalculated
            //currSession->pushTreatmentFreqs(freqs); //not sure if this one is necessary, but it is the freequency of each wave at the start of each treatment round
            //over 1 second, apply the domFreq+offset every 1/16 seconds on each node

            progress->setValue(40 + (rounds * 14));

            QTimer::singleShot(1000, this, &Device::treatmentPart2);
        }
    }
}

void Device::treatmentPart2(){
    if(ongoing && powerState){
        sessionStage = TREATMENT_PART_2;

        turnOnGreenlight();
        headset->applyTreatment(domFreq + offset);
        currSession->pushOffset(domFreq + offset);

        offset+=5;
        if (batteryLife < 9){
            qDebug()<<"Not enough power to continue";
            stopSession();
            return;
        }
        batteryLife -= 9;
        batteryBar->setValue(batteryLife);

        rounds++;

        //update window: round i of r complete  (show as percent)

        QTimer::singleShot((7 *150), this, &Device::treatment);
    }
}

void Device::readEndBaseline(){
    if(ongoing && powerState){
        sessionsDone++;

        sessionStage = READ_END_BASELINE;
        QVector<QVector<int>> endBaseline = headset->getDomFreq();

        float endBaseFreq = calcDomFreq(endBaseline);

        currSession->addEndBaselines(endBaseline);


        currSession->setEndDomFreq(endBaseFreq);

        qDebug() << "treatment has been performed. Start baseline: " << startBaseFreq <<" end baseline  " << endBaseFreq;

        currSession->endSession();
        currSession->consoleOut();
        currSession->setSessionNumber(sessionsDone);
        logs.push_back(currSession);

        ongoing = false;
        //turnOffBluelight();
        progress->setValue(100);
        sessionStage = NO_STAGE;

        rounds = 0;
    }

}

void Device::pauseSession(){
    //pause the timer
    //pause any calls to the headset
    qInfo("Session Paused");
    turnOffGreenlight();
    ongoing = false;
    pauseTimer.start(15000);
}

void Device::resumeSession(){
    qInfo("Session Resumed");
    ongoing = true;
    pauseTimer.stop();

    switch(sessionStage)
    {
        case NO_STAGE:
            break;
        case START_SESSION:
            startSession();
            break;
        case READ_START_BASELINE:
            readStartBaseline();
            break;
        case READ_TREATMENT_BASELINE:
            readTreatmentBaseline();
            break;
        case TREATMENT:
            treatment();
            break;
        case TREATMENT_PART_2:
            treatmentPart2();
            break;
        case READ_END_BASELINE:
            readEndBaseline();
            break;
    }
}

void Device::pauseTimeout(){
    qInfo("Session Timeout after 15 seconds");
    // turn off lights
    sessionStage = NO_STAGE;
    turnOffGreenlight();
    turnOffBluelight();
    togglePower();
}

void Device::stopSession(){
    qInfo("Session Stopped");
    sessionStage = NO_STAGE;
    turnOffGreenlight();
    turnOffBluelight();
    ongoing = false;
    pauseTimer.stop();
}

void Device::turnOffBluelight(){
    bluelightOn = false;
    bluelight->setStyleSheet("QLabel { background-color : white;}");
}

void Device::turnOnBluelight(){
    bluelightOn = true;
    bluelight->setStyleSheet("QLabel { background-color : blue;}");
}

void Device::turnOffGreenlight(){
    greenlightOn = false;
    greenlight->setStyleSheet("QLabel { background-color : white;}");
}

void Device::turnOnGreenlight(){
    greenlightOn = true;
    greenlight->setStyleSheet("QLabel { background-color : green;}");
}

void Device::turnOffRedlight(){
    redlightOn = false;
    redlight->setStyleSheet("QLabel { background-color : white;}");
}

void Device::flashRedlight(){
    if(!headsetConn && powerState){
        if(redlightOn){
            redlightOn = false;
            redlight->setStyleSheet("QLabel { background-color : white;}");
            QTimer::singleShot(200, this, &Device::flashRedlight);
        }
        else{
            redlightOn = true;
            redlight->setStyleSheet("QLabel { background-color : red;}");
            QTimer::singleShot(200, this, &Device::flashRedlight);
        }
    }
    else{
        redlight->setStyleSheet("QLabel { background-color : white;}");
    }
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


void Device::togglePower(){
    turnOffBluelight();
    turnOffRedlight();
    turnOffGreenlight();
    pcConn = false;
    powerState = !powerState;
    if(!powerState) qInfo("Turn off device");
    else {
        qInfo("Turn on device");
        sessionStage = NO_STAGE;    
        if (headsetConn){
            turnOnBluelight();
        }
    }    
}

void Device::toggleHeadsetConn(){
    //qDebug() << headsetConn;
    headsetConn = !headsetConn;
    // if it is now connected, device stops beeping, turn red light off and resume session if it was paused
    //qDebug() << sessionStage;
    if(headsetConn){
        qInfo("Headset connected");

//        if(bluelightOn){
        if (sessionStage != 0){
            qInfo("Device stops beeping");
            turnOffRedlight();
            resumeSession();
        } /*else {
            turnOnBluelight();
        }*/
        turnOnBluelight();
    }
    // if it is now unconnected, device beeps, red light flashes, current session is paused
    else{
        qInfo("Headset NOT connected");
        if (sessionStage != 0){
//        if(bluelightOn){
            qInfo("Device start beeping");
            flashRedlight();
            pauseSession();
        } /*else {
            turnOffBluelight();
        }*/
        turnOffBluelight();
    }
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

QDateTime* Device::getDate(){
    return currDate;
}

bool Device::getPowerState(){
    return powerState;
}

bool Device::isOngoing(){
    return ongoing;
}

int Device::getSessionStage(){
    return sessionStage;

}

bool Device::getHeadsetConn(){
    return headsetConn;
}
int Device::getBatteryLife(){
    return batteryLife;
}
bool Device::getPower(){
    return powerState;
}
QVector<SessionLog*> Device::getLogs(){
    return logs;
}
SessionLog* Device::getCurrSession(){
    return currSession;
}

float Device::calcDomFreq(QVector<QVector<int>> baseFreqs){
    //baseFreqs is a nested vector of freq,amp for the 4 wave  lengths being read
    //caalculates the dominent frequency from the output of headset->getDomFreq
    //equation from the test doc
    qDebug("calculating the dominant frequency");
    int top = (baseFreqs[0][0] * (baseFreqs[0][1] * baseFreqs[0][1]) + baseFreqs[1][0] * (baseFreqs[1][1] * baseFreqs[1][1]) + baseFreqs[2][0] * (baseFreqs[2][1] * baseFreqs[2][1]) + baseFreqs[3][0] * (baseFreqs[3][1] * baseFreqs[3][1]));
    int bot = baseFreqs[0][1] + baseFreqs[1][1] + baseFreqs[2][1]  + baseFreqs[3][1];
    return (top / bot);
}

void Device::uploadLogs(PC *pc){
    if (pcConn)
    {
        for (int i = 0; i < logs.length(); i++){
            pc->uploadLog(logs[i]);
        }
    }
    else
    {
        qInfo("Please connection to PC");
    }
}

int readBaselineSig(){
    return 0;
}

void Device::connToPC()
{
    pcConn = !pcConn;
    if (pcConn)
    {
        qInfo("PC is now connected");
    }
    else
    {
        qInfo("PC is disconnected");
    }

}
