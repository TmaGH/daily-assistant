#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QVector>
#include <QInputDialog>

#include "tasks.h"
#include "binaryserializer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public Serializable
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QVariant toVariant() const override;
    void fromVariant(const QVariant& variant) override;
    void saveData();

private:
    Ui::MainWindow *ui;
    Tasks *tasks;
    BinarySerializer *biser;

};

#endif // MAINWINDOW_H
