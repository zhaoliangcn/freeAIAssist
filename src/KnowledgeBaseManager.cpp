#include "KnowledgeBaseManager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

KnowledgeBaseManager::KnowledgeBaseManager(QObject *parent) 
{
}

void KnowledgeBaseManager::addKnowledgeBase(const QString &name, const QString &path)
{
    knowledgeBases.insert(name, path);
    saveToDistributedStorage(name, path);
}

void KnowledgeBaseManager::initializeDistributedStorage()
{
    // 初始化分布式存储系统
}

void KnowledgeBaseManager::saveToDistributedStorage(const QString &name, const QString &path)
{
    // 将文件保存到分布式存储系统
}

void KnowledgeBaseManager::removeFromDistributedStorage(const QString &name)
{
    // 从分布式存储系统中移除文件
}

void KnowledgeBaseManager::removeKnowledgeBase(const QString &name)
{
    knowledgeBases.remove(name);
}

void KnowledgeBaseManager::switchKnowledgeBase(const QString &name)
{
    if (knowledgeBases.contains(name)) {
        currentKnowledgeBase = name;
    }
}

void KnowledgeBaseManager::saveConfig(const QString &path)
{
    QFile file(path);
    if (file.open(QIODevice::WriteOnly)) {
        QJsonObject json;
        json.insert("current", currentKnowledgeBase);
        QJsonObject bases;
        for (auto it = knowledgeBases.begin(); it != knowledgeBases.end(); ++it) {
            bases.insert(it.key(), it.value());
        }
        json.insert("bases", bases);
        QJsonDocument doc(json);
        file.write(doc.toJson());
        file.close();
    }
}

void KnowledgeBaseManager::loadConfig(const QString &path)
{
    QFile file(path);
    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        QJsonObject json = doc.object();
        currentKnowledgeBase = json["current"].toString();
        QJsonObject bases = json["bases"].toObject();
        for (auto it = bases.begin(); it != bases.end(); ++it) {
            knowledgeBases.insert(it.key(), it.value().toString());
        }
        file.close();
    }
}