#ifndef OPTIONS_H
#define OPTIONS_H

#include <QWidget>

#include "serializable.h"
#include "serversync.h"

namespace Ui {
class Options;
}

class Options : public QWidget, public Serializable
{
    Q_OBJECT

public:
    explicit Options(BinarySerializer *biser, QWidget *parent = nullptr);
    ServerSync *getSyncThread();
    ~Options();

    QVariant toVariant() const override;
    void fromVariant(const QVariant& variant) override;

signals:
    void dataChanged();
    void restore();

private:
    Ui::Options *ui;
    BinarySerializer *biser;
    QString ip;
    quint16 port;
    ServerSync *syncThread = nullptr;

    void setSyncEnabled(bool state);

private slots:
    void encryptionStateChanged(int state);
    void syncStateChanged(int state);
    void restoreData();
    void onError(int socketError, const QString &message);

};

#endif // OPTIONS_H
