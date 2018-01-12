#include "mainwindow.h"
#include "ui_mainwindow.h"

/*
 * The role of MainWindow is to connect all the widgets together
*/

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    tasks(new Tasks),
    biser(new BinarySerializer(QUrl("qrc:/daily_assistant_data"))),
    options(new Options(biser))
{
    // Load old data if exists
    biser->load(*this);

    ui->setupUi(this);
    ui->stackedWidget->addWidget(tasks);
    ui->stackedWidget->addWidget(options);

    // Buttons
    connect(ui->tasksButton, &QPushButton::clicked, [this] { ui->stackedWidget->setCurrentWidget(tasks); });
    connect(ui->optionsButton, &QPushButton::clicked, [this] { ui->stackedWidget->setCurrentWidget(options); });

    // Signals
    connect(tasks, &Tasks::dataChanged, this, &MainWindow::saveData);
    connect(options, &Options::dataChanged, this, &MainWindow::saveData);
}

void MainWindow::saveData()
{
    biser->save(*this);
}

/*
 * Serialization and destructor
*/

QVariant MainWindow::toVariant() const
{
    QVariantMap map;
    map.insert("tasks", tasks->toVariant());
    map.insert("options", options->toVariant());
    return map;
}

void MainWindow::fromVariant(const QVariant& variant)
{
    QVariantMap map = variant.toMap();
    const QVariant &tasksData = map.value("tasks");
    tasks->fromVariant(tasksData);
    const QVariant &optionsData = map.value("options");
    options->fromVariant(optionsData);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete tasks;
    delete biser;
    delete options;
}
