#include "binaryserializer.h"

BinarySerializer::BinarySerializer(const QString &path) :
    filepath(path)
{}

QString BinarySerializer::getFilepath() const
{
    return filepath;
}

void BinarySerializer::save(const Serializable& serializable)
{
        if(QFile::exists(filepath)) {
            QFile::copy(filepath, filepath+".backup");
            QFile::remove(filepath);
        }
        QFile file(filepath);
        file.open(QFile::WriteOnly);
        QDataStream dataStream(&file);
        dataStream << serializable.toVariant();
        file.close();
        if(QFile::exists(filepath)) {
            QFile::remove(filepath+".backup");
        } else {
            qDebug() << "Saving failed. Restore old data from backup.";
        }
        qDebug() << "Saved.";
}

void BinarySerializer::load(Serializable& serializable)
{
        QFileInfo check_file(filepath);
        if (check_file.exists() && check_file.isFile()) {
            QFile file(filepath);
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

void BinarySerializer::setFilepath(const QString &path)
{
    filepath = path;
}
