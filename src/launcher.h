#pragma once

#include <QObject>
#include <QStringList>
#include "helper/configmanager.h"

class Launcher : public QObject, public IConfigObserver
{
    Q_OBJECT
    Q_PROPERTY(QStringList bottomRowIcons READ bottomRowIcons NOTIFY iconDataChanged)
    Q_PROPERTY(QStringList gridIcons READ gridIcons NOTIFY iconDataChanged)
    Q_PROPERTY(QStringList gridAppNames READ gridAppNames NOTIFY iconDataChanged)
    Q_PROPERTY(int columns READ columns NOTIFY iconDataChanged)
    Q_PROPERTY(int spacing READ spacing NOTIFY iconDataChanged)
    Q_PROPERTY(int iconSize READ iconSize NOTIFY iconDataChanged)

public:
    explicit Launcher(QObject *parent = nullptr);
    QStringList bottomRowIcons() const;
    QStringList gridIcons() const;
    QStringList gridAppNames() const;

    int columns() const;
    int spacing() const;
    int iconSize() const;

    Q_INVOKABLE void launchApp(const QString &desktopFileName);
    void onConfigReloaded() override;

signals:
    void iconDataChanged();

private:
    void loadConfig();
    void loadApps();
    QStringList filterAndSortDesktopFiles() const;

    QStringList m_allValidApps;
    QStringList m_bottomRowApps;
    QStringList m_gridApps;
    QStringList m_bottomRowIcons;
    QStringList m_gridIcons;

    int m_columns;
    int m_spacing;
    int m_iconSize;
};
