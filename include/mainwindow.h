#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include "httpconfigdialog.h"
#include "aiassistantconfigdialog.h"
#include "speechwindow.h"
#include "chat_history.h"
#include "commonsystempromptdialog.h"
#include "TextSegment.h"
#include "knowledgebase.h"
#include "chat_window.h" 
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();  // 假设按钮的对象名为 pushButton
    void handleNetworkReply(QNetworkReply *reply);
    void on_pushButton_selectFile_clicked(); 
    void on_AiServerConfig_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_selectFile2_clicked();
    void on_pushButton_3_clicked();
    void on_menuItem_triggered();
    void on_actionexit_triggered();
    void on_actioncommonprompt_triggered(); 
    void on_actionfileimport_triggered();
    void on_actionkbmanager_triggered();
    void on_actionchatwindow_triggered();
protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    void SimpleRequest();
    void RequestWithContext();
    Ui::MainWindow *ui;
    QNetworkAccessManager *networkManager;
    QString url;
    QString header;
    QString model;
    QString assistantrole;
    HttpConfigDialog configdlg;
    AiAssistantConfigDialog assistconfig;
    SpeechWindow  speechwindow;
    ChatHistoryWindow  chathistorywindow;
    CommonSystemPromptDialog  commonSystemPromptDialog;
    TextSegment textSegment;
    KnowledgeBase knowledgeBase;
    ChatWindow chatWindow;
    std::vector<QString> requests;
};
#endif // MAINWINDOW_H
