#include "options.h"
#include "ui_options.h"

Options::Options(BinarySerializer *biser, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Options),
    biser(biser)
{
    // Server information

    ip = "10.54.118.101";
    port = 41815;

    // ------------------


    ui->setupUi(this);
    connect(ui->encryption, &QCheckBox::stateChanged, this, &Options::encryptionStateChanged);
    connect(ui->sync, &QCheckBox::stateChanged, this, &Options::syncStateChanged);
    ui->sync->setEnabled(false);
}

void Options::onError(int socketError, const QString &message)
{
    qDebug() << "Socket error" << socketError << ":" << message;
}

ServerSync* Options::getSyncThread()
{
    return syncThread;
}

void Options::setSyncEnabled(bool state) {
    if(state && !ui->sync->isEnabled())
        ui->sync->setEnabled(true);
    else if(!state)
        ui->sync->setEnabled(false);
}

void Options::encryptionStateChanged(int state)
{
// Encryption not implemented
    if(state == 2) {
        setSyncEnabled(true);
    } else {
        setSyncEnabled(false);
    }
    emit dataChanged();
}

void Options::syncStateChanged(int state)
{
    if(state == 2 && syncThread == nullptr) {
        syncThread = new ServerSync(ip, port, biser);
        connect(syncThread, &ServerSync::error, this, &Options::onError);
    } else {
        delete syncThread;
        syncThread = nullptr;
    }
    emit dataChanged();
}

QVariant Options::toVariant() const
{
    QVariantMap map;
    map.insert("encryption", ui->encryption->isChecked());
    map.insert("sync", ui->sync->isChecked());
    return map;
}

void Options::fromVariant(const QVariant& variant)
{
    QVariantMap map;
    map = variant.toMap();
    ui->encryption->setChecked(map.value("encryption").toBool());
    ui->sync->setChecked(map.value("sync").toBool());
}

Options::~Options()
{
    delete ui;
    delete syncThread;
}
