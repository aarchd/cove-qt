#include "configmanager.h"

#include <QStandardPaths>
#include <QFile>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QDebug>

ConfigManager::ConfigManager(QObject *parent)
    : QObject(parent)
{
    connect(&m_reloadTimer, &QTimer::timeout, this, &ConfigManager::load);
    m_reloadTimer.setInterval(5000); // 5 seconds for example
    m_reloadTimer.start();

    load();
}

ConfigManager& ConfigManager::instance()
{
    static ConfigManager manager;
    return manager;
}

QJsonObject ConfigManager::config() const
{
    return m_config;
}

void ConfigManager::reload()
{
    load();
}

void ConfigManager::load()
{
    const QString path = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/cove/config.json";
    QFile file(path);

    if (!file.open(QIODevice::ReadOnly)) {
        emit configLoadFailed("Cannot open config file: " + path);
        qWarning() << "ConfigManager: Failed to open file:" << path;
        return;
    }

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &error);
    file.close();

    if (error.error != QJsonParseError::NoError) {
        emit configLoadFailed("JSON parse error: " + error.errorString());
        qWarning() << "ConfigManager: JSON error:" << error.errorString();
        return;
    }

    if (!doc.isObject()) {
        emit configLoadFailed("Root JSON is not an object.");
        qWarning() << "ConfigManager: Root is not an object.";
        return;
    }

    m_config = doc.object();
    emit configReloaded();
}
