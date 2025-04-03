#include "chat_history.h"
#include "ui_chat_history.h"
ChatHistoryWindow::ChatHistoryWindow(QWidget *parent):
    ui(new Ui::ChatHistoryWindow),  // 创建UI对象
    QDialog(parent)
{
    ui->setupUi(this);
    // 初始化数据加载和显示逻辑
    loadChatHistory();
    // 设置窗口属性，添加最大化和最小化按钮
    setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);
}

ChatHistoryWindow::~ChatHistoryWindow() {
    delete ui;
}

void ChatHistoryWindow::displayLogEntries() {
     std::vector<OperationLogger::LogEntry> entries = logger.getLogEntries();
     // 设置表格的列数为3
     ui->listWidgetHistory->setColumnCount(3);
     // 设置表格的列标题
     QStringList headers; headers << "Datetime" << "Request" << "Response";
     ui->listWidgetHistory->setHorizontalHeaderLabels(headers);
     // 清空表格现有内容
     ui->listWidgetHistory->setRowCount(0);
     for (const auto& entry : entries) {
         int row = ui->listWidgetHistory->rowCount();
         ui->listWidgetHistory->insertRow(row);
         // 设置时间戳列
         ui->listWidgetHistory->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(entry.timestamp)));
         // 设置请求列
         ui->listWidgetHistory->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(entry.request)));
         // 设置响应列
         ui->listWidgetHistory->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(entry.response)));
     }
}

void ChatHistoryWindow::showEvent(QShowEvent *event) {
    loadChatHistory();
    QDialog::showEvent(event);
}

void ChatHistoryWindow::loadChatHistory() {
    // 实现数据加载逻辑
    // 示例：添加一些测试数据
    // listWidgetHistory->addItem("聊天记录1");
    // listWidgetHistory->addItem("聊天记录2");
    logger.reloadLogs();
    displayLogEntries();
}