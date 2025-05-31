#include "chat_window.h"
#include "ui_chat_window.h"
#include "OperationLogger.h"
#include "aiconfig.h"
#include <QDir>
#include <QCoreApplication>
#include <QtConcurrent/QtConcurrent>
#include "ContentInject.hpp"


ChatWindow::ChatWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChatWindow),
    enableTTS(false),
    mcpClient(nullptr)
{
 
    ui->setupUi(this);
    setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);

    networkManager = new QNetworkAccessManager(this);
    connect(networkManager, &QNetworkAccessManager::finished, this, &ChatWindow::handleNetworkReply);

    mcpClient = new MCPClient(this);
    connect(mcpClient, &MCPClient::messageReceived, this, &ChatWindow::handleMCPMessage);
    connect(mcpClient, &MCPClient::errorOccurred, this, &ChatWindow::handleMCPError);


    // Initialize knowledge base list
    initKnowledgeBaseList();

    // Connect TTS checkbox signal
    connect(ui->ttsCheckBox, &QCheckBox::stateChanged, this, &ChatWindow::on_ttsCheckBox_stateChanged);
}

void ChatWindow::initKnowledgeBaseList()
{
    QString absPath = QCoreApplication::applicationDirPath() + "/KnowledgeBase";
    QDir dir(absPath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    QStringList knowledgeBases = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    knowledgeBases.prepend(u8"无");
    ui->knowledgeBaseComboBox->addItems(knowledgeBases);
    connect(ui->knowledgeBaseComboBox, &QComboBox::currentTextChanged, this, &ChatWindow::on_knowledgeBaseComboBox_currentIndexChanged);
}

void ChatWindow::sendMessage(const QString &message)
{

    if (enableTTS) {
        QtConcurrent::run([this, message]() {
            speechwindow.SpeekText(message);
        });
    }

    ContextInjector injector;

    bool useKB = false;
    std::string prompt ;
    // 添加知识库内容作为上下文
    QString knowledgeBasePath = QCoreApplication::applicationDirPath() + "/KnowledgeBase/" + currentKnowledgeBase;
    QDir knowledgeBaseDir(knowledgeBasePath);
    QStringList files = knowledgeBaseDir.entryList(QDir::Files);
    
    if(files.size() > 0) {
        useKB = true;
        foreach (const QString &file, files) {
            QFile f(knowledgeBasePath + "/" + file);
            if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QString content = f.readAll();
                injector.add_context(file.toStdString(), content.toStdString(), 0.9f);
                f.close();
            }
        }
    }
    if(useKB) {
        // 生成带上下文的prompt
       prompt = injector.generate_prompt(
            u8"技术专家",
            message.toStdString(),
            AIConfig::instance().getSystemPrompt().toStdString()
        );
    }
    

    QNetworkRequest request;
    request.setUrl(QUrl(AIConfig::instance().getUrl()));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QByteArray token = "Bearer " + AIConfig::instance().getToken().toUtf8();
    request.setRawHeader(QByteArray("Authorization"),QByteArray(token));

    QJsonObject json;
    json["model"] = AIConfig::instance().getModel();
    QJsonArray messagesArray;
    for (const QString &history : messageHistory) {
        if (history.startsWith(u8"**用户**")) {
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
    if(useKB) {
        messagesArray.append(QJsonObject({
            {"role", "user"},
            {"content", prompt.c_str()}
        }));
    }
    else
    {
        messagesArray.append(QJsonObject({
            {"role", "user"},
            {"content", message}
        }));
    }

    QJsonObject jsonDataSystemMessage;
    jsonDataSystemMessage["role"]="system";
    jsonDataSystemMessage["content"]=getSystemPrompt().toStdString().c_str();
    messagesArray.append(jsonDataSystemMessage);

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

void ChatWindow::on_knowledgeBaseComboBox_currentIndexChanged(const QString &text)
{
    currentKnowledgeBase = text;
}

void ChatWindow::handleMCPError(const QString &error)
{
    messageHistory.append(QString("**MCP错误**: %1").arg(error));
    ui->chatDisplay->setMarkdown(messageHistory.join("\n\n"));
}

void ChatWindow::on_ttsCheckBox_stateChanged(int state)
{
    enableTTS = state == Qt::Checked;
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
        
                                messageHistory.append(QString(u8"**用户**: %1").arg(currentMessage));
                                messageHistory.append(QString("**AI**: %1").arg(content));
                                ui->chatDisplay->setMarkdown(messageHistory.join("\n\n"));
                                if (enableTTS) {
                                    QtConcurrent::run([this, content]() {
                                        speechwindow.SpeekText(content);
                                    });
                                }
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

void ChatWindow::setSystemPrompt(const QString &prompt)
{
    systemPrompt = prompt;
}
QString ChatWindow::getSystemPrompt() const
{
    return systemPrompt;
}
