#pragma once

#include <QObject>
#include <QStringList>
#include "helper/configmanager.h"

class Launcher : public QObject, public IConfigObserver
{
    Q_OBJECT
    Q_PROPERTY(QStringList allIcons READ allIcons NOTIFY iconDataChanged)
    Q_PROPERTY(QStringList allAppNames READ allAppNames NOTIFY iconDataChanged)
    Q_PROPERTY(int spacing READ spacing NOTIFY iconDataChanged)
    Q_PROPERTY(int iconSize READ iconSize NOTIFY iconDataChanged)

public:
    explicit Launcher(QObject *parent = nullptr);
    QStringList allIcons() const;
    QStringList allAppNames() const;

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
    QStringList m_allIcons;
    QStringList m_allAppNames;

    int m_spacing;
    int m_iconSize;
};
