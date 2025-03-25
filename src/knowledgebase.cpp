#include "knowledgebase.h"

KnowledgeBase::KnowledgeBase(QObject *parent) : QObject(parent)
{
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