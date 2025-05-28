#include "applauncher.h"
#include "desktopfile.h"

#include <QStandardPaths>
#include <QFileInfo>
#include <QDir>
#include <QDebug>
#include <QProcess>
#include <QRegularExpression>
#include <QSettings>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

bool AppLauncher::launchApp(const QString &desktopFileName)
{
    const QStringList appDirs = QStandardPaths::standardLocations(QStandardPaths::ApplicationsLocation);

    for (const QString &dir : appDirs) {
        QString path = QDir(dir).filePath(desktopFileName);
        if (!QFileInfo::exists(path))
            continue;

        DesktopFile desktopFile(path);
        if (!desktopFile.isValid()) {
            qWarning() << "Invalid desktop file:" << path;
            return false;
        }

        QSettings settings(path, QSettings::IniFormat);
        settings.beginGroup("Desktop Entry");
        QString execLine = settings.value("Exec").toString();
        settings.endGroup();

        if (execLine.isEmpty()) {
            qWarning() << "Exec line empty in:" << path;
            return false;
        }

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
            qDebug() << "Launched process PID:" << pid;
            return true;
        } else {
            qWarning() << "fork() failed";
            return false;
        }
    }

    qWarning() << "Desktop file not found:" << desktopFileName;
    return false;
}
