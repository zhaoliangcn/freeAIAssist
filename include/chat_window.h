#ifndef CHAT_WINDOW_H
#define CHAT_WINDOW_H

#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "mcpclient.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class ChatWindow;
}
QT_END_NAMESPACE

class ChatWindow : public QDialog
{
    Q_OBJECT

public:
    void sendMessage(const QString &message);
    explicit ChatWindow(QWidget *parent = nullptr);
    ~ChatWindow();

private:
    Ui::ChatWindow *ui;
    QNetworkAccessManager *networkManager;
    QStringList messageHistory;
    QString currentMessage;

    // MCP client related members
    MCPClient* mcpClient;

private slots:
    void handleNetworkReply(QNetworkReply *reply);
    void on_sendButton_clicked();
    void on_clearContextButton_clicked();


    void handleMCPMessage(const QByteArray &message);
    void handleMCPError(const QString &error);
};

#endif // CHAT_WINDOW_H