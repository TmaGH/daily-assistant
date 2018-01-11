#include "tasks.h"
#include "ui_tasks.h"

Tasks::Tasks(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Tasks),
    mTasks()
{
    ui->setupUi(this);
    connect(ui->addTaskButton, &QPushButton::clicked,
            this, &Tasks::addTaskDialogue);
    ui->taskProgressBar->setValue(0);
}

void Tasks::addTaskDialogue()
{
    bool ok;
    QString name = QInputDialog::getText(this,
                                         tr("Add task"),
                                         tr("Task name"),
                                         QLineEdit::Normal,
                                         tr("Untitled task"), &ok);
    if(ok && !name.isEmpty()) {
        Task *task = new Task(name);
        addTask(task);
        updateStatus();
        emit dataChanged();
        qDebug() << "Added new task.";
    }
}

void Tasks::addTask(Task *task)
{
    if(task != nullptr) {
        connect(task, &Task::removed, this, &Tasks::removeTask);
        connect(task, &Task::statusChanged, this, &Tasks::taskStatusChanged);
        connect(task, &Task::dataChanged, this, &Tasks::taskDataChanged);
        connect(task, &Task::taskIndexChanged, this, &Tasks::updateTaskOrder);
        mTasks.append(task);
        ui->tasks->addWidget(task);
    }
}

void Tasks::removeTask(Task* task)
{
    mTasks.removeOne(task);
    ui->tasks->removeWidget(task);
    task->setParent(0);
    delete task;
    updateStatus();
    emit dataChanged();
}

void Tasks::taskStatusChanged()
{
    updateStatus();
}

void Tasks::taskDataChanged()
{
    emit dataChanged();
}

void Tasks::updateTaskOrder(Task *task, int increment)
{
    int index = ui->tasks->indexOf(task);
    ui->tasks->removeWidget(task);
    mTasks.remove(index);
    ui->tasks->insertWidget(index+increment, task);
    mTasks.insert(index+increment, task);
    emit dataChanged();
}

void Tasks::updateStatus()
{
    int completedCount = 0;
    for(auto t : mTasks) {
        if(t->isCompleted()) {
            completedCount++;
        }
    }
    int todoCount = mTasks.size();

    ui->statusLabel->setText(QString("Completed %1 / %2")
                             .arg(completedCount)
                             .arg(todoCount));
    ui->taskProgressBar->setValue(static_cast<int>(static_cast<float>(completedCount*100)/todoCount));
}

Tasks::~Tasks()
{
    delete ui;
}

QVariant Tasks::toVariant() const
{
    QVariantMap map;
    QVariantList list;

    for(const auto& task : mTasks) {
        list.append(task->toVariant());
    }
    map.insert("tasks", list);

    return map;
}

void Tasks::fromVariant(const QVariant& variant)
{
    QVariantMap map = variant.toMap();

    QVariantList list = map.value("tasks").toList();
    for(const QVariant& data : list) {
        Task *task = new Task("");
        task->fromVariant(data);
        addTask(task);
    }
    updateStatus();
}
