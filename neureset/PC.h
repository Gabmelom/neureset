#ifndef PC_H
#define PC_H

#include <QVector>
#include <QListWidget>

#include "SessionLog.h"

class PC
{
public:
    PC();

    ~PC();

    void uploadLog(SessionLog *log);

    // Getters
    QVector<SessionLog*> getLogs() const { return logs; }

private:
    bool checkLogExist(SessionLog *log);

    QVector<SessionLog*> logs;
    SessionLog* selectedLog;
};

#endif // PC_H
