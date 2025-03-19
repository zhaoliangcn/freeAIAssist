#ifndef REQUESTBUILD_H
#define REQUESTBUILD_H

#include <QString>
#include <QJsonObject>
#include <QJsonArray>

class RequestBuild
{
public:
    RequestBuild(const QString &model, const QString &assistantRole);

    void addUserMessage(const QString &message);
    void addFileContent(const QString &fileName, const QString &content);
    QByteArray buildRequest() const;

private:
    QString model;
    QString assistantRole;
    QJsonArray messages;
};

#endif // REQUESTBUILD_H