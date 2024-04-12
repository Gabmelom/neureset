#include "PC.h"

#include <QDebug>

PC::PC()
{

}

PC::~PC()
{

}

void PC::uploadLog(SessionLog *log)
{
    QVector<QVector<int>> startBaseline = log->getStartBaseline();
    QVector<QVector<int>> endBaseline = log->getEndBaseline();

    qInfo() << "Start baseline data:";
    for (const auto& row : startBaseline) {
        qInfo() << row;
    }

    qInfo() << "End baseline data:";
    for (const auto& row : startBaseline) {
        qInfo() << row;
    }

    logs.push_back(log);
}

void PC::selectLog()
{

}

void PC::viewLog()
{

}
