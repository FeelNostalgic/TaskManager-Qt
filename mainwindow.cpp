#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "taskwidget.h"

#include <QtWidgets>
#include <QPushButton>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QDebug>

using namespace std;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), showCompletedOnly(false)
{   
    ui->setupUi(this);

    taskManager.LoadTasksFromFile(filename);
    updateTaskList(taskManager.ToVector());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateTaskList(vector<Task> tasks)
{
    QVBoxLayout * layV = new QVBoxLayout();

    QLayout *existingLayout = ui->scrollContents->layout();
    if (existingLayout)
    {
        QLayoutItem *child;
        while ((child = ui->scrollContents->layout()->takeAt(0)) != nullptr)
        {
            delete child->widget();
            delete child;
        }
        delete existingLayout;
    }

    for (const auto& currentTask : tasks)
    {
        TaskWidget *taskWidget = new TaskWidget(currentTask, &taskManager);
        connect(taskWidget, &TaskWidget::taskCompletedChanged, this, &MainWindow::onTaskCompletedChanged);
        connect(taskWidget, &TaskWidget::taskPriorityChanged, this, &MainWindow::onTaskPriorityChanged);
        connect(taskWidget, &TaskWidget::taskDelete, this, &MainWindow::onTaskDelete);

        layV->addWidget(taskWidget);
    }

    ui->scrollContents->setLayout(layV);
}

void MainWindow::addTask(const QString& description, int priority)
{
    taskManager.AddTask(description, priority);
    taskManager.SaveTasksToFile(filename);
    updateTaskList(taskManager.ToVector());
}

void MainWindow::on_pushButton_AddTask_released()
{
    if(ui->textEdit_Description->toPlainText() != "")
    {
        addTask(ui->textEdit_Description->toPlainText(), ui->spinBox_PriorityAddTask->value());
    }
}

void MainWindow::onTaskCompletedChanged(int taskId, bool completed)
{
    taskManager.ChangeTaskCompleted(taskId, completed);
    taskManager.SaveTasksToFile(filename);
}

void MainWindow::onTaskPriorityChanged(int taskId, int priority)
{
    taskManager.ChangeTaskPriority(taskId, priority);
    taskManager.SaveTasksToFile(filename);
    updateTaskList(taskManager.ToVector());
}

void MainWindow::onTaskDelete(int taskId)
{
    taskManager.DeleteTask(taskId);
    taskManager.SaveTasksToFile(filename);
    updateTaskList(taskManager.ToVector());
}

void MainWindow::on_textEdit_Search_textChanged()
{
    QTextEdit * textEdit = (QTextEdit*) sender();
    updateTaskList(taskManager.FindTaskByDescription(textEdit->toPlainText()));
}

void MainWindow::on_pushButton_FilterPriority_released()
{
    updateTaskList(taskManager.FindTasksByPriority(ui->spinBox_PrioritySearch->value()));
}

void MainWindow::on_pushButton_Clear_released()
{
    updateTaskList(taskManager.ToVector());
}

void MainWindow::on_actionShowCompleted_triggered()
{
    showCompletedOnly = !showCompletedOnly;
    if(showCompletedOnly)
    {
        updateTaskList(taskManager.FindTasksByCompleted());
    }else
    {
        updateTaskList(taskManager.ToVector());
    }
}

