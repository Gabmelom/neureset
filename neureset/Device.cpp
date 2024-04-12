#include "Device.h"
#include "Headset.h"
#include "SessionLog.h"

#include "QDebug"
#include "QVector"
#include "QThread"



Device::Device(QListWidget* list, QProgressBar* progress) : list(list), progress(progress){
    headset = new Headset(7,this);
    pc = new PC();   //immplement after that  class has been maade
    currDate = new QDateTime(QDateTime::currentDateTime());
    batteryLife = 100; //stored as an int, should be a flloat once exact calculations are written
    powerState = 0;
    sessionStage = NO_STAGE;

    connect(&pauseTimer, &QTimer::timeout, this, &Device::pauseTimeout);
}

Device::~Device(){
    delete headset;
    delete currDate;
}

void Device::replaceBattery(){
    qDebug("Changing the battery");
    batteryLife = 100;
}

void Device::startSession(){
    sessionNum++;
    offset = 5;
    rounds = 0;
    sessionStage = START_SESSION;
    ongoing = true;

    qDebug("Session started");
    //follows ssequeence ddiagram for the main use case

    //stores all important info over the entire session
    //SessionLog *sessLog = new SessionLog();
    currSession = new SessionLog();
    currSession->setStartDateTime(currDate->toString());

    progress->setValue(15);

    QTimer::singleShot(1000, this, &Device::readStartBaseline);
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
        //the treatment bits, according to the recent test doc
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
        sessionStage = TREATMENT;
        if (rounds >= ROUNDS)
        {
            readEndBaseline();
        }
        else
        {
            qDebug() << "round " << 1 + rounds;
            currSession->setRound(1 + rounds);

            QVector<QVector<int>> freqs = headset->getDomFreq();
            //domFreq = calcDomFreq(freqs); //This might or might not be recalculated
            //currSession->pushTreatmentFreqs(freqs); //not sure if this one is necessary, but it is the freequency of each wave at the start of each treatment round
            //over 1 second, apply the domFreq+offset every 1/16 seconds on each node
            //toggle green light on

            progress->setValue(40 + (rounds * 14));

            QTimer::singleShot(1000, this, &Device::treatmentPart2);
        }
    }
}

void Device::treatmentPart2(){
    if(ongoing && powerState){
        sessionStage = TREATMENT_PART_2;
        headset->applyTreatment(domFreq + offset);
        //toggle green light off
        currSession->pushOffset(domFreq + offset);
        offset+=5;
        rounds++;
        //update window: round i of r complete  (show as percent)

        QTimer::singleShot((7 *150), this, &Device::treatment);
    }
}

void Device::readEndBaseline(){
    if(ongoing && powerState){
        sessionStage = READ_END_BASELINE;
        QVector<QVector<int>> endBaseline = headset->getDomFreq();

        float endBaseFreq = calcDomFreq(endBaseline);

        currSession->addEndBaselines(endBaseline);

        currSession->setEndDomFreq(endBaseFreq);

        qDebug() << "treatment has been performed. Start baseline: " << startBaseFreq <<" end baseline  " << endBaseFreq;

        currSession->setEndDateTime(currDate->toString());

        currSession->consoleOut();
        logs.push_back(currSession);

        list->addItem(QString("Session %1       Date: %2").arg(sessionNum).arg(currDate->toString()));
        ongoing = false;
        progress->setValue(100);
    }
}

void Device::pauseSession(){
    //pause the timer
    //pause any calls to the headset
    qInfo("Session Paused");
    qInfo("Device start beeping");
    // flash red light
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
            sessionNum--;
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
    sessionStage = -1;
    sessionNum--;
    togglePower();
}

void Device::stopSession(){
    qInfo("Session Stopped");
    sessionNum--;
    sessionStage = -1;
    ongoing = false;
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

void Device::togglePower(){
    if(powerState) qInfo("Turn off device");
    else qInfo("Turn on device");

    powerState = !powerState;
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

void Device::uploadSessionLog(int selected)
{
    SessionLog *log = logs[selected];
    pc->uploadLog(log);
}

int readEEG(int site){
    return site;
}

int readBaselineSig(){
    return 0;
}
