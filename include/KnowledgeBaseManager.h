#ifndef KNOWLEDGEBASEMANAGER_H
#define KNOWLEDGEBASEMANAGER_H

#include <QObject>
#include <QMap>
#include <QString>

class KnowledgeBaseManager 
{
public:
    explicit KnowledgeBaseManager(QObject *parent = nullptr);

    void addKnowledgeBase(const QString &name, const QString &path);
    void removeKnowledgeBase(const QString &name);
    void switchKnowledgeBase(const QString &name);
    void saveConfig(const QString &path);
    void loadConfig(const QString &path);

private:
    QMap<QString, QString> knowledgeBases;
    QString currentKnowledgeBase;
    QString distributedStoragePath;
    void initializeDistributedStorage();
    void saveToDistributedStorage(const QString &name, const QString &path);
    void removeFromDistributedStorage(const QString &name);
};

#endif // KNOWLEDGEBASEMANAGER_H