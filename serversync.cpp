#include "serversync.h"

ServerSync::ServerSync(const QString& ip, const quint16& port, BinarySerializer *biser, QObject *parent) :
    QThread(parent),
    hostIP(ip),
    port(port),
    biser(biser)
{
    QMutexLocker locker(&mutex);
    connect(this, &ServerSync::syncNow, this, &ServerSync::attemptSend);
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

void ServerSync::attemptRestore()
{
    mutex.lock();
    QString serverIP = hostIP;
    quint16 serverPort = port;
    mutex.unlock();
    restoreData(serverIP, serverPort);
}

void ServerSync::run()
{
    mutex.lock();
    QString serverIP = hostIP;
    quint16 serverPort = port;
    mutex.unlock();

    while(!quit) {
        backupData(serverIP, serverPort);
        mutex.lock();
        cond.wait(&mutex);
        serverIP = hostIP;
        serverPort = port;
        mutex.unlock();
    }
}

void ServerSync::backupData(const QString& serverIP, const quint16& serverPort)
{

    const int timeout = 5000;

    QTcpSocket *socket = new QTcpSocket;
    socket->connectToHost(serverIP, serverPort);

    if(!socket->waitForConnected(timeout)) {
        emit error(socket->error(), socket->errorString());
        return;
    }

    QString path = biser->getFilepath();

    QFileInfo check_file(path);
    if (check_file.exists() && check_file.isFile()) {
        QFile file(path);

        if(!file.open(QFile::ReadOnly)) {
            qDebug() << "Can't open file.";
            return;
        }

        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_10);

        QByteArray q = file.readAll();
        file.close();

        qint8 operation = OPERATIONS::BACKUP;
        out << operation;

        qint64 blockSize = q.size();
        out << blockSize;
        block.append(q);
        out.device()->seek(0);
        qDebug() << "Block size:" << blockSize;
        qint64 x = 0;
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

void ServerSync::restoreData(const QString& serverIP, const quint16& serverPort)
{
    const int timeout = 5000;

    QTcpSocket *socket = new QTcpSocket;
    socket->connectToHost(serverIP, serverPort);

    if(!socket->waitForConnected(timeout)) {
        emit error(socket->error(), socket->errorString());
        return;
    }

    QString filepath = biser->getFilepath();

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_10);
    qint8 operation = OPERATIONS::RESTORE;
    out << operation;

    out.device()->seek(0);

    qint64 x = 0;
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

    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_10);
    qint64 blockSize = 0;

    /*
     * Beginning of data will include an 8 byte integer which informs us the size of the actual block in bytes.
     * When this data has been received, it will written into quint64 blockSize.
    */

    do {
        if (!socket->waitForReadyRead(5000)) {
            return;
        }

        in.startTransaction();
        if(socket->bytesAvailable() >= sizeof(blockSize)) {
            if(blockSize == 0) {
                in >> blockSize;
                qDebug() << "Block size:" << blockSize;
            }
            /*
                 * Use blockSize to determine when all of the data has been received, then read all to QByteArray.
                */

            QByteArray line;
            if(socket->bytesAvailable() >= blockSize) {
                line = socket->readAll();

                if(line.size() == blockSize)
                    qDebug() << "All incoming data written to QByteArray," << line.size() << "bytes. Next, saving data to file...";

                if(QFile::exists(filepath)) {
                    QFile::copy(filepath, filepath+".backup");
                    QFile::remove(filepath);
                }
                QFile file(filepath);

                if (!file.open(QIODevice::WriteOnly)) {
                    qDebug() << "Can't open file for writing.";
                    return;
                }

                file.write(line);
                file.close();

                if(QFile::exists(filepath) && file.size() == line.size()) {
                    qDebug() << "Succesfully saved data to file," << file.size() << "bytes.";
                    QFile::remove(filepath+".backup");
                } else {
                    qDebug("Saving data failed. Restoring previous data...");
                    QFile::copy(filepath+".backup", filepath);
                    QFile::remove(filepath+".backup");
                }
            }
        }
    } while (!in.commitTransaction());
}

ServerSync::~ServerSync()
{
    mutex.lock();
    quit = true;
    cond.wakeOne();
    mutex.unlock();
    wait();
}
