#ifndef AIASSISTANTCONFIGDIALOG_H
#define AIASSISTANTCONFIGDIALOG_H

#include <QDialog>

namespace Ui {
class AiAssistantConfigDialog;
}

class AiAssistantConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AiAssistantConfigDialog(QWidget *parent = nullptr);
    ~AiAssistantConfigDialog();
    QString getAssistantIdentity() const;
    QString getSystemPrompt() const;
    void setSystemPrompt(const QString &prompt);

private slots:
    void onAccepted();
    void onRejected();

private:
    Ui::AiAssistantConfigDialog *ui;
};

#endif // AIASSISTANTCONFIGDIALOG_H