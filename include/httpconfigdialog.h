#ifndef HTTPCONFIGDIALOG_H
#define HTTPCONFIGDIALOG_H

#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>
QT_BEGIN_NAMESPACE
namespace Ui {
class HttpConfigDialog;
}
QT_END_NAMESPACE
class HttpConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HttpConfigDialog(QWidget *parent = nullptr);
    ~HttpConfigDialog();

    QString getUrl() const;
    QString getModel() const;

    QString getToken() const;
    void setToken(const QString &token);
    void saveConfig();

private slots:
    void testConnection();
    void handleTestResponse(QNetworkReply *reply);

private:
    void init_ui();  // 添加初始化UI的私有函数声明
    void onConfigChanged(int index);  // 添加配置改变处理函数声明
    Ui::HttpConfigDialog *ui;
    QNetworkAccessManager *networkManager;
};

#endif // HTTPCONFIGDIALOG_H
