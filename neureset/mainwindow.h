#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QChartView>
#include <QDebug>

#include <cmath>

#include "Device.h"


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
    void updateDate();
    void changeDate();
    void changeTime();
    void changeBattery();
    void devicePageChanged(int index);
    void pcPageChanged(int index);
    void displayLogDetails(QListWidgetItem*);
    void uploadLogs();
    void pcBackButtonPressed();

    // Update UI based on device state
    void updatePCToggle(bool);
    void updateHeadsetToggle(bool);
    void updateBatteryLife(int);
    void updateProgressMessage(QString message);
    void updateProgressBar(int);
    void updateTreatmentGraph(QVector<WaveForm> waveforms, int site);
    void toggleRedlight(bool);
    void toggleGreenlight(bool);
    void toggleBluelight(bool);
    

private:
    Ui::MainWindow *ui;
    Device *device;
    PC *pc;
    void init();

    int currentDeviceScreen;
    QListWidget* currentDeviceList;

    int currentPCScreen;
    QListWidget* currentPCList;

    void displayDeviceLogs();
    void displayPCLogs();

    // Charting
    QtCharts::QChartView* waveformChart;
    QtCharts::QChart* chart;
    void initWaveformGraph();
    QtCharts::QSplineSeries* graphWaveform(QVector<WaveForm> waveforms);

    const float GRAPH_X_MAX = 80*M_PI;
    const float GRAPH_Y_MAX = 50;

};
#endif // MAINWINDOW_H
