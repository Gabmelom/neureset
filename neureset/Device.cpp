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
    setBatteryLevel(100);
    warningPopupShown = false;
    powerState = 0;
    headsetConn = 0;
    sessionStage = NO_STAGE;
    toggleBluelight(false);
    toggleRedlight(false);
    toggleGreenlight(false);
    progress = 0;
    rounds = 0;
    sessionsDone = 0;
    offset = 5;
    pcConn = false;
    ongoing = false;
      
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
        pauseTimer.stop();

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
    if(ongoing && powerState && !pauseTimer.isActive()){
        toggleGreenlight(false);
        sessionStage = TREATMENT;
        endTime = endTime.addSecs(300);
        if (rounds >= ROUNDS)
        {
            setBatteryLevel(batteryLife -3);
            updateProgressMessage("Calculating final baselines...");
            updateETALabel("ETA: 60 s");
            // 'Calculate' for 5 seconds
            int step = (95 - progress) / 5; 
            while (progress < 95)
            {
                progress += step;
                updateProgressBar(progress);
                QThread::msleep(1000);
            }
            readEndBaseline();
        }
        else
        {
            qInfo() << "Starting round " << (1 + rounds) << " of " << ROUNDS;
            updateProgressMessage("Starting round " + QString::number(1 + rounds) + " of " + QString::number(ROUNDS));

            // Update UI
            progress = 40 + (rounds * 14);
            updateProgressBar(progress);
            setBatteryLevel(batteryLife - 3);

            QTimer::singleShot(1000, this, &Device::treatmentPart2);
        }
    }
}

void Device::treatmentPart2(){  // treatment application
    if(ongoing && powerState && !pauseTimer.isActive()){
        sessionStage = TREATMENT_PART_2;

        toggleGreenlight(true);
        updateProgressMessage("Applying therapy...");
        applyTherapy(treatmentFrequency + offset);
        currSession->addTreatmentFreq(treatmentFrequency + offset);

        offset+=5;
        rounds++;

        qInfo() << "Round " << rounds << " complete";
        updateProgressMessage("Round " + QString::number(rounds) + " of " + QString::number(ROUNDS) + " complete");
        updateETALabel("ETA: " + QString::number(20 - 2*rounds) + " mins");
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
    if(ongoing && powerState && !pauseTimer.isActive()){
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
        updateProgressBar(progress);
        updateETALabel("ETA: 25 mins");
        setBatteryLevel(batteryLife - 5);

        QTimer::singleShot(1000, this, &Device::readTreatmentBaseline);
    }
}

void Device::readTreatmentBaseline(){
    if(ongoing && powerState && !pauseTimer.isActive()){
        sessionStage = READ_TREATMENT_BASELINE;

        updateProgressMessage("Calculating treatment baseline...");

        QVector<float> treatmentBaselines = readBaselines(READ_TREATMENT_BASELINE);
        treatmentFrequency = avgDomFreq(treatmentBaselines);

        // Log information
        currSession->setBaseTreatmentFreq(treatmentFrequency);

        qDebug() << "Treatment frequency: " << treatmentFrequency;

        // Update UI
        progress = 30;
        updateProgressBar(progress);
        updateETALabel("ETA: 24 mins");
        setBatteryLevel(batteryLife - 5);

        endTime = endTime.addSecs(60);

        QTimer::singleShot(5000, this, &Device::treatment); //waits 5 seconds
    }
}

void Device::readEndBaseline(){
    if(ongoing && powerState && !pauseTimer.isActive()){
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

        ongoing = false;
        sessionStage = NO_STAGE;
        rounds = 0;

        // Update UI
        progress = 100;
        updateProgressBar(progress);
        updateETALabel("ETA: -");
        setBatteryLevel(batteryLife - 5);
        

        // Log information
        updateProgressMessage("Session complete");
        qInfo() << "Therapy session #" << sessionsDone << " complete";
        currSession->setSessionNumber(sessionsDone);
        currSession->setEndBaselines(endBaselines);
        currSession->setEndDomFreq(endBaseFreq);
        currSession->endSession(&endTime);
        currSession->consoleOut();

        logs.push_back(currSession);
    }

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

// Session control functions

void Device::pauseSession(){
    // If the session is already paused, do nothing
    if (pauseTimer.isActive()){
        qInfo("Session already paused");
        return;
    }
    qInfo("Session paused");
    toggleGreenlight(false);

    // Count 15 seconds before timing out
    pauseTimerCount = 15;
    pauseTimer.start(1000); 
}

// Timeout function for pausing the session, shuts down the device after 15 seconds
void Device::pauseTimeout(){
    if (pauseTimerCount > 0){
        qInfo() << "Session paused. Time remaining: " << pauseTimerCount << " seconds";
        updateProgressMessage("Session paused. Timeout in " + QString::number(pauseTimerCount) + "...");
        pauseTimerCount--;
        return;
    }
    qInfo("Session timed out after 15 seconds");
    updateProgressMessage("Session timed out after 15 seconds.");
    stopSession();


    uiShowPopup("Session timed out.\n Shutting down.");
    QTimer::singleShot(2000, this, &Device::togglePower);
}

void Device::stopSession(){
    pauseTimer.stop();
    qInfo("Session stopped");
    updateProgressMessage("Session stopped");
    sessionStage = NO_STAGE;
    rounds = 0;
    toggleGreenlight(false);
    ongoing = false;
}

// Admin functions that simulate hardware management

void Device::setBatteryLevel(int level){
    batteryLife = level;
    updateBatteryLevel(level); // UI update

    // Check if battery is low
    if (powerState){
        if (batteryLife <= 1) {
            updateProgressMessage("Battery critically low, shutting down...");
            qInfo("Battery critically low, shutting down...");
            uiShowPopup("WARNING: No battery.\n Shutting down.");
            if (isOngoing()) 
                stopSession();
            QTimer::singleShot(2000, this, &Device::togglePower);
            return;
        }
        if (batteryLife <= 10){
            updateProgressMessage("Battery low, pausing session...");
            if (isOngoing())
                pauseSession();
        }
        if (batteryLife <= 25 && !warningPopupShown){
            uiShowPopup("WARNING: \nDevice battery low");
            warningPopupShown = true;
            return;
        }
    }
}

void Device::togglePower(){
    toggleBluelight(false);
    toggleRedlight(false);
    toggleGreenlight(false);
    powerState = !powerState;
    uiTogglePower(powerState);
    warningPopupShown = false;
    if(!powerState){
        qInfo("Turning off device");
        uiHidePopup();
        stopSession();
    } 
    else {
        qInfo("Turning on device");
        uiHidePopup();
        setBatteryLevel(batteryLife); // Check battery level, update UI
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

// UI functions

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
