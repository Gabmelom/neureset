#ifndef SESSIONLOG_H
#define SESSIONLOG_H

#include <vector>
#include <QDateTime>

using namespace std;

class SessionLog
{
public:
    SessionLog();
    ~SessionLog();

    void addStartBaselines(vector<int> baseline);
    void addEndBaselines(vector<int> baseline);

private:
    vector<int> startBaselines;
    vector<int> endBaselines;
    QDateTime startDateTime;
    QDateTime endDateTime;
};

#endif // SESSIONLOG_H
