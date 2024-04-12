#ifndef PC_H
#define PC_H

#include <vector>
#include <QListWidget>

#include "SessionLog.h"

class PC
{
public:
    PC();

    ~PC();

    void uploadLog(SessionLog *log);

    // Getters
    vector<SessionLog*> getLogs() const { return logs; }

    // Setters

private:
    vector<SessionLog*> logs;
    SessionLog* selectedLog;
};

#endif // PC_H
