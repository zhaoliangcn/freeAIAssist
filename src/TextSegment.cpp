#include "TextSegment.h"
#include "ui_TextSegment.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>

TextSegment::TextSegment(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TextSegment)
{
    ui->setupUi(this);
}

TextSegment::~TextSegment()
{
    delete ui;
}

void TextSegment::on_processButton_clicked()
{
    QString text = ui->resultText->toPlainText();
    QStringList segments = splitTextByEmptyLines(text);
    ui->resultText->setText(segments.join("\n---\n"));
}

void TextSegment::on_selectFileButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("选择文件"), "", tr("文本文件 (*.txt);;所有文件 (*)"));
    if (!fileName.isEmpty()) {
        ui->fileNameText->setText(fileName);
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            in.setCodec("UTF-8");
            QString content = in.readAll();
            QStringList segments = splitTextByEmptyLines(content);
            ui->resultText->setText(segments.join("\n---\n")); // 用分隔符显示分割结果
            file.close();
        }
    }
}

QStringList TextSegment::splitTextByNewlines(const QString &text)
{
    return text.split("\n");
}

QStringList TextSegment::splitTextByEmptyLines(const QString &text)
{
    QStringList segments = text.split("\n\n");
    for (int i = 0; i < segments.size(); ++i) {
        segments[i] = QString("## Segment %1\n\n%2").arg(i+1).arg(segments[i]);
    }
    return segments;
}