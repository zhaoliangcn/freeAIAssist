#ifndef AICONFIG_H
#define AICONFIG_H

#include <QObject>
#include <QJsonObject>

class AIConfig : public QObject
{
    Q_OBJECT

public:
    static AIConfig& instance();
private:
    explicit AIConfig(QObject *parent = nullptr);

    void saveConfig(const QJsonObject &config);
    QJsonObject loadConfig();

    
    QString getAssistantIdentity() const;
    void setAssistantIdentity(const QString &identity);
    QString getSystemPrompt() const;
    void setSystemPrompt(const QString &prompt);
    QString getUrl() const;
    void setUrl(const QString &url);
    QString getModel() const;
    void setModel(const QString &model);
    QString getToken() const;
    void setToken(const QString &token);
    QString getRole() const;
    void setRole(const QString &role);
    QString getRolePrompt() const;
    void setRolePrompt(const QString &prompt);
    
private:
    static AIConfig* m_instance;
    QString configFilePath;
    QMutex mutex;
    QString  AssistantIdentity;
    QString  SystemPrompt;
    QString  Url;
    QString  Model;
    QString  Token;
    QString  Role;
    QString  RolePrompt;

};

#endif // AICONFIG_H