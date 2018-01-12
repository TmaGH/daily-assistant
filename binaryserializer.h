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
    explicit BinarySerializer(const QUrl& path);
    void save(const Serializable& serializable);
    void load(Serializable& serializable);
    void setFilepath(const QUrl &path);
    QUrl getFilepath() const;

private:
    QUrl filepath;

};

#endif // BINARYSERIALIZER_H
