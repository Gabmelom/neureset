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
    connect(ui->toggleHeadsetBtn, SIGNAL(pressed()), this, SLOT(toggleHeadset()));
    connect(ui->replaceBatteryBtn, SIGNAL(pressed()), this, SLOT(changeBattery()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    currentDeviceScreen = 0;
    ui->DeviceScreen->setCurrentIndex(currentDeviceScreen);
    currentDeviceList = ui->HomeMenuList;
    currentDeviceList->setCurrentRow(0);

    device = new Device(ui->sessionProgressBar, ui->redlight, ui->bluelight, ui->greenlight);
    pc = new PC();
    // CRASHES AFTER MERGE CONFLICT
    // QTimer* timer = new QTimer(this);
    // connect(timer, &QTimer::timeout,this, &MainWindow::updateGui);
    // timer->start(1000);
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
            qInfo("Start Session");
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
    //qDebug () <<device->getPower();
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
    // TODO: Stop any ongoing session?
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
        if(optionSelected == 0 && !device->getHeadsetConn()) return; // Device needs a headset connection to start a new session
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

void MainWindow::toggleHeadset()
{
    qDebug() << "ADMIN: toggle heeadset";
    device->toggleHeadsetConn();
}


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

void MainWindow::displayLogDetails(QListWidgetItem *item)
{
    int logIndex = ui->pcLogList->row(item);
    auto log = pc->getLogs()[logIndex];
    
    qInfo("Displaying log details for session %d", log->getSessionNumber());
    ui->pcScreen->setCurrentIndex(1); // Switch to log details page

    // Display log details
    ui->sessionNumberLabel->setText(QString("Session %1").arg(log->getSessionNumber()));
    ui->sessionDateLabel->setText(log->getStartDateTime().toString());
    auto duration = log->getStartDateTime().secsTo(log->getEndDateTime());
    ui->sessionDurationLabel->setText(QString("Duration: %1 minutes").arg(duration/60));

    ui->startDomFreqLabel->setText(QString("Start Dominant Frequency: %1").arg(log->getStartDomFreq()));
    ui->endDomFreqLabel->setText(QString("End Dominant Frequency: %1").arg(log->getEndDomFreq()));

    // Display treatment frequencies for each EEG site
    auto startBaselines = log->getStartBaseline();
    auto endBaselines = log->getEndBaseline();
    for (int site = 0; site < startBaselines.size(); site++) {
        auto startFrequency = startBaselines[site][0];
        auto startAmplitude = startBaselines[site][1];
        auto endFrequency = endBaselines[site][0];
        auto endAmplitude = endBaselines[site][1];


        // Add entry to ui->siteBaselineList
        ui->siteBaselineList->addItem(QString("Site %1: Start Freq: %2, Start Amp: %3, End Freq: %4, End Amp: %5")
            .arg(site).arg(startFrequency).arg(startAmplitude).arg(endFrequency).arg(endAmplitude));
    }
}

void MainWindow::pcBackButtonPressed()
{
    ui->pcScreen->setCurrentIndex(0); // Switch to log list page
}


void MainWindow::updateGui(){
    if  (!device->getPower()){
        ui->DeviceScreen->setCurrentIndex(0);
        devicePageChanged(0);
    }
    device->getDate()->setTime(device->getDate()->time().addSecs(1));
    updateDate();
    ui->currBattery->setText(QString::number(device->getBatteryLife()));
}
