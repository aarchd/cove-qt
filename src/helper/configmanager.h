#pragma once

#include <QObject>
#include <QJsonObject>
#include <QTimer>
#include <QFileSystemWatcher>

class IConfigObserver
{
public:
    virtual ~IConfigObserver() = default;
    virtual void onConfigReloaded() = 0;
};


class ConfigManager : public QObject
{
    Q_OBJECT

public:
    static ConfigManager& instance();
    QJsonObject config() const;

    void reload();
    void registerObserver(IConfigObserver *observer);

Q_SIGNALS:
    void configReloaded();
    void configLoadFailed(const QString &error);

private:
    explicit ConfigManager(QObject *parent = nullptr);
    void load();
    void onConfigFileChanged(const QString &path);

    QJsonObject m_config;
    QFileSystemWatcher m_watcher;
    QString m_configPath;
    QList<IConfigObserver *> m_observers;
};
