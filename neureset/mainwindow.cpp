#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    init();

    connect(ui->power, SIGNAL(pressed()), this, SLOT(powerPressed()));
    connect(ui->home, SIGNAL(pressed()), this, SLOT(homePressed()));
    connect(ui->up, SIGNAL(pressed()), this, SLOT(upPressed()));
    connect(ui->down, SIGNAL(pressed()), this, SLOT(downPressed()));
    connect(ui->select, SIGNAL(pressed()), this, SLOT(selectPressed()));
    connect(ui->play_pause, SIGNAL(pressed()), this, SLOT(playPausePressed()));
    connect(ui->stop, SIGNAL(pressed()), this, SLOT(stopPressed()));
    connect(ui->DeviceScreen, SIGNAL(currentChanged(int)), this, SLOT(devicePageChanged(int)));
    connect(ui->pcScreen, SIGNAL(currentChanged(int)), this, SLOT(pcPageChanged(int)));
    connect(ui->uploadLogsButton, SIGNAL(pressed()), this, SLOT(uploadLogs()));
    connect(ui->pcLogList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(displayLogDetails(QListWidgetItem*)));
    connect(ui->pcBackButton, SIGNAL(pressed()), this, SLOT(pcBackButtonPressed()));
    connect(ui->dateEdit, SIGNAL(editingFinished()), this, SLOT(changeDate()));
    connect(ui->timeEdit,  SIGNAL(editingFinished()), this, SLOT(changeTime()));
    
    connect(ui->replaceBatteryBtn, SIGNAL(pressed()), this, SLOT(changeBattery()));
    
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    // Initialize device UI elements
    currentDeviceScreen = 0;
    ui->DeviceScreen->setCurrentIndex(currentDeviceScreen);
    currentDeviceList = ui->HomeMenuList;
    currentDeviceList->setCurrentRow(0);
    updateBatteryLife(100);
    toggleBluelight(false);
    toggleRedlight(false);
    toggleGreenlight(false);

    // Main singleton objects
    device = new Device();
    pc = new PC();
    initWaveformGraphs();

    // UI -> Device signals
    connect(ui->pcConnButton, SIGNAL(pressed()), device, SLOT(togglePC()));
    connect(ui->toggleHeadsetBtn, SIGNAL(pressed()), device, SLOT(toggleHeadset()));

    // Device -> UI signals
    connect(device, SIGNAL(updateBatteryLevel(int)), this, SLOT(updateBatteryLife(int)));
    connect(device, SIGNAL(uiTogglePC(bool)), this, SLOT(updatePCToggle(bool)));
    connect(device, SIGNAL(uiToggleHeadset(bool)), this, SLOT(updateHeadsetToggle(bool)));
    connect(device, SIGNAL(uiToggleBluelight(bool)), this, SLOT(toggleBluelight(bool)));
    connect(device, SIGNAL(uiToggleRedlight(bool)), this, SLOT(toggleRedlight(bool)));
    connect(device, SIGNAL(uiToggleGreenlight(bool)), this, SLOT(toggleGreenlight(bool)));
    connect(device, SIGNAL(updateProgressMessage(QString)), this, SLOT(updateProgressMessage(QString)));
    connect(device, SIGNAL(updateProgressBar(int)), this, SLOT(updateProgressBar(int)));
    connect(device, SIGNAL(updateTreatmentGraph(QVector<WaveForm>, int)), this, SLOT(updateTreatmentGraph(QVector<WaveForm>, int)));

    
}

void MainWindow::initWaveformGraphs(){
    using namespace QtCharts;

    // Session treatment graph
    treatmentGraph = new QChart();
    treatmentGraph->legend()->hide();

    treatmentGraphView = new QChartView(treatmentGraph);
    treatmentGraphView->setRenderHint(QPainter::Antialiasing);
    treatmentGraphView->setParent(ui->graphFrame);
    treatmentGraphView->resize(ui->graphFrame->size());

    // PC log details page, start graph
    pcStartGraph = new QChart();
    pcStartGraph->legend()->hide();

    pcStartGraphView = new QChartView(pcStartGraph);
    pcStartGraphView->setRenderHint(QPainter::Antialiasing);
    pcStartGraphView->setParent(ui->startFrame);
    pcStartGraphView->resize(ui->startFrame->size());

    // PC log details page, end graph
    pcEndGraph = new QChart();
    pcEndGraph->legend()->hide();

    pcEndGraphView = new QChartView(pcEndGraph);
    pcEndGraphView->setRenderHint(QPainter::Antialiasing);
    pcEndGraphView->setParent(ui->endFrame);
    pcEndGraphView->resize(ui->endFrame->size()); 
}

QtCharts::QSplineSeries* MainWindow::graphWaveform(QVector<WaveForm> waveforms){
    auto series = new QtCharts::QSplineSeries();

    for (int i = 0; i < GRAPH_X_MAX; i++){
        float y = 0;
        for (auto waveform : waveforms){
            y += waveform.amplitude * sin(waveform.frequency * i);
        }
        series->append(i, y);
    }

    return series;
}

void MainWindow::updateTreatmentGraph(QVector<WaveForm> waveforms, int site){
    treatmentGraph->removeAllSeries();

    auto series = graphWaveform(waveforms);
    treatmentGraph->addSeries(series);
    
    treatmentGraph->createDefaultAxes();
    treatmentGraph->setTitle(QString("EEG Site %1").arg(site+1));
}


void MainWindow::devicePageChanged(int index)
{
    // Page indexes:
    //  0 - off
    //  1 - Menu page
    //  2 - New session page
    //  3- Session log page
    //  4- Time & date page
    currentDeviceScreen = index;
    switch(currentDeviceScreen)
    {
        case 0:
                //qDebug("off");
            break;
        case 1:
            currentDeviceList = ui->HomeMenuList;
            currentDeviceList->setCurrentRow(0);
            break;
        case 2:
            device->startSession();
            break;

        case 3:
            currentDeviceList = ui->deviceLogList;
            currentDeviceList->setCurrentRow(0);
            selectPressed();
            displayDeviceLogs();
            break;

        case 4:
            updateDate();
            break;
            //ui->screen->powerOffPage;
        default:
            currentDeviceList = nullptr;
    }

}

void MainWindow::pcPageChanged(int index)
{
    // Page indexes:
    //  0 - Menu page
    //  1 - Log details page
    currentPCScreen = index;

    switch(currentPCScreen)
    {
        case 0:
            currentPCList = ui->pcLogList;
            displayPCLogs();
            currentPCList->setCurrentRow(0);
            break;
        default:
            currentPCList = nullptr;
    }
}


void MainWindow::powerPressed()
{
    device->togglePower();
    if (device->getPower()){    //power  on
        ui->DeviceScreen->setCurrentIndex(1);
        devicePageChanged(1);
    } else {
        ui->DeviceScreen->setCurrentIndex(0);
        devicePageChanged(0);
    }

}

void MainWindow::homePressed()
{
    if (currentDeviceScreen == 0) return;
    ui->DeviceScreen->setCurrentIndex(1);
}

void MainWindow::upPressed()
{
    if(currentDeviceList == nullptr) return;
    if(currentDeviceList->currentRow() == 0) return;
    currentDeviceList->setCurrentRow(currentDeviceList->currentRow() - 1);
}

void MainWindow::downPressed()
{
    if(currentDeviceList == nullptr) return;
    if(currentDeviceList->currentRow() == currentDeviceList->count() - 1) return;
    currentDeviceList->setCurrentRow(currentDeviceList->currentRow() + 1);
}

void MainWindow::selectPressed()
{
    if(!device->getPowerState()) return;
    if(currentDeviceScreen == 1){ // Home page

        // Menu list options:
        //  0 - New session page
        //  1 - Session log page
        //  2 - Time & date page
        int optionSelected = currentDeviceList->currentRow();
        ui->DeviceScreen->setCurrentIndex(optionSelected+2);
    }
    else if(currentDeviceScreen == 2){ // Session log page
        // TODO: upload session log?
    }
}

void MainWindow::playPausePressed()
{
    if(device->getSessionStage() != NO_STAGE && device->getPowerState()){
        if(device->isOngoing()){
            device->pauseSession();
        }
        else{
            device->resumeSession();
        }
    }
}

void MainWindow::stopPressed()
{
    if(device->getPowerState()) device->stopSession();
}

void MainWindow::pcBackButtonPressed()
{
    ui->pcScreen->setCurrentIndex(0); // Switch to log list page
}


// UI methods for time and date settings

void MainWindow::changeDate(){
    qInfo("changing the date");
    //ui->dateEdit->setDate(device->getDate()->date());
    device->getDate()->setDate(ui->dateEdit->date());
    //qDebug() << device->getDate()->date();
}

void MainWindow::changeTime(){
    qInfo("changing the time");
    //ui->dateEdit->dateTimeChanged()
    device->getDate()->setTime(ui->timeEdit->time());
    //qDebug() << device->getDate()->time();
}

void MainWindow::updateDate(){
   // qInfo("update the date/time");
    ui->dateEdit->setDate(device->getDate()->date());
    ui->timeEdit->setTime(device->getDate()->time());
}

void MainWindow::changeBattery()
{
    qDebug() << "ADMIN: replacing battery";
    device->replaceBattery();
}




// UI methods for PC session history and device logs

// Upload logs from device to PC
void MainWindow::uploadLogs()
{
    device->uploadLogs(pc);
    displayPCLogs();
}

// For each log, render the log to the list, include only the date and session number
void MainWindow::displayDeviceLogs(){
    ui->deviceLogList->clear();
    auto logs = device->getLogs();
    qInfo("Number of device logs: %d", logs.size());

    for (const auto& log : logs) {
        int i = log->getSessionNumber();
        QDateTime date = log->getStartDateTime();
        ui->deviceLogList->addItem(QString("Session %1       Date: %2").arg(i).arg(date.toString()));
    }
}

// For each log, render the log to the list, include only the date and session number
void MainWindow::displayPCLogs()
{
    ui->pcLogList->clear();
    auto logs = pc->getLogs();
    qInfo("Number of pc logs: %d", logs.size());

    for (const auto& log : logs) {

        int sessionNumber = log->getSessionNumber();
        auto date = log->getStartDateTime().toString();
        
        ui->pcLogList->addItem(QString("Session %1       Date: %2").arg(sessionNumber).arg(date));
    }
}

// Display log details for a selected log in the PC session history
void MainWindow::displayLogDetails(QListWidgetItem *item)
{
    ui->pcTable->clearContents();
    pcStartGraph->removeAllSeries();
    pcEndGraph->removeAllSeries();

    int logIndex = ui->pcLogList->row(item);
    auto log = pc->getLogs()[logIndex];
    
    qInfo("Displaying log details for session %d", log->getSessionNumber());
    ui->pcScreen->setCurrentIndex(1); // Switch to log details page

    // Session details
    ui->sessionNumberLabel->setText(QString("Session %1").arg(log->getSessionNumber()));
    ui->sessionDateLabel->setText(log->getStartDateTime().toString());
    auto duration = log->getStartDateTime().secsTo(log->getEndDateTime());
    ui->sessionDurationLabel->setText(QString("Duration: %1 minutes %2 seconds").arg(duration/60).arg(duration%60));
    
    // Format labels for dominant frequencies
    ui->startDomFreqLabel->setText(QString("Avg. start baseline: %1 Hz").arg(log->getStartDomFreq(), 0, 'f', 2));
    ui->endDomFreqLabel->setText(QString("Avg. end baseline: %1 Hz").arg(log->getEndDomFreq(), 0, 'f', 2));
    ui->treatDomFreqLabel->setText(QString("Base treatment: %1 Hz").arg(log->getBaseTreatmentFreq(), 0, 'f', 2));

    // Display treatment frequencies for each EEG site
    auto startBaselines = log->getStartBaselines();
    auto endBaselines = log->getEndBaselines();
    ui->pcTable->setRowCount(startBaselines.size());
    ui->pcTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    for (int site = 0; site < startBaselines.size(); site++) {
        ui->pcTable->setItem(site, 0, new QTableWidgetItem(QString("Site %1").arg(site+1)));
        ui->pcTable->setItem(site, 1, new QTableWidgetItem(QString("%1 Hz").arg(startBaselines[site])));
        ui->pcTable->setItem(site, 2, new QTableWidgetItem(QString("%1 Hz").arg(endBaselines[site])));
    }

    // Create before and after average waveform graphs
    auto startWaveforms = log->getStartWaveForms();
    auto endWaveforms = log->getEndWaveForms();

    auto startAverages = averageWaveforms(startWaveforms);
    auto endAverages = averageWaveforms(endWaveforms);

    auto startSeries = graphWaveform(startAverages);
    auto endSeries = graphWaveform(endAverages);

    pcStartGraph->addSeries(startSeries);
    pcStartGraph->createDefaultAxes();
    pcStartGraphView->resize(ui->startFrame->size());
    
    pcEndGraph->addSeries(endSeries);
    pcEndGraph->createDefaultAxes();
    pcEndGraphView->resize(ui->endFrame->size());

}

// Returns a vector of waveforms averaged across sites
QVector<WaveForm> MainWindow::averageWaveforms(QVector<QVector<WaveForm>> waveforms){
    QVector<WaveForm> averages = QVector<WaveForm>();
    for (int i = 0; i < waveforms[0].size(); i++){ // For each waveform
        auto averageWaveform = WaveForm();
        for (int j = 0; j < waveforms.size(); j++){ // For each site
            averageWaveform.amplitude += waveforms[j][i].amplitude;
            averageWaveform.frequency += waveforms[j][i].frequency;
        }
        averageWaveform.amplitude /= waveforms.size();
        averageWaveform.frequency /= waveforms.size();

        averages.push_back(averageWaveform);
    }
    return averages;
}



// Methods to update UI based on device state

void MainWindow::updatePCToggle(bool pcConn)
{
    pcConn? qInfo("Computer: ON") : qInfo("Computer: OFF");
    pcConn? ui->pcConnButton->setText("Computer: ON") : ui->pcConnButton->setText("Computer: OFF");
}

void MainWindow::updateHeadsetToggle(bool headsetConn)
{
    headsetConn? qInfo("Headset: ON") : qInfo("Headset: OFF");
    headsetConn? ui->toggleHeadsetBtn->setText("Headset: ON") : ui->toggleHeadsetBtn->setText("Headset: OFF");
}

void MainWindow::updateBatteryLife(int level)
{
    ui->batteryBar->setValue(level);

    auto lowBattery = QString("rgb(229, 10, 10)");
    auto mediumBattery = QString("rbg(229, 165, 10)");
    auto highBattery = QString("rgb(10, 229, 10)");

    QString batteryColor;
    if (level < 10) batteryColor = lowBattery;
    else if (level < 20) batteryColor = mediumBattery;
    else batteryColor = highBattery;

    QString style = QString("selection-background-color: %1;").arg(batteryColor);

}

void MainWindow::toggleRedlight(bool state)
{
    state? 
        ui->redlight->setStyleSheet("QLabel { background-color : red;}") : 
        ui->redlight->setStyleSheet("QLabel { background-color : white;}");
}

void MainWindow::toggleGreenlight(bool state)
{
    state? 
        ui->greenlight->setStyleSheet("QLabel { background-color : green;}") : 
        ui->greenlight->setStyleSheet("QLabel { background-color : white;}");
}

void MainWindow::toggleBluelight(bool state)
{
    state? 
        ui->bluelight->setStyleSheet("QLabel { background-color : blue;}") : 
        ui->bluelight->setStyleSheet("QLabel { background-color : white;}");
}

void MainWindow::updateProgressMessage(QString message)
{
    ui->sessionProgressLabel->setText(message);
}

void MainWindow::updateProgressBar(int percentage)
{
    ui->sessionProgressBar->setValue(percentage);
}
