#include "taskWidget.h"
#include <QHBoxLayout>
#include <QString>

TaskWidget::TaskWidget(const Task& task, TaskManager* taskManager,QWidget *parent) : QWidget(parent), m_task(task), m_taskManager(taskManager)
{
    QHBoxLayout *layH = new QHBoxLayout();

    // Check Box
    m_checkBox = new QCheckBox(this);
    m_checkBox->setFixedSize(25,25);
    m_checkBox->setCheckState(task.bCompleted ? Qt::Checked : Qt::Unchecked);

    // Description
    m_descriptionLabel = new QLabel(task.sDescription, this);
    if (task.bCompleted) {
        m_descriptionLabel->setStyleSheet("QLabel { text-decoration: line-through; }");
    }

    m_descriptionLabel->setWordWrap(true);

    // Priority
    m_slider = new QSlider(this);
    m_slider->setOrientation(Qt::Horizontal);
    m_slider->setSliderPosition(task.iPriority);
    m_slider->setMaximum(15);
    m_slider->setMinimum(0);
    m_slider->setFixedSize(75,25);

    m_priorityLabel = new QLabel(QString::number(task.iPriority),this);
    m_priorityLabel->setFixedSize(30,25);

    // Delete button
    m_deleteButton = new QPushButton("Delete", this);
    m_deleteButton->setFixedSize(80, 25);

    layH->addWidget(m_checkBox);
    layH->addWidget(m_descriptionLabel);
    layH->addWidget(m_priorityLabel);
    layH->addWidget(m_slider);
    layH->addWidget(m_deleteButton);

    this->setLayout(layH);
    this->setMinimumHeight(30);

    connect(m_checkBox, &QCheckBox::stateChanged, this, &TaskWidget::onCheckBoxStateChanged);
    connect(m_slider, &QSlider::sliderReleased, this, &TaskWidget::onSliderReleased);
    connect(m_slider, &QSlider::valueChanged, this, &TaskWidget::onSliderValueChanged);
    connect(m_deleteButton, SIGNAL(released()), this, SLOT(onDeletePressed()));
}

void TaskWidget::onCheckBoxStateChanged(int state)
{
    bool completed = (state == Qt::Checked);
    if (state == Qt::Checked) {
        m_descriptionLabel->setStyleSheet("QLabel { text-decoration: line-through; }");
    }else
    {
        m_descriptionLabel->setStyleSheet("");
    }
    emit taskCompletedChanged(m_task.iID, completed);
}

void TaskWidget::onSliderReleased()
{
    emit taskPriorityChanged(m_task.iID, m_slider->value());
}

void TaskWidget::onSliderValueChanged(int value)
{
    m_priorityLabel->setText(QString::number(value));
}

void TaskWidget::onDeletePressed()
{
    emit taskDelete(m_task.iID);
}
