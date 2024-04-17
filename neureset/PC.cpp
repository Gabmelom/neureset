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
    QVector<QVector<float>> startBaseline = log->getStartBaseline();
    QVector<QVector<float>> endBaseline = log->getEndBaseline();

    
    // qInfo() << "Start baseline data:";
    // for (const auto& row : startBaseline) {
    //     qInfo() << row;
    // }

    // qInfo() << "End baseline data:";
    // for (const auto& row : startBaseline) {
    //     qInfo() << row;
    // }

    if (!checkLogExist(log))
    {
        logs.push_back(log);
    }
}

bool PC::checkLogExist(SessionLog *log)
{
    for (int i = 0; i < logs.size(); i++)
    {
        if (log->getSessionNumber() == logs[i]->getSessionNumber())
        {
            return true;
        }
    }

    return false;
}
