#include "desktopfile.h"
#include <QSettings>

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
