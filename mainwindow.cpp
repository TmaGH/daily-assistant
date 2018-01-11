#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    tasks(new Tasks),
    biser(new BinarySerializer(QUrl("qrc:/daily_assistant_data")))
{
    biser->load(*this);
    ui->setupUi(this);

    ui->stackedWidget->addWidget(tasks);

    connect(ui->tasksButton, &QPushButton::clicked, [this] {
        ui->stackedWidget->setCurrentWidget(tasks);
    connect(tasks, &Tasks::dataChanged, this, &MainWindow::saveData);
    });
}

void MainWindow::saveData()
{
    biser->save(*this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete tasks;
    delete biser;
}

QVariant MainWindow::toVariant() const
{
    QVariantMap map;
    map.insert("tasks", tasks->toVariant());
    return map;
}

void MainWindow::fromVariant(const QVariant& variant)
{
    QVariantMap map = variant.toMap();
    const QVariant &tasksData = map.value("tasks");
    tasks = new Tasks();
    tasks->fromVariant(tasksData);
}
