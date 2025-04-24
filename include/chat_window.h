#ifndef CHAT_WINDOW_H
#define CHAT_WINDOW_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
QT_BEGIN_NAMESPACE
namespace Ui {
class ChatWindow;
}
QT_END_NAMESPACE

class ChatWindow : public QWidget
{
    Q_OBJECT

public:
    void sendMessage(const QString &message);
    explicit ChatWindow(QWidget *parent = nullptr);
    ~ChatWindow();

private:
    Ui::ChatWindow *ui;
    QNetworkAccessManager *networkManager;

private slots:
    void handleNetworkReply(QNetworkReply *reply);
    void on_sendButton_clicked();
};

#endif // CHAT_WINDOW_H