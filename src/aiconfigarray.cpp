#include "aiconfigarray.h"
#include <QFile>
#include <QJsonDocument>

AIConfigArray::AIConfigArray(QObject *parent) : AIConfig(parent)
{
}

void AIConfigArray::saveConfigArray(const QJsonArray &configArray)
{
    QMutexLocker locker(&mutex);

    QFile file(configFilePath);
    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument doc(configArray);
        file.write(doc.toJson());
        file.close();
    }
}

QJsonArray AIConfigArray::loadConfigArray()
{
    QMutexLocker locker(&mutex);

    QJsonArray configArray;
    QFile file(configFilePath);
    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        configArray = doc.array();
        file.close();
    }
    return configArray;
}

QJsonObject AIConfigArray::getConfigByIndex(int index)
{
    QMutexLocker locker(&mutex);
    QJsonArray configArray = loadConfigArray();
    if (index >= 0 && index < configArray.size()) {
        return configArray[index].toObject();
    }
    return QJsonObject();
}