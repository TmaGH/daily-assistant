#include "serversync.h"

ServerSync::ServerSync(const QString& ip, const quint16& port, BinarySerializer *biser, QObject *parent) :
    QThread(parent),
    hostIP(ip),
    port(port),
    biser(biser)
{
    QMutexLocker locker(&mutex);
    connect(this, &ServerSync::syncNow, this, &ServerSync::attemptSend);
    start();
}

void ServerSync::attemptSend()
{
    if (!isRunning()) {
        start();
    }
    else {
        //cond.wakeOne();
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

    QString path = biser->getFilepath();

    QFileInfo check_file(path);
    if (check_file.exists() && check_file.isFile()) {
        QFile file(path);

        if(!file.open(QFile::ReadOnly)) {
            qDebug() << "Can't open file.";
            return;
        }

        QByteArray block;
        block = file.readAll();
        qDebug() << sizeof(block);
        file.close();

        qint64 x = 0;

        QTcpSocket *socket = new QTcpSocket;
        socket->connectToHost(serverIP, serverPort);

        if(!socket->waitForConnected(timeout)) {
            emit error(socket->error(), socket->errorString());
            return;
        }
        while(x < block.size()) {
            qint64 y = socket->write(block);
            if(y >= 0) {
            x += y;
            qDebug() << x << "bytes written.";
            } else {
                qDebug() << "Error occured while writing data.";
            }
        }
        socket->waitForBytesWritten();
        socket->disconnectFromHost();
    } else {
        qDebug() << "No data to send.";
    }
}

ServerSync::~ServerSync()
{
    mutex.lock();
    quit = true;
    cond.wakeOne();
    mutex.unlock();
    wait();
}
