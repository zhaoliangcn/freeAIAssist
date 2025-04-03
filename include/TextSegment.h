#ifndef TEXTSegment_H
#define TEXTSegment_H

#include <QDialog>

namespace Ui {
class TextSegment;
}

class TextSegment : public QDialog
{
    Q_OBJECT

public:
    explicit TextSegment(QWidget *parent = nullptr);
    ~TextSegment();

private slots:
    void on_selectFileButton_clicked();
    void on_processButton_clicked();

private:
    Ui::TextSegment *ui;
    void splitText(const QString &text);
    QStringList splitTextByNewlines(const QString &text);
    QStringList splitTextByEmptyLines(const QString &text);
};

#endif // TEXTSegment_H