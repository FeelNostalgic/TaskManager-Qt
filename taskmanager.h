#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <queue>
#include <QString>
#include "task.h"

class TaskManager
{
private:
    std::priority_queue<Task> tasks;
    int nextTaskId;

private:
    void RebuildQueueByVector(std::vector<Task>& temp);
    Task FindTaskById(int id);
    void loadNextTaskId();
    void saveNextTaskId();

public:
    TaskManager();
    ~TaskManager();
    void AddTask(const QString& description, int priority);
    std::priority_queue<Task> GetTasks();
    void ChangeTaskCompleted(int id, bool completed);
    std::vector<Task> FindTasksByPriority(int priority);
    std::vector<Task> FindTaskByDescription(const QString& description);
    std::vector<Task> FindTasksByCompleted();
    void ChangeTaskPriority(int id, int priority);
    void DeleteTask(int id);
    std::vector<Task> ToVector();

    void SaveTasksToFile(const QString& filename);
    void LoadTasksFromFile(const QString& filename);
};

#endif //TASKMANAGER_H
