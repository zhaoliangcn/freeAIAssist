#ifndef AICONFIGARRAY_H
#define AICONFIGARRAY_H

#include "aiconfig.h"
#include <QJsonArray>

class AIConfigArray : public AIConfig
{
    Q_OBJECT

public:
    explicit AIConfigArray(QObject *parent = nullptr);

    void saveConfigArray(const QJsonArray &configArray);
    QJsonArray loadConfigArray();
    QJsonObject getConfigByIndex(int index);
};

#endif // AICONFIGARRAY_H