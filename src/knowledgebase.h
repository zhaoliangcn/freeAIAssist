#ifndef KNOWLEDGEBASE_H
#define KNOWLEDGEBASE_H

#include <QObject>
#include <QString>
#include <QMap>

class KnowledgeBase : public QObject
{
    Q_OBJECT

public:
    explicit KnowledgeBase(QObject *parent = nullptr);

    void addDocument(const QString &id, const QString &content);
    QString getDocument(const QString &id) const;
    QMap<QString, QString> searchDocuments(const QString &query) const;
    void removeDocument(const QString &id);

private:
    QMap<QString, QString> documents;
};

#endif // KNOWLEDGEBASE_H