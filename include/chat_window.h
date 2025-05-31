#ifndef CHAT_WINDOW_H
#define CHAT_WINDOW_H

#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "mcpclient.h"
#include "speechwindow.h"
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
    void setSystemPrompt(const QString &prompt);
    QString getSystemPrompt() const;
private:
    Ui::ChatWindow *ui;
    QNetworkAccessManager *networkManager;
    QStringList messageHistory;
    QString currentMessage;
    QString currentKnowledgeBase;
    QString systemPrompt;

    // MCP client related members
    MCPClient* mcpClient;


    void  initKnowledgeBaseList();

    bool enableTTS;
    SpeechWindow  speechwindow;
private slots:
    void handleNetworkReply(QNetworkReply *reply);
    void on_sendButton_clicked();
    void on_clearContextButton_clicked();
    void on_knowledgeBaseComboBox_currentIndexChanged(const QString &text);

    void handleMCPMessage(const QByteArray &message);
    void handleMCPError(const QString &error);

private slots:
    void on_ttsCheckBox_stateChanged(int state);
};

#endif // CHAT_WINDOW_H