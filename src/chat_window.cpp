#include "chat_window.h"
#include "ui_chat_window.h"
#include "OperationLogger.h"


ChatWindow::ChatWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChatWindow)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);
    networkManager = new QNetworkAccessManager(this);
    connect(networkManager, &QNetworkAccessManager::finished, this, &ChatWindow::handleNetworkReply);
}

void ChatWindow::sendMessage(const QString &message)
{
    QNetworkRequest request;
    request.setUrl(QUrl("http://localhost:1234/v1/chat/completions"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["model"] = "qwen2.5-coder-3b-instruct";
    QJsonArray messagesArray;
    for (const QString &history : messageHistory) {
        if (history.startsWith("**用户**")) {
            messagesArray.append(QJsonObject({
                {"role", "user"},
                {"content", history.mid(7)}
            }));
        } else if (history.startsWith("**AI**")) {
            messagesArray.append(QJsonObject({
                {"role", "assistant"},
                {"content", history.mid(6)}
            }));
        }
    }
    messagesArray.append(QJsonObject({
        {"role", "user"},
        {"content", message}
    }));
    json["messages"] = messagesArray;

    networkManager->post(request, QJsonDocument(json).toJson());
}

void ChatWindow::on_sendButton_clicked()
{
    QString message = ui->chatInput->text();
    if (message.isEmpty()) {
        return; 
    }
    currentMessage = message;
    sendMessage(message);
    ui->chatInput->clear();
}

void ChatWindow::on_clearContextButton_clicked()
{
    messageHistory.clear();
    ui->chatDisplay->clear();
}

void ChatWindow::handleNetworkReply(QNetworkReply *reply)
{
    QString errorMessage;
    if (reply->error() == QNetworkReply::NoError) {
        OperationLogger logger;
        logger.logRequestResponse(currentMessage.toStdString(), reply->readAll().toStdString());
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
        if (!jsonDoc.isNull() && jsonDoc.isObject()) {
            QJsonObject jsonObj = jsonDoc.object();
            if (jsonObj.contains("choices") && jsonObj["choices"].isArray()) {
                QJsonArray choicesArray = jsonObj["choices"].toArray();
                for (const auto &choice : choicesArray) {
                    if (choice.isObject()) {
                        QJsonObject choiceObj = choice.toObject();
                        if (choiceObj.contains("message") && choiceObj["message"].isObject()) {
                            QJsonObject messageObj = choiceObj["message"].toObject();
                            if (messageObj.contains("content") && messageObj["content"].isString()) {
                                QString content = messageObj["content"].toString();
                                messageHistory.append(QString("**用户**: %1").arg(currentMessage));
                                messageHistory.append(QString("**AI**: %1").arg(content));
                                ui->chatDisplay->setMarkdown(messageHistory.join("\n\n"));
                            }
                        }
                    }
                }
            }
        }
    } else {
        errorMessage = QString("网络请求失败: %1").arg(reply->errorString());
    }

    if (!errorMessage.isEmpty()) {
        messageHistory.append(QString("**错误**: %1").arg(errorMessage));
        ui->chatDisplay->setMarkdown(messageHistory.join("\n\n"));
    }
    reply->deleteLater();
}

ChatWindow::~ChatWindow()
{
    delete ui;
}