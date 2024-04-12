#ifndef PC_H
#define PC_H

#include <vector>

#include "SessionLog.h"

class PC
{
public:
    PC();

    ~PC();

    void uploadLog(SessionLog *log);
    void selectLog();
    void viewLog();


private:
    vector<SessionLog*> logs;
    SessionLog* selectedLog;
};

#endif // PC_H
