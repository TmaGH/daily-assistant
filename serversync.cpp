#include "serversync.h"

ServerSync::ServerSync(const QString& ip, const quint16& port, BinarySerializer *biser, QObject *parent) :
    QThread(parent),
    hostIP(ip),
    port(port),
    biser(biser),
    timer(new QTimer)
{
    QMutexLocker locker(&mutex);
    connect(timer, &QTimer::timeout, this, &ServerSync::attemptSend);
    timer->start(60000);
    start();
    sendData(hostIP, this->port);
}

void ServerSync::attemptSend()
{
    if (!isRunning()) {
        start();
    }
    else {
        cond.wakeOne();
    }
}

void ServerSync::run()
{
    mutex.lock();
    QString serverIP = hostIP;
    quint16 serverPort = port;
    mutex.unlock();

    while(!quit) {
        sendData(serverIP, serverPort);
        mutex.lock();
        cond.wait(&mutex);
        serverIP = hostIP;
        serverPort = port;
        mutex.unlock();
    }
}

void ServerSync::sendData(const QString& serverIP, const quint16& serverPort)
{
    const int timeout = 5000;

    QTcpSocket *socket = new QTcpSocket();
    socket->connectToHost(serverIP, serverPort);

    if(!socket->waitForConnected(timeout)) {
        emit error(socket->error(), socket->errorString());
        return;
    }

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_10);
    QString path = biser->getFilepath();

    QFileInfo check_file(path);
    if (check_file.exists() && check_file.isFile()) {
        QFile file(path);

        if(!file.open(QFile::ReadOnly)) {
            qDebug() << "Can't open file.";
            return;
        }

        QByteArray q = file.readAll();
        block.append(q);
        file.close();

        out.device()->seek(0);

        qint64 x = 0;
        while(x < block.size()) {
            qint64 y = socket->write(block);
            x += y;
            qDebug() << x;
        }
        socket->disconnectFromHost();
        qDebug() << "Sucesfully sent data.";
    } else {
        qDebug() << "No data to send.";
    }
}

ServerSync::~ServerSync()
{
    mutex.lock();
    quit = true;
    QString serverIP = hostIP;
    quint16 serverPort = port;
    cond.wakeOne();
    mutex.unlock();
    wait();
    sendData(serverIP, serverPort);
}
