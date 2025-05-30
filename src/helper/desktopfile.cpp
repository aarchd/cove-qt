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
#include <QProcess>
#include <QRegularExpression>
#include <sys/wait.h>

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
    m_name = desktopFile.value("Name").toString();
    m_execLine = desktopFile.value("Exec").toString();
    desktopFile.endGroup();

    m_valid = (m_type == "Application") && !m_iconName.isEmpty();
}

bool DesktopFile::isValid() const { return m_valid; }
QString DesktopFile::name() const { return m_name; }
QString DesktopFile::execLine() const { return m_execLine; }
QIcon DesktopFile::icon() const { return QIcon::fromTheme(m_iconName); }

QList<QPair<QString, QString>> DesktopFile::loadDesktopEntries(const QStringList &desktopFileNames, const QSize &iconSize)
{
    QList<QPair<QString, QString>> entries;

    const QString cacheDir = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/cove/icons";
    QDir().mkpath(cacheDir);

    const QStringList appDirs = QStandardPaths::standardLocations(QStandardPaths::ApplicationsLocation);

    for (const QString &desktopFileName : desktopFileNames) {
        if (!desktopFileName.endsWith(".desktop", Qt::CaseInsensitive)) {
            qWarning() << "Skipping non-.desktop file:" << desktopFileName;
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

            QString cacheFileName = desktopFileName + QString("_%1x%2.png").arg(iconSize.width()).arg(iconSize.height());
            QString cachedIconPath = QDir(cacheDir).filePath(cacheFileName);

            if (!QFile::exists(cachedIconPath)) {
                QPixmap pixmap = icon.pixmap(iconSize);
                if (pixmap.isNull()) {
                    qWarning() << "Pixmap is null for icon:" << desktopFileInfo.absoluteFilePath();
                    continue;
                }
                if (!pixmap.save(cachedIconPath)) {
                    qWarning() << "Failed to save icon to cache:" << cachedIconPath;
                    continue;
                }
            }

            entries.append(qMakePair(desktopFile.name(), QUrl::fromLocalFile(cachedIconPath).toString()));

            found = true;
            break;
        }

        if (!found) {
            qWarning() << "Could not find icon for app desktop file:" << desktopFileName;
        }
    }

    std::sort(entries.begin(), entries.end(), [](const QPair<QString, QString> &a, const QPair<QString, QString> &b) {
        return a.first.toLower() < b.first.toLower();
    });

    return entries;
}

QStringList DesktopFile::filterDesktopFiles()
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

    return validApps;
}

bool DesktopFile::launch(const QString &desktopFileName)
{
    const QStringList appDirs = QStandardPaths::standardLocations(QStandardPaths::ApplicationsLocation);
    QString fullPath;

    for (const QString &appDirPath : appDirs) {
        QDir appDir(appDirPath);
        QFileInfo desktopFileInfo(appDir.filePath(desktopFileName));
        if (desktopFileInfo.exists() && desktopFileInfo.isFile()) {
            fullPath = desktopFileInfo.absoluteFilePath();
            break;
        }
    }

    if (fullPath.isEmpty()) {
        qWarning() << "Desktop file not found:" << desktopFileName;
        return false;
    }

    DesktopFile desktopFile(fullPath);
    if (!desktopFile.isValid()) {
        qWarning() << "Invalid desktop file:" << fullPath;
        return false;
    }

    QString execLine = desktopFile.execLine();
    execLine.replace(QRegularExpression(" ?%[fFuUdDnNickvm]"), "");

    QStringList args = QProcess::splitCommand(execLine);
    if (args.isEmpty()) {
        qWarning() << "Failed to parse Exec line:" << execLine;
        return false;
    }

    QByteArray cmd = args.takeFirst().toLocal8Bit();
    QByteArrayList argBytes;
    argBytes << cmd;
    for (const QString &arg : args)
        argBytes << arg.toLocal8Bit();

    std::vector<char*> argv;
    for (QByteArray &arg : argBytes)
        argv.push_back(arg.data());
    argv.push_back(nullptr);

    pid_t pid = fork();
    if (pid == 0) {
        setsid();
        execvp(cmd.data(), argv.data());
        _exit(127);
    } else if (pid > 0) {
        qDebug() << "Launched process PID:" << pid << "from desktop file:" << fullPath;
        return true;
    } else {
        qWarning() << "fork() failed";
        return false;
    }
}
