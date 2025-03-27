#ifndef KNOWLEDGEBASE_H
#define KNOWLEDGEBASE_H

#include <QDialog
#include <QString>

namespace Ui {
class KnowledgeBase;
}<QMap>


class KnowledgeBase : public QDialog
{
    Q_OBJECT

public:
    explicit KnowledgeBase(QDialog *parent = nullptr);

    void addDocument(const QString &id, const QString &content);
    QString getDocument(const QString &id) const;
    QMap<QString, QString> searchDocuments(const QString &query) const;
    void removeDocument(const QString &id);
    void AddFile(const QString &filePath);

private:
    QMap<QString, QString> documents;
};

#endif // KNOWLEDGEBASE_H