#include "chat_window.h"
#include "ui_chat_window.h"


ChatWindow::ChatWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatWindow)
{
    ui->setupUi(this);
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
    json["messages"] = QJsonArray({QJsonObject({
        {"role", "user"},
        {"content", message}
    })});

    networkManager->post(request, QJsonDocument(json).toJson());
}

void ChatWindow::on_sendButton_clicked()
{
    QString message = ui->chatInput->text();
    sendMessage(message);
    ui->chatInput->clear();
}

void ChatWindow::handleNetworkReply(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
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
                                ui->chatDisplay->setMarkdown(content);
                            }
                        }
                    }
                }
            }
        }
    }
    reply->deleteLater();
}

ChatWindow::~ChatWindow()
{
    delete ui;
}