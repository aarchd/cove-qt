#include "dock.h"
#include "helper/desktopfile.h"
#include "helper/desktopiconloader.h"
#include "helper/configmanager.h"

#include <QStandardPaths>
#include <QSettings>
#include <QDir>
#include <QFileInfo>
#include <QFile>
#include <QDebug>
#include <QUrl>
#include <QPixmap>
#include <QRegularExpression>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

Dock::Dock(QObject *parent) : QObject(parent)
{
    loadConfig();
    loadDesktopIcons();
    ConfigManager::instance().registerObserver(this);
}

void Dock::onConfigReloaded() {
    loadConfig();
    loadDesktopIcons();
    emit dockConfigChanged();
}

int Dock::widthPercent() const { return m_widthPercent; }
int Dock::heightPercent() const { return m_heightPercent; }
int Dock::iconWidthPercent() const { return m_iconWidthPercent; }
int Dock::iconHeightPercent() const { return m_iconHeightPercent; }
int Dock::bottomMarginPercent() const { return m_bottomMarginPercent; }
QString Dock::bgColor() const { return m_bgColor; }

QStringList Dock::iconPaths() const
{
    return m_iconPaths;
}

void Dock::loadConfig()
{
    const QJsonObject root = ConfigManager::instance().config();
    const QJsonObject dockObj = root.value("dock").toObject();
    const QJsonObject dockConfig = dockObj.value("config").toObject();
    m_bgColor = dockConfig.value("bg_color").toString("#88000000");
    m_widthPercent = dockConfig.value("width_percent").toInt(30);
    m_heightPercent = dockConfig.value("height_percent").toInt(10);
    m_iconWidthPercent = dockConfig.value("icon_width_percent").toInt(80);
    m_iconHeightPercent = dockConfig.value("icon_height_percent").toInt(80);
    m_bottomMarginPercent = dockConfig.value("bottom_margin_percent").toInt(2);

    m_apps.clear();
    const QJsonArray appsArray = dockObj.value("apps").toArray();
    for (const QJsonValue &val : appsArray) {
        if (val.isString()) {
            m_apps.append(val.toString());
        }
    }
}

void Dock::loadDesktopIcons()
{
    QSize iconSize(64, 64); 
    m_iconPaths = DesktopIconLoader::loadDesktopIcons(m_apps, iconSize);
    emit iconPathsChanged();
}
