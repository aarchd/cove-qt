#pragma once

#include <QString>
#include <QIcon>
#include <QStringList>
#include <QSize>

class DesktopFile
{
public:
    explicit DesktopFile(const QString &filePath);

    static QList<QPair<QString, QString>> loadDesktopEntries(const QStringList &desktopFileNames, const QSize &iconSize = QSize(64, 64));
    static QStringList filterDesktopFiles();
    static bool launch(const QString &desktopFileName);

    bool isValid() const;
    QString name() const;
    QString execLine() const;
    
    QIcon icon() const;

private:
    QString m_filePath;
    QString m_type;
    QString m_iconName;
    QString m_name;
    QString m_execLine;
    bool m_valid = false;

    void parse();
};
