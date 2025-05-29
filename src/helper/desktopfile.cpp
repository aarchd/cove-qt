#include "desktopfile.h"
#include <QSettings>
#include <QStandardPaths>
#include <QDir>
#include <QFileInfo>
#include <QPixmap>
#include <QIcon>
#include <QUrl>
#include <QDebug>
#include <QFile>

DesktopFile::DesktopFile(const QString &filePath)
    : m_filePath(filePath)
{
    parse();
}

void DesktopFile::parse()
{
    QSettings desktopFile(m_filePath, QSettings::IniFormat);

    desktopFile.beginGroup("Desktop Entry");
    m_type = desktopFile.value("Type").toString();
    m_iconName = desktopFile.value("Icon").toString();
    desktopFile.endGroup();

    m_valid = (m_type == "Application") && !m_iconName.isEmpty();
}

bool DesktopFile::isValid() const
{
    return m_valid;
}

QString DesktopFile::type() const
{
    return m_type;
}

QString DesktopFile::iconName() const
{
    return m_iconName;
}

QString DesktopFile::filePath() const
{
    return m_filePath;
}

QIcon DesktopFile::icon() const
{
    QIcon icon = QIcon::fromTheme(m_iconName);
    if (icon.isNull()) {
        icon = QIcon(m_iconName);
    }
    return icon;
}

QStringList DesktopFile::loadDesktopIcons(const QStringList &desktopFileNames, const QSize &iconSize)
{
    QStringList iconPaths;

    const QString cacheDir = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/cove/icons";
    QDir().mkpath(cacheDir);

    const QStringList appDirs = QStandardPaths::standardLocations(QStandardPaths::ApplicationsLocation);

    QHash<QString, QString> cachedIcons;

    for (const QString &desktopFileName : desktopFileNames) {
        if (!desktopFileName.endsWith(".desktop", Qt::CaseInsensitive)) {
            qWarning() << "Skipping non-.desktop file:" << desktopFileName;
            continue;
        }

        const QString cacheFileName = desktopFileName + QString("_%1x%2.png").arg(iconSize.width()).arg(iconSize.height());
        const QString cachedIconPath = QDir(cacheDir).filePath(cacheFileName);

        if (QFile::exists(cachedIconPath)) {
            iconPaths << QUrl::fromLocalFile(cachedIconPath).toString();
            continue;
        }

        bool found = false;

        for (const QString &appDirPath : appDirs) {
            QDir appDir(appDirPath);
            QFileInfo desktopFileInfo(appDir.filePath(desktopFileName));

            if (!desktopFileInfo.exists() || !desktopFileInfo.isFile())
                continue;

            DesktopFile desktopFile(desktopFileInfo.absoluteFilePath());
            if (!desktopFile.isValid()) {
                qWarning() << "Invalid desktop file or no icon:" << desktopFileInfo.absoluteFilePath();
                continue;
            }

            QIcon icon = desktopFile.icon();
            if (icon.isNull()) {
                qWarning() << "Icon is null for desktop file:" << desktopFileInfo.absoluteFilePath();
                continue;
            }

            QPixmap pixmap = icon.pixmap(iconSize);
            if (pixmap.isNull()) {
                qWarning() << "Pixmap is null for icon:" << desktopFileInfo.absoluteFilePath();
                continue;
            }

            if (!pixmap.save(cachedIconPath)) {
                qWarning() << "Failed to save icon to cache:" << cachedIconPath;
                continue;
            }

            iconPaths << QUrl::fromLocalFile(cachedIconPath).toString();
            found = true;
            break;
        }

        if (!found) {
            qWarning() << "Could not find icon for app desktop file:" << desktopFileName;
        }
    }

    return iconPaths;
}
