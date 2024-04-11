#ifndef PC_H
#define PC_H

#include <vector>

#include "SessionLog.h"

class PC
{
public:
    PC();

    ~PC();

    void uploadLogs(vector<SessionLog> logs);
    void selectLog();
    void viewLog();


private:
    vector<SessionLog> logs;
    SessionLog selecftedLog;
};

#endif // PC_H
