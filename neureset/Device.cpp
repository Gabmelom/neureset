#include "Device.h"
#include "Headset.h"
#include "SessionLog.h"

#include "QDebug"
#include "QVector"
#include "QThread"


Device::Device(MainWindow *window, QListWidget* list) : window(window), list(list){
    headset = new Headset(7,this);
    //pcConn = new PC   //immplement after that  class has been maade
    currDate = new QDateTime(QDateTime::currentDateTime());
    batteryLife = 100; //stored as an int, should be a flloat once exact calculations are written
    powerState = 0;
}

Device::~Device(){
    delete headset;
    delete currDate;
}

void Device::replaceBattery(){
    qDebug("Changing the baattery");
    batteryLife = 100;
}

void Device::startSession(){
    sessionNum++;

    qDebug("Session started");
    //follows ssequeence ddiagram for the main use case

    //stores all important info over the entire session
    //SessionLog *sessLog = new SessionLog();
    currSession = new SessionLog();
    currSession->setStartDateTime(currDate->toString());

    QTimer::singleShot(1000, this, &Device::readStartBaseline);
}

void Device::readStartBaseline(){
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

    QTimer::singleShot(1000, this, &Device::readTreatmentBaseline);
}

void Device::readTreatmentBaseline(){
    domFreq = calcDomFreq(headset->getDomFreq());
    qDebug()<<"dom freq for treatment:"<<domFreq;

    QTimer::singleShot(1000, this, &Device::treatment);
}

void Device::treatment(){
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

        QTimer::singleShot(1000, this, &Device::treatmentPart2);
    }
}

void Device::treatmentPart2()
{
    headset->applyTreatment(domFreq + offset);
    //toggle green light off
    currSession->pushOffset(domFreq + offset);
    offset+=5;
    rounds++;
    //update window: round i of r complete  (show as percent)

    QTimer::singleShot((7 *150), this, &Device::treatment);
}

void Device::readEndBaseline()
{
    QVector<QVector<int>> endBaseline = headset->getDomFreq();

    float endBaseFreq = calcDomFreq(endBaseline);

    currSession->addEndBaselines(endBaseline);

    currSession->setEndDomFreq(endBaseFreq);

    qDebug() << "treatment has been performed. Start baseline: " << startBaseFreq <<" end baseline  " << endBaseFreq;

    currSession->setEndDateTime(currDate->toString());

    currSession->consoleOut();
    logs.push_back(currSession);

    list->addItem(QString("Session %1       Date: %2").arg(sessionNum).arg(currDate->toString()));
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


int readEEG(int site){
    return site;
}

int readBaselineSig(){
    return 0;
}
