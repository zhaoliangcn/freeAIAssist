#include "chat_window.h"
#include "ui_chat_window.h"
#include "OperationLogger.h"
#include "aiconfig.h"



ChatWindow::ChatWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChatWindow),
    mcpClient(nullptr)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);
    networkManager = new QNetworkAccessManager(this);
    connect(networkManager, &QNetworkAccessManager::finished, this, &ChatWindow::handleNetworkReply);
    mcpClient = new MCPClient(this);
    connect(mcpClient, &MCPClient::messageReceived, this, &ChatWindow::handleMCPMessage);
    connect(mcpClient, &MCPClient::errorOccurred, this, &ChatWindow::handleMCPError);
}

void ChatWindow::sendMessage(const QString &message)
{
    QNetworkRequest request;
    request.setUrl(QUrl(AIConfig::instance().getUrl()));
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

void ChatWindow::handleMCPMessage(const QByteArray &message)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(message);
    if (!jsonDoc.isNull() && jsonDoc.isObject()) {
        QJsonObject jsonObj = jsonDoc.object();
        if (jsonObj.contains("results") && jsonObj["results"].isArray()) {
            QJsonArray resultsArray = jsonObj["results"].toArray();
            QString content = "百度搜索结果：\n";
            for (const auto &result : resultsArray) {
                if (result.isObject()) {
                    QJsonObject resultObj = result.toObject();
                    if (resultObj.contains("title") && resultObj.contains("link")) {
                        content += QString("• %1 [%2]\n")
                            .arg(resultObj["title"].toString())
                            .arg(resultObj["link"].toString());
                    }
                }
            }
            messageHistory.append(QString("**MCP**: %1").arg(content));
            ui->chatDisplay->setMarkdown(messageHistory.join("\n\n"));
        }
    }
}

void ChatWindow::handleMCPError(const QString &error)
{
    messageHistory.append(QString("**MCP错误**: %1").arg(error));
    ui->chatDisplay->setMarkdown(messageHistory.join("\n\n"));
}

void ChatWindow::handleNetworkReply(QNetworkReply *reply)
{
    QString errorMessage;
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

                                OperationLogger logger;
                                logger.logRequestResponse(currentMessage.toStdString(), content.toStdString());
        
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
    delete mcpClient;
}