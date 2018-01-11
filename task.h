#ifndef TASK_H
#define TASK_H

#include <QWidget>
#include <QDebug>
#include <QString>
#include <QInputDialog>
#include <QCheckBox>

#include "work.h"
#include "notes.h"

namespace Ui {
class Task;
}

class Task : public QWidget, public Serializable
{
    Q_OBJECT

public:
    explicit Task(const QString& name, QWidget *parent = nullptr);

    QString name() const;
    bool isCompleted() const;
    void setName(const QString& name);
    void updateTimeSpent();
    ~Task();

    QVariant toVariant() const override;
    void fromVariant(const QVariant& variant) override;

public slots:
    void rename();
    void completed();
    void workOrNotesDataChanged();

signals:
    void removed(Task* task);
    void statusChanged();
    void dataChanged();
    void taskIndexChanged(Task* task, int increment);

private:
    Ui::Task *ui;
    Work *workMenu;
    Notes *notes;

private slots:
    void checked(bool checked);

};

#endif // TASK_H
