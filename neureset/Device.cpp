#include "Device.h"
#include "Headset.h"
#include "SessionLog.h"

#include <QDebug>
#include <QVector>
#include <QThread>

#include <cmath>

Device::Device() {
    headset = new Headset(7,this);
    currDate = new QDateTime(QDateTime::currentDateTime());
    endTime = *currDate;
    batteryLife = 100;
    powerState = 0;
    headsetConn = 0;
    sessionStage = NO_STAGE;
    toggleBluelight(false);
    toggleRedlight(false);
    toggleGreenlight(false);
    updateBatteryLevel(batteryLife);
    progress = 0;
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
    if(batteryLife < 25){
        qInfo("Device does not have enough battery to complete a session.");
        updateProgressMessage("Replace the battery to start a new session.");
        return;
    }
    if(headsetConn){
        if (sessionStage != NO_STAGE){
            resumeSession();
            return;
        }
        updateProgressMessage("Starting session...");
        toggleBluelight(true);
        ongoing = true;

        sessionStage = START_SESSION;
        currSession = new SessionLog(logs.length() + 1);
        currSession->startSession(currDate);

        progress = 15;
        updateProgressBar(progress);

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
        toggleGreenlight(false);
        sessionStage = TREATMENT;
        endTime = endTime.addSecs(300);
        if (rounds >= ROUNDS)
        {
            batteryLife -= 3;
            updateBatteryLevel(batteryLife);
            updateProgressMessage("Calculating final baselines...");
            QTimer::singleShot(5000, this, &Device::readEndBaseline);
        }
        else
        {
            qInfo() << "Starting round " << (1 + rounds) << " of " << ROUNDS;
            updateProgressMessage("Starting round " + QString::number(1 + rounds) + " of " + QString::number(ROUNDS));
            if (batteryLife < 1) {
                updateProgressMessage("Low battery, please replace...");
                pauseSession();
                return;
            }

            // Update UI
            progress = 40 + (rounds * 14);
            batteryLife -= 3;

            updateProgressBar(progress);
            updateBatteryLevel(batteryLife);

            QTimer::singleShot(1000, this, &Device::treatmentPart2);
        }
    }
}

void Device::treatmentPart2(){  // treatment application
    if(ongoing && powerState){
        sessionStage = TREATMENT_PART_2;

        toggleGreenlight(true);
        updateProgressMessage("Applying therapy...");
        applyTherapy(treatmentFrequency + offset);
        currSession->addTreatmentFreq(treatmentFrequency + offset);

        offset+=5;
        if (batteryLife <= 10){
            updateProgressMessage("Low battery, please replace...");
            pauseSession();
            return;
        }
        // batteryLife -= 9;
        // updateBatteryLevel(batteryLife); // UI update
        rounds++;

        qInfo() << "Round " << rounds << " complete";
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
// Optionally graph the waveforms of a random site
QVector<float> Device::readBaselines(E_SESSION_STAGE stage){
    
    FREQ_BAND omittedBand = (FREQ_BAND)(rand() % 5); // Omit a random frequency band
    int siteGraphed = rand() % headset->getNumNodes(); // Random site to graph
    
    QVector<float> dominantFrequencies;

    for (int i = 0; i < headset->getNumNodes(); i++){
        QVector<WaveForm> siteWaveforms = headset->getSiteWaveForms(omittedBand);
        
        // Determine the dominant frequency for the site
        float domFreq = calcDomFreq(siteWaveforms);
        dominantFrequencies.push_back(domFreq);

        // Store starting waveforms in the session log
        if (stage == READ_START_BASELINE){
            currSession->addStartWaveForms(siteWaveforms);
        }

        // Graph a sample waveform during the treatment
        if (i == siteGraphed && stage == READ_TREATMENT_BASELINE){
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

        QVector<float> startBaselines = readBaselines(READ_START_BASELINE);
        float dominantFrequency = avgDomFreq(startBaselines);

        // Log information
        currSession->setStartDomFreq(dominantFrequency);
        currSession->setStartBaselines(startBaselines);
        endTime = currDate->addSecs(60);

        qDebug() << "Average starting baseline: " << dominantFrequency;

        // Update UI
        progress = 28;
        batteryLife -= 5;
        updateProgressBar(progress);
        updateBatteryLevel(batteryLife);

        QTimer::singleShot(1000, this, &Device::readTreatmentBaseline);
    }
}

void Device::readTreatmentBaseline(){
    if(ongoing && powerState){
        sessionStage = READ_TREATMENT_BASELINE;

        updateProgressMessage("Calculating treatment baseline...");

        QVector<float> treatmentBaselines = readBaselines(READ_TREATMENT_BASELINE);
        treatmentFrequency = avgDomFreq(treatmentBaselines);

        // Log information
        currSession->setBaseTreatmentFreq(treatmentFrequency);

        qDebug() << "Treatment frequency: " << treatmentFrequency;

        // Update UI
        progress = 30;
        batteryLife -= 5;
        updateProgressBar(progress);
        updateBatteryLevel(batteryLife);

        endTime = endTime.addSecs(60);

        QTimer::singleShot(5000, this, &Device::treatment); //waits 5 seconds
    }
}

void Device::readEndBaseline(){
    if(ongoing && powerState){
        sessionsDone++;

        endTime = endTime.addSecs(300);

        sessionStage = READ_END_BASELINE;

        // Add random offsets to starting waveforms, calculate end dominant frequency
        QVector<float> endBaselines;
        for (int i = 0; i < currSession->getStartWaveForms().length(); i++){
            QVector<WaveForm> endWaveForms;
            for (int j = 0; j < currSession->getStartWaveForms()[i].length(); j++){
                WaveForm currWave = currSession->getStartWaveForms()[i][j];
                currWave.frequency += (rand() % 1);
                endWaveForms.push_back(currWave);
            }
            float domFreq = calcDomFreq(endWaveForms);
            endBaselines.push_back(domFreq);
            currSession->addEndWaveForms(endWaveForms);
        }

        float endBaseFreq = avgDomFreq(endBaselines);

        // Update UI
        batteryLife -= 5;
        progress = 100;
        updateProgressBar(progress);
        updateBatteryLevel(batteryLife);

        // Log information
        updateProgressMessage("Session complete");
        qInfo() << "Therapy session #" << sessionsDone << " complete";
        currSession->setSessionNumber(sessionsDone);
        currSession->setEndBaselines(endBaselines);
        currSession->setEndDomFreq(endBaseFreq);
        currSession->endSession(&endTime);
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
    updateProgressMessage("Session Paused");
    toggleGreenlight(false);
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
    toggleGreenlight(false);
    toggleBluelight(false);
    togglePower();
}

void Device::stopSession(){
    qInfo("Session Stopped");
    sessionStage = NO_STAGE;
    rounds = 0;
    toggleGreenlight(false);
    toggleBluelight(false);
    ongoing = false;
    pauseTimer.stop();
}

// Admin functions that simulate hardware management

void Device::togglePower(){
    toggleBluelight(false);
    toggleRedlight(false);
    toggleGreenlight(false);
    powerState = !powerState;
    if(!powerState) qInfo("Turn off device");
    else {
        qInfo("Turn on device");
        sessionStage = NO_STAGE;    
        if (headsetConn){
            toggleBluelight(true);
        }
    }    
}

void Device::toggleHeadset(){
    headsetConn = !headsetConn;
    uiToggleHeadset(headsetConn);
    // if it is now connected, device stops beeping, turn red light off and resume session if it was paused
    if(headsetConn){

        if (sessionStage != NO_STAGE){
            qInfo("Device stops beeping");
            toggleRedlight(false);
            resumeSession();
        } 
        toggleBluelight(true);
    }
    // if it is now unconnected, device beeps, red light flashes, current session is paused
    else{
        if (sessionStage != NO_STAGE){
            qInfo("Device start beeping");
            flashRedlight();
            pauseSession();
        } 
        toggleBluelight(false);
    }
}

void Device::togglePC()
{
    pcConn = !pcConn;
    uiTogglePC(pcConn);
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
        qInfo("Please connect to PC");
    }
}

void Device::toggleRedlight(bool state){
    redlightOn = state;
    uiToggleRedlight(state);
}

void Device::toggleGreenlight(bool state){
    greenlightOn = state;
    uiToggleGreenlight(state);
}

void Device::toggleBluelight(bool state){
    bluelightOn = state;
    uiToggleBluelight(state);
} 

void Device::flashRedlight(){
    if(!headsetConn && powerState){
        redlightOn = !redlightOn;
        toggleRedlight(redlightOn);
        QTimer::singleShot(200, this, &Device::flashRedlight);
    }
    else{
        redlightOn = false;
        toggleRedlight(false);
    }
}

void Device::setBatteryLevel(int level){
    batteryLife = level;
    updateBatteryLevel(level); // UI update
}
