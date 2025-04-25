#include "aiconfig.h"
#include <QFile>
#include <QJsonDocument>

AIConfig* AIConfig::m_instance = nullptr;

AIConfig::AIConfig(QObject *parent) : QObject(parent)
{
    configFilePath = "config.json";
}
AIConfig& AIConfig::instance()
{
    if (!m_instance)
    {
        m_instance = new AIConfig();
    }
    return *m_instance;
}

void AIConfig::saveConfig(const QJsonObject &config)
{
    QMutexLocker locker(&mutex);

    QFile file(configFilePath);
    if (file.open(QIODevice::WriteOnly)) {
        QJsonObject fullConfig = config;
        fullConfig["AssistantIdentity"] = AssistantIdentity;
        fullConfig["SystemPrompt"] = SystemPrompt;
        fullConfig["Url"] = Url;
        fullConfig["Model"] = Model;
        fullConfig["Token"] = Token;
        fullConfig["Role"] = Role;
        fullConfig["RolePrompt"] = RolePrompt;
        QJsonDocument doc(fullConfig);
        file.write(doc.toJson());
        file.close();
    }
}

QJsonObject AIConfig::loadConfig()
{
    QMutexLocker locker(&mutex);

    QJsonObject config;
    QFile file(configFilePath);
    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        config = doc.object();
        AssistantIdentity = config["AssistantIdentity"].toString();
        SystemPrompt = config["SystemPrompt"].toString();
        Url = config["Url"].toString();
        Model = config["Model"].toString();
        Token = config["Token"].toString();
        Role = config["Role"].toString();
        RolePrompt = config["RolePrompt"].toString();
        file.close();
    }
    return config;
}

QString AIConfig::getAssistantIdentity() const
{
    return AssistantIdentity;
}

void AIConfig::setAssistantIdentity(const QString &identity)
{
    AssistantIdentity = identity;
}

QString AIConfig::getSystemPrompt() const
{
    return SystemPrompt;
}

void AIConfig::setSystemPrompt(const QString &prompt)
{
    SystemPrompt = prompt;
}

QString AIConfig::getUrl() const
{
    return Url;
}

void AIConfig::setUrl(const QString &url)
{
    Url = url;
}

QString AIConfig::getModel() const
{
    return Model;
}

void AIConfig::setModel(const QString &model)
{
    Model = model;
}

QString AIConfig::getToken() const
{
    return Token;
}

void AIConfig::setToken(const QString &token)
{
    Token = token;
}

QString AIConfig::getRole() const
{
    return Role;
}

void AIConfig::setRole(const QString &role)
{
    Role = role;
}

QString AIConfig::getRolePrompt() const
{
    return RolePrompt;
}

void AIConfig::setRolePrompt(const QString &prompt)
{
    RolePrompt = prompt;
}