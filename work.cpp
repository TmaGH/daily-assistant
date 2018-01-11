#include "work.h"
#include "ui_work.h"
#include "mainwindow.h"

Work::Work(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Work),
    qtime(new QTime),
    timer(new QTimer),
    timeSpent(time {0,0,0}),
    timeLeft(time {0,0,0})
{
    ui->setupUi(this);
    connect(ui->setButton, &QPushButton::clicked,
            this, &Work::setTime);

    connect(timer, &QTimer::timeout, this, &Work::updateTime);

    connect(ui->completeButton, &QPushButton::clicked, [this] {
        this->close();
        emit completed();
    });
    connect(ui->startButton, &QPushButton::clicked, [this] {
        if(timer->isActive()) {
            timer->stop();
            ui->set25->setEnabled(true);
            ui->set45->setEnabled(true);
            ui->set60->setEnabled(true);
            ui->setButton->setEnabled(true);
            emit dataChanged();
        } else {
            timer->start(1000);
            ui->set25->setEnabled(false);
            ui->set45->setEnabled(false);
            ui->set60->setEnabled(false);
            ui->setButton->setEnabled(false);
        }
    });
    connect(ui->cancelButton, &QPushButton::clicked, [this] {
        this->close();
        clearTime();
        emit dataChanged();
    });
    connect(ui->set25, &QPushButton::clicked, [this] {
        clearTime();
        timeLeft.minutes = 25;
        displayTime();

    });
    connect(ui->set45, &QPushButton::clicked, [this] {
        clearTime();
        timeLeft.minutes = 45;
        displayTime();
    });
    connect(ui->set60, &QPushButton::clicked, [this] {
        clearTime();
        timeLeft.hours = 1;
        displayTime();
    });
}

int Work::minutesSpent()
{
    return timeSpent.minutes + timeSpent.hours * 60;

}

void Work::setTime() {
    *qtime = ui->timeEdit->time();
    timeLeft.hours = qtime->hour();
    timeLeft.minutes = qtime->minute();
    timeLeft.seconds = qtime->second();
    displayTime();
}

void Work::displayTime()
{
    ui->hours->display(timeLeft.hours);
    ui->minutes->display(timeLeft.minutes);
    ui->seconds->display(timeLeft.seconds);
}

void Work::updateTime() {

    if(timeLeft.seconds != 0 || timeLeft.minutes != 0 || timeLeft.hours != 0) {
        timeSpent.seconds += 1;
        if(timeSpent.seconds == 60) {
            timeSpent.minutes += 1;
            timeSpent.seconds = 0;
            emit dataChanged();
            if(timeSpent.minutes == 60) {
                timeSpent.hours += 1;
                timeSpent.minutes = 0;
            }
        }

        timeLeft.seconds -= 1;
        if(timeLeft.seconds < 0) {
            timeLeft.minutes -= 1;
            timeLeft.seconds = 59;
            if(timeLeft.minutes < 0) {
                timeLeft.hours -= 1;
                timeLeft.minutes = 59;
            }
        }
        displayTime();
    } else {
        timer->stop();
        QMessageBox::information(this, "Session Ended", "Work session has ended. Take a break if you wish.");
        emit dataChanged();
    }
}

void Work::clearTime() {
    timeLeft = time {0,0,0};
}

Work::~Work()
{
    delete ui;
    delete qtime;
    delete timer;
}

QVariant Work::toVariant() const
{
    QVariantMap map;
    map.insert("hours", timeSpent.hours);
    map.insert("minutes", timeSpent.minutes);
    map.insert("seconds", timeSpent.seconds);
    return map;
}

void Work::fromVariant(const QVariant& variant)
{
    QVariantMap map = variant.toMap();
    timeSpent.hours = map.value("hours").toInt();
    timeSpent.minutes = map.value("minutes").toInt();
    timeSpent.seconds = map.value("seconds").toInt();
}
