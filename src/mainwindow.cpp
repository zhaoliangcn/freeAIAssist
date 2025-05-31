#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include "filedig.h"
#include "ContentInject.hpp"
#include <QtConcurrent/QtConcurrent>
#include "OperationLogger.h"

// 新增菜单项处理函数
void MainWindow::on_menuItem_triggered() {
    // 在这里实现菜单项的响应动作
    chathistorywindow.exec();
}

void MainWindow::on_actionexit_triggered() {
    // 在这里实现退出操作，例如关闭窗口
    this->close();
}

void MainWindow::on_actioncommonprompt_triggered() {
    commonSystemPromptDialog.show();
}

void MainWindow::on_actionchatwindow_triggered()
{
    chatWindow.setSystemPrompt(assistconfig.getAssistantIdentity());
    chatWindow.exec();
}
void MainWindow::on_actionkbmanager_triggered()
{
    knowledgeBase.exec();
}
void MainWindow::on_actionfileimport_triggered() {
    textSegment.exec();
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{



    ui->setupUi(this);

    // 初始化样式选择器
 //   ui->styleSelector = new QComboBox(this);
    connect(ui->styleSelector, QOverload<int>::of(&QComboBox::currentIndexChanged),
    [this](int index){
        QString style = ui->styleSelector->itemText(index);
        if(style == "默认") {
            qApp->setStyleSheet("");
        }
        else if (style == "深色") {
            //获取当前程序路径 
            QString currentPath = QCoreApplication::applicationDirPath();
            //拼接路径
            QString filePath = currentPath + "/styles/dark.qss";
            QFile file(filePath);
            if (file.open(QFile::ReadOnly | QFile::Text)) {
                QString styleSheet = QLatin1String(file.readAll());
                qApp->setStyleSheet(styleSheet);
                file.close();
            }
        } else if (style == "自定义") {
            //获取当前程序路径 
            QString currentPath = QCoreApplication::applicationDirPath();
            //拼接路径
            QString filePath = currentPath + "/styles/custom.qss";
            QFile file(filePath);
            if (file.open(QFile::ReadOnly | QFile::Text)) {
                QString styleSheet = QLatin1String(file.readAll());
                qApp->setStyleSheet(styleSheet);
                file.close();
            }
        }
    });
    
    // 语言切换功能
    QFile translationsFile(":/i18n/translations.json");
    if (translationsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QByteArray jsonData = translationsFile.readAll();
        translationsFile.close();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
        translations = jsonDoc.object();
    }

    connect(ui->languageSelector, QOverload<int>::of(&QComboBox::currentIndexChanged),
        [this](int index){
            QString lang = ui->languageSelector->itemText(index);
            if (lang == "English") {
                lang = "en"; // 这里可以根据实际情况设置语言代码
            } else if (lang == u8"中文") {
                lang = "zh"; // 这里可以根据实际情况设置语言代码    
            }
            else if (lang == u8"Chinese") {
                lang = "zh"; // 这里可以根据实际情况设置语言代码    
            }
            else if (lang == "Français") {
                lang = "fr"; // 这里可以根据实际情况设置语言代码
            }
            else if (lang == "Русский") {
                lang = "ru"; // 这里可以根据实际情况设置语言代码
            }
            else if (lang == "Italiano") {
                lang = "it"; // 这里可以根据实际情况设置语言代码
            }
            changeLanguage(lang);
        });

    // 启用拖放支持
    setAcceptDrops(true);
    networkManager = new QNetworkAccessManager(this);
    connect(networkManager, &QNetworkAccessManager::finished, this, &MainWindow::handleNetworkReply);
    this->resize(800, 600);
    ui->textEditUserQuery->resize(600, 600);
    url="http://localhost:1234/v1/chat/completions";
    model ="qwen2.5-coder-3b-instruct";
    assistantrole = u8"高级软件工程师，具有多年开发经验";
    // 修改窗口标题为 “AI小助手”
    setWindowTitle(u8"AI小助手");
    // 连接菜单项的信号槽
    connect(ui->actionchathistory, &QAction::triggered, this, &MainWindow::on_menuItem_triggered);
    // 连接actionexit的信号槽
    connect(ui->actionexit, &QAction::triggered, this, &MainWindow::on_actionexit_triggered);
    // 连接actioncommonprompt的信号槽
    connect(ui->actioncommonprompt, &QAction::triggered, this, &MainWindow::on_actioncommonprompt_triggered);
    // 连接actionfileimport的信号槽
    connect(ui->actionfileimport, &QAction::triggered, this, &MainWindow::on_actionfileimport_triggered);

    connect(ui->actionkbmanage, &QAction::triggered, this, &MainWindow::on_actionkbmanager_triggered);
    
    connect(ui->actionchat, &QAction::triggered, this, &MainWindow::on_actionchatwindow_triggered);

    
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
                                 QtConcurrent::run([this, content]() {
                                    speechwindow.SpeekText(content);
                                });
                                OperationLogger logger;
                                logger.logRequestResponse(ui->textEditUserQuery->toPlainText().toStdString(), content.toStdString());
                        
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

void MainWindow::changeLanguage(const QString &lang)
{
    QString translationFile = QCoreApplication::applicationDirPath() + "/i18n/translations_" + lang + ".qm";
    if (translator.load(translationFile)) {
        qApp->installTranslator(&translator);
        ui->retranslateUi(this);
        //QMessageBox::information(this, tr(u8"调试信息"), tr(u8"成功加载语言文件: %1").arg(translationFile));
    }
    else
    {
        //QMessageBox::warning(this, tr(u8"调试信息"), tr(u8"加载语言文件失败: %1").arg(translationFile));
        qDebug() << "Failed to load translation file: " << translationFile;
    }
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

    QByteArray token = "Bearer " + configdlg.getToken().toUtf8();
    request.setRawHeader(QByteArray("Authorization"),QByteArray(token));

    networkManager->post(request, postData);
}
void MainWindow::RequestWithContext()
{
    ContextInjector injector;

    // 添加知识片段（模拟检索结果）
    if(!ui->lineEditFilePath1->text().isEmpty())
    {
        std::string  filecontent = ui->textEditFileContent->toPlainText().toStdString();
        if (filecontent.size() > 4096) {
            size_t pos = filecontent.find("\n", 4096);
            filecontent = filecontent.substr(0, pos);
        }
        injector.add_context(ui->lineEditFilePath1->text().toStdString(), filecontent, 0.92f);
    }
    if(!ui->lineEditFilePath2->text().isEmpty())
    {
        std::string  filecontent = ui->textEditFileContent2->toPlainText().toStdString();
        if (filecontent.size() > 4096) {
            size_t pos = filecontent.find("\n", 4096);
            filecontent = filecontent.substr(0, pos);
        }
        injector.add_context(ui->lineEditFilePath2->text().toStdString(), filecontent, 0.87f);
    }

    // 生成带上下文的prompt
    std::string prompt = injector.generate_prompt(
        u8"技术专家", 
        ui->textEditUserQuery->toPlainText().toStdString(),
        assistconfig.getSystemPrompt().toStdString()
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

    requests.push_back(postData.toStdString().c_str());

  //  QNetworkRequest request(QUrl("http://localhost:1234/v1/chat/completions"));
    QNetworkRequest request(QUrl(configdlg.getUrl()));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QByteArray token = "Bearer " + configdlg.getToken().toUtf8();
    request.setRawHeader(QByteArray("Authorization"),QByteArray(token));

    networkManager->post(request, postData);
}

// 添加拖放事件处理函数
void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        // 获取鼠标位置
        QPoint pos = event->pos();
        
        // 判断鼠标是否在 textEditFileContent 控件上
        if (ui->textEditFileContent->geometry().contains(pos)) {
            event->acceptProposedAction();
        }
        // 判断鼠标是否在 textEditFileContent2 控件上
        else if (ui->textEditFileContent2->geometry().contains(pos)) {
            event->acceptProposedAction();
        }
    }
}

void MainWindow::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    if (mimeData->hasUrls()) {
        QList<QUrl> urlList = mimeData->urls();
        if (!urlList.isEmpty()) {
            QString filePath = urlList.first().toLocalFile();
            QPoint pos = event->pos();

            // 判断拖放位置并填充相应的文本框
            if (ui->textEditFileContent->geometry().contains(pos)) {
                // 处理文件并填充到 textEditFileContent
                std::string filecontent;
                FileDig filedig;
                filecontent = filedig.getFileContent(filePath.toStdString());
                
                QFileInfo fileInfo(filePath);
                QString fileName = fileInfo.fileName();
                ui->lineEditFilePath1->setText(fileName);
                ui->textEditFileContent->setText(fileName + "\n" + filecontent.c_str());
            } else if (ui->textEditFileContent2->geometry().contains(pos)) {
                // 处理文件并填充到 textEditFileContent2
                std::string filecontent;
                FileDig filedig;
                filecontent = filedig.getFileContent(filePath.toStdString());

                QFileInfo fileInfo(filePath);
                QString fileName = fileInfo.fileName();
                ui->lineEditFilePath2->setText(fileName);
                ui->textEditFileContent2->setText(fileName + "\n" + filecontent.c_str());
            }
        }
    }
}
