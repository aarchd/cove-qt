#pragma once

#include <QObject>
#include <QJsonObject>
#include <QTimer>

class ConfigManager : public QObject
{
    Q_OBJECT

public:
    static ConfigManager& instance();
    QJsonObject config() const;

    void reload();

signals:
    void configReloaded();
    void configLoadFailed(const QString &error);

private:
    explicit ConfigManager(QObject *parent = nullptr);
    void load();

    QJsonObject m_config;
    QTimer m_reloadTimer;
};
