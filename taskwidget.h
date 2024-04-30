#ifndef TASKWIDGET_H
#define TASKWIDGET_H

#include <QWidget>
#include <QCheckBox>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QSlider>

#include "task.h"
#include "taskManager.h"

class TaskWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TaskWidget(const Task& task, TaskManager* taskManager, QWidget *parent = nullptr);

signals:
    void taskCompletedChanged(int taskId, bool completed);
    void taskPriorityChanged(int taskId, int priority);
    void taskDelete(int taskId);

private slots:
    void onCheckBoxStateChanged(int state);
    void onSliderReleased();
    void onSliderValueChanged(int value);
    void onDeletePressed();

private:
    Task m_task;
    TaskManager *m_taskManager;
    QCheckBox *m_checkBox;
    QLabel *m_descriptionLabel;
    QPushButton *m_deleteButton;
    QSlider *m_slider;
    QLabel *m_priorityLabel;
};

#endif // TASKWIDGET_H
