#ifndef TASK_H
#define TASK_H

#include <QString>

class Task
{
public:
    int iID;
    QString sDescription;
    int iPriority;
    bool bCompleted;

    Task(int i, QString d, int p) ;
    Task(int id) ;

    void MarkAsCompleted(bool completed) ;

    bool operator<(const Task& other) const ;
};

#endif // TASK_H
