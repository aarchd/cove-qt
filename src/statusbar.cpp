#include "statusbar.h"
#include "helper/configmanager.h"

#include <QStandardPaths>
#include <QDir>
#include <QDebug>
#include <QJsonArray>

StatusBar::StatusBar(QObject *parent)
    : QObject(parent)
{
    loadConfig();
    ConfigManager::instance().registerObserver(this);
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

int StatusBar::heightPercent() const { return m_heightPercent; }
QString StatusBar::backgroundColor() const { return m_backgroundColor; }
QString StatusBar::textColor() const { return m_textColor; }
bool StatusBar::timeFormat24() const { return m_timeFormat24; }
