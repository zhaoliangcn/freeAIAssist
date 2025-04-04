#include "aiassistantconfigdialog.h"
#include "ui_aiassistantconfigdialog.h"
#include <QPushButton>

AiAssistantConfigDialog::AiAssistantConfigDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AiAssistantConfigDialog)
{
    ui->setupUi(this);
    ui->textEditRole->setText(u8"你是一个高级软件工程师，具有多年开发经验");
    
    // 设置按钮文字
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(u8"确定");
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(u8"取消");
    
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &AiAssistantConfigDialog::onAccepted);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &AiAssistantConfigDialog::onRejected);
}

AiAssistantConfigDialog::~AiAssistantConfigDialog()
{
    delete ui;
}

QString AiAssistantConfigDialog::getAssistantIdentity() const
{
    return ui->textEditRole->toPlainText();
}

QString AiAssistantConfigDialog::getSystemPrompt() const
{
    return ui->textEditSystemPrompt->toPlainText();
}

void AiAssistantConfigDialog::setSystemPrompt(const QString &prompt)
{
    ui->textEditSystemPrompt->setText(prompt);
}

void AiAssistantConfigDialog::onAccepted()
{
    // 保存配置
    accept();
}

void AiAssistantConfigDialog::onRejected()
{
    // 取消操作
    reject();
}
