#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <Device.h>
#include "Device.h"

//#include "Device.h"
#include <QDebug>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Device;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    Ui::MainWindow* getUi() { return ui; }
private slots:
    void powerPressed();
    void homePressed();
    void upPressed();
    void downPressed();
    void selectPressed();
    void playPausePressed();
    void stopPressed();
    void pageChanged(int index);
    void updateDate();
    void changeDate();
    void changeTime();

    void uploadSession();


private:
    Ui::MainWindow *ui;
    Device *device;
    void init();
    int currentScreen;
    QListWidget* currentList;

};
#endif // MAINWINDOW_H
