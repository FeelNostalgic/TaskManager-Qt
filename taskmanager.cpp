#include "taskManager.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <QDebug>
#include <filesystem>

using namespace std;

TaskManager::TaskManager() : nextTaskId(0)
{
    loadNextTaskId();
}

TaskManager::~TaskManager()
{
    saveNextTaskId();
}

void TaskManager::RebuildQueueByVector(vector<Task>& temp)
{
    tasks = priority_queue<Task>();
    for (const auto& t : temp)
    {
        tasks.push(t);
    }
}

Task TaskManager::FindTaskById(int id)
{
    priority_queue<Task> auxQueue = tasks;
    while (!auxQueue.empty())
    {
        Task current = auxQueue.top();
        auxQueue.pop();
        if (current.iID == id) return current;
    }

    return Task(-1);
}

void TaskManager::loadNextTaskId()
{
    ifstream file("nextTaskID.txt");
    if (file.is_open())
    {
        file >> nextTaskId;
        file.close();
    }
    else if(!filesystem::exists("nextTaskID.txt"))
    {
        saveNextTaskId();
    }
}

void TaskManager::saveNextTaskId()
{
    ofstream file("nextTaskID.txt");
    if (file.is_open())
    {
        file << nextTaskId;
        file.close();
    }
}

void TaskManager::AddTask(const QString& description, int priority)
{
    nextTaskId++;
    saveNextTaskId();

    tasks.push(Task(nextTaskId, description, priority));
    qDebug() << "Task added";
}

priority_queue<Task> TaskManager::GetTasks()
{
    return tasks;
}

void TaskManager::ChangeTaskCompleted(int id, bool completed)
{
    vector<Task> temp;
    while (!tasks.empty())
    {
        Task current = tasks.top();
        tasks.pop();
        if (current.iID == id) current.MarkAsCompleted(completed);
        temp.push_back(current);
    }
    RebuildQueueByVector(temp);

    qDebug() << "Task " << id << " state updated";
}

vector<Task> TaskManager::FindTasksByPriority(int priority)
{
    vector<Task> result;
    priority_queue<Task> auxQueue = tasks;
    while (!auxQueue.empty())
    {
        Task current = auxQueue.top();
        auxQueue.pop();
        if (current.iPriority == priority) result.push_back(current);
    }

    return result;
}

vector<Task> TaskManager::FindTaskByDescription(const QString& description)
{
    vector<Task> result;
    priority_queue<Task> auxQueue = tasks;
    while (!auxQueue.empty())
    {
        Task current = auxQueue.top();
        auxQueue.pop();
        if (current.sDescription.toStdString().find(description.toStdString()) != string::npos) result.push_back(current);
    }

    return result;
}

vector<Task> TaskManager::FindTasksByCompleted()
{
    vector<Task> result;
    priority_queue<Task> auxQueue = tasks;
    while (!auxQueue.empty())
    {
        Task current = auxQueue.top();
        auxQueue.pop();
        if (current.bCompleted) result.push_back(current);
    }

    return result;
}

void TaskManager::ChangeTaskPriority(int id, int priority)
{
    vector<Task> temp;
    while (!tasks.empty())
    {
        Task current = tasks.top();
        tasks.pop();
        if (current.iID == id) current.iPriority = priority;
        temp.push_back(current);
    }
    RebuildQueueByVector(temp);

    qDebug() << "Task " << id << " priority updated to " << priority;
}

void TaskManager::DeleteTask(int id)
{
    vector<Task> temp;
    while (!tasks.empty())
    {
        Task current = tasks.top();
        tasks.pop();
        if (current.iID != id) temp.push_back(current);
    }
    RebuildQueueByVector(temp);
}

vector<Task> TaskManager::ToVector()
{
    vector<Task> result;
    priority_queue<Task> auxQueue = tasks;
    while (!auxQueue.empty())
    {
        Task current = auxQueue.top();
        auxQueue.pop();
        result.push_back(current);
    }
    return result;
}

void TaskManager::SaveTasksToFile(const QString& filename)
{
    ofstream file(filename.toStdString());
    vector<Task> temp;
    string separator = "~";
    if (file.is_open())
    {
        while (!tasks.empty())
        {
            Task current = tasks.top();
            tasks.pop();
            file << current.iID << separator << current.sDescription.toStdString() << separator << current.iPriority << separator << current.bCompleted << endl;
            temp.push_back(current);
        }
        file.close();
        RebuildQueueByVector(temp);
    }else
        cout << "Unable to open file at save" << endl;
}

void TaskManager::LoadTasksFromFile(const QString& filename)
{
    ifstream file(filename.toStdString());
    if (file.is_open())
    {
        string line;
        while (getline(file, line))
        {
            int id, priority;
            string description;
            bool completed;
            char separator;
            stringstream ss(line);
            ss >> id >> separator;
            getline(ss, description, '~');
            ss >> priority >> separator >> completed;
            Task task(id, QString::fromStdString(description), priority);
            task.bCompleted = completed;
            if(FindTaskById(id).iID == -1) tasks.push(task);
        }
        file.close();
    }
    else if(!filesystem::exists(filename.toStdString()))
    {
        SaveTasksToFile(filename);
    }
    else
        cout << "Unable to open file at load" << endl;
}
