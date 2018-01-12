#ifndef WORK_H
#define WORK_H

#include <QWidget>
#include <QDebug>
#include <QTime>
#include <QTimer>
#include <QMessageBox>

#include "binaryserializer.h"

namespace Ui {
class Work;
}

class Work : public QWidget, public Serializable
{
    Q_OBJECT

public:
    explicit Work(QWidget *parent = nullptr);
    int minutesSpent();
    ~Work();

    QVariant toVariant() const override;
    void fromVariant(const QVariant& variant) override;

private slots:
    void setTime();
    void updateTime();


signals:
    void completed();
    void dataChanged();

private:
    Ui::Work *ui;
    QTime *qtime;
    QTimer *timer;

    struct time
    {
        int hours;
        int minutes;
        int seconds;
    } timeSpent, timeLeft;

    void clearTime();
    void displayTime();
    void stop();
    void start();

};

#endif // WORK_H
