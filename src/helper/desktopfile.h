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

    bool isValid() const;
    QString type() const;
    QString iconName() const;
    QString filePath() const;

    QIcon icon() const;

private:
    QString m_filePath;
    QString m_type;
    QString m_iconName;
    bool m_valid = false;

    void parse();
};
