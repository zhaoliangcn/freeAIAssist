#include "requestbuild.h"

RequestBuild::RequestBuild(const QString &model, const QString &assistantRole)
    : model(model), assistantRole(assistantRole)
{
    QJsonObject systemMessage;
    systemMessage["role"] = "system";
    systemMessage["content"] = assistantRole;
    messages.append(systemMessage);
}

void RequestBuild::addUserMessage(const QString &message)
{
    QJsonObject userMessage;
    userMessage["role"] = "user";
    userMessage["content"] = message;
    messages.append(userMessage);
}

void RequestBuild::addFileContent(const QString &fileName, const QString &content)
{
    QJsonObject fileMessage;
    fileMessage["type"] = "text";
    fileMessage["text"] = fileName + "\n" + content;
    messages.append(fileMessage);
}

QByteArray RequestBuild::buildRequest() const
{
    QJsonObject request;
    request["messages"] = messages;
    request["model"] = model;
    request["stream"] = false;
    request["prompt"] = "请用中文回答";
    return QJsonDocument(request).toJson();
}