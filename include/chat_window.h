#ifndef CHAT_WINDOW_H
#define CHAT_WINDOW_H

#include <QDialog>
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
    

private slots:
    void handleNetworkReply(QNetworkReply *reply);
    void on_sendButton_clicked();
    void on_clearContextButton_clicked();
};

#endif // CHAT_WINDOW_H