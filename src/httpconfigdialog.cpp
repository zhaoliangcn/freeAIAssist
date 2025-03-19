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
    connect(ui->testButton, &QPushButton::clicked, this, &HttpConfigDialog::testConnection);
    connect(networkManager, &QNetworkAccessManager::finished, this, &HttpConfigDialog::handleTestResponse);
    ui->lineEditUrl->setText("http://localhost:1234/v1/chat/completions");
    ui->lineEditModel->setText("qwen2.5-coder-3b-instruct");
    
    // 设置按钮文字
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(u8"确定");
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(u8"取消");
}

HttpConfigDialog::~HttpConfigDialog()
{
    delete ui;
}

QString HttpConfigDialog::getUrl() const
{
    return ui->lineEditUrl->text();
}

QString HttpConfigDialog::getModel() const
{
    return ui->lineEditModel->text();
}

void HttpConfigDialog::testConnection()
{
    QString url = ui->lineEditUrl->text();
    if (url.isEmpty()) {
        QMessageBox::warning(this, tr(u8"警告"), tr(u8"请先输入服务器地址"));
        return;
    }

    ui->testButton->setEnabled(false);
    ui->testButton->setText(tr(u8"测试中..."));

    QNetworkRequest request(url);
    networkManager->get(request);
}

void HttpConfigDialog::handleTestResponse(QNetworkReply *reply)
{
    ui->testButton->setEnabled(true);
    ui->testButton->setText(tr(u8"测试连接"));

    if (reply->error() == QNetworkReply::NoError) {
        QMessageBox::information(this, tr(u8"成功"), tr(u8"连接测试成功！"));
    } else {
        QMessageBox::critical(this, tr(u8"错误"), tr(u8"连接测试失败：%1").arg(reply->errorString()));
    }

    reply->deleteLater();
}
