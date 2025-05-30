#include "launcher.h"
#include "helper/desktopfile.h"

#include <QStandardPaths>
#include <QDir>
#include <QDebug>
#include <QJsonArray>

Launcher::Launcher(QObject *parent)
    : QObject(parent)
{
    loadConfig();
    loadApps();
    ConfigManager::instance().registerObserver(this);
}

void Launcher::onConfigReloaded()
{
    loadConfig();
    loadApps();
    Q_EMIT iconDataChanged();
}

void Launcher::loadConfig()
{
    const QJsonObject root = ConfigManager::instance().config();
    const QJsonObject launcherObj = root.value("launcher").toObject();
    const QJsonObject config = launcherObj.value("config").toObject();

    QString path = config.value("wallpaperPath").toString("/usr/share/cove/wallpaper.png");
    m_wallpaperPath = QUrl::fromLocalFile(path).toString();
    m_spacing = config.value("spacing").toInt(10);
    m_iconSize = config.value("iconSize").toInt(64);
    m_emptyPane = config.value("emptyPane").toInt(1);
    m_bottomRowColor = config.value("bottomRowColor").toString("#22000000");
    m_bottomRowWidthPercent = config.value("bottomRowWidthPercent").toInt(80);
    m_bottomRowRadius = config.value("bottomRowRadius").toInt(10);
    
    m_favAppsNames.clear();
    const QJsonArray favAppsArray = launcherObj.value("favApps").toArray();
    for (const QJsonValue &val : favAppsArray) {
        if (val.isString()) {
            m_favAppsNames.append(val.toString());
        }
    }
}

void Launcher::loadApps()
{
    m_allValidApps = DesktopFile::filterDesktopFiles();

    m_allApps = DesktopFile::loadDesktopEntries(m_allValidApps);

    m_allAppNames.clear();
    m_allIcons.clear();
    for (const auto &entry : m_allApps) {
        m_allAppNames << entry.first;
        m_allIcons << entry.second;
    }

    QList<QPair<QString, QString>> favApps = DesktopFile::loadDesktopEntries(m_favAppsNames);

    m_favAppsIcons.clear();
    for (const auto &entry : favApps) {
        m_favAppsIcons << entry.second;
    }
}

QStringList Launcher::allIcons() const { return m_allIcons; }
QStringList Launcher::allAppNames() const { return m_allAppNames; }
QStringList Launcher::favAppsNames() const { return m_favAppsNames; }
QStringList Launcher::favAppsIcons() const { return m_favAppsIcons; }

QString Launcher::wallpaperPath() const { return m_wallpaperPath; }
int Launcher::spacing() const { return m_spacing; }
int Launcher::iconSize() const { return m_iconSize; }
int Launcher::emptyPane() const { return m_emptyPane; }
QString Launcher::bottomRowColor() const { return m_bottomRowColor; }
int Launcher::bottomRowWidthPercent() const { return m_bottomRowWidthPercent; }
int Launcher::bottomRowRadius() const { return m_bottomRowRadius; }

void Launcher::launchApp(const QString &desktopFileName)
{
    DesktopFile::launch(desktopFileName);
}
