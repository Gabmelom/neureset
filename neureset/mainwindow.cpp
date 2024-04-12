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
    connect(ui->Screen, SIGNAL(currentChanged(int)), this, SLOT(pageChanged(int)));

    connect(ui->dateEdit, SIGNAL(editingFinished()), this, SLOT(changeDate()));
    connect(ui->timeEdit,  SIGNAL(editingFinished()), this, SLOT(changeTime()));

    connect(ui->tglHeadsetBtn, SIGNAL(pressed()), this, SLOT(toggleHeadset()));
    connect(ui->batteryBtn, SIGNAL(pressed()), this, SLOT(changeBattery()));

    connect(ui->uploadSession, SIGNAL(pressed()), this, SLOT(uploadSession()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    currentScreen = 0;  //power off screen
    ui->Screen->setCurrentIndex(currentScreen);
    //currentList = ui->HomeMenuList;
    //currentList->setCurrentRow(4);


    device = new Device(ui->sessionLogList, ui->sessionProgressBar,
                        ui->redlight, ui->bluelight, ui->greenlight);

    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout,this, &MainWindow::updateGui);
    timer->start(1000);
}



void MainWindow::pageChanged(int index)
{
    // Page indexes:
    //  0 - off
    //  1 - Menu page
    //  2 - New session page
    //  3- Session log page
    //  4- Time & date page
    currentScreen = index;
    //qDebug()<<index;
    switch(currentScreen)
    {
        case 0:
                //qDebug("off");
            break;
        case 1:
            currentList = ui->HomeMenuList;
            currentList->setCurrentRow(0);
            break;
        case 2:
            qInfo("Start Session");
            device->startSession();
            break;

        case 3:
            currentList = ui->sessionLogList;
            currentList->setCurrentRow(0);
            selectPressed();
            break;

        case 4:
            updateDate();
            break;
            //ui->screen->powerOffPage;
        default:
            currentList = nullptr;
    }

}


void MainWindow::powerPressed()
{
    device->togglePower();
    //qDebug () <<device->getPower();
    if (device->getPower()){    //power  on
        ui->Screen->setCurrentIndex(1);
        pageChanged(1);
    } else {
        ui->Screen->setCurrentIndex(0);
        pageChanged(0);
    }

}

void MainWindow::homePressed()
{
    // TODO: Stop any ongoing session
    ui->Screen->setCurrentIndex(1);
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
    if(currentScreen == 1){ // Home page

        // Menu list options:
        //  0 - New session page
        //  1 - Session log page
        //  2 - Time & date page
        int optionSelected = currentList->currentRow();
        switch(optionSelected){
            case 0:
                if(device->getPowerState() && device->getHeadsetConn()){
                    ui->Screen->setCurrentIndex(optionSelected+2);
                }
                break;
            default:
                if(device->getPowerState()){
                    ui->Screen->setCurrentIndex(optionSelected+2);
                }
                break;
        }
    }
    else if(currentScreen == 2){ // Session log page
        // TODO
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

void MainWindow::uploadSession()
{
    int selected = currentList->currentRow();
    qInfo("Upload session %d", selected);
    device->uploadSessionLog(selected);

}

void MainWindow::updateGui(){
    if  (not device->getPower()){
        ui->Screen->setCurrentIndex(0);
        pageChanged(0);
    }
    device->getDate()->setTime(device->getDate()->time().addSecs(1));
    updateDate();
    ui->currBattery->setText(QString::number(device->getBatteryLife()));
}
