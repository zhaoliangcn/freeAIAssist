#include "KnowledgeBase.h"
#include "ui_KnowledgeBase.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QListWidget>
#include <QLineEdit>
#include <QTextBrowser>

KnowledgeBase::KnowledgeBase(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KnowledgeBase)
{
    ui->setupUi(this);

}

KnowledgeBase::~KnowledgeBase()
{
    delete ui;
}

void KnowledgeBase::on_search(const QString &text)
{
    // 实现搜索功能
}

void KnowledgeBase::on_docSelected(QListWidgetItem *item)
{
    // 实现文档内容展示
}

void KnowledgeBase::addDocument(const QString &id, const QString &content)
{
    documents[id] = content;
}

QString KnowledgeBase::getDocument(const QString &id) const
{
    return documents.value(id, QString());
}

QMap<QString, QString> KnowledgeBase::searchDocuments(const QString &query) const
{
    QMap<QString, QString> results;
    for (auto it = documents.constBegin(); it != documents.constEnd(); ++it) {
        if (it.value().contains(query)) {
            results[it.key()] = it.value();
        }
    }
    return results;
}

void KnowledgeBase::removeDocument(const QString &id)
{
    documents.remove(id);
}

void KnowledgeBase::AddFile(const QString &filePath) {
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString content = file.readAll();
        file.close();

        QString id = QFileInfo(filePath).fileName();
        addDocument(id, content);

        QString destination = QCoreApplication::applicationDirPath() + "/KnowledgeBase/" + id;
        QFile::copy(filePath, destination);
    }
}