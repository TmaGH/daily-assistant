#include "binaryserializer.h"

BinarySerializer::BinarySerializer(const QUrl &path) :
    filepath(path)
{}

void BinarySerializer::save(const Serializable& serializable)
{
    QString path = filepath.path();
    if(path.isNull()) {
        qDebug() << "Set filepath before calling this.";
    } else {
        if(QFile::exists(path)) {
            QFile::copy(path, path+".backup");
            QFile::remove(path);
        }
        QFile file(path);
        file.open(QFile::WriteOnly);
        QDataStream dataStream(&file);
        dataStream << serializable.toVariant();
        file.close();
        if(QFile::exists(path)) {
            QFile::remove(path+".backup");
        } else {
            qDebug() << "Saving failed. Restore old data from backup.";
        }
        qDebug() << "Saved.";
    }
}

void BinarySerializer::load(Serializable& serializable)
{
    QString path = filepath.path();
    if(path.isNull()) {
        qDebug() << "Set filepath before calling this.";
    } else {
        QFileInfo check_file(path);
        if (check_file.exists() && check_file.isFile()) {
            QFile file(path);
            file.open(QFile::ReadOnly);
            QDataStream dataStream(&file);
            QVariant variant;
            dataStream >> variant;
            serializable.fromVariant(variant);
            file.close();
            qDebug() << "Loaded.";
        } else {
            qDebug() << "No data to load.";
        }
    }
}

void BinarySerializer::setFilepath(const QUrl &path)
{
    filepath = path;
}
