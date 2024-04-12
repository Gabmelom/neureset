#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    connect(ui->DeviceScreen, SIGNAL(currentChanged(int)), this, SLOT(pageChanged(int)));
    connect(ui->uploadLogsButton, SIGNAL(pressed()), this, SLOT(uploadLogs()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    currentScreen = 0;
    ui->DeviceScreen->setCurrentIndex(currentScreen);
    currentList = ui->HomeMenuList;
    currentList->setCurrentRow(0);

    device = new Device(ui->sessionProgressBar);
    pc = new PC();
}

void MainWindow::pageChanged(int index)
{
    // Page indexes:
    //  0 - Menu page
    //  1 - New session page
    //  2 - Session log page
    //  3 - Time & date page
    currentScreen = index;

    switch(currentScreen)
    {
        case 0:
            currentList = ui->HomeMenuList;
            currentList->setCurrentRow(0);
            break;
        case 1:
            qInfo("Start Session");
            device->startSession();
            break;
        case 2:
            currentList = ui->deviceLogList;
            displayDeviceLogs();
            currentList->setCurrentRow(0);
            selectPressed();
            break;
        default:
            currentList = nullptr;
    }

}


void MainWindow::powerPressed()
{
    device->togglePower();
}

void MainWindow::homePressed()
{
    // TODO: Stop any ongoing session
    ui->DeviceScreen->setCurrentIndex(0);
}

void MainWindow::upPressed()
{
    if(currentList == nullptr) return;
    if(currentList->currentRow() == 0) return;
    currentList->setCurrentRow(currentList->currentRow() - 1);
}

void MainWindow::downPressed()
{
    if(currentList == nullptr) return;
    if(currentList->currentRow() == currentList->count() - 1) return;
    currentList->setCurrentRow(currentList->currentRow() + 1);
}

void MainWindow::selectPressed()
{
    if(currentScreen == 0){ // Home page

        // Menu list options:
        //  0 - New session page
        //  1 - Session log page
        //  2 - Time & date page
        int optionSelected = currentList->currentRow();
        ui->DeviceScreen->setCurrentIndex(optionSelected+1);
    }
    else if(currentScreen == 2){ // Session log page
        // TODO: upload session log?
    }
}

void MainWindow::playPausePressed()
{
    if(device->getSessionStage() != -1){
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
    device->stopSession();
}

void MainWindow::uploadLogs()
{
    device->uploadLogs(pc);
}

// For each log, render the log to the list, include only the date and session number
void MainWindow::displayDeviceLogs(){
    ui->deviceLogList->clear();
    QVector<SessionLog*> logs = device->getLogs();
    qInfo("Number of device logs: %d", logs.size());

    for(int i = 0; i < logs.size(); i++){
        SessionLog *log = logs[i];
        QDateTime date = log->getStartDateTime();
        ui->deviceLogList->addItem(QString("Session %1       Date: %2").arg(i).arg(date.toString()));
    }
}

// For each log, render the log to the list, include only the date and session number
void MainWindow::displayPCLogs()
{
    ui->pcLogList->clear();
    auto logs = pc->getLogs();
    for (const auto& log : logs) {
        int sessionNumber = log->getSessionNumber();
        QString date = log->getStartDateTime().toString();
        QString endDate = log->getEndDateTime().toString();
        ui->pcLogList->addItem(QString("Session %1      Date: %2").arg(sessionNumber).arg(date));
    }
}