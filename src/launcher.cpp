#include "launcher.h"
#include "helper/applauncher.h"
#include "helper/desktopfile.h"

#include <QStandardPaths>
#include <QDir>
#include <QDebug>
#include <algorithm>
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
    emit iconDataChanged();
}

void Launcher::loadConfig()
{
    const QJsonObject root = ConfigManager::instance().config();
    const QJsonObject launcherObj = root.value("launcher").toObject();
    const QJsonObject config = launcherObj.value("config").toObject();

    m_spacing = config.value("spacing").toInt(10);
    m_iconSize = config.value("iconSize").toInt(64);
    m_bottomRowColor = config.value("bottomRowColor").toString("#22000000");
    m_bottomRowWidthPercent = config.value("bottomRowWidthPercent").toInt(80);
    m_bottomRowRadius = config.value("bottomRowRadius").toInt(10);
    
    m_favApps.clear();
    const QJsonArray favAppsArray = launcherObj.value("favApps").toArray();
    for (const QJsonValue &val : favAppsArray) {
        if (val.isString()) {
            m_favApps.append(val.toString());
        }
    }
}

QStringList Launcher::filterAndSortDesktopFiles() const
{
    QStringList validApps;
    const QStringList appDirs = QStandardPaths::standardLocations(QStandardPaths::ApplicationsLocation);

    for (const QString &dir : appDirs) {
        QDir d(dir);
        const QStringList files = d.entryList(QStringList("*.desktop"), QDir::Files);
        for (const QString &file : files) {
            const QString fullPath = d.filePath(file);
            DesktopFile df(fullPath);
            if (df.isValid())
                validApps.append(file);
        }
    }

    std::sort(validApps.begin(), validApps.end(), [](const QString &a, const QString &b) {
        DesktopFile dfa(a), dfb(b);
        return dfa.iconName().toLower() < dfb.iconName().toLower();
    });

    return validApps;
}

void Launcher::loadApps()
{
    m_allValidApps = filterAndSortDesktopFiles();
    m_allIcons = DesktopFile::loadDesktopIcons(m_allValidApps);
    m_allAppNames = DesktopFile::loadDesktopNames(m_allValidApps);
    m_favAppsIcons = DesktopFile::loadDesktopIcons(m_favApps);
}

QStringList Launcher::allIcons() const { return m_allIcons; }
QStringList Launcher::allAppNames() const { return m_allAppNames; }
QStringList Launcher::favApps() const { return m_favApps; }
QStringList Launcher::favAppsIcons() const { return m_favAppsIcons; }

int Launcher::spacing() const { return m_spacing; }
int Launcher::iconSize() const { return m_iconSize; }
QString Launcher::bottomRowColor() const { return m_bottomRowColor; }
int Launcher::bottomRowWidthPercent() const { return m_bottomRowWidthPercent; }
int Launcher::bottomRowRadius() const { return m_bottomRowRadius; }

void Launcher::launchApp(const QString &desktopFileName)
{
    AppLauncher::launchApp(desktopFileName);
}
