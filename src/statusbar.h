#pragma once

#include <QObject>
#include <QStringList>
#include "helper/configmanager.h"
#include "helper/battery.h"

class StatusBar : public QObject, public IConfigObserver
{
    Q_OBJECT
    Q_PROPERTY(int heightPercent READ heightPercent NOTIFY statusBarChanged)
    Q_PROPERTY(QString backgroundColor READ backgroundColor NOTIFY statusBarChanged)
    Q_PROPERTY(QString textColor READ textColor NOTIFY statusBarChanged)
    Q_PROPERTY(bool timeFormat24 READ timeFormat24 NOTIFY statusBarChanged)
    Q_PROPERTY(QString batteryIcon READ batteryIcon NOTIFY statusBarChanged)
    Q_PROPERTY(int batteryPercentage READ batteryPercentage NOTIFY statusBarChanged)

public:
    explicit StatusBar(QObject *parent = nullptr);
    
    int heightPercent() const;
    QString backgroundColor() const;
    QString textColor() const;
    bool timeFormat24() const;
    QString batteryIcon() const;
    int batteryPercentage() const;

    void onConfigReloaded() override;

Q_SIGNALS:
    void statusBarChanged();

private:
    void loadConfig();
    void updateBatteryStatus();

    int m_heightPercent;
    QString m_backgroundColor;
    QString m_textColor;
    bool m_timeFormat24;
    Battery m_battery;
    QString m_batteryIcon;
    int m_batteryPercentage;
};
