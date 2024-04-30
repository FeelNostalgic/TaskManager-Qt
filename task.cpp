#include "task.h"

Task::Task(int i, QString d, int p) : iID(i), sDescription(d), iPriority(p), bCompleted(false) {};
Task::Task(int id) : iID(id) {};

void Task::MarkAsCompleted(bool completed)
{
    bCompleted = completed;
}

bool Task::operator<(const Task& other) const
{
    return iPriority < other.iPriority;
}
