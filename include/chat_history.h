#ifndef CHAT_HISTORY_H
#define CHAT_HISTORY_H

#include <QDialog>
#include <OperationLogger.h>

namespace Ui {
    class ChatHistoryWindow;
    }
    
//#include "OperationLogger.hpp" // 添加对OperationLogger.hpp的引用

class ChatHistoryWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ChatHistoryWindow(QWidget *parent = nullptr);
    ~ChatHistoryWindow();

protected:
    void showEvent(QShowEvent *event) override;

private slots:
    void loadChatHistory();

private:
    Ui::ChatHistoryWindow *ui;
    OperationLogger logger; // 添加OperationLogger对象
    void displayLogEntries(); // 添加获取日志信息的方法
};

#endif // CHAT_HISTORY_H