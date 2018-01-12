#ifndef TASKEDIT_H
#define TASKEDIT_H

#include <QWidget>

namespace Ui {
class TaskEdit;
}

class TaskEdit : public QWidget
{
    Q_OBJECT

public:
    explicit TaskEdit(QWidget *parent = 0);
    ~TaskEdit();

private:
    Ui::TaskEdit *ui;
};

#endif // TASKEDIT_H
