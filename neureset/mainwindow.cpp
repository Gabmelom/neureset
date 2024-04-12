#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Device.h"

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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    currentScreen = 0;
    ui->Screen->setCurrentIndex(currentScreen);
    currentList = ui->HomeMenuList;
    currentList->setCurrentRow(0);

    device = new Device(ui->sessionLogList);
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
            currentList = ui->sessionLogList;
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
    ui->Screen->setCurrentIndex(0);
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
        ui->Screen->setCurrentIndex(optionSelected+1);
    }
    else if(currentScreen == 2){ // Session log page
        // TODO
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

