#ifndef BINARYSERIALIZER_H
#define BINARYSERIALIZER_H

#include <QFile>
#include <QDataStream>
#include <QDebug>
#include <QFileInfo>
#include <QString>
#include <QUrl>

#include "serializable.h"


class BinarySerializer
{
public:
    explicit BinarySerializer(const QString& path);
    void save(const Serializable& serializable);
    void load(Serializable& serializable);
    void setFilepath(const QString &path);
    QString getFilepath() const;

private:
    QString filepath;

};

#endif // BINARYSERIALIZER_H
