#ifndef TASKS_H
#define TASKS_H

#include <QWidget>

#include "task.h"
#include "serializable.h"

namespace Ui {
class Tasks;
}

class Tasks : public QWidget, public Serializable
{
    Q_OBJECT

public:
    explicit Tasks(QWidget *parent = 0);
    ~Tasks();
    void updateStatus();
    void addTask(Task *task);

    QVariant toVariant() const override;
    void fromVariant(const QVariant& variant) override;

public slots:
    void addTaskDialogue();
    void removeTask(Task* task);
    void taskStatusChanged();
    void taskDataChanged();
    void updateTaskOrder(Task* task, int increment);

signals:
    void dataChanged();

private:
    Ui::Tasks *ui;
    QVector<Task*> mTasks;
};

#endif // TASKS_H
