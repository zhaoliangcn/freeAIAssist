#ifndef COMMONSYSTEMPROMPTDIALOG_H
#define COMMONSYSTEMPROMPTDIALOG_H

#include <QDialog>
#include <QContextMenuEvent>
QT_BEGIN_NAMESPACE
namespace Ui { class CommonSystemPromptDialog; }
QT_END_NAMESPACE

class CommonSystemPromptDialog : public QDialog
{
    Q_OBJECT

public:
    CommonSystemPromptDialog(QWidget *parent = nullptr);
    ~CommonSystemPromptDialog();
    void contextMenuEvent(QContextMenuEvent *event);
private:
    Ui::CommonSystemPromptDialog *ui;
};
#endif // COMMONSYSTEMPROMPTDIALOG_H