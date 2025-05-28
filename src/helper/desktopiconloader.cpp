#include "desktopiconloader.h"
#include "desktopfile.h"

#include <QStandardPaths>
#include <QDir>
#include <QFileInfo>
#include <QPixmap>
#include <QIcon>
#include <QUrl>
#include <QDebug>
#include <QFile>

QStringList DesktopIconLoader::loadDesktopIcons(const QStringList &desktopFileNames, const QSize &iconSize)
{
    QStringList iconPaths;

    // Cache directory
    const QString cacheDir = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/cove/icons";
    QDir().mkpath(cacheDir);

    // Cache app dirs once
    const QStringList appDirs = QStandardPaths::standardLocations(QStandardPaths::ApplicationsLocation);

    // Memoize cached icons by desktop filename + size to avoid duplicate work
    QHash<QString, QString> cachedIcons;

    for (const QString &desktopFileName : desktopFileNames) {
        if (!desktopFileName.endsWith(".desktop", Qt::CaseInsensitive)) {
            qWarning() << "Skipping non-.desktop file:" << desktopFileName;
            continue;
        }

        // Compose cache filename
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
            // Optionally add fallback icon path here
        }
    }

    return iconPaths;
}
