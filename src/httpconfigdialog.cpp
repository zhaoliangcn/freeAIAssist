#include <QLabel>
#include <QPushButton>
#include "ui_httpconfigdialog.h"
#include "httpconfigdialog.h"
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

HttpConfigDialog::HttpConfigDialog(QWidget *parent) :
    QDialog(parent),  // 调用父类构造函数
    ui(new Ui::HttpConfigDialog),  // 创建UI对象
    networkManager(new QNetworkAccessManager(this))  // 创建网络管理器
{
    ui->setupUi(this);  // 初始化UI
    // 连接测试按钮点击信号到测试连接槽函数
    connect(ui->testButton, &QPushButton::clicked, this, &HttpConfigDialog::testConnection);
    // 连接网络管理器完成信号到处理响应槽函数
    connect(networkManager, &QNetworkAccessManager::finished, this, &HttpConfigDialog::handleTestResponse);
    // 设置默认URL
    ui->lineEditUrl->setText("http://localhost:1234/v1/chat/completions");
    // 设置默认模型名称
    ui->lineEditModel->setText("qwen2.5-coder-3b-instruct");
    
    // 设置按钮文字
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(u8"确定");
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(u8"取消");
}

HttpConfigDialog::~HttpConfigDialog()
{
    delete ui;  // 释放UI对象
}

QString HttpConfigDialog::getUrl() const
{
    return ui->lineEditUrl->text();  // 返回URL输入框的内容
}

QString HttpConfigDialog::getModel() const
{
    return ui->lineEditModel->text();  // 返回模型名称输入框的内容
}

void HttpConfigDialog::testConnection()
{
    QString url = ui->lineEditUrl->text();  // 获取URL
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
    } else {
        QMessageBox::critical(this, tr(u8"错误"), tr(u8"连接测试失败：%1").arg(reply->errorString()));
    }

    reply->deleteLater();  // 安全删除回复对象
}
