#include "statusbar.h"
#include "helper/configmanager.h"

#include <QStandardPaths>
#include <QDir>
#include <QDebug>
#include <QJsonArray>
#include <QIcon>
#include <QPixmap>
#include <QSize>

StatusBar::StatusBar(QObject *parent)
    : QObject(parent)
{
    loadConfig();
    ConfigManager::instance().registerObserver(this);

    connect(&m_battery, &Battery::percentageChanged, this, &StatusBar::updateBatteryStatus);
    connect(&m_battery, &Battery::stateChanged, this, &StatusBar::updateBatteryStatus);

    updateBatteryStatus();
}

void StatusBar::onConfigReloaded()
{
    loadConfig();
    Q_EMIT statusBarChanged();
}

void StatusBar::loadConfig()
{
    const QJsonObject root = ConfigManager::instance().config();
    const QJsonObject statusBarObj = root.value("statusBar").toObject();
    const QJsonObject config = statusBarObj.value("config").toObject();

    m_heightPercent = config.value("heightPercent").toInt(5);
    m_backgroundColor = config.value("backgroundColor").toString("#88000000");
    m_textColor = config.value("textColor").toString("#99FFFFFF");
    m_timeFormat24 = config.value("timeFormat24").toBool(true);
}

void StatusBar::updateBatteryStatus()
{
    int percentage = m_battery.percentage();
    int state = m_battery.state();

    // unknown = empty
    if (state == 0 || state == 3) {
        state = 0;  
    }

    QString iconName;

    // https://gjs-docs.gnome.org/upowerglib10~1.0/upowerglib.devicestate
    switch (state) {
        case 1: 
            iconName = "battery-charging-symbolic";
            break;
        case 2: 
            if (percentage >= 90)
                iconName = "battery-full-symbolic";
            else if (percentage >= 60)
                iconName = "battery-good-symbolic";
            else if (percentage >= 30)
                iconName = "battery-medium-symbolic";
            else if (percentage >= 10)
                iconName = "battery-low-symbolic";
            else
                iconName = "battery-caution-symbolic";
            break;
        case 4: 
            iconName = "battery-full-symbolic";
            break;
        case 0: 
        default:
            iconName = "battery-empty-symbolic"; 
            break;
    }

    QIcon icon = QIcon::fromTheme(iconName);
    if (!icon.isNull()) {
        QSize iconSize(24, 24);
        QPixmap pixmap = icon.pixmap(iconSize);

        QString cacheDir = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/cove/icons";
        QDir().mkpath(cacheDir);
        QString filePath = cacheDir + "/" + iconName + ".png";
        pixmap.save(filePath);
        m_batteryIcon = QUrl::fromLocalFile(filePath).toString();
    } else {
        m_batteryIcon.clear();
    }

    m_batteryPercentage = percentage;

    Q_EMIT statusBarChanged();
}

int StatusBar::heightPercent() const { return m_heightPercent; }
QString StatusBar::backgroundColor() const { return m_backgroundColor; }
QString StatusBar::textColor() const { return m_textColor; }
bool StatusBar::timeFormat24() const { return m_timeFormat24; }
QString StatusBar::batteryIcon() const { return m_batteryIcon; }
int StatusBar::batteryPercentage() const { return m_batteryPercentage; }
