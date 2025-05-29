#pragma once

#include <QString>
#include <QIcon>
#include <QStringList>
#include <QSize>

class DesktopFile
{
public:
    explicit DesktopFile(const QString &filePath);

    static QStringList loadDesktopIcons(const QStringList &desktopFileNames, const QSize &iconSize = QSize(64, 64));
    static QStringList loadDesktopNames(const QStringList &desktopFileNames);

    bool isValid() const;
    QString type() const;
    QString iconName() const;
    QString filePath() const;
    QString name() const;

    QIcon icon() const;

private:
    QString m_filePath;
    QString m_type;
    QString m_iconName;
    QString m_name;
    bool m_valid = false;

    void parse();
};
