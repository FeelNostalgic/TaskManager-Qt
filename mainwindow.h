#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "taskmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_AddTask_released();
    void onTaskCompletedChanged(int taskId, bool completed);
    void onTaskPriorityChanged(int taskId, int priority);
    void onTaskDelete(int taskId);
    void on_textEdit_Search_textChanged();
    void on_pushButton_FilterPriority_released();
    void on_pushButton_Clear_released();
    void on_actionShowCompleted_triggered();

private:
    Ui::MainWindow *ui;
    TaskManager taskManager;
    QString const filename = "tasks.txt";
    bool showCompletedOnly;

private:
    void addTask(const QString& description, int priority);
    void updateTaskList(std::vector<Task> tasks);
};
#endif // MAINWINDOW_H
