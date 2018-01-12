#ifndef NOTES_H
#define NOTES_H

#include <QWidget>
#include <QString>

#include "binaryserializer.h"

namespace Ui {
class Notes;
}

class Notes : public QWidget, public Serializable
{
    Q_OBJECT

public:
    explicit Notes(QWidget *parent = nullptr);
    void displayMinutesSpent(int &minutesSpent);
    ~Notes();

    QVariant toVariant() const override;
    void fromVariant(const QVariant& variant) override;

signals:
    void dataChanged();

private:
    Ui::Notes *ui;
};

#endif // NOTES_H
