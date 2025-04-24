#include <QLabel>
#include <QPushButton>
#include "ui_httpconfigdialog.h"
#include "httpconfigdialog.h"
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

HttpConfigDialog::HttpConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HttpConfigDialog),
    networkManager(new QNetworkAccessManager(this))
{
    ui->setupUi(this);
    init_ui();  // 调用初始化UI的函数
}

void HttpConfigDialog::init_ui()
{
    // 连接信号和槽
    connect(ui->testButton, &QPushButton::clicked, this, &HttpConfigDialog::testConnection);
    connect(networkManager, &QNetworkAccessManager::finished, this, &HttpConfigDialog::handleTestResponse);
    connect(ui->comboBoxConfig, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &HttpConfigDialog::onConfigChanged);
    
    // 设置默认URL和模型
    ui->comboBoxUrl->setCurrentText("http://localhost:1234/v1/chat/completions");
    ui->comboBoxModel->setCurrentText("qwen2.5-coder-3b-instruct");
    
    // 设置默认token
    setToken("sk-yNhfdrrAbhSAyKQr34E569Db02504302A369747b32664d9e");
    
    // 设置按钮文字
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(u8"确定");
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(u8"取消");
}

void HttpConfigDialog::onConfigChanged(int index)
{
    QString config = ui->comboBoxConfig->itemText(index);
    
    if (config == u8"ollama默认配置") {
        ui->comboBoxUrl->setCurrentText("http://localhost:11434/chat");
        ui->comboBoxModel->setCurrentText("qwen2.5-coder:3b");
    } else if (config == u8"lmstudio默认配置") {
        ui->comboBoxUrl->setCurrentText("http://localhost:1234/v1/chat/completions");
        ui->comboBoxModel->setCurrentText("qwen2.5-coder-3b-instruct");
    } else if (config == u8"onapi默认配置") {
        ui->comboBoxUrl->setCurrentText("http://localhost:3000/v1/chat/completions");
        ui->comboBoxModel->setCurrentText("qwen2.5-coder-7b-instruct");
    }
}

HttpConfigDialog::~HttpConfigDialog()
{
    delete ui;  // 释放UI对象
}

QString HttpConfigDialog::getUrl() const
{
    return ui->comboBoxUrl->currentText();  // 返回URL输入框的内容
}

QString HttpConfigDialog::getModel() const
{
    return ui->comboBoxModel->currentText();  // 返回模型名称选择框的内容
}

QString HttpConfigDialog::getToken() const
{
    return ui->lineEditToken->text();  // 返回访问令牌输入框的内容
}

void HttpConfigDialog::setToken(const QString &token)
{
    ui->lineEditToken->setText(token);  // 设置访问令牌输入框的内容
}

void HttpConfigDialog::testConnection()
{
    QString url = ui->comboBoxUrl->currentText();  // 获取URL
    if (url.isEmpty()) {  // 检查URL是否为空
        QMessageBox::warning(this, tr(u8"警告"), tr(u8"请先输入服务器地址"));
        return;
    }

    ui->testButton->setEnabled(false);  // 禁用测试按钮
    ui->testButton->setText(tr(u8"测试中..."));  // 更新按钮文本

    QNetworkRequest request(url);  // 创建网络请求
    networkManager->get(request);  // 发送GET请求
}

void HttpConfigDialog::handleTestResponse(QNetworkReply *reply)
{
    ui->testButton->setEnabled(true);  // 重新启用测试按钮
    ui->testButton->setText(tr(u8"测试连接"));  // 恢复按钮文本

    if (reply->error() == QNetworkReply::NoError) {  // 检查是否有错误
        QMessageBox::information(this, tr(u8"成功"), tr(u8"连接测试成功！"));
    }
    else if(reply->error() == QNetworkReply::ContentNotFoundError)
    {
        QMessageBox::information(this, tr(u8"成功"), tr(u8"连接测试成功！"));
    }
    else if(reply->error() == QNetworkReply::AuthenticationRequiredError) 
    {
        QMessageBox::critical(this, tr(u8"错误"), tr(u8"连接测试失败：%1").arg(reply->errorString()));
    }
    else {
        QMessageBox::critical(this, tr(u8"错误"), tr(u8"连接测试失败：%1").arg(reply->errorString()));
    }

    reply->deleteLater();  // 安全删除回复对象
}
