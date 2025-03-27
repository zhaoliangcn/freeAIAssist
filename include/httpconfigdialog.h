#ifndef HTTPCONFIGDIALOG_H
#define HTTPCONFIGDIALOG_H

#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>

namespace Ui {
class HttpConfigDialog;
}

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

private slots:
    void testConnection();
    void handleTestResponse(QNetworkReply *reply);

private:
    Ui::HttpConfigDialog *ui;
    QNetworkAccessManager *networkManager;
};

#endif // HTTPCONFIGDIALOG_H
