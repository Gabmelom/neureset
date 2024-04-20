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
    QVector<float> startBaseline = log->getStartBaselines();
    QVector<float> endBaseline = log->getEndBaselines();

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
