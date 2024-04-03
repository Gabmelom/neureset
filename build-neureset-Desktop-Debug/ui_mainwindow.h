/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTimeEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QFrame *deviceFrame;
    QPushButton *power;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *session_flow;
    QPushButton *play_pause;
    QSpacerItem *horizontalSpacer;
    QPushButton *stop;
    QWidget *horizontalLayoutWidget_2;
    QHBoxLayout *indicator_lights;
    QLabel *redlight;
    QSpacerItem *horizontalSpacer_3;
    QLabel *greenlight;
    QSpacerItem *horizontalSpacer_4;
    QLabel *bluelight;
    QPushButton *home;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *screen_nav;
    QPushButton *up;
    QPushButton *select;
    QPushButton *down;
    QStackedWidget *Screen;
    QWidget *HomePage;
    QListWidget *HomeMenuList;
    QWidget *NewSessionPage;
    QProgressBar *sessionProgressBar;
    QLabel *sessionTimer;
    QWidget *SessionLogPage;
    QListWidget *sessionLogList;
    QWidget *TimeAndDatePage;
    QWidget *horizontalLayoutWidget_3;
    QHBoxLayout *horizontalLayout;
    QLabel *dateLabel;
    QDateEdit *dateEdit;
    QWidget *horizontalLayoutWidget_4;
    QHBoxLayout *horizontalLayout_2;
    QLabel *timeLabel;
    QTimeEdit *timeEdit;
    QPushButton *batteryIcon;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->setEnabled(true);
        MainWindow->resize(1249, 780);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        deviceFrame = new QFrame(centralwidget);
        deviceFrame->setObjectName(QString::fromUtf8("deviceFrame"));
        deviceFrame->setEnabled(true);
        deviceFrame->setGeometry(QRect(10, 10, 551, 611));
        deviceFrame->setFrameShape(QFrame::StyledPanel);
        deviceFrame->setFrameShadow(QFrame::Raised);
        power = new QPushButton(deviceFrame);
        power->setObjectName(QString::fromUtf8("power"));
        power->setGeometry(QRect(500, 10, 41, 41));
        power->setStyleSheet(QString::fromUtf8(""));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/power-off.svg"), QSize(), QIcon::Normal, QIcon::Off);
        power->setIcon(icon);
        power->setIconSize(QSize(30, 30));
        horizontalLayoutWidget = new QWidget(deviceFrame);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(240, 450, 101, 41));
        session_flow = new QHBoxLayout(horizontalLayoutWidget);
        session_flow->setObjectName(QString::fromUtf8("session_flow"));
        session_flow->setContentsMargins(0, 0, 0, 0);
        play_pause = new QPushButton(horizontalLayoutWidget);
        play_pause->setObjectName(QString::fromUtf8("play_pause"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/play-pause.svg"), QSize(), QIcon::Normal, QIcon::Off);
        play_pause->setIcon(icon1);
        play_pause->setIconSize(QSize(25, 25));

        session_flow->addWidget(play_pause);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        session_flow->addItem(horizontalSpacer);

        stop = new QPushButton(horizontalLayoutWidget);
        stop->setObjectName(QString::fromUtf8("stop"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/stop.svg"), QSize(), QIcon::Normal, QIcon::Off);
        stop->setIcon(icon2);
        stop->setIconSize(QSize(25, 25));

        session_flow->addWidget(stop);

        horizontalLayoutWidget_2 = new QWidget(deviceFrame);
        horizontalLayoutWidget_2->setObjectName(QString::fromUtf8("horizontalLayoutWidget_2"));
        horizontalLayoutWidget_2->setGeometry(QRect(20, 20, 71, 22));
        indicator_lights = new QHBoxLayout(horizontalLayoutWidget_2);
        indicator_lights->setObjectName(QString::fromUtf8("indicator_lights"));
        indicator_lights->setContentsMargins(0, 0, 0, 0);
        redlight = new QLabel(horizontalLayoutWidget_2);
        redlight->setObjectName(QString::fromUtf8("redlight"));
        redlight->setStyleSheet(QString::fromUtf8("background-color:red; opacity:1;"));

        indicator_lights->addWidget(redlight);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        indicator_lights->addItem(horizontalSpacer_3);

        greenlight = new QLabel(horizontalLayoutWidget_2);
        greenlight->setObjectName(QString::fromUtf8("greenlight"));
        greenlight->setStyleSheet(QString::fromUtf8("background-color:forestgreen; opacity:1;"));

        indicator_lights->addWidget(greenlight);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        indicator_lights->addItem(horizontalSpacer_4);

        bluelight = new QLabel(horizontalLayoutWidget_2);
        bluelight->setObjectName(QString::fromUtf8("bluelight"));
        bluelight->setStyleSheet(QString::fromUtf8("background-color:blue; opacity:0.1;"));

        indicator_lights->addWidget(bluelight);

        home = new QPushButton(deviceFrame);
        home->setObjectName(QString::fromUtf8("home"));
        home->setGeometry(QRect(20, 80, 51, 41));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/icons/house.svg"), QSize(), QIcon::Normal, QIcon::Off);
        home->setIcon(icon3);
        home->setIconSize(QSize(30, 30));
        verticalLayoutWidget = new QWidget(deviceFrame);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(490, 200, 51, 151));
        screen_nav = new QVBoxLayout(verticalLayoutWidget);
        screen_nav->setObjectName(QString::fromUtf8("screen_nav"));
        screen_nav->setContentsMargins(0, 0, 0, 0);
        up = new QPushButton(verticalLayoutWidget);
        up->setObjectName(QString::fromUtf8("up"));
        up->setEnabled(true);
        up->setAutoFillBackground(false);
        up->setStyleSheet(QString::fromUtf8(""));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/icons/arrow-up.svg"), QSize(), QIcon::Normal, QIcon::Off);
        up->setIcon(icon4);
        up->setIconSize(QSize(30, 30));

        screen_nav->addWidget(up);

        select = new QPushButton(verticalLayoutWidget);
        select->setObjectName(QString::fromUtf8("select"));
        QFont font;
        font.setPointSize(15);
        select->setFont(font);
        select->setStyleSheet(QString::fromUtf8(""));
        select->setIconSize(QSize(30, 30));

        screen_nav->addWidget(select);

        down = new QPushButton(verticalLayoutWidget);
        down->setObjectName(QString::fromUtf8("down"));
        down->setStyleSheet(QString::fromUtf8(""));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/icons/arrow-down.svg"), QSize(), QIcon::Normal, QIcon::Off);
        down->setIcon(icon5);
        down->setIconSize(QSize(30, 30));

        screen_nav->addWidget(down);

        Screen = new QStackedWidget(deviceFrame);
        Screen->setObjectName(QString::fromUtf8("Screen"));
        Screen->setEnabled(true);
        Screen->setGeometry(QRect(80, 80, 401, 361));
        Screen->setStyleSheet(QString::fromUtf8("border: solid 1px;"));
        HomePage = new QWidget();
        HomePage->setObjectName(QString::fromUtf8("HomePage"));
        HomePage->setStyleSheet(QString::fromUtf8("background-color:white;"));
        HomeMenuList = new QListWidget(HomePage);
        new QListWidgetItem(HomeMenuList);
        new QListWidgetItem(HomeMenuList);
        new QListWidgetItem(HomeMenuList);
        HomeMenuList->setObjectName(QString::fromUtf8("HomeMenuList"));
        HomeMenuList->setGeometry(QRect(10, 10, 381, 341));
        HomeMenuList->setFont(font);
        HomeMenuList->setSpacing(2);
        Screen->addWidget(HomePage);
        NewSessionPage = new QWidget();
        NewSessionPage->setObjectName(QString::fromUtf8("NewSessionPage"));
        NewSessionPage->setStyleSheet(QString::fromUtf8("background-color:white;"));
        sessionProgressBar = new QProgressBar(NewSessionPage);
        sessionProgressBar->setObjectName(QString::fromUtf8("sessionProgressBar"));
        sessionProgressBar->setGeometry(QRect(100, 190, 221, 31));
        sessionProgressBar->setValue(24);
        sessionTimer = new QLabel(NewSessionPage);
        sessionTimer->setObjectName(QString::fromUtf8("sessionTimer"));
        sessionTimer->setGeometry(QRect(130, 120, 141, 51));
        Screen->addWidget(NewSessionPage);
        SessionLogPage = new QWidget();
        SessionLogPage->setObjectName(QString::fromUtf8("SessionLogPage"));
        SessionLogPage->setStyleSheet(QString::fromUtf8("background-color:white;"));
        sessionLogList = new QListWidget(SessionLogPage);
        sessionLogList->setObjectName(QString::fromUtf8("sessionLogList"));
        sessionLogList->setGeometry(QRect(10, 10, 381, 341));
        Screen->addWidget(SessionLogPage);
        TimeAndDatePage = new QWidget();
        TimeAndDatePage->setObjectName(QString::fromUtf8("TimeAndDatePage"));
        TimeAndDatePage->setStyleSheet(QString::fromUtf8("background-color:white;"));
        horizontalLayoutWidget_3 = new QWidget(TimeAndDatePage);
        horizontalLayoutWidget_3->setObjectName(QString::fromUtf8("horizontalLayoutWidget_3"));
        horizontalLayoutWidget_3->setGeometry(QRect(100, 120, 201, 31));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget_3);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        dateLabel = new QLabel(horizontalLayoutWidget_3);
        dateLabel->setObjectName(QString::fromUtf8("dateLabel"));

        horizontalLayout->addWidget(dateLabel);

        dateEdit = new QDateEdit(horizontalLayoutWidget_3);
        dateEdit->setObjectName(QString::fromUtf8("dateEdit"));

        horizontalLayout->addWidget(dateEdit);

        horizontalLayoutWidget_4 = new QWidget(TimeAndDatePage);
        horizontalLayoutWidget_4->setObjectName(QString::fromUtf8("horizontalLayoutWidget_4"));
        horizontalLayoutWidget_4->setGeometry(QRect(90, 180, 211, 21));
        horizontalLayout_2 = new QHBoxLayout(horizontalLayoutWidget_4);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        timeLabel = new QLabel(horizontalLayoutWidget_4);
        timeLabel->setObjectName(QString::fromUtf8("timeLabel"));

        horizontalLayout_2->addWidget(timeLabel);

        timeEdit = new QTimeEdit(horizontalLayoutWidget_4);
        timeEdit->setObjectName(QString::fromUtf8("timeEdit"));

        horizontalLayout_2->addWidget(timeEdit);

        Screen->addWidget(TimeAndDatePage);
        batteryIcon = new QPushButton(deviceFrame);
        batteryIcon->setObjectName(QString::fromUtf8("batteryIcon"));
        batteryIcon->setEnabled(false);
        batteryIcon->setGeometry(QRect(500, 570, 41, 31));
        batteryIcon->setStyleSheet(QString::fromUtf8("border:none;"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/icons/battery-full.svg"), QSize(), QIcon::Normal, QIcon::Off);
        batteryIcon->setIcon(icon6);
        batteryIcon->setIconSize(QSize(30, 30));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1249, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        Screen->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        play_pause->setText(QString());
        stop->setText(QString());
        redlight->setText(QString());
        greenlight->setText(QString());
        bluelight->setText(QString());
        home->setText(QString());
        up->setText(QString());
        select->setText(QCoreApplication::translate("MainWindow", "OK", nullptr));
        down->setText(QString());

        const bool __sortingEnabled = HomeMenuList->isSortingEnabled();
        HomeMenuList->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem = HomeMenuList->item(0);
        ___qlistwidgetitem->setText(QCoreApplication::translate("MainWindow", "NEW SESSION", nullptr));
        QListWidgetItem *___qlistwidgetitem1 = HomeMenuList->item(1);
        ___qlistwidgetitem1->setText(QCoreApplication::translate("MainWindow", "SESSION LOG", nullptr));
        QListWidgetItem *___qlistwidgetitem2 = HomeMenuList->item(2);
        ___qlistwidgetitem2->setText(QCoreApplication::translate("MainWindow", "TIME & DATE", nullptr));
        HomeMenuList->setSortingEnabled(__sortingEnabled);

        sessionTimer->setText(QCoreApplication::translate("MainWindow", "10 : 00", nullptr));
        dateLabel->setText(QCoreApplication::translate("MainWindow", "Date", nullptr));
        timeLabel->setText(QCoreApplication::translate("MainWindow", "Time", nullptr));
        batteryIcon->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
