﻿#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>
#include "filedig.h"
#include "ContentInject.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    networkManager = new QNetworkAccessManager(this);
    connect(networkManager, &QNetworkAccessManager::finished, this, &MainWindow::handleNetworkReply);
    this->resize(800, 600);
    ui->textEditUserQuery->resize(600, 600);
    url="http://localhost:1234/v1/chat/completions";
    model ="qwen2.5-coder-3b-instruct";
    assistantrole = u8"高级软件工程师，具有多年开发经验";
    // 修改窗口标题为 “AI小助手”
    setWindowTitle(u8"AI小助手");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    //SimpleRequest();
    RequestWithContext();
}

void MainWindow::handleNetworkReply(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QString responseText = QString::fromUtf8(responseData);

        // 解析 responseText 为 JSON
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
        if (!jsonDoc.isNull() && jsonDoc.isObject()) {
            QJsonObject jsonObj = jsonDoc.object();
            // 这里可以根据实际的 JSON 结构来处理数据
            // 例如，假设响应中有一个 "choices" 数组，每个元素包含一个 "message" 对象
            if (jsonObj.contains("choices") && jsonObj["choices"].isArray()) {
                QJsonArray choicesArray = jsonObj["choices"].toArray();
                for (const auto& choice : choicesArray) {
                    if (choice.isObject()) {
                        QJsonObject choiceObj = choice.toObject();
                        if (choiceObj.contains("message") && choiceObj["message"].isObject()) {
                            QJsonObject messageObj = choiceObj["message"].toObject();
                            if (messageObj.contains("content") && messageObj["content"].isString()) {
                                QString content = messageObj["content"].toString();
                                ui->textEdit->append(content); // 将解析后的内容添加到 QTextEdit 控件中
                                 speechwindow.SpeekText(content);
                                //ttv.convertTextToSpeech(content, "output.mp3");
                        
                            }
                        }
                    }
                }
            }
        } else {
            ui->textEdit->append("Failed to parse JSON response.");
        }
    } else {
        QString errorText = "Error: " + reply->errorString();
        ui->textEdit->append(errorText); // 将错误信息添加到 QTextEdit 控件中
    }
    reply->deleteLater();
}

void MainWindow::on_pushButton_selectFile_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr(u8"选择文件"), "", tr(u8"所有文件 (*.*)"));
    if (!filePath.isEmpty()) {
        std::string filecontent ;
        FileDig filedig;
        filecontent = filedig.getFileContent(filePath.toStdString());
        if(filecontent.size()>4096)
        {
            size_t pos = filecontent.find("\n",4096);
            filecontent=filecontent.substr(0,pos);
        }
        // 获取文件名
        QFileInfo fileInfo(filePath);
        QString fileName = fileInfo.fileName();

        ui->lineEditFilePath1->setText(fileName);

        // 将文件名和文件内容添加到 QLineEdit 中
        QString combinedText = fileName + "\n" ;
        combinedText+= filecontent.c_str();
        ui->textEditFileContent->setText(combinedText);


        // 打开文件
//        QFile file(filePath);
//        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
//            // 设置 QTextStream 的编码为 UTF-8
//            QTextStream in(&file);
//            in.setCodec("UTF-8"); // 设置编码为 UTF-8
//            QString fileContent = in.readAll();
//            file.close();
    
//            // 获取文件名
//            QFileInfo fileInfo(filePath);
//            QString fileName = fileInfo.fileName();
    
//            // 将文件名和文件内容添加到 QLineEdit 中
//            QString combinedText = fileName + "\n" + fileContent;
//            ui->textEditFileContent->setText(combinedText);
//        } else {
//            // 若文件打开失败，添加错误信息到 QLineEdit
//            ui->textEditFileContent->setText(tr(u8"无法打开文件: %1").arg(filePath));
//        }
    }
}

void MainWindow::on_AiServerConfig_clicked()
{
    // 显示配置对话框
    int result = configdlg.exec();
    if (result == QDialog::Accepted) {
        // 获取配置窗口中设置的 url、header 和 body 值
        url = configdlg.getUrl();
        model = configdlg.getModel();
    }
}

void MainWindow::on_pushButton_2_clicked()
{

    int result = assistconfig.exec();
    if (result == QDialog::Accepted) {
        // 获取配置窗口中设置的 url、header 和 body 值
       assistantrole= assistconfig.getAssistantIdentity();
    }
}

void MainWindow::on_pushButton_selectFile2_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr(u8"选择文件"), "", tr(u8"所有文件 (*.*)"));
    if (!filePath.isEmpty()) {
        // 打开文件
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            std::string fileContent ;
            FileDig filedig;
            fileContent = filedig.getFileContent(filePath.toStdString());
            if(fileContent.size()>4096)
            {
                size_t pos = fileContent.find("\n",4096);
                fileContent=fileContent.substr(0,pos);
            }

            // 获取文件名
            QFileInfo fileInfo(filePath);
            QString fileName = fileInfo.fileName();

            ui->lineEditFilePath2->setText(fileName);

            // 将文件名和文件内容添加到 QLineEdit 中
            QString combinedText = fileName + "\n" + fileContent.c_str();
            ui->textEditFileContent2->setText(combinedText);
        } else {
            // 若文件打开失败，添加错误信息到 QLineEdit
            ui->textEditFileContent2->setText(tr(u8"无法打开文件: %1").arg(filePath));
        }
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    speechwindow.exec();
}
void MainWindow::SimpleRequest()
{

    ui->textEdit->setText("");

    model = configdlg.getModel();

    QString inputText = ui->textEditUserQuery->toPlainText();  // 假设文本框的对象名为 lineEdit

    QString inputText2 = ui->textEditFileContent->toPlainText();

    QString inputText3 = ui->textEditFileContent2->toPlainText();

    speechwindow.SpeekText(inputText);

    QJsonObject jsonDataSystemMessage;
    jsonDataSystemMessage["role"]="system";
    jsonDataSystemMessage["content"]=assistantrole;


    QJsonObject jsontext1;
    jsontext1["type"]="text";
    jsontext1["text"]=inputText;
    QJsonObject jsontext2;
    jsontext2["type"]="text";
    jsontext2["text"]=inputText2;

    QJsonObject jsontext3;
    jsontext3["type"]="text";
    jsontext3["text"]=inputText3;

    QJsonArray jsontextarray;
    jsontextarray.append(jsontext1);
    jsontextarray.append(jsontext2);
    jsontextarray.append(jsontext3);


    QJsonObject jsonDataMessage;
    jsonDataMessage["role"]="user";
    jsonDataMessage["content"]=jsontextarray;

    QJsonArray aimesages;
    aimesages.append(jsonDataMessage);
    aimesages.append(jsonDataSystemMessage);


    QJsonObject jsonData;
    jsonData["messages"] = aimesages;
    jsonData["model"] = model;
    jsonData["stream"] = false;
    jsonData["prompt"] = u8"请用中文回答";

    // 将 QJsonObject 转换为 QJsonDocument，然后再转换为 QByteArray，以便发送给服务器
    // 假设服务器期望的请求数据格式为 JSON，并且使用 POST 方法发送数据



    QJsonDocument jsonDoc(jsonData);
    QByteArray postData = jsonDoc.toJson();

  //  QNetworkRequest request(QUrl("http://localhost:1234/v1/chat/completions"));
    QNetworkRequest request(QUrl(configdlg.getUrl()));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    networkManager->post(request, postData);
}
void MainWindow::RequestWithContext()
{
    ContextInjector injector;

    // 添加知识片段（模拟检索结果）
    if(!ui->lineEditFilePath1->text().isEmpty())
    {
        injector.add_context(ui->lineEditFilePath1->text().toStdString(), ui->textEditFileContent->toPlainText().toStdString(), 0.92f);
    }
    if(!ui->lineEditFilePath2->text().isEmpty())
    {
        injector.add_context(ui->lineEditFilePath2->text().toStdString(),  ui->textEditFileContent2->toPlainText().toStdString(), 0.87f);
    }

    // 生成带上下文的prompt
    std::string prompt = injector.generate_prompt(
        u8"技术专家", 
        ui->textEditUserQuery->toPlainText().toStdString(),
        u8"分条目列出要点"
    );

    ui->textEdit->setText("");

    model = configdlg.getModel();

    QString inputText = ui->textEditUserQuery->toPlainText();  // 假设文本框的对象名为 lineEdit

    speechwindow.SpeekText(inputText);

    QJsonObject jsonDataSystemMessage;
    jsonDataSystemMessage["role"]="system";
    jsonDataSystemMessage["content"]=assistantrole;


    QJsonObject jsonDataMessage;
    jsonDataMessage["role"]="user";
    jsonDataMessage["content"]=prompt.c_str();

    QJsonArray aimesages;
    aimesages.append(jsonDataMessage);
    aimesages.append(jsonDataSystemMessage);


    QJsonObject jsonData;
    jsonData["messages"] = aimesages;
    jsonData["model"] = model;
    jsonData["stream"] = false;
    jsonData["prompt"] = u8"请用中文回答";

    // 将 QJsonObject 转换为 QJsonDocument，然后再转换为 QByteArray，以便发送给服务器
    // 假设服务器期望的请求数据格式为 JSON，并且使用 POST 方法发送数据
    QJsonDocument jsonDoc(jsonData);
    QByteArray postData = jsonDoc.toJson();

  //  QNetworkRequest request(QUrl("http://localhost:1234/v1/chat/completions"));
    QNetworkRequest request(QUrl(configdlg.getUrl()));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    networkManager->post(request, postData);
}