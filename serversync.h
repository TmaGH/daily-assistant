#ifndef SERVERSYNC_H
#define SERVERSYNC_H

#include <QThread>
#include <QtNetwork>
#include <QElapsedTimer>

#include "binaryserializer.h"

class ServerSync : public QThread
{
    Q_OBJECT
public:
    explicit ServerSync(const QString &ip, const quint16 &port, BinarySerializer *biser, QObject *parent = nullptr);
    ~ServerSync();

    void run() override;

public slots:
    void attemptSend();

signals:
    void error(int socketError, const QString &message);
    void syncNow();

private:
    QString hostIP;
    quint16 port;
    QMutex mutex;
    QWaitCondition cond;
    bool quit = false;
    BinarySerializer *biser;

    void sendData(const QString& serverIP, const quint16& serverPort);



};

#endif // SERVERSYNC_H
