#include "configmanager.h"

#include <QStandardPaths>
#include <QFile>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QDebug>

ConfigManager::ConfigManager(QObject *parent)
    : QObject(parent)
{
    m_configPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/cove/config.json";

    connect(&m_watcher, &QFileSystemWatcher::fileChanged,
            this, &ConfigManager::onConfigFileChanged);

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

void ConfigManager::registerObserver(IConfigObserver *observer)
{
    if (!m_observers.contains(observer)) {
        m_observers.append(observer);
    }
}

void ConfigManager::load()
{
    QFile file(m_configPath);
    if (!file.open(QIODevice::ReadOnly)) {
        emit configLoadFailed("Cannot open config file: " + m_configPath);
        qWarning() << "ConfigManager: Failed to open file:" << m_configPath;
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
    for (IConfigObserver *observer : m_observers) {
        observer->onConfigReloaded();
    }

    if (!m_watcher.files().contains(m_configPath)) {
        m_watcher.addPath(m_configPath);
    }
}

void ConfigManager::onConfigFileChanged(const QString &path)
{
    qDebug() << "ConfigManager: file changed, reloading..." << path;

    m_watcher.removePath(path);
    QTimer::singleShot(200, this, [this]() {
        load();
    });
}

