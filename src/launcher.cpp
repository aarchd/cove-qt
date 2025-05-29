#include "launcher.h"
#include "helper/desktopiconloader.h"
#include "helper/applauncher.h"
#include "helper/desktopfile.h"

#include <QStandardPaths>
#include <QDir>
#include <QDebug>
#include <algorithm>

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

    m_columns = config.value("columns").toInt(4);
    m_spacing = config.value("spacing").toInt(10);
    m_iconSize = config.value("iconSize").toInt(64);
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

    const int bottomCount = m_columns;
    m_bottomRowApps = m_allValidApps.mid(0, bottomCount);
    m_gridApps = m_allValidApps.mid(bottomCount);

    m_bottomRowIcons = DesktopIconLoader::loadDesktopIcons(m_bottomRowApps);
    m_gridIcons = DesktopIconLoader::loadDesktopIcons(m_gridApps);
}

QStringList Launcher::bottomRowIcons() const { return m_bottomRowIcons; }
QStringList Launcher::gridIcons() const { return m_gridIcons; }
int Launcher::columns() const { return m_columns; }
int Launcher::spacing() const { return m_spacing; }
int Launcher::iconSize() const { return m_iconSize; }

void Launcher::launchApp(const QString &desktopFileName)
{
    AppLauncher::launchApp(desktopFileName);
}
