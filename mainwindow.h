#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QVector>
#include <QInputDialog>

#include "tasks.h"
#include "binaryserializer.h"
#include "options.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public Serializable
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void saveData();
    ~MainWindow();

    QVariant toVariant() const override;
    void fromVariant(const QVariant& variant) override;


private:
    Ui::MainWindow *ui;
    Tasks *tasks;
    BinarySerializer *biser;
    Options *options;
    ServerSync *syncThread = nullptr;

};

#endif // MAINWINDOW_H
