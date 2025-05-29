#pragma once

#include <QObject>
#include <QStringList>
#include "helper/configmanager.h"

class Launcher : public QObject, public IConfigObserver
{
    Q_OBJECT
    Q_PROPERTY(QStringList allIcons READ allIcons NOTIFY iconDataChanged)
    Q_PROPERTY(QStringList allAppNames READ allAppNames NOTIFY iconDataChanged)
    Q_PROPERTY(QStringList favAppsIcons READ favAppsIcons NOTIFY iconDataChanged)
    Q_PROPERTY(int spacing READ spacing NOTIFY iconDataChanged)
    Q_PROPERTY(int iconSize READ iconSize NOTIFY iconDataChanged)
    Q_PROPERTY(int emptyPane READ emptyPane NOTIFY iconDataChanged)
    Q_PROPERTY(QString bottomRowColor READ bottomRowColor NOTIFY iconDataChanged)
    Q_PROPERTY(int bottomRowWidthPercent READ bottomRowWidthPercent NOTIFY iconDataChanged)
    Q_PROPERTY(int bottomRowRadius READ bottomRowRadius NOTIFY iconDataChanged)

public:
    explicit Launcher(QObject *parent = nullptr);
    QStringList allIcons() const;
    QStringList allAppNames() const;
    QStringList favApps() const;
    QStringList favAppsIcons() const;
    QString bottomRowColor() const;

    int spacing() const;
    int iconSize() const;
    int emptyPane() const;
    int bottomRowWidthPercent() const;
    int bottomRowRadius() const;

    Q_INVOKABLE void launchApp(const QString &desktopFileName);
    void onConfigReloaded() override;

signals:
    void iconDataChanged();

private:
    void loadConfig();
    void loadApps();

    QStringList m_allValidApps;
    QStringList m_allIcons;
    QStringList m_allAppNames;
    QStringList m_favApps;
    QStringList m_favAppsIcons;

    int m_spacing;
    int m_iconSize;
    int m_emptyPane;
    QString m_bottomRowColor;
    int m_bottomRowWidthPercent;
    int m_bottomRowRadius;
};
