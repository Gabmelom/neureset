#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <Device.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

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

private:
    Ui::MainWindow *ui;
    void init();
    int currentScreen;
    QListWidget* currentList;

    Device* device;
};
#endif // MAINWINDOW_H
