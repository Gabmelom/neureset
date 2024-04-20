#include "Device.h"
#include "Headset.h"
#include "SessionLog.h"

#include <QDebug>
#include <QVector>
#include <QThread>

#include <cmath>

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

// Follows sequence diagram for the main use case
void Device::startSession(){
    if(!powerState){
        qInfo("Device is off");
        return;
    }
    if(batteryLife < (ROUNDS * 9)){
        qInfo("Device does not have enough battery to complete a session.");
        updateProgressMessage("Replace the battery to start session.");
        return;
    }
    if(headsetConn){
        if (sessionStage != 0){
            resumeSession();
            return;
        }
        updateProgressMessage("Starting session...");
        turnOnBluelight();
        ongoing = true;

        sessionStage = START_SESSION;
        currSession = new SessionLog(logs.length() + 1);
        currSession->startSession();

        progress->setValue(15); // TODO: Extract to signal instead

        QTimer::singleShot(1000, this, &Device::readStartBaseline);
    }
    else{
        qInfo("Cannot start session without headset connection");   
        updateProgressMessage("Connect headset to start session");
    }
     
}

void Device::applyTherapy(float freq){
    headset->applyTreatment(freq);
}

void Device::treatment(){   //between rounds
    if(ongoing && powerState){
        turnOffGreenlight();
        sessionStage = TREATMENT;
        if (rounds >= ROUNDS)
        {
            QTimer::singleShot(5000, this, &Device::readEndBaseline);
        }
        else
        {
            // qDebug() << "Batttery life: " << batteryLife;
            qDebug() << "Starting round " << 1 + rounds;

            if (batteryLife < 9) {     //19 is for quick testing, something liike 9 gives realistic test results for middle of sessionn. 10 could be used ofr start
                qDebug()<<"Not enough power to continue";
                checkBatteryLevel();
                pauseSession();
                return;
            } else if (batteryLife <= 20) {
                qInfo()<<"battery low: "<<batteryLife<<", please pause and replace";
                checkBatteryLevel();
            }

            progress->setValue(40 + (rounds * 14)); // TODO: extract to MainWindow

            QTimer::singleShot(1000, this, &Device::treatmentPart2);
        }
    }
}

void Device::treatmentPart2(){  //round bit
    if(ongoing && powerState){
        sessionStage = TREATMENT_PART_2;

        turnOnGreenlight();
        updateProgressMessage("Applying therapy...");
        applyTherapy(treatmentFrequency + offset);
        currSession->addTreatmentFreq(treatmentFrequency + offset);

        offset+=5;
        if (batteryLife < 9){
            qDebug()<<"Not enough power to continue";
            stopSession();
            return;
        }
        batteryLife -= 9;
        batteryBar->setValue(batteryLife);
        checkBatteryLevel();
        rounds++;

        //update window: round i of r complete  (show as percent)
        updateProgressMessage("Round " + QString::number(rounds) + " of " + QString::number(ROUNDS) + " complete");
        QTimer::singleShot((62*20), this, &Device::treatment);

    }
}

// Calculate a dominant frequency for an EEG site
float Device::calcDomFreq(QVector<WaveForm> waveForms){
    float top = 0;
    float bot = 0;
    for (int i = 0; i < waveForms.length(); i++){
        top += waveForms[i].frequency * pow(waveForms[i].amplitude, 2);
        bot += pow(waveForms[i].amplitude, 2);
    }
    return top / bot;
}

// Calculate the average dominant frequency for all sites
float Device::avgDomFreq(QVector<float> dominantFrequencies){
    float sum = 0;
    for (int i = 0; i < dominantFrequencies.length(); i++){
        sum += dominantFrequencies[i];
    }
    return sum / dominantFrequencies.length();
}

// For each site, calculate the dominant frequency and store it in a vector
// Optionally graph the waveforms of a random site, default is false
QVector<float> Device::readBaselines(bool graph){
    FREQ_BAND omittedBand = (FREQ_BAND)(rand() % 5); // Omit a random frequency band
    int siteGraphed = rand() % headset->getNumNodes(); // Random site to graph
    QVector<float> dominantFrequencies;

    for (int i = 0; i < headset->getNumNodes(); i++){
        QVector<WaveForm> siteWaveforms = headset->getSiteWaveForms(omittedBand);
        float domFreq = calcDomFreq(siteWaveforms);
        dominantFrequencies.push_back(domFreq);

        if (i == siteGraphed && graph){
            emit updateTreatmentGraph(siteWaveforms, i);
        }
    }

    return dominantFrequencies;
}

// Read the starting baselines for each site
void Device::readStartBaseline(){ 
    if(ongoing && powerState){
        sessionStage = READ_START_BASELINE;

        updateProgressMessage("Calculating starting baselines...");

        QVector<float> startBaselines = readBaselines();
        float dominantFrequency = avgDomFreq(startBaselines);

        // Log information
        currSession->setStartDomFreq(dominantFrequency);
        currSession->setStartBaselines(startBaselines);

        qDebug() << "Average starting baseline: " << dominantFrequency;
        progress->setValue(28);

        QTimer::singleShot(1000, this, &Device::readTreatmentBaseline);
    }
}

void Device::readTreatmentBaseline(){
    if(ongoing && powerState){
        sessionStage = READ_TREATMENT_BASELINE;

        updateProgressMessage("Calculating treatment baseline...");

        QVector<float> treatmentBaselines = readBaselines(true);
        treatmentFrequency = avgDomFreq(treatmentBaselines);

        // Log information
        currSession->setBaseTreatmentFreq(treatmentFrequency);

        qDebug() << "Treatment frequency: " << treatmentFrequency;
        progress->setValue(30);

        QTimer::singleShot(5000, this, &Device::treatment); //waits 5 seconds
    }
}

void Device::readEndBaseline(){
    if(ongoing && powerState){
        sessionsDone++;

        sessionStage = READ_END_BASELINE;
        
        updateProgressMessage("Calculating final baselines...");

        // Add random offsets to starting baselines
        QVector<float> endBaselines;
        for (int i = 0; i < currSession->getStartBaselines().length(); i++){
            float offset = (float(rand())/float((RAND_MAX))) * 5 - 2.5;
            endBaselines.push_back(currSession->getStartBaselines()[i] + (rand() % 5));
        }
        float endBaseFreq = avgDomFreq(endBaselines);

        // Log information
        progress->setValue(100);
        updateProgressMessage("Session complete");
        qInfo() << "Therapy session #" << sessionsDone << " complete";
        currSession->setSessionNumber(sessionsDone);
        currSession->setEndBaselines(endBaselines);
        currSession->setEndDomFreq(endBaseFreq);
        currSession->endSession();
        currSession->consoleOut();

        logs.push_back(currSession);

        ongoing = false;
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
    pauseTimer.start(15000);    //15000 for testing (15 secoonds), should be 300000
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
            readTreatmentBaseline();
            break;
        case TREATMENT_PART_2:  //these should return to the baseline reading, since a new baseline should be calculated after pausing (brain state has  changed)
            readTreatmentBaseline();
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
    rounds = 0;
    turnOffGreenlight();
    turnOffBluelight();
    ongoing = false;
    pauseTimer.stop();
}

//admin functions that simmulate  hardware managament

void Device::replaceBattery(){
    qDebug() << "Changing the battery, was at " << batteryLife;
    if (powerState){
        qInfo("turrn off the device  before changing the battery");
    } else {
        batteryLife = 100;
        batteryBar->setValue(batteryLife);
        checkBatteryLevel();
        pauseTimer.stop();
    }

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

void Device::checkBatteryLevel()
{
    if(batteryLife > 20)
    {
        batteryBar->setStyleSheet("selection-background-color: rgb(38, 162, 105);");
    }
    else if (batteryLife <= 20 && batteryLife > 10)
    {
        batteryBar->setStyleSheet("selection-background-color: rgb(229, 165, 10);");
    }
    else
    {
        batteryBar->setStyleSheet("selection-background-color: rgb(192, 28, 40);");
    }
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










// TODO: Decouple UI from class
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









// TODO: Move to own enum class
QString Device::bandToString(FREQ_BAND band){
    switch(band){
        case ALPHA:
            return "Alpha";
        case BETA:
            return "Beta";
        case DELTA:
            return "Delta";
        case THETA:
            return "Theta";
        case GAMMA:
            return "Gamma";
        default:
            return "Unknown";
    }
}
