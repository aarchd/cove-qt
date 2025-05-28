#pragma once

#include <QObject>
#include <QStringList>
#include <QSizeF>

class Dock : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList iconPaths READ iconPaths NOTIFY iconPathsChanged)

    Q_PROPERTY(int widthPercent READ widthPercent NOTIFY dockConfigChanged)
    Q_PROPERTY(int heightPercent READ heightPercent NOTIFY dockConfigChanged)
    Q_PROPERTY(int iconWidthPercent READ iconWidthPercent NOTIFY dockConfigChanged)
    Q_PROPERTY(int iconHeightPercent READ iconHeightPercent NOTIFY dockConfigChanged)
    Q_PROPERTY(int bottomMarginPercent READ bottomMarginPercent NOTIFY dockConfigChanged)

public:
    explicit Dock(QObject *parent = nullptr);

    QStringList iconPaths() const;

    int widthPercent() const;
    int heightPercent() const;
    int iconWidthPercent() const;
    int iconHeightPercent() const;
    int bottomMarginPercent() const;

signals:
    void iconPathsChanged();
    void dockConfigChanged();

private:
    void loadConfig();
    void loadDesktopIcons();

    QStringList m_iconPaths;

    int m_widthPercent;
    int m_heightPercent;
    int m_iconWidthPercent;
    int m_iconHeightPercent;
    int m_bottomMarginPercent;

    QStringList m_apps; // desktop file names from config
};
